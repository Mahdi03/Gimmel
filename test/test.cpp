#include "wav.h"

int main() {
    WAVLoader loader { "audio/Gmaj.wav" }; //Pick an input sound to test
    WAVWriter writer { "audio/out.wav" };

    float input, output;
    while (loader.readSample(&input)) { //Sample loop
        //Effects go here
        output = input;

        writer.writeSample(output); //Write modified sample to output file
    }

    return 0;
}