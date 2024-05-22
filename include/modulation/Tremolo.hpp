#ifndef GIMMEL_TREMOLO_HPP
#define GIMMEL_TREMOLO_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    /**
     * @brief This class implements a basic tremolo effect 
     * @tparam T floating-point type for input and output sample data such as `float`, `double`, or `long double`,
     * up to user what precision they are looking for (float is more performant)
     */
    template <typename T>
    class Tremolo : public Effect<T> {
    private:
        int sampleRate;
        float speed = 1000.f, depth = 1.f; // if speed set at audio rate, ring modulation 
        giml::SinOsc osc;

    public:
        Tremolo() = delete;
        Tremolo (int samprate) : sampleRate(samprate), osc(samprate) {
            this->osc.setFrequency(1000.f / this->speed);
        }

        /**
         * @brief Advances oscillator and returns an enveloped version of the input
         * @param in current sample
         * @return `in` enveloped by `osc`
         */
        T processSample(T in) {
            if (!(this->enabled)) {
                return in;
            }
            float gain = this->osc.processSample() * 0.5 + 0.5; // waveshape SinOsc output to make it unipolar
            gain *= this->depth; // scale by depth
            return in * (1 - gain); // return in * waveshaped SinOsc 
        }

        /**
         * @brief sets the rate of `osc`
         * @param millisPerCycle desired modulation frequency in milliseconds
         */
        void setSpeed(float millisPerCycle) { // set speed of LFO
            if (millisPerCycle < 0.05) {millisPerCycle = 0.05;} // osc frequency ceiling at 20kHz to avoid aliasing
            this->speed = millisPerCycle;
            this->osc.setFrequency(1000.f / this->speed); // convert to Hz (milliseconds to seconds)
        }

        /**
         * @brief sets the amount of gain reduction when `osc` is at its peak
         * @param d modulation depth (clamped to [0,1])
         */
        void setDepth(float d) { // set depth
            if (d < 0.f) {this->depth = 0.f;}
            else if (d > 1) {this->depth = 1.f;}
            else {this->depth = d;}
        }
    };
}
#endif