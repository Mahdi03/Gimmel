#ifndef GIMMEL_DETUNE_HPP
#define GIMMEL_DETUNE_HPP
#include <math.h>
#include "../utility.hpp"
namespace giml {
    template <typename T>
    class Detune {
    private:
        int sampleRate;
        const float twoPi = 2.f * static_cast<float>(M_PI);
        float pitchRatio = 1.f;
        float windowSize = 22.f; // switch to samples (int)
        giml::JoelsCircularBuffer buffer;
        giml::Phasor osc;

    public:
        T processSample(T in) {
            buffer.writeSample(input); // write sample to delay buffer

            int readIndex = static_cast<int>(round( // readpoint 1
                osc.processSample() * msToSamps(windowSize))); 
            int readIndex2 = static_cast<int>(round( // readpoint 2
                (fmod(osc.getPhase() + 0.5f, 1) *  msToSamps(windowSize)))); // fmod may be a problem

            float output = buffer.readSample(readIndex); // get sample
            float output2 = buffer.readSample(readIndex2); // get sample 2

            float windowOne = cosf((((phaseTap - 0.5f) / 2.f)) * twoPi); // gain windowing 
            float windowTwo = cosf(((fmod((phaseTap + 0.5f), 1.f) - 0.5f) / 2.f) * twoPi);// ^

            return output * windowOne + output2 * windowTwo; // windowed output
        }

        void setPitchRatio(float ratio) {
            pitchRatio = ratio;
            osc.setFrequency(fabs(1000.f * ((1.f - pitchRatio) / windowSize)));
        }

        void setWindowSize(float sizeMillis, int samprate) {
            windowSize = millisToSamples(sizeMillis, samprate);
        }
    };
}
#endif