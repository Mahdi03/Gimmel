#include "wav.h"
#include "../include/Biquad.hpp"

#include <chrono>
static long long timeElapsed = 0L;
static long long iterations = 0L;

#define BENCHMARK_CODE_AVG(code) { \
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); \
        code \
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
        timeElapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count(); \
        iterations++; \
        if (iterations > 48000 * 5) { \
			std::cout << "average " << timeElapsed / iterations << "ns avg every 5 sec" << std::endl; \
			timeElapsed = iterations = 0; \
		} \
   }

int main() {
    WAVLoader loader { "audio/homemadeLick.wav" }; //Pick an input sound to test
    WAVWriter writer { "audio/out.wav", loader.sampleRate };

    giml::Biquad<float> b{ loader.sampleRate };
    /*t.setDepth(1.f);
    t.setSpeed(750.f);*/
    b.enable();
    b.setType(giml::Biquad<float>::BiquadUseCase::BPF);
    b.setParams(22395.f);


    float input, output;
    int i = 1;
    while (loader.readSample(&input)) { //Sample loop
        //Effects go here
        output = b.processSample(input);
        writer.writeSample(output); //Write modified sample to output file
        if (i % 100 == 0) {
            std::cout << "Wrote 100 samples" << std::endl;
        }
        i++;
    }

    return 0;
}