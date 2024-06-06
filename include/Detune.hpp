#ifndef GIML_DETUNE_HPP
#define GIML_DETUNE_HPP
#include <math.h>
#include "Utility.hpp"
namespace giml {
    /**
     * @brief This class implements a basic time-domain pitchshifter 
     * @tparam T floating-point type for input and output sample data such as `float`, `double`, or `long double`,
     * up to user what precision they are looking for (float is more performant)
     */
    template <typename T>
    class Detune : public Effect<T> {
    private:
        int sampleRate;
        float pitchRatio = 1.f;
        float windowSize = 22.f;
        giml::CircularBuffer<T> buffer;
        giml::Phasor<T> osc;

    public:
        Detune() = delete;
        Detune(int sampleRate) : sampleRate(sampleRate), osc(sampleRate) {
            this->osc.setFrequency(1000.f * ((1.f - this->pitchRatio) / this->windowSize));
            this->buffer.allocate(100000); // max windowSize is samplesToMillis(100,000)
        }

        /**
         * @brief Writes and returns sample from delay line. Return is 100% wet
         * @param in current sample
         * @return past input value. Changes in temporal distance from current sample
         * create pitch-shifting via the doppler effect 
         */
        T processSample(T in) {
            this->buffer.writeSample(in); // write sample to delay buffer
            if (!(this->enabled)) {
                return in;
            }
            float readIndex =  this->osc.processSample() * millisToSamples(this->windowSize, this->sampleRate); // readpoint 1
            float readIndex2 = ::fmod(this->osc.getPhase() + 0.5f, 1) * millisToSamples(this->windowSize, this->sampleRate); // readpoint 2

            T output = this->buffer.readSample(readIndex); // get sample
            T output2 = this->buffer.readSample(readIndex2); // get sample 2

            T windowOne = ::cosf((this->osc.getPhase() - 0.5) * M_PI); // gain windowing
            T windowTwo = ::cosf((::fmod(this->osc.getPhase() + 0.5f, 1.f) - 0.5) * M_PI);// ^
            
            return output * windowOne + output2 * windowTwo; // windowed output
        }

        /**
         * @brief Set the pitch change ratio
         * @param ratio of desired pitch to input 
         */
        void setPitchRatio(float ratio) {
            this->pitchRatio = ratio;
            this->osc.setFrequency(1000.f * ((1.f - ratio) / this->windowSize));
        }

        /**
         * @brief set the maximum delay 
         * @param sizeMillis the max amount of milliseconds any sample
         * can be delayed by
         * @ TODO: clamp to `samplesToMillis(100,000)`
         */
        void setWindowSize(float sizeMillis) {
            this->windowSize = sizeMillis;
        }
    };
}
#endif