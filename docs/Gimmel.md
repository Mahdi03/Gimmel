# **Gimmel: Beginner's Guide to Digital Audio Effects**
**Digital audio** is a subfield of digital signal processing (DSP). To operate on real-world continuous signals with a digital computer, their amplitude is measured as discrete **samples** at a regular rate and stored as binary code. 

Digital audio effects are achieved by mathematically transforming the values of these samples before playback, and can enhance signals with qualities desirable to the human ear. Digital audio is ubiquitous in fields ranging from music & film production to telecommunications & broadcasting. 

The goal of Gimmel is to provide lightweight implementations of the most common digital audio effects, augmented by thorough documentation that gives the project pedagogical utility in the teaching of digital audio.

## Amplitude Domain Effects
Digital audio effects can be grouped into a few categories. One is **amplitude domain effects**, which, as the name implies, operate on the amplitude of signals. Because amplitude is distinct from perceptual loudness, lots of tricky math goes into amplitude domain effects. Functions that map amplitude values to **decibels (dB)**, a unit that tracks perceptual loudness, are extremely useful in these effects.

### Dynamic-Range Compression 
**Dynamic range compression** is one of the most widely used effects in audio. By lowering the volume of loud sections on an audio clip, the entire clip's volume can be raised without fear of **clipping**. This allows for increased perceptual loudness, and creates consistency in a clip's volume. 

A typical compressor is adjustable via a small handful of parameters. `thresh` adjusts the amplitude level at which compression is triggered. 

When the threshold is met or exceeded, the volume will be reduced by a factor determined in the `ratio` parameter. For every 1dB above the threshold, the volume of that sample will be reduced by `ratio` dB. 

Compressors typically have a delayed reaction between when the threshold is exceeded and the compression is fully applied. How quickly a compressor reacts can be adjusted with an `attack` parameter, which is typically measured in milliseconds. Its inverse, the `release` parameter, determines how quickly the compression wanes following the input signal's return to values under the threshold.

Most compressors also have a `knee` parameter, which affects input samples near the threshold. Compressors with a "hard knee" act with boolean logic, where an input value either triggers compression or doesn't. Compressors with a "soft knee" consider values near the threshold, compressing them gently.

```
//TO-DO:
-implement and test compressor
```

### Noise Gate
A side effect of dynamic-range compression is that it often results in the amplification of noise in a signal, reducing the signal-to-noise ratio. This is most audible when the track is devoid of musical content, but a noise floor remains. To avoid this, a signal can be processed with a noise gate, which silences output when the signal is below a certain threshold. A noise gate is the reverse of a **limiter**, a compressor whose `ratio` exceeds 20:1.  

### Saturation
Saturation (known colloquially as "distortion" and by its flavors overdrive, fuzz, etc) is a form of audio distortion where the peaks of a waveform are "clipped" off, originally produced by exceeding the capacity of analog circuits. 

Saturation can be conceptualized as a form of aggressive compression that brings higher frequency components of a signal to equal volume with the fundamental, while also introducing inter-modulation distortion. Producing saturation in the digital domain must be done carefully, because it produces high frequencies that can cause a type of artifact known as **aliasing**.

Distortion can be achieved through **waveshaping** algorithms, blah blah blah...

To avoid aliasing, use **oversampling** blah blah blah...

```
//TO-DO: 
-Survey distortion techniques. Choose which one(s) to implement. 
-Introduce and implement oversampling using sinc interpolation
```

## Time-Domain Effects
**Time-domain** effects leverage memory of a signal's past values to create output samples. Digital audio is optimal for this, because it is capable of asymptotically perfect recording, storage and playback of audio samples. The first commercial digital audio devices were in fact time-domain effects, made by Eventide Inc. in the 1970s. 

Because time-domain effects rarely reference signal values more than three seconds in the past, computational resources can be saved by using a **circular buffer**, which has a fixed length and overwrites the oldest values as time moves forward.

```
//TO-DO: 
-Implement circular buffer
```

All of the following time-domain effects will make use of a circular buffer. 

### Delay
**Delay** effects involve blending the current input signal with reproductions of its past values. When these reproductions are spaced far enough in time to be perceived as discrete repeats, the resultant effect is deemed **delay** or **echo**.

A delay that simply repeats an event that happened `delayTime` samples ago can be invoked by calling the `readSample()` function from our circular buffer with `writeIndex - delayTime` as the argument. 

Delay effects typically have **feedback**, where blah blah blah...

Delay effects are typically low-pass filtered, blah blah blah...

```
//TO-DO: 
-Implement Delay with feedback
```

### Reverb
**Reverb** effects mimic the physical phenomenon of acoustic reflection, where audio in a room is not only heard coming from its source but also reflected from many surfaces.

Early analog reverb effects leveraged physical mediums like springs blah blah blah...

```
//TO-DO: 
-Survey reverb techniques. Choose which one(s) to implement. 
```

## Modulation Effects
**Modulation** effects involve the manipulation of an audio signal by a periodic function, typically an elemental waveform produced by a digital **oscillator**. In modulation, the signal being modified is called the **carrier** and the signal that modifies it is called the **modulator**. 

Modulator waveforms typically have a frequency lower than 20Hz, perceived as rhythm instead of pitch if sonified. An oscillator that produces such waveforms is known as a **low-frequency oscillator (LFO)**.

### Tremolo
**Tremolo** is the simplest modulation effect. It is sometimes referred to by its more technical name, **amplitude modulation**. Tremolo is achieved by simply multiplying each amplitude value of the carrier by the corresponding amplitude value of the modulator at each timestep. 

### Chorus
**Chorus** is a popular effect that simulates the phenomenon of multiple voices singing the same note, like a choir. Because the exact tone produced by each singer is a few cents sharp or flat of the target note, and fluctuates, the resultant summed signal involves time-varying constructive and destructive interference. This is simulated by blending a signal with pitch-shifted copies of itself, whose pitch ratio with the dry signal varies over time. 

```
//TO-DO: 
-Implement chorus with depth, rate...
```

### Detune
**Detune** is a less common but similar effect to chorus. Detune sums a signal with a copy of itself that remains at a fixed pitch interval, effectively a general-purpose pitchshift optimized for a pitch change of less than one half-step.

```
//TO-DO: 
-optimize Jafftune code with circular buffer, more windows?, variable windowSize?
```

### Phaser
Phasing is the product of summing a signal with a delayed copy of itself. Constructive and destructive interference produce comb-like filtering that was popularized in music of the 1960s. 

```
//TO-DO: 
-Survey Phasers. Choose one to implement. 
```

## Equalization 
Audio equalization (EQ) refers to the attenuation of certain frequencies in an audio signal, and has a wide range of applications. EQ is accomplished through **filters** that track amplitude over time and respond according to user-friendly parameters. 

Filters can be FIR or IIR blah blah blah...

The most popular digital filter is the **biquad**, a second-order filter that leverages previous inputs and outputs. 

```
//TO-DO: 
-Implement and parameterize biquad for lo/hi-pass, peak/notch, lo/hi-shelf. 
```

### Amplifier Modeling
Amplifier Modeling is the art of replicating the processing of analog (generally vacuum tube) guitar amplifiers. It is typically accomplished through emulation, not simulation. Simple models use static waveshaping, that set output values based on input samples, and more advanced models use time-varying techniques such as convolutional modeling and WaveNets.

```
//TO-DO: 
-Survey modeling techniques. Choose one to implement.
```

### Convolution
Convolution is a signal processing technique with many applications, but has historically been prohibitive in digital audio due to the immense amount of computing power that it requires. It involves producing an output signal for every input sample, and sums these signals to create a final output. Modern computers are capable of perceptually real-time convolution given that the convolution kernel is reasonably small (<1 second). 

```
//TO-DO: 
-Implement time-domain convolution
```

## User Interface / Handy Functions
- dBtoA: Converts a dB value to its equivalent amplitude
```
template:
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
```

- wet/dry-Mixer:
- normalize
- noDenormals()
