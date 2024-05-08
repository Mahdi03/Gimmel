#ifndef GIMMEL_TREMOLO_HPP
#define GIMMEL_TREMOLO_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Tremolo {
    private:
        int sampleRate;
        float speed, depth; // if speed set at audio rate, ring modulation. If depth > 1, phase distortion 
        giml::SinOsc osc;

    public:
        T processSample(T in) { 
            float gain = osc.processSample() * 0.5 + 0.5 // waveshape SinOsc output to make it unipolar
            gain *= depth; // scale by depth
          return in * (1 - gain); // return in * waveshaped SinOsc 
        }

        void setSpeed(float millisPerCycle) { // set speed of LFO
            osc.setFrequency(millisPerCycle * 0.001f); // convert to Hz (milliseconds to seconds)
        }

        void setDepth(float d) { // set depth
            if (d < 0.f) {depth = 0.f;}
            else if (d > 1) {depth = 1.f;}
            else {depth = d;}
        }
    };
}
#endif