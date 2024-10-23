### Phaser
The phaser effect is created by summing a signal with multiple copies of itself whose phases has been manipulated. Constructive and destructive interference produces time-variant comb filtering that was popularized in music of the 1960s. 

Phasers are traditionally implemented with all-pass filters, which have a flat amplitude response across the frequency spectrum but distort phase. While this phase distortion is inaudible on its own, the combination of a phase-distorted signal with the original creates the aforementioned comb filtering. An oscillator modulates the frequencies being filtered out, resulting in a time-variant system. 

Most phasers use a bank of numerous all-pass filters in parallel, with each filter parametrized by a different **center frequency**. The farther a frequency component lays from the center frequency, the more its phase is distorted. The center frequencies of all the filters are modulated by a single LFO waveform, typically a triangle wave. 

Phasers also typically have a feedback component, which determines the "intensity" of the effect. This feedback is *instantaneous* in analog circuits, which presents a problem for digital implementations.

Gimmel's phaser implements a design from Will Pirkle's book [*Designing Audio Effects in C++*](https://www.willpirkle.com/), which resolves the *zero-delay feedback* with state-space methods from Zavalashin's book [*The Art of Virtual Analog Filter Design*](https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_2.1.0.pdf?srsltid=AfmBOorPylM-C_N8qvyxZOHrZCX6__YE8B7oG1tBXQxyNZf4LUZcXz8N).