#ifndef GIMMEL_COMPRESSOR_HPP
#define GIMMEL_COMPRESSOR_HPP

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

    class detectdB {
    private:
        float y1last = 0.f;
        float yL_last = 0.f;

    public:
        float process(float xL, float aA, float aR) {
            // decoupled peak detector (from Reiss et al. 2011) (Eq. 17)
            float y1 = std::max(xL, (aR * this->y1last) + ((1.f - aR) * xL)); // max (in, filt(in))
            this->y1last = y1;
            float yL = (aA * yL_last) + ((1 - aA) * y1);
            this->yL_last = yL;
          return yL;
        }
    };

    template <typename T>
    class Compressor : public Effect<T> {
    private:
        int sampleRate;
        float thresh_dB = 0.f, ratio = 2.f, knee_dB = 1.f;
        float aRelease = 0.f, aAttack = 0.f;
        float makeupGain_dB = 0.f;
        //float y1last = 0.f, yL_last = 0.f;
        detectdB detector; 

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
        Compressor() = delete; // Do not allow an empty constructor, they must pass in a sampleRate
        Compressor(int sampleRate) : sampleRate(sampleRate) {}

        inline T processSample(const T& in) override {
            if (!(this->enabled)) {
                return in;
            }

            float xG = giml::aTodB(in); // xG
            float yG = computeGain(xG, this->thresh_dB, this->ratio, this->knee_dB); // yG
            float xL = xG - yG; // xL
            float yL = detector.process(xL, this->aAttack, this->aRelease); // yL
            float cdB = makeupGain_dB - yL; // cdB = M - yL

            float gain = giml::dBtoA(cdB); // lin()
          return (in * gain); // apply gain
        }

        void setAttack(float attackMillis) { // calculated from Reiss et al. 2011 (Eq. 7)
            if (attackMillis <= 0.f) {
                attackMillis = 0.000000000000000001f;
                std::cout << "Attack time set to pseudo-zero value, supply a positive float" << std::endl;
            }
            float t = attackMillis * 0.001f;
            this->aAttack = ::powf( GIML_E , -1.f / (t * this->sampleRate) );
        }

        void setRelease(float releaseMillis) { // // 
            if (releaseMillis <= 0.f) {
                releaseMillis = 0.000000000000000001f;
                std::cout << "Release time set to pseudo-zero value, supply a positive float" << std::endl;
            }
            float t = releaseMillis * 0.001f;
            this->aRelease = ::powf( GIML_E , -1.f / (t * this->sampleRate) );
        }

        void setRatio(float r) {
            if (r <= 1.f) { 
                r = 1.f + 0.000000000000000001f;
                std::cout << "Ratio must be greater than 1" << std::endl; // necessary? 
            }
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