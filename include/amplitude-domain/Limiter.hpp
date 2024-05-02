#ifndef GIMMEL_LIMITER_HPP
#define GIMMEL_LIMITER_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Limiter {
    private:
        float threshold = 1.f, ratio = 1.f;
        float gain = 1.f;
        int sampleRate;

    public:
        Limiter(int sampleRate) : sampleRate(sampleRate) {}
        ~Limiter() {}
        //Copy constructor
        Limiter(const Limiter& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->gain = c.gain;
        }
        //Copy assignment constructor
        Limiter& operator=(const Limiter& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->gain = c.gain;
            return *this;
        }

        T processSample(T in) {
            float magnitude = ::fabs(in);
            if (magnitude > this->threshold) {
                 //Then we want to limit
                float diff = magnitude - this->threshold; // calculate how far above thresh 
                gain = (1 - (diff * ratio)); // scale diff by ratio, set gain 
            }
            else {
                gain = 1.f;
                }
              return in * gain;
        }

        void setRatio(float r) {
            this->ratio = r;
        }

        void setThreshold(float t) {
            this->threshold = dBtoA(t);
        }        
    };
}
#endif