#ifndef GIML_DETUNE_HPP
#define GIML_DETUNE_HPP
#include <math.h>
#include "utility.hpp"
#include "oscillator.hpp"
namespace giml {
    /**
     * @brief This class implements a time-domain pitchshifter 
     * @tparam T floating-point type for input and output sample data such as `float`, `double`, or `long double`,
     * up to user what precision they are looking for (float is more performant)
     */
    template <typename T>
    class Detune : public Effect<T> {
    private:
        int sampleRate;
        float pitchRatio = 1.f, windowSize = 22.f; 
        giml::CircularBuffer<T> buffer;
        giml::Phasor<T> osc;

    public:
        Detune() = delete;
        Detune(int samprate, float maxWindowMillis = 300.f) : sampleRate(samprate), osc(samprate) {
            this->osc.setFrequency(1000.f * ((1.f - this->pitchRatio) / this->windowSize));
            this->buffer.allocate(giml::millisToSamples(maxWindowMillis, samprate));
        }

        /**
         * @brief Writes and returns samples from delay line.
         * @param in current sample
         * @return past input value. Changes in temporal distance from current sample
         * create pitch-shifting via the doppler effect 
         */
        T processSample(T in) {
            this->buffer.writeSample(in); // write sample to delay buffer

            if (!(this->enabled)) {
                return in;
            }
            T phase = this->osc.processSample();
            float readIndex = phase * millisToSamples(this->windowSize, this->sampleRate); // readpoint 1
            float readIndex2 = ::fmod(phase + 0.5f, 1) * millisToSamples(this->windowSize, this->sampleRate); // readpoint 2

            T output = this->buffer.readSample(readIndex); // get sample
            T output2 = this->buffer.readSample(readIndex2); // get sample 2

            T windowOne = ::cosf((phase - 0.5f) * M_PI); // gain windowing
            T windowTwo = ::cosf((::fmod(phase + 0.5f, 1.f) - 0.5f) * M_PI);// ^
            
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
         * @brief set the maximum delay (windowSize)
         * @param sizeMillis the max amount of delay in milliseconds 
         * when `osc` is at its peak
         */
        void setWindowSize(float sizeMillis) {
            if (sizeMillis > giml::samplesToMillis(this->buffer.size(), this->sampleRate)) {
                sizeMillis = giml::samplesToMillis(this->buffer.size(), this->sampleRate);
            }
            this->windowSize = sizeMillis;
        }
    };
}
#endif