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
            this->pBackingArr = (T*)calloc(this->bufferSize, sizeof(T)); //zero-fill values
        }
        //Constructor
        CircularBuffer() {}
        //Copy Contructor
        CircularBuffer(const CircularBuffer& c) {
            //There is no previous object, this object is being created new
            //We need to deep copy over the entire array
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

    /*
    / <summary>
    / To use this class, first instantiate it, and then call `.allocate()` with a given size
    / </summary>
    / <typeparam name="T">type of data to store</typeparam>
    */
     //template <typename T>
     //class CircularBuffer {
     //private:
     //    T* pBackingArr = nullptr;
     //    size_t currIndex = 0;
     //protected:
     //    size_t length = 0;
     //   
     //    inline void incrementIndex() {
     //        if (this->currIndex >= this->length - 1) {
     //            this->currIndex = 0;
     //        }
     //        else {
     //            this->currIndex++;
     //        }
     //    }
     //    inline void decrementIndex() {
     //        if (this->currIndex <= 0) {
     //            this->currIndex = length - 1;
     //        }
     //        else {
     //            this->currIndex--;
     //        }
     //    }
     //    inline void insertValueAt(int index, const T& f) {
     //        pBackingArr[index] = f;
     //    }
     //public:
     //    //Constructor
     //    CircularBuffer() {}
     //    CircularBuffer(bool forwardsDirection) : insertionDirection(forwardsDirection) {}


     //    void allocate(size_t size) {
     //        this->length = size;
     //        this->pBackingArr = (T*)malloc(this->length * sizeof(T));
     //        if (!(this->pBackingArr)) {
     //            //Could not allocate a size this large in heap
     //        }
     //    }
     //    ~CircularBuffer() {
     //        if (this->pBackingArr) {
     //            free(this->pBackingArr);
     //        }
     //    }
     //    inline virtual void insertValue(const T& f) {
     //        // We want to store these values in an order that makes convolving easier
     //        this->insertValueAt(currIndex, f);
     //        this->decrementIndex();
     //    }
     //    //inline virtual T readNextValue() {
     //    //    //This function will decrement the array pointer and return the very next value in the array
     //    //    T returnVal = this->at(currIndex);
     //    //    this->incrementIndex();
     //    //    return returnVal;
     //    //}
     //    inline T at(const size_t& index) const {
     //        return this->pBackingArr[index];
     //    }
     //    inline size_t size() const {
     //        return this->length;
     //    }
     //};
    
     //template <typename T>
     //class CircularBufferForOscilloscope : public CircularBuffer<T> {
     //    /*
     //    For this class we want to insert a value, and then read all the values up to the last value which is the newly inserted value

     //    */
     //private:
     //    int currWriteIndex = 0, currReadIndex = 0;

     //    inline void incrementWriteIndex() {
     //        if (this->currWriteIndex >= this->length - 1) {
     //            this->currWriteIndex = 0;
     //        }
     //        else {
     //            this->currWriteIndex++;
     //        }
     //    }
     //    inline void decrementWriteIndex() {
     //        if (this->currWriteIndex <= 0) {
     //            this->currWriteIndex = length - 1;
     //        }
     //        else {
     //            this->currWriteIndex--;
     //        }
     //    }

     //    inline void incrementReadIndex() {
     //        if (this->currReadIndex >= this->length - 1) {
     //            this->currReadIndex = 0;
     //        }
     //        else {
     //            this->currReadIndex++;
     //        }
     //    }
     //    inline void decrementReadIndex() {
     //        if (this->currReadIndex <= 0) {
     //            this->currReadIndex = length - 1;
     //        }
     //        else {
     //            this->currReadIndex--;
     //        }
     //    }

     //public:
     //    CircularBufferForOscilloscope() {
     //        this->currReadIndex = this->currWriteIndex + 1; //Start it off by 1 since the current read index
     //    }
     //    ~CircularBufferForOscilloscope() {}

     //    inline void writeSample(const T& f) {
     //        this->insertValueAt(this->currWriteIndex, f);
     //        this->incrementWriteIndex();
     //    }

     //    inline void resetReadHeadIndex() {
     //        //Set the read index to the next place after where the previous value has been written to such that if we cycle around a full length, we will end up at the last value that has been inputted right now
     //        this->currReadIndex = this->currWriteIndex;
     //    }

     //    inline T readNextValue() {
     //        T returnVal = this->at(currReadIndex);
     //        this->incrementReadIndex();
     //        return returnVal;
     //    }


     //};

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