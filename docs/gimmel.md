# **Gimmel: Beginner's Guide to Digital Audio Effects**

**Digital audio** is a subfield of **digital signal processing** ([DSP](https://en.wikipedia.org/wiki/Digital_signal_processing)), a field that encompasses the analysis and transformation of signals using digital computers. 

To operate on real-world continuous signals with a digital computer, their amplitude is measured as discrete **samples** at a regular rate and stored as binary data. 

<!--TODO: Add basic image of sampled wave here-->

<!--<figure>
<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/c/c3/Signal_Sampling.svg/600px-Signal_Sampling.svg.png" />
<figcaption>
</figcaption>
</figure>-->

**Digital audio effects** are achieved by mathematically transforming the values of these samples before playback, and can enhance signals with qualities desirable to the human ear. Digital audio is ubiquitous in fields ranging from music & film production to telecommunications & broadcasting. 

The goal of **Gimmel** is to provide lightweight implementations of the most common digital audio effects, augmented by thorough documentation that gives the project pedagogical utility.

The following table of contents links to guides on the effects implemented in **Gimmel**:

<ul>
<li><a href="#amplitude-domain-effects">Amplitude-Domain Effects</a></li>
    <ul>
    <li><a href="./compressor.md">Dynamic-Range Compression</a></li>
    <li><a href="./saturation.md">Saturation</a></li>
    </ul>
<li><a href="#equalization">Equalization</a></li>
    <ul>
    <li><a href="./compressor.md">Biquad</a></li>
    <li><a href="./saturation.md">Filter</a></li>
    </ul>
<li><a href="#time-domain-effects">Time-Domain Effects</a></li>
    <ul>
    <li><a href="./delay.md">Delay</a></li>
    <li><a href="./reverb.md">Reverb</a></li>
    </ul>
<li><a href="#modulation-effects">Modulation Effects</a></li>
    <ul>
    <li><a href="./tremolo.md">Tremolo</a></li>
    <li><a href="./chorus.md">Chorus</a></li>
    <li><a href="./detune.md">Detune</a></li>
    <li><a href="./phaser.md">Phaser</a></li>
    </ul>
</ul>
<hr />

## Amplitude-Domain Effects
Digital audio effects can be grouped into a few categories. One is **amplitude domain effects**, which, as the name implies, operate on the amplitude of signals. While audio amplitude is linear, humans hear volume on a logarithmic scale. Because amplitude is distinct from perceptual loudness, lots of tricky math goes into amplitude domain effects. Functions that map amplitude values to **decibels (dB)**, a unit that tracks perceptual loudness, are integral components. To convert a sample value to dB, the following equation is used:

$$
\text{dB} = 20\log_{10}(A)
$$

where $\text{dB}$ is the decibels, and $A$ is the amplitude value.  

Amplitude-domain effects implemented in **Gimmel**: [Compressor](./compressor.md), [Saturation](./saturation.md)

## Equalization
Audio equalization (EQ) refers to the attenuation of certain frequencies in an audio signal, and has a wide range of applications. EQ is accomplished through digital **filters** that track amplitude over time and respond according to user-friendly parameters. 

Digital filters work by storing the values of past samples, whose values are combined with the current sample in specified proportions to produce an output. The proportions are determined by **filter coefficients**, which can be used to parameterize the filter. 

The number of past samples a filter stores is known as its **order**, an $N$-th order filter stores values from $N$ samples in the past.

<!--TODO: FIR vs. IIR venn-diagram or bullet points-->

Filters that store only past *input* samples are categorized as **finite impulse response** ([FIR](https://en.wikipedia.org/wiki/Finite_impulse_response)) filters, and are useful because they are always [stable](https://en.wikipedia.org/wiki/BIBO_stability).

**Infinite impulse response** ([IIR](https://en.wikipedia.org/wiki/Infinite_impulse_response)) filters store both past *inputs* **and** past *outputs* , meaning that they utilize [feedback](https://en.wikipedia.org/wiki/Feedback) in their design. This can make them unstable, but also allows IIR filters of very low order to perform operations that would require an arbitrarily high-order FIR filter.

EQs implemented in **Gimmel**: [Biquad](./biquad.md), [Filter](./filter.md)

## Time-Domain Effects
Similar to digital filters, **time-domain** effects leverage memory of a signal's past values to create output samples. Digital audio is optimal for this, because it is capable of nearly perfect recording, storage and playback of audio samples. The first commercial digital audio devices were in fact time-domain effects, made by [Eventide Inc.](https://en.wikipedia.org/wiki/Eventide,_Inc) in the 1970s. 

Processing tens of thousands of samples per second, the storing of past samples for time-domain effects presents a problem for memory management. Because time-domain effects rarely reference signal values more than three seconds in the past, computational resources can be saved by using a **circular buffer**, which has a fixed length and overwrites the oldest values as time moves forward.

Time-domain effects implemented in **Gimmel**: [Delay](./delay.md), [Reverb](./reverb.md)

## Modulation Effects
**Modulation** effects involve the manipulation of an audio signal by a periodic function, typically an elemental waveform produced by an [oscillator](https://en.wikipedia.org/wiki/Electronic_oscillator). When one signal modulates another, the signal being modified is called the **carrier** and the signal that modifies it is called the **modulator**. 

Modulator waveforms typically have a frequency lower than 20Hz, perceived as rhythm instead of pitch when sonified. An oscillator that produces such waveforms is known as a **low-frequency oscillator** ([LFO](https://en.wikipedia.org/wiki/Electronic_oscillator)).

Modulation effects implemented in **Gimmel**: [Tremolo](./tremolo.md), [Chorus](./chorus.md), [Detune](./detune.md), [Phaser](./phaser.md)