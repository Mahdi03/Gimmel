### Phaser
The phaser effect created by summing a signal with a copy of itself whose phase has been manipulated. Constructive and destructive interference produce time-variant comb filtering that was popularized in music of the 1960s. 

Phasers are traditionally implemented with all-pass filters, which have a flat amplitude response across the frequency spectrum but distort phase. While this phase distortion is inaudible on its own, the combination of a phase-distorted signal with the original creates the aforementioned comb filtering. An oscillator modulates the frequencies being filtered out, resulting in a time-variant system. 

Most phasers use a bank of numerous all-pass filters in series, with each filter parametrized by a different **center frequency**. The farther a frequency component lays from the center frequency, the more its phase is distorted. The center frequencies of all the filters are modulated by a single LFO waveform, typically a triangle wave. 

Phasers also typically have a feedback component, which determines the 'intensity' of the effect. 

Gimmel's phaser implements a design from Will Pirkle's *Designing Audio Effects in C++*, which notably overcomes the difficultly of implementing the feedback component in a digital system.