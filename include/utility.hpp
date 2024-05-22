#ifndef UTILITY_HPP
#define UTILITY_HPP

#define _USE_MATH_DEFINES //For M_PI... do we need this?
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
     * @param sampleRate sample rate of your project
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
     * @brief Phase Accumulator / Unipolar Saw Oscillator. 
     * Can be used as a control signal and/or waveshaped into other waveforms. 
     * Will cause aliasing if sonified 
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

        /**
         * @brief Sets the oscillator's sample rate 
         * @param sampRate sample rate of your project
         */
        virtual void setSampleRate(int sampRate) {
            sampleRate = sampRate;
            phaseIncrement = frequency / static_cast<float> (sampleRate);
        }

        /**
         * @brief Sets the oscillator's frequency
         * @param freqHz frequency in hertz (cycles per second)
         */
        virtual void setFrequency(float freqHz) {
            frequency = freqHz;
            phaseIncrement = abs(frequency) / static_cast<float> (sampleRate);
        }

        /**
         * @brief Increments and returns `phase` 
         * @return `phase` (after increment)
         */
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

        /**
         * @brief Sets `phase` manually 
         * @param ph User-defined phase 
         * (will be wrapped to the range [0,1] by processSample()) 
         */
        virtual void setPhase(float ph) { // set phase manually 
            phase = ph;
        }
        
        /**
         * @brief Returns `phase` without incrementing. 
         * If `frequency` is negative, returns `1 - phase`
         * @return `phase` 
         */
        virtual float getPhase() {
            if (this->frequency < 0) { // if negative frequency...
                return 1.f - this->phase; // return reverse phasor
            }
            else {
                return this->phase; // return phasor
            }
        }
    };

    /**
     * @brief Bipolar Sine Oscillator that inherits from `giml::phasor`. 
     * Implemented as an ideal unipolar saw wave waveshaped with `sinf`
     */
    class SinOsc : public Phasor {
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
        
        /**
         * @brief Increments and returns `phase` 
         * @return `sinf(phase)` (after increment)
         */
        float processSample() override {
            phase += phaseIncrement; // increment phase
            if (phase >= 1.f) { // if waveform zenith...
                phase -= 1.f; // wrap phase
            }

            if (frequency < 0) { // if negative frequency...
                return ::sinf(GIML_TWO_PI * (1.f - this->phase)); // return reverse phasor
            } 
            else {
                return ::sinf(GIML_TWO_PI * this->phase); // return phasor
            }
        }
    };

    /**
     * @brief Bipolar Ideal Triangle Oscillator that inherits from `giml::phasor`
     * Best used as a control signal, will cause aliasing if sonified 
     */
    class TriOsc : public Phasor {
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

        /**
         * @brief Increments and returns `phase` 
         * @return Waveshaped `phase` (after increment)
         */
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


    /**
     * @brief Circular buffer implementation, handy for effects that require a delay line
     */
    template <typename T>
    class CircularBuffer {
    private:
        T* pBackingArr = nullptr;
        size_t bufferSize = 0;
        size_t writeIndex = 0;

    public:
        /**
         * @brief function that allocates an array of `size` indices
         * @param size in a delay line, the number of past samples stored
         */
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

        /**
         * @brief Writes a new sample to the buffer
         * @param input sample value
         */
        void writeSample(float input) {
            this->pBackingArr[writeIndex] = input;
            writeIndex++;
            if (writeIndex >= this->bufferSize) {
                writeIndex = 0; // circular logic 
            }
        }

        /**
         * @brief Reads a sample from the buffer
         * @param delayInSamples access a sample this many samples ago
         * @return `buffer[writeIndex - delayInSamples]`
         */
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

    /**
     * @brief Effect class that implements a bypass switch (enabled by default)
     */
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