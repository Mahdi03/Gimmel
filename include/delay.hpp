#ifndef GIML_DELAY_HPP
#define GIML_DELAY_HPP
#include <math.h>
#include "utility.hpp"
#include "filter.hpp"
namespace giml {
    /**
     * @brief This class implements a basic delay with feedback effect. 
     * @tparam T floating-point type for input and output sample data such as `float`, `double`, or `long double`,
     * up to user what precision they are looking for (float is more performant)
     */
    template <typename T>
    class Delay : public Effect<T> {
    private:
        int sampleRate;
        T feedback = 0, delayTime = 0, blend = 0.5;
        giml::onePole<T> loPass;
        giml::CircularBuffer<T> buffer;

    public:
        Delay() = delete;
        Delay(int samprate, T maxDelayMillis = 3000) : sampleRate(samprate) {
            this->buffer.allocate(giml::millisToSamples(maxDelayMillis, samprate)); // max delayTime = maxDelay
            this->loPass.setG(0.5);
        }
        
        /**
        * @brief Writes and returns sample from delay line. Return is 100% wet
        * @param in value to be written to delay line, after combined with feedback
        * @return sample from `delayTime` milliseconds ago
        */
        T processSample(T in) {
            if (!(this->enabled)) {
                return in;
            }

            T readIndex = millisToSamples(this->delayTime, this->sampleRate);
            T y_0 = loPass.lpf(in + this->buffer.readSample(readIndex) * this->feedback);
            this->buffer.writeSample(y_0); // write sample to delay buffer

          return giml::linMix<float>(in, y_0, this->blend);
        }

        /**
        * @brief Set feedback gain.  
        * @param fbGain gain in linear amplitude. Be careful setting above 1!
        */
        void setFeedback(T fbGain) {
            this->feedback = fbGain;
        }

        /**
        * @brief Set feedback gain based on a t60 time value  
        * @param timeMillis desired decay time in milliseconds
        */
        void setFeedback_t60(T timeMillis) {
            T normalizedDecay = millisToSamples(timeMillis, this->sampleRate) / 
            millisToSamples(this->delayTime, this->sampleRate);
            this->feedback = giml::t60<T>(static_cast<int>(::round(normalizedDecay)));
        }

        /**
        * @brief Set delay time
        * @param sizeMillis delay time in milliseconds. 
        * Clamped to `samplesToMillis(bufferSize)`
        */
        void setDelayTime(T sizeMillis) { 
            if (sizeMillis > samplesToMillis(buffer.size(), this->sampleRate)) {
                sizeMillis = samplesToMillis(buffer.size(), this->sampleRate);
            }
            this->delayTime = sizeMillis;
        }

        /**
        * @brief Set blend (linear)
        * @param gVal percentage of wet to blend in. Clamped to [0,1]
        */
        void setBlend(T gVal) { 
            if (gVal < 0) {gVal = 0;}
            if (gVal > 1) {gVal = 1;}
            this->blend = gVal;
        }
    };
}
#endif