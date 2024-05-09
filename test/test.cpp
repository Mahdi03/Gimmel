#include "wav.h"
#include "../include/modulation/Tremolo.hpp"

int main() {
    WAVLoader loader { "audio/Gmaj.wav" }; //Pick an input sound to test
    WAVWriter writer { "audio/out.wav", loader.sampleRate };

    giml::Tremolo<float> t{ loader.sampleRate };
    t.setDepth(1.f);
    t.setSpeed(750.f);

    float input, output;
    while (loader.readSample(&input)) { //Sample loop
        //Effects go here
        output = t.processSample(input);

        writer.writeSample(output); //Write modified sample to output file
    }

    return 0;
}