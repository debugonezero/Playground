# Riemann Manifold Visualization Terminal
**Engine:** WebGPU Core 1.1 / Metal 4.0  
**Hardware Target:** Apple A18 Pro / M-Series / High-End Desktop GPU  
**Developer:** @debugonezero

## Overview
This playground is a hardware-accelerated research dashboard for visualizing the **Riemann Zeta Function** critical strip. It utilizes the modern WebGPU standard to perform real-time manifold reconstruction at 120Hz.

## Technical Specifications
- **Display P3 Gamut:** Unlocks wide-color depth on compatible OLED/Retina displays.
- **Symmetric Functional Equation:** Real-time simulation of the symmetry $s \longleftrightarrow 1-s$.
- **High-Refresh Logic:** Optimized for ProMotion displays (120Hz) with 48,000+ points per frame.
- **Memory Alignment:** Strict 16-byte uniform buffer packing for Metal 4 compatibility.

## Mathematics
The manifold is governed by the **Functional Equation**, which defines the reflection symmetry across the critical line:

$$\zeta(s) = 2^s \pi^{s-1} \sin\left(\frac{\pi s}{2}\right) \Gamma(1-s) \zeta(1-s)$$

The distribution is further refined via the **Tau-Symmetric Integral**:

$$\Phi(s) = \int_{0}^{\infty} \frac{x^{s-1}}{e^{\tau x} - 1} dx$$

## Performance
Designed for the **iPhone 16 Pro Max** and **i9 iMac (5700 XT)**. The engine prioritizes dedicated GPU cores via `high-performance` power preference for zero-latency vertex manipulation.
