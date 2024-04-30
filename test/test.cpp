
#include "al/graphics/al_Mesh.hpp"
#include <iostream>


template <typename T>
class CircularBuffer {
private:
	size_t length, currIndex = 0;
	T* pBackingArr = nullptr;
	bool insertionDirection = false; //True if we want to insert forwards
	inline void incrementIndex();
	inline void decrementIndex();
public:
	CircularBuffer() {}
	CircularBuffer(bool forwardsDirection) : insertionDirection(forwardsDirection) {
		
	}
	void allocate(size_t size);
	~CircularBuffer();
	inline void insertValue(const T& f);
	inline T readNextValue();
	inline T at(const size_t &index) const;
	inline size_t size() const;
	inline size_t getCurrIndex() const;
};


template <typename T>
inline void CircularBuffer<T>::incrementIndex() {
	if (this->currIndex >= this->length - 1) {
		currIndex = 0;
	}
	else {
		currIndex++;
	}
}

template <typename T>
inline void CircularBuffer<T>::decrementIndex() {
	if (this->currIndex <= 0) {
		currIndex = length - 1;
	}
	else {
		currIndex--;
	}
}


template <typename T>
void CircularBuffer<T>::allocate(size_t size) {
	this->length = size;
	this->pBackingArr = (T*)malloc(this->length * sizeof(T));
	if (!(this->pBackingArr)) {
		//Could not allocate a size this large in heap
	}
}

template <typename T>
CircularBuffer<T>::~CircularBuffer() {
	if (this->pBackingArr) {
		free(this->pBackingArr);
	}
}

template <typename T>
inline void CircularBuffer<T>::insertValue(const T& f) {
	// We want to store these values in an order that makes convolving easier
	pBackingArr[currIndex] = f;
	if (this->insertionDirection) {
		this->incrementIndex();
	}
	else {
		this->decrementIndex();
	}
}

template <typename T>
inline T CircularBuffer<T>::readNextValue() {
	//This function will decrement the array pointer and return the very next value in the array
	T returnVal = pBackingArr[currIndex];
	this->incrementIndex();
	return returnVal;
}



template <typename T>
inline size_t CircularBuffer<T>::getCurrIndex() const {
	return this->currIndex;
}

template <typename T>
inline T CircularBuffer<T>::at(const size_t &index) const {
	return this->pBackingArr[index];
}

template <typename T>
inline size_t CircularBuffer<T>::size() const {
	return this->length;
}



class Oscilloscope : public al::Mesh {
public:
	Oscilloscope(int samplerate) : bufferSize(samplerate) {
		buffer.allocate(bufferSize);
		this->primitive(al::Mesh::LINE_STRIP);
		float multiplicand = 2.f / static_cast<float>(bufferSize);
		for (int i = 0; i < bufferSize; i++) {
			this->vertex(i * multiplicand - 1.f, 0);
			buffer.insertValue(0.f);
		}
	}
	~Oscilloscope() {}

	void writeSample(float sample) {
		buffer.insertValue(sample);
	}

	void update() {
		for (int i = 0; i < bufferSize; i++) {
			this->vertices()[i][1] = buffer.readNextValue();
		}
	}

private:
	int bufferSize;
	CircularBuffer<float> buffer{ true };
};


#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"

class TestApp : public al::App {
private:
	Oscilloscope inputScope{ static_cast<int>(al::AudioIO().framesPerBuffer()) };
	Oscilloscope outputScope{ static_cast<int>(al::AudioIO().framesPerBuffer()) };

	//TODO: Parameters can go here
	al::Parameter volControl{ "volControl", "", 0.f, -96.f, 6.f };
	
public:
	void onInit() {
		// set up GUI
		auto GUIdomain = al::GUIDomain::enableGUI(al::App::defaultWindowDomain());
		auto& gui = GUIdomain->newGUI();

		//TODO: Add any parameters to show on UI here
		gui.add(volControl);
		

	}

	void onCreate() {
		
	}

	bool onKeyDown(const al::Keyboard& k) override {
		return true;
	}

	void onAnimate(double dt) {
		this->inputScope.update();
		this->outputScope.update();
	}

	void onDraw(al::Graphics& g) {
		g.clear(0);
		g.camera(al::Viewpoint::IDENTITY); // Ortho [-1:1] x [-1:1]
		g.color(1.f, 0.34f, 0.2f); //Hot pink
		g.draw(inputScope);
		g.color(0.007f, 0.333f, 1.f); //Neon blue
		g.draw(outputScope);
	}

	void onSound(al::AudioIOData& io) override {
		
	}


};


#include "../include/all.hpp" // Example include
int main() {

	a=1;
	Cow myCow;
	myCow.Moo();

	// int blockSize = 64;				// how many samples per block? - affects latency!!!
	// float sampleRate = 44100;		// sampling rate (samples/second)
	// int outputChannels = 2;			// how many output channels to open
	// int inputChannels = 1;			// how many input channels to open

	// // check if we have any audio devices
	// if (al::AudioDevice::numDevices() == 0) {
	// 	printf("Error: No audio devices detected. Exiting...\n");
	// 	exit(EXIT_FAILURE);
	// }

	// // list all detected audio devices
	// printf("Audio devices found:\n");
	// al::AudioDevice::printAll();
	// printf("\n");

	// TestApp app;
	// //Search for keywords amongst devices
	// app.audioIO().deviceIn(al::AudioDevice("Microphone"));
	// app.audioIO().deviceOut(al::AudioDevice("Speaker"));
	// app.configureAudio(sampleRate, blockSize, outputChannels, inputChannels);

	// app.start();

	return 0;
}