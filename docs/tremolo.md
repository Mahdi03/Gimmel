# Tremolo
**Tremolo** is the simplest modulation effect. It is sometimes referred to by its more technical name, **amplitude modulation**. Traditional amplitude modulation in audio synthesis is achieved by multiplying each amplitude value of a "carrier" signal by the corresponding amplitude value of a "modulator" signal at each timestep. 

To make a tremolo effect suited for musical signal input, a **unipolar** oscillator is used, that is, an oscillator whose output ranges between 0 and 1. This value can be scaled by a `depth` parameter and subtracted from 1, scaling the input by a variable gain. The `return` statement simply involves multiplying the input by the gain value. 

<!--TO-DO: add diagram--->