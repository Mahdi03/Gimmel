#ifndef UTILITY_H
#define UTILITY_H
#include <math.h>
namespace giml {

    /**
    * @brief Converts dB value to linear amplitude,
    * the native format of audio samples
    *
    * @param dBVal input value in dB
    * @return input value in amplitude
    */
    float dBtoA(float dBVal) {
        return ::powf(10.f, dBVal / 20.f);
    }

    /**
    * @brief Converts linear amplitude to dB,
    * a measure of perceived loudness
    *
    * @param ampVal input value in linear amplitude
    * @return input value in dB
    */
    float aTodB(float ampVal) {
        return 20.f * ::log10f(::fabs(ampVal));
    }

    /**
    * @brief Converts a quantity of milliseconds to an
    * equivalent quantity of samples
    *
    * @param msVal input value in milliseconds
    * @param sampleRate samplerate of your project
    * @return msVal translated to samples
    */
    int millisToSamples(float msVal, int sampleRate) {
        return msVal * sampleRate / 1000.f;
    }

    /**
    * @brief Converts a quantity of samples to an
    * equivalent quantity of milliseconds
    *
    * @param numSamples input value in samples
    * @param sampleRate samplerate of your project
    * @return numSamples translated to milliseconds
    */
    float samplesToMillis(int numSamples, int sampleRate) {
        return numSamples / sampleRate * 1000.f;
    }

    /**
    * @brief Phase Accumulator / Unipolar Saw Oscillator
    */
    class Phasor {
    protected:
        int sampleRate;
        float phase, frequency, phaseIncrement;

    public:
        Phasor(int sampRate) : sampleRate(sampRate) {}
        ~Phasor() {}
        // Copy constructor
        Phasor(const Phasor& c) {
            this->sampleRate = c.sampleRate;
            this->phase = c.phase;
            this->frequency = c.frequency;
            this->phaseIncrement = c.phaseIncrement;
        }
        // Copy assignment constructor
        Phasor& operator=(const Phasor& c) {
            this->sampleRate = c.sampleRate;
            this->phase = c.phase;
            this->frequency = c.frequency;
            this->phaseIncrement = c.phaseIncrement;
            return *this;
        }

        virtual void setSampleRate(int samprate) {
            sampleRate = samprate;
            phaseIncrement = frequency / static_cast<float> (sampleRate);
        }

        virtual void setFrequency(float freq) {
            frequency = freq;
            phaseIncrement = abs(frequency) / static_cast<float> (sampleRate);
        }

        virtual float processSample() {
            phase += phaseIncrement; // increment phase
            if (phase >= 1.f) { // if waveform zenith...
                phase -= 1.f; // wrap phase
            }

            if (frequency < 0) { // if negative frequency...
                return 1.f - phase; // return reverse phasor
            }
            else {
                return phase; // return phasor
            }
        }

        virtual void setPhase(float ph) { // set phase manually 
            phase = ph;
        }

        virtual float getPhase() { // get phase without advancing readpoint
            return phase;
        }
    };

    /**
    * @brief Bipolar Sine Oscillator
    */
    class SinOsc : public Phasor {
    private:
        const float pi = static_cast<float>(M_PI);
        const float twoPi = pi * 2.f;

    public:
        SinOsc(int sampRate) : Phasor(sampRate) {}
        // ~SinOsc() {} // idk how these should look in a derived class
        // // Copy constructor
        // SinOsc(const SinOsc& c) {
        //     this->sampleRate = c.sampleRate;
        //     this->phase = c.phase;
        //     this->frequency = c.frequency;
        //     this->phaseIncrement = c.phaseIncrement;
        // }
        // // Copy assignment constructor
        // SinOsc& operator=(const SinOsc& c) {
        //     this->sampleRate = c.sampleRate;
        //     this->phase = c.phase;
        //     this->frequency = c.frequency;
        //     this->phaseIncrement = c.phaseIncrement;
        //     return *this;
        // }

        float processSample() override {
            phase += phaseIncrement; // increment phase
            if (phase >= 1.f) { // if waveform zenith...
                phase -= 1.f; // wrap phase
            }

            if (frequency < 0) { // if negative frequency...
                return sinf(twoPi * (1.f - phase)); // return reverse phasor
            }
            else {
                return sinf(twoPi * phase); // return phasor
            }
        }
    };

    class JoelsCircularBuffer { // simplest circular buffer I could think of 
    protected:
        static const int bufferSize = 1000000; // we can adjust this
        int maxIndex = bufferSize - 1;
        float buffer[bufferSize];
        int writeIndex;

    public:
        virtual void writeSample(float input) {
            buffer[writeIndex] = input;
            writeIndex += 1;

            if (writeIndex >= maxIndex) {
                writeIndex -= maxIndex; // circular logic 
            }
        }

        virtual float readSample (int delayInSamples) {
            if (delayInSamples > maxIndex) { // limit delay to maxIndex
                delayInSamples = maxIndex;
            }
            int readIndex = writeIndex - delayInSamples; // calculate readIndex
            if (readIndex < 0) {
                readIndex += bufferSize; // circular logic
            } 
          return buffer[readIndex]; 
        }
    };
}
#endif