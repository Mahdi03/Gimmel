#ifndef GIMMEL_COMPRESSOR_HPP
#define GIMMEL_COMPRESSOR_HPP

#include <math.h>
#include "../utility.hpp"

namespace giml {
    template <typename T>
    class Compressor : public Effect<T> {
    private:
        int sampleRate;
        float thresholdB = 1.f, ratio = 1.f, knee = 1.f;
        float attackIncrement = 0.f, releaseDecrement = 0.f;
        float ramp = 1.f;


    public:
        Compressor() = delete; // Do not allow an empty constructor, they must pass in a sampleRate
        Compressor(int sampleRate) : sampleRate(sampleRate) {}

        inline T processSample(const T& in) override {
            if (!(this->enabled)) {
                return in;
            }

            float magnitude = ::fabs(in);
            float diff = magnitude - this->threshold;
            float gain = 1.f;

            if (magnitude >= this->threshold) { // if samp > thresh... 
                if (this->ramp >= 1.f) { // and ramp is max...
                    this->ramp = 1.f;
                    gain = 1.f - (diff * this->ratio);
                } 
                else { // if ramp is not...
                    this->ramp += this->attackIncrement;
                    gain = (1.f - (diff / denom) * this->ratio); 
                }
            } 
            else { // if samp < thresh...
                if (this->ramp <= 0.f) { // and ramp is min...
                    this->ramp = 0.f;
                    gain = 1.f;
                } 
                else { // if ramp is not... 
                    this->ramp -= this->releaseDecrement;
                    //gain = (1.f - (this->ramp * diff * this->ratio)); // <- bug, diff would be negative here
                    gain = 1.f;
                }
            }
          return in * gain;
        }

        void setAttackTime(float attackMillis) {
            if (attackMillis <= 0.f) {
                attackMillis = 0.000000000000000001f;
                std::cout << "Attack time set to psuedo-zero value, supply a positive float" << std::endl;
            }
            this->attackIncrement = 1.f / millisToSamples(attackMillis, this->sampleRate);
        }

        void setReleaseTime(float releaseMillis) {
            if (releaseMillis <= 0.f) {
                releaseMillis = 0.000000000000000001f;
                std::cout << "Release time set to psuedo-zero value, supply a positive float" << std::endl;
            }
            this->releaseDecrement = 1.f / millisToSamples(releaseMillis, this->sampleRate);
        }

        void setRatio(float r) {
            this->ratio = r;
        }

        void setThreshold(float thresh) {
            this->threshold = dBtoA(thresh);
        }
    };
}
#endif