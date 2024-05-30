#ifndef GIMMEL_COMPRESSOR_HPP
#define GIMMEL_COMPRESSOR_HPP

#include <math.h>
#include "../utility.hpp"

namespace giml {
    template <typename T>
    class Compressor : public Effect<T> {
    private:
        int sampleRate;
        float thresh_dB = 0.f, ratio = 1.f, knee_dB = 1.f;
        float attackIncrement = 0.f, releaseDecrement = 0.f;
        float ramp = 1.f;


    public:
        Compressor() = delete; // Do not allow an empty constructor, they must pass in a sampleRate
        Compressor(int sampleRate) : sampleRate(sampleRate) {}

        inline T processSample(const T& in) override {
            if (!(this->enabled)) {
                return in;
            }

            float in_dB = giml::aTodB(in); // measure input in dB
            float diff = in_dB - this->thresh_dB; // calculate diff from threshold 
            float gain_dB = 0.f; // initialize gain at unity 

            if (in_dB < this->thresh_dB - this->knee_dB) { // if input < thresh - knee
                gain_dB = in_dB;
            } 
            else if (in_dB <= this->thresh_dB + this->knee_dB) { // if input is inside knee
                gain_dB + 
                ((1.f / (this->ratio - 1)) *
                ::powf(diff + (this->knee_dB), 2.f)) /
                (4.f * this->knee_dB);
            } 
            else { // if input > thresh + knee
                gain_dB = this->thresh_dB
                //+ (diff / this->ratio)
                ;
            }

            gain_dB = gain_dB - in_dB; // calculate gain attenuation 
          return in * giml::dBtoA(gain_dB); // apply gain as linear amplitude 
        }

        void setAttackTime(float attackMillis) {
            if (attackMillis <= 0.f) {
                attackMillis = 0.000000000000000001f;
                std::cout << "Attack time set to pseudo-zero value, supply a positive float" << std::endl;
            }
            this->attackIncrement = 1.f / millisToSamples(attackMillis, this->sampleRate);
        }

        void setReleaseTime(float releaseMillis) {
            if (releaseMillis <= 0.f) {
                releaseMillis = 0.000000000000000001f;
                std::cout << "Release time set to pseudo-zero value, supply a positive float" << std::endl;
            }
            this->releaseDecrement = 1.f / millisToSamples(releaseMillis, this->sampleRate);
        }

        void setRatio(float r) {
            this->ratio = r;
        }

        void setThresh(float threshdB) {
            this->thresh_dB = threshdB;
        }
        
        void setKnee(float widthdB) {
            if (widthdB <= 0.f) {
                widthdB = 0.000000000000000001f;
                std::cout << "Knee set to pseudo-zero value, supply a positive float" << std::endl;
            }
            this->knee_dB = widthdB;
        }
    };
}
#endif