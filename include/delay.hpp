#ifndef GIML_DELAY_HPP
#define GIML_DELAY_HPP
#include <math.h>
#include "utility.hpp"
namespace giml {
    /**
     * @brief This class implements a basic delay with feedback effect
     * @tparam T floating-point type for input and output sample data such as `float`, `double`, or `long double`,
     * up to user what precision they are looking for (float is more performant)
     */
    template <typename T>
    class Delay : public Effect<T> {
    private:
        int sampleRate;
        float feedback = 0.f, delayTime = 0.f;
        giml::CircularBuffer<float> buffer;

    public:
        Delay() = delete;
        Delay(int samprate, float maxDelayMillis = 3000.f) : sampleRate(samprate) {
            this->buffer.allocate(giml::millisToSamples(maxDelayMillis, samprate)); // max delayTime = maxDelay
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

            int readIndex = ::round(millisToSamples(this->delayTime, this->sampleRate));
            float output = this->buffer.readSample(readIndex); 
            this->buffer.writeSample(in + this->feedback * output); // write sample to delay buffer

          return output;
        }

        /**
        * @brief Set feedback gain.  
        * @param fbGain gain in linear amplitude. Be careful setting above 1!
        */
        void setFeedback(float fbGain) {
            this->feedback = fbGain;
        }

        /**
        * @brief Set delay time
        * @param sizeMillis delay time in milliseconds. 
        * Clamped to `samplesToMillis(bufferSize)`
        */
        void setDelayTime(float sizeMillis) { // confirm with Mahdi
            if (sizeMillis > samplesToMillis(buffer.size(), this->sampleRate)) {
                sizeMillis = samplesToMillis(buffer.size(), this->sampleRate);
            }
            this->delayTime = sizeMillis;
        }
    };
}
#endif