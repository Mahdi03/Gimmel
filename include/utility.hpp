#ifndef UTILITY_HPP
#define UTILITY_HPP

#define _USE_MATH_DEFINES //For M_PI
#include <math.h>
#define GIML_TWO_PI 6.28318530717958647692

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
    int millisToSamples(float msVal, int sampRate) {
        return msVal * sampRate / 1000.f;
    }

    /**
    * @brief Converts a quantity of samples to an
    * equivalent quantity of milliseconds
    *
    * @param numSamples input value in samples
    * @param sampleRate samplerate of your project
    * @return numSamples translated to milliseconds
    */
    float samplesToMillis(int numSamples, int sampRate) {
        return numSamples / sampRate * 1000.f;
    }

    /**
    * @brief Phase Accumulator / Unipolar Saw Oscillator
    */
    class Phasor {
    protected:
        int sampleRate;
        float phase = 0.f, frequency = 0.f, phaseIncrement = 0.f;

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

        virtual void setSampleRate(int sampRate) {
            sampleRate = sampRate;
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
            if (frequency < 0) { // if negative frequency...
                return 1.f - phase; // return reverse phasor
            }
            else {
                return phase; // return phasor
            }
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

        /**
    * @brief Bipolar Ideal Triangle Oscillator
    */
    class TriOsc : public Phasor {
    private:
        const float pi = static_cast<float>(M_PI);
        const float twoPi = pi * 2.f;

    public:
        TriOsc(int sampRate) : Phasor(sampRate) {}
        // ~TriOsc() {} // idk how these should look in a derived class
        // // Copy constructor
        // SinOsc(const TriOsc& c) {
        //     this->sampleRate = c.sampleRate;
        //     this->phase = c.phase;
        //     this->frequency = c.frequency;
        //     this->phaseIncrement = c.phaseIncrement;
        // }
        // // Copy assignment constructor
        // TriOsc& operator=(const TriOsc& c) {
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
                return fabs((1 - phase) * 2 - 1) * 2 - 1; // return reverse phasor
            }
            else {
                return fabs(phase * 2 - 1) * 2 - 1; // return phasor
            }
        }
    };

    template <typename T>
    class CircularBuffer {
    private:
        T* pBackingArr = nullptr;
        size_t bufferSize = 0;
        size_t writeIndex = 0;

    public:
        void allocate(size_t size) {
            if (this->pBackingArr) {
                free(this->pBackingArr);
            }
            this->bufferSize = size;
            this->pBackingArr = (T*)calloc(this->bufferSize, sizeof(T)); // zero-fill values
        }
        //Constructor
        CircularBuffer() {}

        //Copy Contructor
        CircularBuffer(const CircularBuffer& c) {
            // There is no previous object, this object is being created new
            // We need to deep copy over the entire array
            this->bufferSize = c.bufferSize;
            this->pBackingArr = (T*)calloc(bufferSize, sizeof(T));
            for (size_t i = 0; i < this->bufferSize; i++) {
                this->pBackingArr[i] = c.pBackingArr[i];
            }
        }
        
        // Copy assignment constructor
        CircularBuffer& operator=(const CircularBuffer& c) {
            //There is a previous object here so first we need to free the previous buffer
            if (this->pBackingArr) {
                free(this->pBackingArr);
            }
            this->bufferSize = c.bufferSize;
            this->pBackingArr = (T*)calloc(bufferSize, sizeof(T));
            for (size_t i = 0; i < this->bufferSize; i++) {
                this->pBackingArr[i] = c.pBackingArr[i];
            }
          return *this;
        }
        ~CircularBuffer() {
            if (this->pBackingArr) {
                free(pBackingArr);
            }
        }

        void writeSample(float input) {
            this->pBackingArr[writeIndex] = input;
            writeIndex++;
            if (writeIndex >= this->bufferSize) {
                writeIndex = 0; // circular logic 
            }
        }

        float readSample(size_t delayInSamples) {
            if (delayInSamples >= this->bufferSize) { // limit delay to maxIndex
                delayInSamples = this->bufferSize - 1;
            }
            int readIndex = writeIndex - delayInSamples; // calculate readIndex
            if (readIndex < 0) {
                readIndex += bufferSize; // circular logic
            }
          return this->pBackingArr[readIndex];
        }
    };

     template <typename T>
     class Effect {
     public:
         Effect() {}
         virtual ~Effect() {}
         virtual void enable() {
             this->enabled = true;
         }

         virtual void disable() {
             this->enabled = false;
         }

         virtual inline T processSample(const T& in) {
             return in;
         }

     protected:
         bool enabled = false;
     };
}
#endif