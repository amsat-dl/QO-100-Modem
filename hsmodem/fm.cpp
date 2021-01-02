
#include "hsmodem.h"

void initfm();
void runfm();

freqmod fmmod;
freqdem fdem = NULL;
nco_crcf fm_dnnco = NULL;
nco_crcf fm_upnco = NULL;

firfilt_crcf fm_q = NULL;

void fmtest()
{
	static int f = 1;
	if (f)
	{
		f = 0;
		initfm();
	}

	runfm();
}

#define CENTERF 1700.0f

/*
* fmin = 1200 Hz
* fmax = 2300 Hz
* fcarrier = 1700 Hz
* 
* kf = max.deviation / samplerate
* max deviation = 2300 - 1700 = 600 Hz
* kf = 600/48000 = 0.0125
* 
* this results in:
* -0.99 ... 1100 Hz
* +0.99 ... 2300 Hz
*/
const int sstv_fsync = 1200;
const int sstv_fblack = 1500;
const int sstv_fwhite = 2300;

const int sstv_maxbw = (sstv_fwhite - sstv_fsync);
const float sstv_kf = ((float)sstv_maxbw / 2.0f) / (float)caprate;
const float sstv_carrier = (sstv_maxbw / 2) + sstv_fsync;

// frequency values per millisecond
#define COLLECT 5
const float per_ms = (float)COLLECT * 1000.0f / 48000.0f;
int realsync = 0;
const int maxpixel = 5000;
const int maxlines = 300;
int pidx = 0;
int lidx = 0;
uint8_t fmap[maxlines][maxpixel*2];

void initfm()
{
	fmmod = freqmod_create(sstv_kf);   // modulator
	fdem = freqdem_create(sstv_kf);

	// create NCO for up-mixing to 1500 Hz
	float fm_RX_RADIANS_PER_SAMPLE = 2.0f * (float)M_PI * sstv_carrier / (float)caprate;
	fm_upnco = nco_crcf_create(LIQUID_NCO);
	nco_crcf_set_phase(fm_upnco, 0.0f);
	nco_crcf_set_frequency(fm_upnco, fm_RX_RADIANS_PER_SAMPLE);

	// create NCO for down-mixing from 1500 Hz
	fm_dnnco = nco_crcf_create(LIQUID_NCO);
	nco_crcf_set_phase(fm_dnnco, 0.0f);
	nco_crcf_set_frequency(fm_dnnco, fm_RX_RADIANS_PER_SAMPLE);

	// RX Filter
	unsigned int flt_h_len = 31;  // filter length
	// we filter at 48k samp rate
	float flt_fc = (float)sstv_fwhite / 2.0f / 48000.0f;          // cutoff frequency
	float flt_As = 40.0f;         // stop-band attenuation

	fm_q = firfilt_crcf_create_kaiser(flt_h_len, flt_fc, flt_As, 0.0f);
	firfilt_crcf_set_scale(fm_q, 2.0f * flt_fc);
}

void runfm()
{
	int synpulse = 0;
	int syncanz = 0;
	float f;
	liquid_float_complex s;			// modulated signal
	liquid_float_complex sbase;
	liquid_float_complex sbasef;
	float syntim = 0;
	float sigtim = 0;
	
	while (keeprunning)
	{
		int ret = io_cap_read_fifo(&f);
		if (ret == 0)
		{
			sleep_ms(1);
			continue;
		}

		nco_crcf_step(fm_dnnco);
		s.real = f;
		s.imag = f;
		nco_crcf_mix_down(fm_dnnco, s, &sbase);

		// sharp filter
		firfilt_crcf_push(fm_q, sbase);    // push input sample
		firfilt_crcf_execute(fm_q, &sbasef); // compute output

		float y;						// output/demodulated message
		freqdem_demodulate(fdem, sbasef, &y);

		// y: -1..+1 (fsync..fwhite)
		y += 1;					// y: 0..2 (fsync..fwhite)
		y *= (sstv_maxbw/2);	// y: 0..maxbw (1100)
		y += sstv_fsync;		// y: fsync..fwhite
		int freq = (int)y;

		printf("%d ", freq);
		if (freq < 2000) printf("\n");

		static int farr[COLLECT];
		static int fidx = 0;

		farr[fidx] = freq;
		if (++fidx < COLLECT) continue;
		fidx = 0;

		for (int i = 0; i < COLLECT; i++)
			freq += farr[i];
		freq /= COLLECT;

		if (freq < sstv_fsync) freq = sstv_fsync;
		if (freq > sstv_fwhite) freq = sstv_fwhite;

		//printf("%d ", freq);
		//if (freq < 2000) printf("\n");

		syntim += per_ms;
		sigtim += per_ms;

		// detect start of sync pulse
		if (synpulse == 0 && freq < 1480 && sigtim > 430.0f)
		{
			// syn pulse starts
			if (sigtim < 431.0f)
			{
				printf("syn pulse missed, forced after %10.6f\n", sigtim);
				realsync = 0;
				syncanz = 0;
			}
			else
			{
				printf("syn pulse starts after %10.6f\n", sigtim);
				realsync = 1;
				if(syncanz < 4) syncanz++;
				if (syncanz == 3)
				{
					printf("picture start\n");
					lidx = 0;
				}
			}
			synpulse = 1;
			syntim = 0;
			if (lidx < maxlines) lidx++;
		}

		// detect end of syn pulse
		if (synpulse == 1 && freq > 1480)
		{
			// syn pulse ends
			// check if valid length
			if (syntim > 4.0f)
			{
				printf("syn pulse ends after %10.6f. In Sync:%d, written:%d to line:%d\n",syntim,realsync,pidx,lidx);
				synpulse = 0;
				sigtim = 0;
				pidx = 0;
			}
		}

		if (synpulse == 0)
		{
			fmap[lidx][pidx * 2] = freq >> 8;
			fmap[lidx][pidx * 2 + 1] = freq & 0xff;
			if (pidx < maxpixel) pidx++;

			if (lidx == 260)
			{
				FILE* fp = fopen("sstv.img", "wb");
				if(fp)
				{
					for (int i = 0; i < 260; i++)
					{
						fwrite(fmap[i], 1, maxpixel*2, fp);
					}
					fclose(fp);
					printf("file saved\n");
					sleep_ms(10000);
				}
			}
		}




		//printf("%10.6f  %10.6f %d\n", f, y, freq);

		/*
		// monitor
		nco_crcf_step(fm_upnco);
		nco_crcf_mix_up(fm_upnco, sbasef, &s);
		float usbf = s.real + s.imag;

		io_pb_write_fifo(usbf * 0.2f); // reduce volume and send to soundcard
		*/
	}
}