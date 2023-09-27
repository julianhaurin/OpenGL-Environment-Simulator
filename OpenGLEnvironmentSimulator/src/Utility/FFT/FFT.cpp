
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


/*
void Ocean::EvaluateWavesFFT(const float time) {

	float kx, kz, len, lambda = -1.0f;
	uint32_t index, index1;

	const uint32_t N = m_GridSideDimension;
	const uint32_t Nplus1 = N + 1;

	for (int m_prime = 0; m_prime < N; m_prime++) {
		kz = M_PI * (2.0f * m_prime - N) / m_Length;
		for (int n_prime = 0; n_prime < N; n_prime++) {
			kx = M_PI * (2 * n_prime - N) / m_Length;
			len = sqrt(kx * kx + kz * kz);
			index = m_prime * N + n_prime;

			m_HTilde[index] = hTilde(time, n_prime, m_prime);
			m_HTildeSlopeX[index] = m_HTilde[index] * std::complex<float>(0, kx);
			m_HTildeSlopeZ[index] = m_HTilde[index] * std::complex<float>(0, kz);
			if (len < 0.000001f) {
				m_HTildeDX[index] = std::complex<float>(0.0f, 0.0f);
				m_HTildeDZ[index] = std::complex<float>(0.0f, 0.0f);
			}
			else {
				m_HTildeDX[index] = m_HTilde[index] * std::complex<float>(0, -kx / len);
				m_HTildeDZ[index] = m_HTilde[index] * std::complex<float>(0, -kz / len);
			}
		}
	}

	for (int m_prime = 0; m_prime < N; m_prime++) {
		m_FFT.ComputeFFT(m_HTilde, m_HTilde, 1, m_prime * N);
		m_FFT.ComputeFFT(m_HTildeSlopeX, m_HTildeSlopeX, 1, m_prime * N);
		m_FFT.ComputeFFT(m_HTildeSlopeZ, m_HTildeSlopeZ, 1, m_prime * N);
		m_FFT.ComputeFFT(m_HTildeDX, m_HTildeDX, 1, m_prime * N);
		m_FFT.ComputeFFT(m_HTildeDZ, m_HTildeDZ, 1, m_prime * N);

	}
	for (int n_prime = 0; n_prime < N; n_prime++) {
		//m_FFT.ComputeFFT(m_HTilde, m_HTilde, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeSlopeX, m_HTildeSlopeX, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeSlopeZ, m_HTildeSlopeZ, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeDX, m_HTildeDX, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeDZ, m_HTildeDZ, N, n_prime);

	}

	int sign;
	float signs[] = { 1.0f, -1.0f };
	glm::vec3 n;
	for (int m_prime = 0; m_prime < N; m_prime++) {
		for (int n_prime = 0; n_prime < N; n_prime++) {
			index = m_prime * N + n_prime;     // index into m_HTilde..
			index1 = m_prime * Nplus1 + n_prime;    // index into vertices

			sign = signs[(n_prime + m_prime) & 1];

			m_HTilde[index].real(m_HTilde[index].real() * sign);
			m_HTilde[index].imag(m_HTilde[index].imag() * sign);

			// height
			m_Vertices[index1].y = m_HTilde[index].real();

			// displacement
			m_HTildeDX[index].real(m_HTildeDX[index].real() * sign);
			m_HTildeDX[index].imag(m_HTildeDX[index].imag() * sign);
			m_HTildeDZ[index].real(m_HTildeDZ[index].real() * sign);
			m_HTildeDZ[index].imag(m_HTildeDZ[index].imag() * sign);

			m_Vertices[index1].x = m_Vertices[index1].ox + m_HTildeDX[index].real() * lambda;
			m_Vertices[index1].z = m_Vertices[index1].oz + m_HTildeDZ[index].real() * lambda;

			// normal
			m_HTildeSlopeX[index].real(m_HTildeSlopeX[index].real() * sign);
			m_HTildeSlopeX[index].imag(m_HTildeSlopeX[index].imag() * sign);
			m_HTildeSlopeZ[index].real(m_HTildeSlopeZ[index].real() * sign);
			m_HTildeSlopeZ[index].imag(m_HTildeSlopeZ[index].imag() * sign);

			n = glm::normalize(glm::vec3(0.0f - m_HTildeSlopeX[index].real(), 1.0f, 0.0f - m_HTildeSlopeZ[index].real()));
			m_Vertices[index1].nx = n.x;
			m_Vertices[index1].ny = n.y;
			m_Vertices[index1].nz = n.z;

			// for tiling
			if (n_prime == 0 && m_prime == 0) {
				m_Vertices[index1 + N + Nplus1 * N].y = m_HTilde[index].real();

				m_Vertices[index1 + N + Nplus1 * N].x = m_Vertices[index1 + N + Nplus1 * N].ox + m_HTildeDX[index].real() * lambda;
				m_Vertices[index1 + N + Nplus1 * N].z = m_Vertices[index1 + N + Nplus1 * N].oz + m_HTildeDZ[index].real() * lambda;

				m_Vertices[index1 + N + Nplus1 * N].nx = n.x;
				m_Vertices[index1 + N + Nplus1 * N].ny = n.y;
				m_Vertices[index1 + N + Nplus1 * N].nz = n.z;
			}
			if (n_prime == 0) {
				m_Vertices[index1 + N].y = m_HTilde[index].real();

				m_Vertices[index1 + N].x = m_Vertices[index1 + N].ox + m_HTildeDX[index].real() * lambda;
				m_Vertices[index1 + N].z = m_Vertices[index1 + N].oz + m_HTildeDZ[index].real() * lambda;

				m_Vertices[index1 + N].nx = n.x;
				m_Vertices[index1 + N].ny = n.y;
				m_Vertices[index1 + N].nz = n.z;
			}
			if (m_prime == 0) {
				m_Vertices[index1 + Nplus1 * N].y = m_HTilde[index].real();

				m_Vertices[index1 + Nplus1 * N].x = m_Vertices[index1 + Nplus1 * N].ox + m_HTildeDX[index].real() * lambda;
				m_Vertices[index1 + Nplus1 * N].z = m_Vertices[index1 + Nplus1 * N].oz + m_HTildeDZ[index].real() * lambda;

				m_Vertices[index1 + Nplus1 * N].nx = n.x;
				m_Vertices[index1 + Nplus1 * N].ny = n.y;
				m_Vertices[index1 + Nplus1 * N].nz = n.z;
			}
		}
	}

}
*/

