# Equalization (Biquad)
<!--TODO-->

One of the most popular digital filters is the **biquad**, a second-order IIR filter that is highly versatile via parameterization of its filter coefficients. While the `processSample` function of **Gimmel**'s `Biquad` class is extremely concise, the calculation of its filter coefficients from user-friendly input parameters constitutes a hundreds of lines of code. 

**Gimmel**'s implementation of the biquad is based on the [Audio EQ Cookbook](https://www.w3.org/TR/audio-eq-cookbook/).