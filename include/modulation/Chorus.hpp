#ifndef GIMMEL_CHORUS_HPP
#define GIMMEL_CHORUS_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Chorus {
    private:
        int sampleRate;
        float rate = 1.f, depth = 100.f, blend = 0.5f;
        giml::CircularBuffer<float> buffer;
        giml::SinOsc osc;

    public:
        Chorus (int samprate) : sampleRate(samprate), osc(samprate) {
            this->osc.setFrequency(rate);
            this->buffer.allocate(100000); // max delay is 100,000 samples
        }

        T processSample(T in) {
            this->buffer.writeSample(in); // write sample to delay buffer

            int readIndex = static_cast<int>(round(this->depth + this->depth * this->osc.processSample())); // calculate readIndex

            float output = buffer.readSample(readIndex); // get sample
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