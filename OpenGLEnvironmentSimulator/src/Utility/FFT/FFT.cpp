
#include "FFT.h"

FFT::FFT(uint32_t N) 
	: m_N(N), m_2pi(M_PI * 2), m_log2N(log(N)/log(2)),
	  m_Reversed(std::vector<uint32_t>(N)), m_Which(0),
	  m_C(std::vector<std::vector<std::complex<float>>>(2)),
	  m_T(std::vector<std::vector<std::complex<float>>>(m_log2N))
{
	// bit reversal prep
	for (int i = 0; i < N; i++) {
		m_Reversed[i] = reverse(i); 
	}

	uint32_t pow2 = 1;
	for (int j = 0; j < m_log2N; j++) {
		m_T[j] = std::vector<std::complex<float>>(pow2);
		for (int k = 0; k < pow2; k++) {
			m_T[j][k] = t(k, pow2 * 2);
			
		}
		pow2 *= 2;
	}

	m_C[0] = std::vector<std::complex<float>>(N);
	m_C[1] = std::vector<std::complex<float>>(N);

}

void FFT::ComputeFFT(
	std::vector<std::complex<float>> input,
	std::vector<std::complex<float>> output,
	const uint32_t stride,
	const uint32_t offset
	) 
{
	for (int i = 0; i < m_N; i++) {
		m_C[m_Which][i] = input[m_Reversed[i] * stride + offset];
	}

	int loops = m_N >> 1;
	int size = 1 << 1;
	int sizeOver2 = 1;
	int w_ = 0;
	for (int i = 1; i <= m_log2N; i++) {
		m_Which ^= 1;
		for (int j = 0; j < loops; j++) {
			for (int k = 0; k < sizeOver2; k++) {	
				m_C[m_Which][size * j + k] = 
					m_C[m_Which ^ 1][size * j + k] +
					m_C[m_Which ^ 1][size * j + sizeOver2 + k] * m_T[w_][k];
			}

			for (int k = sizeOver2; k < size; k++) {
				m_C[m_Which][size * j + k] = 
					m_C[m_Which ^ 1][size * j - sizeOver2 + k] -
					m_C[m_Which ^ 1][size * j + k] * m_T[w_][k - sizeOver2];
			}
		}
		loops >>= 1;
		size <<= 1;
		sizeOver2 <<= 1;
		w_++;

	}

	for (int i = 0; i < m_N; i++) {
		output[i * stride + offset] = m_C[m_Which][i];
	}

}

// private member functions

uint32_t FFT::reverse(uint32_t i) const {
	uint32_t res = 0;
	for (int j = 0; j < m_log2N; j++) {
		res = (res << 1) + (i & 1);
		i >>= 1;
	}
	return res;
}

std::complex<float> FFT::t(uint32_t x, uint32_t N) const {
	return std::complex<float>(cos(m_2pi * x / N), sin(m_2pi * x / N));

}


