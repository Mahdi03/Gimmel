# Reverb
**Reverb** effects mimic the physical phenomenon of acoustic reflection, where audio in a room is not only heard coming from its source but also reflected from many surfaces.

Early analog reverb effects leveraged physical mediums like springs to blend current inputs with past inputs as distorted by their transduction to physical oscillation and back.

<!--TODO: Image of schroeder block diagram-->

A popular reverb implementation is the [Schroeder reverb](https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html), which chains [all-pass](https://en.wikipedia.org/wiki/All-pass_filter) and [comb](https://en.wikipedia.org/wiki/Comb_filter) filters in series to simulate acoustic reflection. **Gimmel**'s reverb implementation is derived from the Schroeder model.

<!---TO-DO: In-depth breakdown of our Reverb--->