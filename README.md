Simplex Noise
===============

*Alternative to ci::Perlin class.*

**This code is originally intended for use with the Cinder C++ library but only depends on GLM.**

Simplex noise is better looking, faster and has proper derivatives without the grid artifacts of Perlin noise derivatives. This code is mostly ported from Stefan Gustavson public domain implementation for the simplex noise, simplex flow noise and their analytical derivatives. The curl noise functions are adapted from Robert Bridson papers to use derivatives instead of finite differences. There's also a series of noise sums and other multi-fractals noises...

```c++
//! Returns a 1D, 2D, 3D or 4D simplex noise
Simplex::noise( const T &v );
//! Returns a 1D, 2D, 3D or 4D simplex ridged noise
Simplex::ridgedNoise( const T &v );
//! Returns a 1D, 2D, 3D or 4D simplex noise with analytical derivatives
Simplex::dnoise( const T &v );
//! Returns a 2D or 3D simplex noise with rotating gradients
Simplex::flowNoise( const T &v, float angle );
//! Returns a 2D or 3D simplex noise with rotating gradients and analytical derivatives
Simplex::dFlowNoise( const T &v, float angle );
//! Returns a 2D or 3D curl of a simplex noise, a simplex flow noise or a fractal brownian motion sum
Simplex::curlNoise( const T &v );
//! Returns a 1D, 2D, 3D or 4D simplex noise fractal brownian motion sum
Simplex::fBm( const T &v, uint8_t octaves, float lacunarity, float gain );
//! Returns a 1D, 2D, 3D or 4D simplex noise fractal brownian motion sum with analytical derivatives
Simplex::dfBm( const T &v, uint8_t octaves, float lacunarity, float gain );
//! Returns a 1D, 2D, 3D or 4D simplex ridged multi-fractal noise sum
Simplex::ridgedMF( const T &v, float ridgeOffset, uint8_t octaves, float lacunarity, float gain );
//! Returns a 2D or 3D simplex noise fractal brownian motion sum variation by Iñigo Quilez
Simplex::iqfBm( const T &v, uint8_t octaves, float lacunarity, float gain );
//! Returns the 2D simplex noise fractal brownian motion sum variation by Iñigo Quilez that use a mat2 to transform each octave
Simplex::iqMatfBm( const T &v, uint8_t octaves, const glm::mat2 &mat, float gain );
```