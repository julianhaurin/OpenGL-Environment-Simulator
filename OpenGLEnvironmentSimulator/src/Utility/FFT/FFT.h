
// keithlantz.net/2011/11/ocean-simulation-part-two-using-the-fast-fourier-transform/

#pragma once

#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>

class FFT 
{
public:
	FFT(const uint32_t N);

	void ComputeFFT(
		std::vector<std::complex<float>> input,
		std::vector<std::complex<float>> output,
		const uint32_t stride,
		const uint32_t offset
	);

private:
	const uint32_t m_N;
	const uint32_t m_log2N;
	const float m_2pi;
	uint32_t m_Which;

	std::vector<uint32_t> m_Reversed;
	std::vector<std::vector<std::complex<float>>> m_C;
	std::vector<std::vector<std::complex<float>>> m_T;
	
	uint32_t reverse(uint32_t i) const;
	std::complex<float> t(uint32_t x, uint32_t N) const;

};


