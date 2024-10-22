# Delay
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