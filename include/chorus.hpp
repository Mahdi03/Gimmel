#ifndef GIML_CHORUS_HPP
#define GIML_CHORUS_HPP
#include <math.h>
#include "utility.hpp"
namespace giml {
    /**
     * @brief This class implements a basic chorus effect
     * @tparam T floating-point type for input and output sample data such as `float`, `double`, or `long double`,
     * up to user what precision they are looking for (float is more performant)
     */
    template <typename T>
    class Chorus : public Effect<T> {
    private:
        int sampleRate;
        float rate = 1.f, depth = 20.f, blend = 0.5f;
        giml::CircularBuffer<T> buffer;
        giml::TriOsc<T> osc;

    public:
        Chorus() = delete;
        Chorus (int samprate, float maxDepthMillis = 150.f) : sampleRate(samprate), osc(samprate) {
            this->osc.setFrequency(this->rate);
            this->buffer.allocate(giml::millisToSamples(maxDepthMillis * 2.f, samprate)); // max delay is 100,000 samples
        }

        /**
         * @brief Writes and returns sample from delay line. 
         * Return is blended with `in`
         * @param in current sample
         * @return `in` blended with past input. Changes in temporal distance 
         * from current sample create pitch-shifting via the doppler effect 
         */
        T processSample(T in) {
            this->buffer.writeSample(in); // write sample to delay buffer

            if (!(this->enabled)) {
                return in;
            }

            float idealReadIndex = millisToSamples(this->depth, this->sampleRate) + 
            (millisToSamples(this->depth, this->sampleRate) * 0.5) * this->osc.processSample();

            T wet = this->buffer.readSample(idealReadIndex);
            return wet * blend + in * (1-blend); // return mix
        }

        /**
         * @brief Set modulation rate- the frequency of the LFO.  
         * @param freq frequency in Hz 
         */
        void setRate(float freq) {
            this->osc.setFrequency(freq); // set frequency in Hz
        }

        /**
         * @brief Set modulation depth- the average delay time
         * @param d depth in milliseconds
         */
        void setDepth(float d) {
            if (giml::millisToSamples(d * 2.f, this->sampleRate) > this->buffer.size()) {
                d = giml::samplesToMillis(this->buffer.size(), this->sampleRate) / 2.f;
            }
            this->depth = d;
        }

        /**
         * @brief Set blend 
         * @param b ratio of wet to dry (clamped to [0,1])
         */
        void setBlend(float b) {
            // clamp b to [0, 1]
            if (b > 1) {b = 1.f;} 
            else if (b < 0) {b = 0.f;}
            // set blend
            this->blend = b;
        }
    };
}
#endif