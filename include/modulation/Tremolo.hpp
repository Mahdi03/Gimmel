#ifndef GIMMEL_TREMOLO_HPP
#define GIMMEL_TREMOLO_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Tremolo : public Effect<T> {
    private:
        int sampleRate;
        float speed = 1.f, depth = 1.f; // if speed set at audio rate, ring modulation 
        giml::SinOsc osc;

    public:
        Tremolo (int samprate) : sampleRate(samprate), osc(samprate) {}

        T processSample(T in) { 
            float gain = osc.processSample() * 0.5 + 0.5; // waveshape SinOsc output to make it unipolar
            gain *= depth; // scale by depth
          return in * (1 - gain); // return in * waveshaped SinOsc 
        }

        void setSpeed(float millisPerCycle) { // set speed of LFO
            if (millisPerCycle <= 0.05) {millisPerCycle = 0.05;} // osc frequency cieling at 20kHz to avoid aliasing
            osc.setFrequency(1000.f / millisPerCycle); // convert to Hz (milliseconds to seconds)
        }

        void setDepth(float d) { // set depth
            if (d < 0.f) {depth = 0.f;}
            else if (d > 1) {depth = 1.f;}
            else {depth = d;}
        }
    };
}
#endif