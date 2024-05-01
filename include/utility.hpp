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
    float aTodB (float ampVal) {
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


    /* User Interface / Handy Functions
    - dBtoA: Converts a dB value to its equivalent amplitude
    ```
    template 
    dBtoA (t dBValue) {
        return 
    }
    ```

    - aTodB: Converts an amplitude value to its equivalent dB rating
    ```
    template:
    aTodB (t aValue) {
        return 
    }
    ```

    - msToSamps: 
    ```
    template:
    msToSamps (t msVal, int sampleRate) {
        return 
    }
    ```

    - sampsToMs: 
    ```
    template:
    sampsToMs (t sampsVal, int sampleRate) {
        return 
    }
    */
}
#endif