#pragma once

#include <portaudio.h>

#define SAMPLE_RATE (22050)

class AudioProcessor {
    
public:
    AudioProcessor() {
        initPortAudio();
    }
    
    ~AudioProcessor() {
        destroyPortAudio();
    }
    
    float get_last_peak_avg() {
        
        // TODO: Needs lock
        return last_peak_avg;
    }
    
private:
    
    static int audioCallback( const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData )
    {
        /* Cast data passed through stream to our structure. */
        float *last_peak_avg = (float*)userData;
        float *in = (float*)inputBuffer;
        (void) outputBuffer; /* Prevent unused variable warning. */
        
        float tot = 0.0;
        for(int i=0; i<framesPerBuffer; i++ )
        {
            float left = *in++;
            tot += left;
        }
        
        *last_peak_avg = tot / framesPerBuffer;
        
        return 0;
    }
    
    bool initPortAudio() {
        
        PaError paErr;
        paErr = Pa_Initialize();
        if( paErr != paNoError ) {
            printf(  "PortAudio error: %s\n", Pa_GetErrorText( paErr ) );
            return false;
        }
        
        /* Open an audio I/O stream. */
        paErr = Pa_OpenDefaultStream( &stream,
                                     1,          /* input channels */
                                     0,          /* output channels */
                                     paFloat32,
                                     SAMPLE_RATE,
                                     256,
                                     audioCallback,
                                     &last_peak_avg );
        if( paErr != paNoError )
            return false;
        
        paErr = Pa_StartStream( stream );
        if( paErr != paNoError )
            return false;
        
        return true;
    }
    
    bool destroyPortAudio() {
        PaError paErr = Pa_CloseStream( stream );
        if( paErr != paNoError )
            printf(  "PortAudio error: %s\n", Pa_GetErrorText( paErr ) );
        
        paErr = Pa_Terminate();
        if( paErr != paNoError )
            printf(  "PortAudio error: %s\n", Pa_GetErrorText( paErr ) );
        
        return true;
    }
    
private:
    float last_peak_avg;
    PaStream *stream;
};
