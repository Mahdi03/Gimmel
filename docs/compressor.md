# Dynamic-Range Compression 
<!--TODO: For all effects, list the parameters in a list so that it's not just word blob-->
**Dynamic range compression** is one of the most widely used effects in audio. By lowering the volume of loud sections on a track, the entire track's volume can be raised without fear of [clipping](https://en.wikipedia.org/wiki/Clipping_(audio)). This allows for increased perceptual loudness, and creates consistency in a clip's volume. 

A typical compressor is adjustable via a small handful of parameters. A compressor's `threshold` adjusts the amplitude level at which gain reduction is applied. It is typically set in terms of a decibel (dB) value.  

When the threshold is met or exceeded, the volume will be reduced by a factor determined in the `ratio` parameter. For every `ratio` dB above the `threshold`, the sample's volume above the `threshold` will be reduced by $\text {1/ratio}$.

Compressors typically have a delayed reaction between when the threshold is exceeded and the compression is fully applied. How quickly a compressor reacts can be adjusted with an `attack` parameter, which is typically measured in milliseconds. Its inverse, the `release` parameter, determines how quickly the compression wanes following the input signal's return to values under the threshold.

Most compressors also have a `knee` parameter, which affects input samples near the threshold. Compressors with a "hard knee" act with boolean logic, where an input value either triggers compression or doesn't. Compressors with a "soft knee" consider values near the threshold, compressing them gently.

<!--<figure>
<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/3e/Compression_knee.svg/440px-Compression_knee.svg.png" />
<figcaption>
</figcaption>
</figure>-->

The compressor implemented in **Gimmel** is based on a [2011 paper](https://aes2.org/publications/elibrary-page/?id=16354) from the Queen Mary University of London, which surveys the history of compressor design and presents a favored implementation. 