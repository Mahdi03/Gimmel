#ifndef GIML_COMPRESSOR_HPP
#define GIML_COMPRESSOR_HPP

#include <math.h>
#include "../utility.hpp"

namespace giml {
    // signal chain 
    // xG = giml::aTodB(in) 
    // yG = computeGain(xG, thresh, ratio, knee) 
    // xL = xG - yG
    // yL = detectdB(xL, aAttack, aRelease) 
    // cdB = M - yL
    // gain = giml::dBtoA(cdB) 
    // return (in * gain)

    template <typename T>
    class Compressor : public Effect<T> {
    private:
        int sampleRate;
        float thresh_dB = 0.f, ratio = 2.f, knee_dB = 1.f;
        float aRelease = 0.f, aAttack = 0.f;
        float makeupGain_dB = 0.f;
        class DetectdB {
        private:
            T y1last = 0;
            T yL_last = 0;

        public:
            T process(T xL, T aA, T aR) {
                // decoupled peak detector (from Reiss et al. 2011) (Eq. 17)
                T y1 = std::max(xL, (aR * this->y1last) + ((1.f - aR) * xL)); // max (in, filt(in))
                this->y1last = y1;
                T yL = (aA * this->yL_last) + ((1 - aA) * y1);
                this->yL_last = yL;
                return yL;
            }
        };
        DetectdB detector; 

    protected: 
        float computeGain(float xG, float thresh, float ratio, float knee) {
            float yG = 1.f;
            // calculate yG
            if (2.f * (xG - thresh) < -knee) { // if input < thresh - knee
                yG = xG;
            } 
            else if (2.f * ::fabs(xG - thresh) <= knee) { // if input is inside knee
                yG = xG + 
                (1.f / (ratio - 1.f)) *
                ::powf((xG - thresh) + (knee / 2.f), 2.f) /
                (2.f * knee); // knee needs to be non-zero
            } 
            else if (2.f * (xG - thresh) > knee) { // if input > thresh + knee
                yG = thresh + ((xG - thresh) / ratio);
            }
          return yG; 
        }

    public:
        //Constructor
        Compressor() = delete; // Do not allow an empty constructor, they must pass in a sampleRate
        Compressor(int sampleRate) : sampleRate(sampleRate) {}
        //Copy Constructor
        Compressor(const Compressor<T>& c) {
            //TODO:
        }
        //TODO: Copy Assignment Operator



        inline T processSample(const T& in) override {
            if (!(this->enabled)) {
                return in;
            }

            T xG = giml::aTodB(in); // xG
            T yG = computeGain(xG, this->thresh_dB, this->ratio, this->knee_dB); // yG
            T xL = xG - yG; // xL
            T yL = this->detector.process(xL, this->aAttack, this->aRelease); // yL
            T cdB = this->makeupGain_dB - yL; // cdB = M - yL

            T gain = giml::dBtoA(cdB); // lin()
            return (in * gain); // apply gain
        }

        void setAttack(float attackMillis) { // calculated from Reiss et al. 2011 (Eq. 7)
            if (attackMillis <= 0.f) {
                attackMillis = 0.000000000000000001f;
                std::cout << "Attack time set to pseudo-zero value, supply a positive float" << std::endl;
            }
            float t = attackMillis * 0.001f;
            this->aAttack = ::powf( M_E , -1.f / (t * this->sampleRate) );
        }

        void setRelease(float releaseMillis) { // // 
            if (releaseMillis <= 0.f) {
                releaseMillis = 0.000000000000000001f;
                std::cout << "Release time set to pseudo-zero value, supply a positive float" << std::endl;
            }
            float t = releaseMillis * 0.001f;
            this->aRelease = ::powf( M_E , -1.f / (t * this->sampleRate) );
        }

        void setRatio(float r) {
            if (r <= 1.f) { 
                r = 1.f + 0.00001f;
                std::cout << "Ratio must be greater than 1" << std::endl; // necessary? 
            }
            this->ratio = r;
        }

        void setThresh(float threshdB) {
            this->thresh_dB = threshdB;
        }
        
        void setKnee(float widthdB) {
            if (widthdB <= 0.f) {
                widthdB = 0.00001f;
                std::cout << "Knee set to pseudo-zero value, supply a positive float" << std::endl;
            }
            this->knee_dB = widthdB;
        }

        void setMakeupGain(float mdB) {
            this->setMakeupGain = mdB;
        }
    };
}
#endif