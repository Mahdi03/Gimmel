# **Gimmel: Objectives and Objects**
# Dynamic-Range Compression
Dynamic range compression is one of the most widely used effects in audio. By lowering the volume of loud sections on an audio clip, the entire clip's volume can be raised without fear of overload. This allows for increased perceptual loudness, and creates consistency in a clip's volume.
```
//TO-DO:
-check gain reduction code
-implement attack, release.
-implement setters, getters, constructor

class Compressor {
public:
    float processSample(float input) {
        float env = fabs(input); // use abs of input for measuring its amplitude
        if (env > thresh) {triggered = true;} 
        else {}
        
        if (triggered) {
            gain = 1.f - (env - thresh) * ratio
        } else {
            gain = 1.f;
        }
      return input * gain;
    }
protected:
    bool triggered = false;
    float ratio = 1.f;
    float thresh = 1.f;
    float gain = 1.f;
    float attackMs;
    float releaseMs;
};
```
# Equalization 
Audio equalization (EQ) refers to the attentuation of certain frequencies in an audio signal, and has a wide range of applications. EQ is accomplished through filters that track change in amplitude over time and respond according to user-friendly parameters. The most popular digital filter is the biquad, a second-order filter that leverages previous inputs and outputs. 
```
//TO-DO: 
-Implement and parameterize biquad for lo/hi-pass, peak/notch, lo/hi-shelf. 
```
# Delay
Delay effects (also known as echo) involve blending the current input signal with reproductions of its past valyes. When these reproductions are spaced far enough in time to be percieved as discrete repeats, the resultant effect is deemed "delay" or "echo." 
```
//TO-DO: 
-Introduce and implement circular buffer
-Implement delay with feedback

class CircularBuffer {
public:
    void writeSample(float input) {
        buffer[writeIndex] = input; // write input to buffer
        writeIndex = (writeIndex + 1) % bufferSize; // increment writeIndex
    }
    float readSample(int delayInSamps) {
        return buffer[writeIndex - delayInSamps]; // read from buffer
    } 
    
protected:
    const int bufferSize = 96000; // arbitrary size (2s max delay)
    int writeIndex; // I believe JUCE implements this as a pointer...
    float[bufferSize] buffer;
}
```
# Reverb
Reverb effects mimic the phsyical phenomenon of acoustic reflection, where audio in a room is not only heard coming from its source but also reflected off many surfaces.
```
//TO-DO: 
-Survey reverb techniques. Choose which one(s) to implement. 
```
# Clip-Distortion
Clip distortion (known colloquially as "overdrive" or simply "distortion") is a form of audio distortion where the peaks of a waveform are "clipped" off, originally a product of exceeding the capacity of analog circuits. Clip distortion acts as a form of compression, bringing higher frequency components of a signal to equal volume with the fundamental, while also introducing intermodulation distortion. Producing clip distortion in digital audio must be done carefully, because it produces high frequencies that can cause a type of artifact called "aliasing."
```
//TO-DO: 
-Survey distortion techniques. Choose which one(s) to implement. 
-Introduce and implement oversampling using sinc interpolation
```
# Chorus
Chorus is a popular effect that simulates the phenomenon of multiple voices singing the same note, like a choir. Because the exact tone produced by each singer is a few cents sharp or flat of the target note, and fluctuates, the resultant summed signal involves time-varying constructive and destructive interference. This is simulated by blending a signal with pitchshifted copies of itself, whose pitch ratio with the dry signal varies over time. 
```
//TO-DO: 
-Implement chorus with depth, rate...
```
# Detune
Detune is a less common but similar effect to chorus. Detune sums a signal with a copy of itself that remains at a fixed pitch interval, effectively a general-purpose pitchshift optimized for a pitch change of less than one half-step.
```
//TO-DO: 
-optimize Jafftune code with circular buffer, more windows?, variable windowSize?
```
# Phaser
Phasing is the product of summing a signal with a delayed copy of itself. Constructive and destructive interference produce comb-like filtering that was popularized in music of the 1960s. 
```
//TO-DO: 
-Survey Phasers. Choose one to implement. 
```
# Amplifier Modeling
Amplifier Modeling is the art of replicating the processing of analog (generally vacuum tube) guitar amplifiers. It is typically accomplished through emulation, not simulation. Simple models use static waveshaping, that set output values based on input samples, and more advanced models use time-varying techniques such as convolutional modeling and WaveNets.
```
//TO-DO: 
-Survey modeling technqiues. Choose one to implement.
```
# Convolution
Convolution is a signal processing technique with many applications, but has historically been prohibitive in digital audio due to the immense amount of computing power that it requires. It involves producing an output signal for every input sample, and sums these signals to create a final output. Modern computers are capable of perceptually real-time convolution given that the convolution kernel is reasonably small (<1 second). 
```
//TO-DO: 
-Implement time-domain convolution
```
## User Interface / Handy Functions
- dBtoA
- aTodB
- msToSamps
- sampsToMs
- wet/dry-Mixer
- normalize
- noDenormals()
