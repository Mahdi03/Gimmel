#ifndef UTILITY_HPP
#define UTILITY_HPP
#define _USE_MATH_DEFINES
#include <math.h>
#define GIML_TWO_PI 6.28318530717958647692
#include <stdlib.h> // For malloc/calloc/free
#include <cstring> 
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
        if (ampVal == 0) {
            ampVal += 1e-6;
        }
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
    float millisToSamples(float msVal, int sampRate) {
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
        return numSamples / (float)sampRate * 1000.f;
    }

    /**
     * @brief Mixes two numbers with linear interpolation
     *
     * @param in1 input 1
     * @param in2 input 2
     * @param mix percentage of input 2 to mix in. Clamped to [0,1]
     * @return in1 * (1-mix) + in2 * mix
     */
    template <typename T>
    T linMix(T in1, T in2, T mix = 0) {
        if (mix < 0) {mix = 0;}
        if (mix > 1) {mix = 1;}
        return in1 * (1-mix) + in2 * mix;
    }

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

    template <typename T>
    class timer {
    protected:
        int n = 0;
        int N = 0;
        bool done = false;

    public:  
        void set(int bigN) {
            this->n = 0;
            this->N = bigN;
            this->done = false;
        }

        void tick() {
            if (done) {return;}
            n++;
            if (n == N) {this->done = true;}
        }

        bool isDone() {return done;}

        int timeS() {return n;}

        T timeU() {
            if (N == 0) {return static_cast<T>(0);} // Avoid division by zero
            if (done) {return 1;}
            return static_cast<T>(n) / static_cast<T>(N);
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
        void writeSample(T input) {
            this->pBackingArr[this->writeIndex] = input;
            this->writeIndex++;
            if (this->writeIndex >= this->bufferSize) {
                this->writeIndex = 0; // circular logic 
            }
        }

        /**
         * @brief Reads a sample from the buffer
         * @param delayInSamples access a sample this many samples ago
         * @return `buffer[writeIndex - delayInSamples]`
         */
        inline T readSample(size_t delayInSamples) const {
            if (delayInSamples >= this->bufferSize) { // limit delay to maxIndex
                delayInSamples = this->bufferSize - 1;
            }
            long long int readIndex = this->writeIndex - delayInSamples; // calculate readIndex
            if (readIndex < 0) {
                readIndex += this->bufferSize; // circular logic
            }
            return this->pBackingArr[readIndex];
        }

        inline T readSample(int delayInSamples) const {
            return this->readSample((size_t)(delayInSamples));
        }

        /**
         * @brief Reads a sample from the buffer using linear interpolation 
         * @param delayInSamples access a sample this many fractional samples ago
         * @return `interpolated sample from delayInSamples ago`
         */
        inline T readSample(float delayInSamples) const {
            size_t readIndex = delayInSamples; // sample 1
            size_t readIndex2 = readIndex + 1; // sample 2
            float frac = delayInSamples - readIndex; // proportion of sample 2 to blend in

            return  // do linear interpolation
                (this->readSample(readIndex) * (1.f - frac)) 
                + (this->readSample(readIndex2) * frac); 
        }

        inline T readSample(double delayInSamples) const {
            return this->readSample((float)delayInSamples);
        }

        size_t size() const {
            return this->bufferSize;
        }
    };

    /**
     * @brief DynamicArray implementation for when we need small resizable arrays
     */
    template <typename T>
    class DynamicArray {
    private:
        T* pBackingArr;
        size_t length, initialCapacity, totalCapacity;

        void resize(size_t newCapacity) {
            T* newSpace = (T*)::realloc(this->pBackingArr, newCapacity * sizeof(T));
            if (newCapacity > this->totalCapacity) {
                //Then we need to 0-initialize the rest of the new space
                ::memset((void*)(newSpace + this->totalCapacity), 0, (newCapacity - this->totalCapacity) * sizeof(T));
            }
            this->pBackingArr = newSpace;
            this->totalCapacity = newCapacity;
        }

    public:
        //Constructor
        DynamicArray(size_t initialCapacity = 4) {
            this->pBackingArr = (T*)::calloc(initialCapacity, sizeof(T)); //Needs to be calloc so that the data is zero-ed out
            this->initialCapacity = initialCapacity;
            this->totalCapacity = initialCapacity;
            this->length = 0;
        }

        //Copy constructor
        DynamicArray(const DynamicArray& d) {
            this->pBackingArr = (T*)::malloc(d.totalCapacity * sizeof(T));
            this->initialCapacity = d.initialCapacity;
            this->totalCapacity = d.totalCapacity;
            this->length = d.length;
            //Deep copy over all values
            for (int i = 0; i < d.length; i++) {
                this->pBackingArr[i] = d.pBackingArr[i];
            }
        }
        //Copy assignment operator
        DynamicArray& operator=(const DynamicArray& d) {
            this->pBackingArr = (T*)::malloc(d.totalCapacity * sizeof(T));
            this->initialCapacity = d.initialCapacity;
            this->totalCapacity = d.totalCapacity;
            this->length = d.length;
            //Deep copy over all values
            for (int i = 0; i < d.length; i++) {
                this->pBackingArr[i] = d.pBackingArr[i];
            }

            return *this;
        }
        //Destructor
        ~DynamicArray() {
            for (size_t i = 0; i < this->length; i++) {
                this->pBackingArr[i].~T(); //Make sure to call the destructor if the object needs to be cleaned up
            }
            ::free(this->pBackingArr);
        }

        size_t size() const {
            return this->length;
        }

        size_t getCapacity() const {
            return this->totalCapacity;
        }

        void pushBack(const T& val) {
            if (this->length == this->totalCapacity) {
                this->resize(this->totalCapacity * 1.5); //Apparently STL lib uses 1.5 as their resize factor for vector
            }
            this->pBackingArr[this->length++] = val;
        }

        void removeAt(size_t indexToRemove) {
            if (indexToRemove >= this->length || indexToRemove < 0) {
                std::cout << "Array access out of bounds" << std::endl;
                throw std::out_of_range("Index out of range");
            }
            for (size_t i = indexToRemove; i < this->length - 1; ++i) {
                this->pBackingArr[i] = this->pBackingArr[i + 1]; //Shift all elements up by 1
            }
            this->length--;

            //Reclaim any unused space if needed
            if (this->length < this->totalCapacity / 2 && this->totalCapacity > 2 * this->initialCapacity) {
                this->resize(this->totalCapacity / 2);
            }
        }

        T popBack() { //Removes & returns the last element in the dynamic array
            if (this->length > 0) {
                T returnVal = (*this)[this->length - 1];
                this->removeAt(this->length - 1);
                return returnVal;
            }
            else {
                std::cout << "Array is already empty!" << std::endl;
            }
        }

        //Array access operators
        T& operator[](size_t index) {
            if (index >= this->length || index < 0) {
                std::cout << "Array access out of bounds" << std::endl;
                throw std::out_of_range("Index out of range");
            }
            return this->pBackingArr[index];
        }
        const T& operator[](size_t index) const {
            if (index >= this->length || index < 0) {
                std::cout << "Array access out of bounds" << std::endl;
                throw std::out_of_range("Index out of range");
            }
            return this->pBackingArr[index];
        }

        //Iterator operators to support range-based for loop syntax
        T* begin() {
            return this->pBackingArr;
        }

        const T* begin() const {
            return this->pBackingArr;
        }

        T* end() {
            return this->pBackingArr + this->length;
        }

        const T* end() const {
            return this->pBackingArr + this->length;
        }
    };


    /**
     * @brief Linked List implementation, handy for effects that require a delay line
     */
    template <typename T>
    class LinkedList {
    private:
        struct Node {
            T value;
            Node* next;
        };
        Node* head = nullptr;
        Node* tail = nullptr;
        size_t length = 0;

        void freeUpRestOfList(Node* startingNode) {
            //Delete the rest including the `startingNode`
            Node* currNode = startingNode;
            Node* tempNodeToDelete = nullptr;
            while (currNode != this->head) {
                tempNodeToDelete = currNode;
                currNode = currNode->next;
                ::free(tempNodeToDelete);
            }
            //startingNode->next = this->head;
        }
    public:
        //Constructor
        LinkedList() {}
        // Copy constructor
        LinkedList(const giml::LinkedList<T>& l) {
            if (l.length != 0) {
                //Then we have elements to deep copy over
                this->head = (Node*)malloc(sizeof(Node));
                this->head->value = l.head->value; //Hopefully this is a deep copy
                this->head->next = nullptr;
                this->length = l.length;

                Node* pCurrNode = this->head, pTheirNode = l.head->next;
                while (pTheirNode) {
                    Node* newNode = (Node*)malloc(sizeof(Node));
                    newNode->value = pTheirNode->value;
                    newNode->next = nullptr;

                    //Link to previous node and continue
                    pCurrNode->next = newNode;
                    pCurrNode = pCurrNode->next;
                    pTheirNode = pTheirNode->next;
                }
                this->tail = pCurrNode; //The last node should be the tail
            }
            else {
                this->head = nullptr;
                this->tail = nullptr;
                this->length = 0;
            }
        }
        // Copy assignment operator
        LinkedList<T>& operator=(const giml::LinkedList<T>& l) {

        }
        //Destructor
        ~LinkedList() {
            // Clean up entire LinkedList
            freeUpRestOfList(this->head);
            //free(this->head);
        }


        size_t size() {
            return this->length;
        }
    };
    // template <typename T>
    // struct isFloatingPoint {
    //     static const bool val = false;
    // };

    // template <>
    // struct isFloatingPoint<float> {
    //     static const bool val = true;
    // };
    // template <>
    // struct isFloatingPoint<double> {
    //     static const bool val = true;
    // };
    // template <>
    // struct isFloatingPoint<long double> {
    //     static const bool val = true;
    // };
}
#endif