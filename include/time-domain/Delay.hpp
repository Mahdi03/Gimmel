#ifndef GIMMEL_DELAY_HPP
#define GIMMEL_DELAY_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Delay : public Effect<T> {
    private:
        int sampleRate;
        float feedback = 0.f, delayTime = 0.f;
        giml::CircularBuffer<float> buffer;

    public:
        Delay() = delete;
        Delay(int samprate) : sampleRate(samprate) {
            this->buffer.allocate(100000); // max delayTime is samplesToMillis(100,000)
        }

        T processSample(T in) {
            
            if (!(this->enabled)) {
                return in;
            }

            int readIndex = ::round(millisToSamples(this->delayTime, this->sampleRate));

            float output = this->buffer.readSample(readIndex); 

            this->buffer.writeSample(in + this->feedback * output); // write sample to delay buffer

          return output;
        }

        void setFeedback(float fbGain) {
            this->feedback = fbGain;
        }

        void setDelayTime(float sizeMillis) {
            if (sizeMillis > samplesToMillis(100000, this->sampleRate)) {
                sizeMillis = samplesToMillis(100000, this->sampleRate);
            }
            this->delayTime = sizeMillis;
        }
    };
}
#endif