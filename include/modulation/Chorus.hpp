#ifndef GIMMEL_CHORUS_HPP
#define GIMMEL_CHORUS_HPP
#include <math.h>
#include "../utility.hpp"
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
        giml::CircularBuffer<float> buffer;
        giml::TriOsc osc;

    public:
        Chorus() = delete;
        Chorus (int samprate) : sampleRate(samprate), osc(samprate) {
            this->osc.setFrequency(this->rate);
            this->buffer.allocate(100000); // max delay is 100,000 samples
        }

        /**
         * @brief Writes and returns sample from delay line. 
         * Return is blended with `in`
         * @param in current sample
         * @return `in` blended with past input. Changes in temporal distance 
         * from current sample create pitch-shifting via the doppler effect 
         * TODO: encapsulate sample interpolation logic in utility.hpp
         */
        T processSample(T in) {
            this->buffer.writeSample(in); // write sample to delay buffer

            if (!(this->enabled)) {
                return in;
            }

            float idealReadIndex = millisToSamples(this->depth, this->sampleRate) + 
            (millisToSamples(this->depth, this->sampleRate) * 0.5) * this->osc.processSample();

            int readIndex = int(idealReadIndex); // calculate readIndex
            
            int readIndex2 = readIndex + 1;

            float frac = idealReadIndex - readIndex;

            float wet = (this->buffer.readSample(readIndex) * (1 - frac)) + (this->buffer.readSample(readIndex2) * frac); // get sample
            //float wet = this->buffer.readInterpSample(idealReadIndex);
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
         * @param freq frequency in Hz 
         */
        void setDepth(float d) {
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