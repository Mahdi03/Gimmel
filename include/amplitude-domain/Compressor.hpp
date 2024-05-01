#ifndef GIMMEL_COMPRESSOR_HPP
#define GIMMEL_COMPRESSOR_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Compressor {
    private:
        float threshold = 1.f, ratio = 1.f;
        float attackIncrement = 0.f, releaseDecrement = 0.f;
        int holdTimeSamples = 0, numHoldTimeSamplesLeft = 0;
        float ramp = 0.f; 
        bool hardKnee = true;
        int sampleRate;

    public:
        Compressor(int sampleRate) : sampleRate(sampleRate) {}
        ~Compressor() {}
        //Copy constructor
        Compressor(const Compressor& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->attackIncrement = c.attackIncrement;
            this->releaseDecrement = c.releaseDecrement;
            this->ramp = c.ramp;
            this->hardKnee = c.hardKnee;
        }
        //Copy assignment constructor
        Compressor& operator=(const Compressor& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->attackIncrement = c.attackIncrement;
            this->releaseDecrement = c.releaseDecrement;
            this->ramp = c.ramp;
            this->hardKnee = c.hardKnee;
            return *this;
        }

        T processSample(T in) {
            if (hardKnee) {
                //TODO:
                if (::fabs(in) > threshold) {
                    //Then we want to start/continue attacking
                    if (this->ramp < 1) {
                        this->ramp += this->attackIncrement;
                    }
                }
                else {
                    //Then we want to start/continue releasing
                    if (this->ramp > 0) {
                        this->ramp -= this->releaseDecrement;
                    }
                }
                return in * (1 - (this->ramp * this->ratio));
            }
            else {
                //TODO: Implement knee curvature
            }
        }

        void setAttackTime(float attackMillis) {
            if (attackMillis <= 0) {
                attackMillis = 0.000000000000000001f;
                std::cout << "Attack time set to psuedo-zero value, supply a positive float" << std::endl;
            }
            this->attackIncrement = 1.f / millisToSamples(attackMillis);
        }

        void setReleaseTime(float releaseMillis) {
            if (releaseMillis <= 0) {
                releaseMillis = 0.000000000000000001f;
                std::cout << "Release time set to psuedo-zero value, supply a positive float" << std::endl;
            }
            this->releaseIncrement = 1.f / millisToSamples(releaseMillis);
        }

        void setHoldTime(float holdMillis) {
            this->holdTimeSamples = millisToSamples(holdMillis);
        }

        void setHardKnee(bool knee) {
            this->hardKnee = knee;
        }
        
    };
}
#endif