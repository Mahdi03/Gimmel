# Chorus
**Chorus** is a popular effect that simulates the phenomenon of multiple voices sounding the same note, as is common in [choir](https://en.wikipedia.org/wiki/Choir) music. Because the exact tone produced by each singer has a slight, fluctuating deviation from the target frequency, the resultant summed signal involves time-varying constructive and destructive interference. This is simulated by blending a signal with pitch-shifted copies of itself, whose pitch ratio with the dry signal varies over time. 

The chorus effect is produced using a modulated delay line. By modulating the delay time of a delayed copy of the signal, a pitch shift is created via the [doppler effect](https://en.wikipedia.org/wiki/Doppler_effect). Increasing delay times mimic a sound source traveling away from the listener, resulting in a decrease in pitch, while decreasing delay times simulate the source moving towards the listener, resulting in an upward pitch shift. 

Most choruses have a `depth` parameter that determines a center delay time with delay times modulating between `2 * depth` and `0` (no delay). Another chorus parameter is `rate`, which sets the frequency of the oscillator.

<!--TO-DO: Experiment with feedback--->