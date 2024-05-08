#ifndef GIMMEL_CHORUS_HPP
#define GIMMEL_CHORUS_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Chorus {
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
          return output * blend + in * (1-blend); // return mix
        }

        void setSpeed(float millisPerCycle) {
            osc.setFrequency(millisPerCycle * 0.001f); // set frequency in Hz
        }

        void setDepth(float d) {
            depth = d;
        }
    };
}
#endif