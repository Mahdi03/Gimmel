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

The remainder of this document constitutes a guide to the effects implemented in **Gimmel**:

<ul>
<li><a href="#amplitude-domain-effects">Amplitude Domain Effects</a></li>
    <ul>
    <li><a href="#dynamic-range-compression">Dynamic-Range Compression</a></li>
    <li><a href="#saturation">Saturation</a></li>
    </ul>
<li><a href="#equalization-biquad">Equalization (Biquad)</a></li>
<li><a href="#time-domain-effects">Time-Domain Effects</a></li>
    <ul>
    <li><a href="#delay">Delay</a></li>
    <li><a href="#reverb">Reverb</a></li>
    </ul>
<li><a href="#modulation-effects">Modulation Effects</a></li>
    <ul>
    <li><a href="#tremolo">Tremolo</a></li>
    <li><a href="#chorus">Chorus</a></li>
    <li><a href="#detune">Detune</a></li>
    <li><a href="#phaser">Phaser</a></li>
    </ul>
</ul>
<hr />

## Amplitude Domain Effects
Digital audio effects can be grouped into a few categories. One is **amplitude domain effects**, which, as the name implies, operate on the amplitude of signals. While audio amplitude is linear, humans hear volume on a logarithmic scale. Because amplitude is distinct from perceptual loudness, lots of tricky math goes into amplitude domain effects. Functions that map amplitude values to **decibels (dB)**, a unit that tracks perceptual loudness, are integral components. To convert a sample value to dB, the following equation is used:

$$
\text{dB} = 20\log_{10}(A)
$$

where $\text{dB}$ is the decibels, and $A$ is the amplitude value.

### Dynamic-Range Compression 
<!--TODO: For all effects, list the parameters in a list so that it's not just word blob-->
**Dynamic range compression** is one of the most widely used effects in audio. By lowering the volume of loud sections on a track, the entire track's volume can be raised without fear of [clipping](https://en.wikipedia.org/wiki/Clipping_(audio)). This allows for increased perceptual loudness, and creates consistency in a clip's volume. 

A typical compressor is adjustable via a small handful of parameters. A compressor's `threshold` adjusts the amplitude level at which gain reduction is applied. It is typically set in terms of a decibel (dB) value.  

When the threshold is met or exceeded, the volume will be reduced by a factor determined in the `ratio` parameter. For every `ratio` dB above the threshold, the sample's volume above the `threshold` will be reduced by $\text{1/ratio}$.

Compressors typically have a delayed reaction between when the threshold is exceeded and the compression is fully applied. How quickly a compressor reacts can be adjusted with an `attack` parameter, which is typically measured in milliseconds. Its inverse, the `release` parameter, determines how quickly the compression wanes following the input signal's return to values under the threshold.

Most compressors also have a `knee` parameter, which affects input samples near the threshold. Compressors with a "hard knee" act with boolean logic, where an input value either triggers compression or doesn't. Compressors with a "soft knee" consider values near the threshold, compressing them gently.

<!--<figure>
<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/3e/Compression_knee.svg/440px-Compression_knee.svg.png" />
<figcaption>
</figcaption>
</figure>-->

The compressor implemented in **Gimmel** is based on a [2011 paper](https://aes2.org/publications/elibrary-page/?id=16354) from the Queen Mary University of London which surveys the history of compressor design and presents an favored implementation.  

### Saturation
Saturation (known colloquially as 'distortion' and by its flavors overdrive, fuzz, etc) is a form of audio distortion where the peaks of a waveform are 'clipped' off, originally produced by exceeding the capacity of analog circuits. 

<!--TODO: Add images of different flavors of clipping for different flavors of distortion/fuzz/overdrive/etc.-->

Saturation can be conceptualized as a form of aggressive compression that brings higher frequency components of a signal to equal volume with the fundamental, while also introducing inter-modulation distortion. Producing saturation in the digital domain must be done carefully, because it produces high frequencies that can cause a type of artifact known as [aliasing](https://en.wikipedia.org/wiki/Aliasing).

Distortion can be achieved through **waveshaping** algorithms, a mathematical function which produces an output sample $y$ given an input sample $x$. **Gimmel**'s saturation is implemented using $\tanh$, a function that roughly simulates the 'soft-clipping' of analog circuits. 

To increase the 'drive' of a given waveshaping function $f(x)$, the input is scaled by a coefficient $d$, and the function divided by $f(d)$. A general form is presented in the equation: 

$$
y = \frac{f(dx)}{f(d)}
$$

<!--TODO: Embed Desmos waveshaping sliders here -->

To avoid aliasing, [oversampling](https://en.wikipedia.org/wiki/Oversampling) is used. By applying the distortion at a higher sample rate, the higher frequencies produced by distortion do not result in aliasing, and the resultant signal can be returned to the original sample rate after the high frequencies have been removed with a filter. 

## Equalization (Biquad)
Audio equalization (EQ) refers to the attenuation of certain frequencies in an audio signal, and has a wide range of applications. EQ is accomplished through digital **filters** that track amplitude over time and respond according to user-friendly parameters. 

Digital filters work by storing the values of past samples, whose values are combined with the current sample in specified proportions to produce an output. The proportions are determined by **filter coefficients**, which can be used to parameterize the filter. 

The number of past samples a filter stores is known as its **order**, an $N$-th order filter stores values from $N$ samples in the past.

<!--TODO: FIR vs. IIR venn-diagram or bullet points-->

Filters that store only past *input* samples are categorized as **finite impulse response** ([FIR](https://en.wikipedia.org/wiki/Finite_impulse_response)) filters, and are useful because they are always [stable](https://en.wikipedia.org/wiki/BIBO_stability).

**Infinite impulse response** ([IIR](https://en.wikipedia.org/wiki/Infinite_impulse_response)) filters store both past *inputs* **and** past *outputs* , meaning that they utilize [feedback](https://en.wikipedia.org/wiki/Feedback) in their design. This can make them unstable, but also allows IIR filters of very low order to perform operations that would require an arbitrarily high-order FIR filter. 

The most popular digital filter is the **biquad**, a second-order IIR filter that is highly versatile via the parameterization of its filter coefficients. While the `processSample` function of **Gimmel**'s `Biquad` class is extremely concise, the calculation of its filter coefficients from user-friendly input parameters constitutes a robust body of work. 

## Time-Domain Effects
Similar to digital filters, **time-domain** effects leverage memory of a signal's past values to create output samples. Digital audio is optimal for this, because it is capable of nearly perfect recording, storage and playback of audio samples. The first commercial digital audio devices were in fact time-domain effects, made by [Eventide Inc.](https://en.wikipedia.org/wiki/Eventide,_Inc) in the 1970s. 

Processing tens of thousands of samples per second, the storing of past samples for time-domain effects presents a problem for memory management. Because time-domain effects rarely reference signal values more than three seconds in the past, computational resources can be saved by using a **circular buffer**, which has a fixed length and overwrites the oldest values as time moves forward.

### Delay
**Delay** effects involve blending the current input sample with past values, particularly those preceding the present by an amount of time perceived as a discrete repeat. The resultant effect is deemed **delay** or **echo**.

A delay that simply repeats an event that happened `delayTime` samples ago can be invoked by calling the `readSample()` function from a circular buffer, with `delayTime` as the argument. The basic formula is

$$
y[n] = x[n-D]
$$

Where $y[n]$ and $x[n]$ are output and input samples at timestep $n$, and $D$ represent a delay time in samples.

Delay effects typically have **feedback** however, where past inputs are recalled with past outputs blended in. The `feedback` parameter influences how much feedback signal is mixed in. The revised equation is 

$$
y[n] = x[n-D] + g \times y[n-D]
$$

The added variable $g$ represents a *feedback gain* and $y[n-D]$ represents an output sample at timestep $[n-D]$. If $g$ is set larger than $1$, the output will grow exponentially and become unstable. 

### Reverb
**Reverb** effects mimic the physical phenomenon of acoustic reflection, where audio in a room is not only heard coming from its source but also reflected from many surfaces.

Early analog reverb effects leveraged physical mediums like springs to blend current inputs with past inputs as distorted by their transduction to physical oscillation and back.

<!--TODO: Image of schroeder block diagram-->

A popular reverb implementation is the [Schroeder reverb](https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html), which chains [all-pass](https://en.wikipedia.org/wiki/All-pass_filter) and [comb](https://en.wikipedia.org/wiki/Comb_filter) filters in series to simulate acoustic reflection. **Gimmel**'s reverb implementation is derived from the Schroeder model.

<!---TO-DO: In-depth breakdown of our Reverb--->

## Modulation Effects
**Modulation** effects involve the manipulation of an audio signal by a periodic function, typically an elemental waveform produced by an [oscillator](https://en.wikipedia.org/wiki/Electronic_oscillator). When one signal modulates another, the signal being modified is called the **carrier** and the signal that modifies it is called the **modulator**. 

Modulator waveforms typically have a frequency lower than 20Hz, perceived as rhythm instead of pitch when sonified. An oscillator that produces such waveforms is known as a **low-frequency oscillator** ([LFO](https://en.wikipedia.org/wiki/Electronic_oscillator)).

### Tremolo
**Tremolo** is the simplest modulation effect. It is sometimes referred to by its more technical name, **amplitude modulation**. Traditional amplitude modulation in audio synthesis is achieved by multiplying each amplitude value of the carrier by the corresponding amplitude value of the modulator at each timestep. 

To make a tremolo effect suited for musical signal input, a **unipolar** oscillator is used, that is, an oscillator whose output ranges between 0 and 1. This value can be scaled by a `depth` parameter and subtracted from 1, allowing for a variable amount of gain to be subtracted from the input. The `return` statement simply involves multiplying the input by the gain value. 

<!--TO-DO: add diagram--->

### Chorus
**Chorus** is a popular effect that simulates the phenomenon of multiple voices sounding the same note, as is common in [choir](https://en.wikipedia.org/wiki/Choir) music. Because the exact tone produced by each singer has a slight, fluctuating deviation from the target frequency, the resultant summed signal involves time-varying constructive and destructive interference. This is simulated by blending a signal with pitch-shifted copies of itself, whose pitch ratio with the dry signal varies over time. 

The chorus effect is produced using a modulated delay line. By modulating the delay time of a delayed copy of the signal, a pitch shift is created via the [doppler effect](https://en.wikipedia.org/wiki/Doppler_effect). Increasing delay times mimic a sound source traveling away from the listener, resulting in a decrease in pitch, while decreasing delay times simulate the source moving towards the listener, resulting in an upward pitch shift. 

Most choruses have a `depth` parameter that determines a center delay time with delay times modulating between `2 * depth` and `0` (no delay). Another chorus parameter is `rate`, which sets the frequency of the oscillator.

<!--TO-DO: Experiment with feedback--->

### Detune
**Detune** is a less common but similar effect to chorus. Detune sums a signal with a copy of itself that remains at a fixed pitch interval, effectively a general-purpose pitchshift optimized for a pitch change of less than one half-step.

Like chorus, the detune effect is achieved by simulating the doppler effect using a modulated delay line. The fixed pitch-change is maintained via the use of a unipolar sawtooth oscillator, whose frequency determines the amount of transposition. 

A problem is presented when the waveform resets from 1 to 0 or vice verse, audible as 'click' if unaddressed. A solution is found in *gain windowing*, where the gain of the signal is smoothly ramped to zero when the click occurs, and blended with a copy whose phase is offset. 

<!--TO-DO: add windowing diagram--->

In **Gimmel**'s implementation, two copies windowed by cosine ramps are used, but there are portions of the signal where the two gains don't add up to 1 and thereby result in a lowering of the signal's volume. To perform more seamless windowing, more windows can be used.

### Phaser
The phaser effect created by summing a signal with a copy of itself whose phase has been manipulated. Constructive and destructive interference produce time-variant comb filtering that was popularized in music of the 1960s. 

Phasers are traditionally implemented with all-pass filters, which have a flat amplitude response across the frequency spectrum but distort phase. While this phase distortion is inaudible on its own, the combination of a phase-distorted signal with the original creates the aforementioned comb filtering. An oscillator modulates the frequencies being filtered out, resulting in a time-variant system. 

Most phasers use a bank of numerous all-pass filters in series, with each filter parametrized by a different **center frequency**. The farther a frequency component lays from the center frequency, the more its phase is distorted. The center frequencies of all the filters are modulated by a single LFO waveform, typically a triangle wave. 

Phasers also typically have a feedback component, which determines the 'intensity' of the effect. 

Gimmel's phaser implements a design from Will Pirkle's *Designing Audio Effects in C++*, which notably overcomes the difficultly of implementing the feedback component in a digital system. 
