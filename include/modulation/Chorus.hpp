#ifndef GIMMEL_CHORUS_HPP
#define GIMMEL_CHORUS_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Chorus : public Effect<T> {
    private:
        int sampleRate;
        float rate = 1.f, depth = 20.f, blend = 0.5f;
        giml::CircularBuffer<float> buffer;
        giml::TriOsc osc;

    public:
        Chorus() = delete;
        Chorus (int samprate) : sampleRate(samprate), osc(samprate) {
            this->osc.setFrequency(rate);
            this->buffer.allocate(100000); // max delay is 100,000 samples
        }

        T processSample(T in) {
            this->buffer.writeSample(in); // write sample to delay buffer

            if (!(this->enabled)) {
                return in;
            }

            float idealReadIndex = millisToSamples(this->depth, this->sampleRate) + 
            (millisToSamples(this->depth, this->sampleRate) * 0.5) * this->osc.processSample();

            int readIndex = int(idealReadIndex); // calculate readIndex
            
            int readIndex2 = readIndex + 1;

            float frac = idealReadIndex - readIndex;

            float output = (this->buffer.readSample(readIndex) * (1 - frac)) + (this->buffer.readSample(readIndex2) * frac); // get sample
          return output * blend + in * (1-blend); // return mix
        }

        void setRate(float freq) {
            this->osc.setFrequency(freq); // set frequency in Hz
        }

        void setDepth(float d) {
            this->depth = d;
        }

        void setBlend(float b) {
            this->blend = b;
        }
    };
}
#endif