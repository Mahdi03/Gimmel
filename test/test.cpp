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

    giml::Biquad<float> t{ loader.sampleRate };
    /*t.setDepth(1.f);
    t.setSpeed(750.f);*/
    t.enable();

    float input, output;
    while (loader.readSample(&input)) { //Sample loop
        //Effects go here
        BENCHMARK_CODE_AVG(output = t.processSample(input);)
        writer.writeSample(output); //Write modified sample to output file
    }

    return 0;
}