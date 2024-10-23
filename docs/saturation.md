# Saturation
Saturation (known colloquially as *distortion* and by its flavors *overdrive*, *fuzz*, etc) is a form of audio distortion where the peaks of a waveform are "clipped" off, originally produced by exceeding the capacity of analog circuits. 

<!--TODO: Add images of different flavors of clipping for different flavors of distortion/fuzz/overdrive/etc.-->

Saturation can be understood as a form of aggressive compression with instantaneous attack and release, consequently operating not only on events but also reshaping a sound's spectrum. Saturation brings higher frequency components of a signal to equal volume with the fundamental, while also introducing inter-modulation distortion. Producing saturation in the digital domain must be done carefully, because it produces high frequencies that can cause a type of artifact known as [aliasing](https://en.wikipedia.org/wiki/Aliasing).

Crude saturation can be achieved through **waveshaping** algorithms, a mathematical function which produces an output sample $y$ given an input sample $x$. **Gimmel**'s saturation is implemented using $\tanh$, a function that roughly simulates the "soft-clipping" of analog circuits. 

To increase the 'drive' of a given waveshaping function $f(x)$, the input is scaled by a coefficient $d$, and the function divided by $f(d)$. A general form is presented in the equation: 

$$
y = \frac{f(dx)}{f(d)}
$$

<!--TODO: Embed Desmos waveshaping sliders here -->

To avoid aliasing, [oversampling](https://en.wikipedia.org/wiki/Oversampling) is used. By applying the distortion at a higher sample rate, the higher frequencies produced by distortion do not result in aliasing, and the resultant signal can be returned to the original sample rate after the high frequencies have been removed with a filter. 