#ifndef GIMMEL_SATURATION_HPP
#define GIMMEL_SATURATION_HPP

#include <math.h>
#include "../utility.hpp"

namespace giml {
    template <typename T>
    class Saturation : public Effect<T> {
    private:
        float drive = 1.f;
        

    public:
        Saturation() {}
        ~Saturation() {}
        //Copy constructor
        Saturation(const Saturation& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->gain = c.gain;
        }
        //Copy assignment constructor
        Saturation& operator=(const Saturation& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->gain = c.gain;
            return *this;
        }

        inline T processSample(const T& in) override {
            if (!(this->enabled)) {
                return in;
            }

            // waveshaping functions
            float x = in;
            float y = x;

            // Araya & Suyama 1996
            // y = ( ((3.f * x) / 2) * (1 - (::powf(x, 2.f) / 3.f)) );

            // Doidec et al. 1998(symmetric)
            // if (x > 0) { // if sign is +
            //     y = (2.f * ::fabs(x) - ::powf(x, 2.f)); 
            // }
            // else { // if sign is -
            //     y = (2.f * ::fabs(x) - ::powf(x, 2.f)) * -1.f;
            // }

            // Doidec et al. 1998 (asymmetric)
            // if (x < -0.08905f) {
            //     y = -0.75 * (
            //         1.f - 
            //         powf(1.f - (::fabs(x) - 0.032847), 12.f) + 
            //         (1.f / 3.f) * (::fabs(x) - 0.032847)
            //     ) + 0.01f;
            // } 
            // else if (x < 0.320018f) {
            //     y = -6.153f * ::powf(x, 2) + 3.9375f * x; 
            // }
            // else {
            //     y = 0.630035f;
            // }
            
            // symmetrical distortion with tanh
            y = tanhf(this->drive * x) / tanhf(this->drive);

            // asymmetrical distortion with 
            // if (x >= 0) { // if x positive 
            //     y = tanhf(1.5f * this->satCoef * x) / tanhf(1.5f * this->satCoef);
            // }
            // else { // if y negative 
            //     y = tanhf(this->satCoef * x) / tanhf(this->satCoef);
            // }

            // oversampling ?
          return y;
        }

        void setGain (float g) {
            this->gain = g;
        }

        void setVolume (float v) {
            this->volume = v;
        }

        void setDrive (float d) {
            if (d <= 0.f) {
                d = 0.000000000000000001f;
                std::cout << "Drive set to pseudo-zero value, supply a positive float" << std::endl;
            }
            this->drive = d;
        }
    };
}
#endif