#include "hsmodem.h"



// create km_symtrack object with basic parameters
//  _ftype  : filter type (e.g. LIQUID_FIRFILT_RRC)
//  _k      : samples per symbol
//  _m      : filter delay (symbols)
//  _beta   : filter excess bandwidth
//  _ms     : modulation scheme (e.g. LIQUID_MODEM_QPSK)
SYMTRACK* km_symtrack_cccf_create(int          _ftype,
                            unsigned int _k,
                            unsigned int _m,
                            float        _beta,
                            int          _ms)
{
    // validate input
    if (_k < 2)
        printf((char*)"symtrack_cccf_create(), filter samples/symbol must be at least 2\n");
    if (_m == 0)
        printf((char*)"symtrack_cccf_create(), filter delay must be greater than zero\n");
    if (_beta <= 0.0f || _beta > 1.0f)
        printf((char*)"symtrack_cccf_create(), filter excess bandwidth must be in [0,1]\n");
    if (_ms == LIQUID_MODEM_UNKNOWN || _ms >= LIQUID_MODEM_NUM_SCHEMES)
        printf((char*)"symtrack_cccf_create(), invalid modulation scheme\n");

    // allocate memory for main object
    SYMTRACK *q = (SYMTRACK *) malloc(sizeof(SYMTRACK));

    // set input parameters
    q->filter_type = _ftype;
    q->k = _k;
    q->m = _m;
    q->beta = _beta;
    q->mod_scheme = _ms == LIQUID_MODEM_UNKNOWN ? LIQUID_MODEM_BPSK : _ms;

    // create automatic gain control
    q->agc = agc_crcf_create();

    // create symbol synchronizer (output rate: 2 samples per symbol)
    if (q->filter_type == LIQUID_FIRFILT_UNKNOWN)
        q->symsync = symsync_crcf_create_kaiser(q->k, q->m, 0.9f, 16);
    else
        q->symsync = symsync_crcf_create_rnyquist(q->filter_type, q->k, q->m, q->beta, 16);
    symsync_crcf_set_output_rate(q->symsync, 2);

    // create equalizer as default low-pass filter with integer symbol delay (2 samples/symbol)
    q->eq_len = 2 * 4 + 1;
    q->eq = eqlms_cccf_create_lowpass(q->eq_len, 0.45f);
    q->eq_strategy = q->SYMTRACK_EQ_DD;

    // nco and phase-locked loop
    q->nco = nco_crcf_create(LIQUID_VCO);

    // demodulator
    q->demod = modem_create((modulation_scheme)q->mod_scheme);

    // set default bandwidth
    km_symtrack_cccf_set_bandwidth(q, 0.9f);

    // reset and return main object
    km_symtrack_cccf_reset(q, 0xff);

    return q;
}

void km_symtrack_cccf_destroy(SYMTRACK *_q)
{
    if (_q == NULL) return;

    // destroy objects
    agc_crcf_destroy(_q->agc);
    symsync_crcf_destroy(_q->symsync);
    eqlms_cccf_destroy(_q->eq);
    nco_crcf_destroy(_q->nco);
    modem_destroy(_q->demod);

    // free main object
    free(_q);
}

void km_symtrack_cccf_reset(SYMTRACK* q, int mode)
{
    if (q == NULL) return;

    // reset objects
    if (mode & 1) agc_crcf_reset(q->agc);
    if (mode & 2) symsync_crcf_reset(q->symsync);
    if (mode & 4) eqlms_cccf_reset(q->eq);
    if (mode & 8) nco_crcf_reset(q->nco);
    if (mode & 0x10) modem_reset(q->demod);

    // reset internal counters
    q->symsync_index = 0;
    q->num_syms_rx = 0;
}

void km_symtrack_cccf_set_bandwidth(SYMTRACK *q, float _bw)
{
    // validate input
    if (_bw < 0)
        printf("symtrack_set_bandwidth(), bandwidth must be in [0,1]\n");

    // set bandwidths accordingly
    float agc_bandwidth = 0.02f * _bw;
    float symsync_bandwidth = 0.001f * _bw;
    float eq_bandwidth = 0.02f * _bw;
    float pll_bandwidth = 0.001f * _bw;

    // automatic gain control
    agc_crcf_set_bandwidth(q->agc, agc_bandwidth);

    // symbol timing recovery
    symsync_crcf_set_lf_bw(q->symsync, symsync_bandwidth);

    // equalizer
    eqlms_cccf_set_bw(q->eq, eq_bandwidth);

    // phase-locked loop
    nco_crcf_pll_set_bandwidth(q->nco, pll_bandwidth);
}

#define MX 10

// execute synchronizer on single input sample
//  _q      : synchronizer object
//  _x      : input data sample
//  _y      : output data array
//  _ny     : number of samples written to output buffer
void km_symtrack_execute(SYMTRACK* q, liquid_float_complex _x, liquid_float_complex* _y, unsigned int* _ny, unsigned int *psym_out)
{
    if (q == NULL) return;

    liquid_float_complex v;   // output sample
    unsigned int i;
    unsigned int num_outputs = 0;

    // run sample through automatic gain control
    agc_crcf_execute(q->agc, _x, &v);

    // symbol synchronizer
    unsigned int nw = 0;
    symsync_crcf_execute(q->symsync, &v, 1, q->symsync_buf, &nw);

    // process each output sample
    for (i = 0; i < nw; i++) {
        // update phase-locked loop
        nco_crcf_step(q->nco);
        nco_crcf_mix_down(q->nco, q->symsync_buf[i], &v);

        // equalizer/decimator
        eqlms_cccf_push(q->eq, v);

        // decimate result, noting that symsync outputs at exactly 2 samples/symbol
        q->symsync_index++;
        if (!(q->symsync_index % 2))
            continue;

        // increment number of symbols received
        q->num_syms_rx++;

        // compute equalizer filter output; updating coefficients is dependent upon strategy
        liquid_float_complex d_hat;
        eqlms_cccf_execute(q->eq, &d_hat);

        // demodulate result, apply phase correction
        unsigned int sym_out;
        modem_demodulate(q->demod, d_hat, &sym_out);
        *psym_out = sym_out;
        float phase_error = modem_get_demodulator_phase_error(q->demod);

        // update pll
        nco_crcf_pll_step(q->nco, phase_error);

        // update equalizer independent of the signal: estimate error
        // assuming constant modulus signal
        // TODO: check lock conditions of previous object to determine when to run equalizer
        liquid_float_complex d_prime;
        d_prime.real = d_prime.imag = 0;
        if (q->num_syms_rx > 200) 
        {
            modem_get_demodulator_sample(q->demod, &d_prime);
            eqlms_cccf_step(q->eq, d_prime, d_hat);
        }

        // save result to output
        _y[num_outputs++] = d_hat;
    }

    /*float fr = nco_crcf_get_frequency(q->nco);
    float ph = nco_crcf_get_phase(q->nco);
    printf("f:%10.6f  ph:%10.6f\n", fr, ph);*/

    * _ny = num_outputs;
}
