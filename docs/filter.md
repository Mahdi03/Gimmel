# Filter
<!--TODO-->
While the biquad is versatile enough to handle most practical applications of EQ, many other filter topologies exist that each have unique strengths and vulnerabilities. The purpose of `filter.md`/`filter.hpp` is to provide a consolidated resource for the expansion of filter implementations in **Gimmel**.

## One-Pole Filter (Needs Improvement)
The one-pole filter is one of the simplest filter topologies. It's a 1st-order IIR filter that simply combines an input value with the previous output value in proportions that add to $1$. Its difference equation can be written as:

$$
y[n] = (1 - \alpha)x[n] + \alpha y[n - 1]
$$

One-poles are extremely common in DSP generally because they're highly efficient, with both low memory footprint and low computational cost.