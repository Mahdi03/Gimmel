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
            float magnitude = ::fabs(static_cast<float>(in));
            float diff = magnitude - this->threshold;
            if (hardKnee) {
                //TODO:
                if (magnitude > this->threshold) {
                    //Then we want to start/continue attacking
                    if (this->ramp < 1) {
                        this->ramp += this->attackIncrement;
                    }
                }
                else {
                    //Then we want to start/continue hold/releasing
                    //If we are in the middle of holding
                    if (this->ramp <= 0) {
                        //Then we are done compressing, just return the sample
                        return in;
                    }
                    //Else we want to either start the hold or 
                    if (this->numHoldTimeSamplesLeft < 0) {
                        //Then this means we need to set 
                    }
                    if (this->ramp > 0) {
                        this->ramp -= this->releaseDecrement;
                    }
                }
                return in * (1 - (this->ramp * this->ratio * diff));
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
            this->attackIncrement = 1.f / millisToSamples(attackMillis, this->sampleRate);
        }

        void setReleaseTime(float releaseMillis) {
            if (releaseMillis <= 0) {
                releaseMillis = 0.000000000000000001f;
                std::cout << "Release time set to psuedo-zero value, supply a positive float" << std::endl;
            }
            this->releaseDecrement = 1.f / millisToSamples(releaseMillis, this->sampleRate);
        }

        void setRatio(float r) {
            this->ratio = r;
        }

        void setThreshold(float t) {
            this->threshold = dBtoA(t);
        }

        void setHoldTime(float holdMillis) {
            this->holdTimeSamples = millisToSamples(holdMillis, this->sampleRate);
        }

        void setHardKnee(bool knee) {
            this->hardKnee = knee;
        }
        
    };
}
#endif