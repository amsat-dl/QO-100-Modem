from pydub import AudioSegment
import os
import sys

def wav2pcm(wav_dir,out_pcm_dir):

    for path,pathname,filenames in os.walk(wav_dir):
        for filename in filenames:
            
            sound = AudioSegment.from_file(os.path.join(path,filename))
            outfilename=os.path.join(out_pcm_dir,filename.split('.wav')[0]+'.pcm')
            sound.export(outfilename, format='s16le', bitrate='8k')



if __name__=="__main__":

    if len(sys.argv) != 3:
        print("usage: python wav2pcm.py in_dir  out_dir")    
    else:
        wav2pcm(sys.argv[1],sys.argv[2])
    
