#ifndef GIMMEL_DETUNE_HPP
#define GIMMEL_DETUNE_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Detune {
    private:
        int sampleRate;
        const float pi = static_cast<float>(M_PI);
        const float twoPi = 2.f * static_cast<float>(M_PI);
        float pitchRatio = 0.941f;
        float windowSize = 22.f;
        giml::CircularBuffer<float> buffer;
        giml::Phasor osc;

    public:
        Detune (int samprate) : sampleRate(samprate), osc(samprate) {
            this->osc.setFrequency(1000.f * ((1.f - this->pitchRatio) / this->windowSize));
            this->buffer.allocate(100000); // max windowSize is samplesToMillis(100,000)
        }

        T processSample(T in) {
            this->buffer.writeSample(in); // write sample to delay buffer

            int readIndex = static_cast<int>(round( // readpoint 1
                this->osc.processSample() * millisToSamples(this->windowSize, this->sampleRate))); 
            int readIndex2 = static_cast<int>(round( // readpoint 2
                (fmod(this->osc.getPhase() + 0.5f, 1)) * millisToSamples(this->windowSize, this->sampleRate))); 

            float output = this->buffer.readSample(readIndex); // get sample
            float output2 = this->buffer.readSample(readIndex2); // get sample 2

            float windowOne = cosf((this->osc.getPhase() - 0.5) * pi); // gain windowing
            float windowTwo = cosf(((fmod((this->osc.getPhase() + 0.5f), 1.f)) - 0.5) * pi);// ^

          return output * windowOne + output2 * windowTwo; // windowed output
        }

        void setPitchRatio(float ratio) {
            this->pitchRatio = ratio;
            this->osc.setFrequency(1000.f * ((1.f - ratio) / this->windowSize));
        }

        void setWindowSize(float sizeMillis) {
            this->windowSize = sizeMillis;
        }
    };
}
#endif