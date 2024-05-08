#ifndef GIMMEL_PHASER_HPP
#define GIMMEL_PHASER_HPP
#include <math.h>
#include "../utility.hpp"

// hmm.. it seems phasers are typically implemented as allpass filters 
// with cutoff modulated
namespace giml {
    template <typename T>
    class Phaser {
    private:
        int sampleRate;
        float speed, depth, blend;
        giml::JoelsCircularBuffer buffer;
        giml::SinOSc osc;

    public:
        T processSample(T in) {
            buffer.writeSample(input); // write sample to delay buffer
            int readIndex = round(depth + osc.processSample()) // calculate readIndex
            float output = buffer.readSample(readIndex); // get sample
          return output * 0.5 + in * 0.5; // return mix
        }

        void setSpeed(float millisPerCycle) {
            osc.setFrequency(millisToSamples(millisPerCycle, sampleRate) * sampleRate);
        }

        void setDepth(float d) {
            depth = d;
        }
    };
}
#endif