# Detune
**Detune** is a less common but similar effect to chorus. Detune sums a signal with a copy of itself that remains at a fixed pitch interval, effectively a general-purpose pitchshift optimized for a pitch change of less than one half-step.

Like chorus, the detune effect is achieved by simulating the doppler effect using a modulated delay line. The fixed pitch-change is maintained via the use of a unipolar sawtooth oscillator, whose frequency determines the amount of transposition. 

A problem is presented when the waveform resets from 1 to 0 or vice verse, audible as a "click" if unaddressed. A solution is found in *gain windowing*, where the gain of the signal is smoothly ramped to zero when the click occurs, and blended with a copy whose phase is offset. 

<!--TO-DO: add windowing diagram--->

In **Gimmel**'s implementation, two copies windowed by cosine ramps are used, but there are portions of the signal where the two gains don't add up to 1 and thereby result in a lowering of the signal's volume. To perform more seamless windowing, more windows can be used.