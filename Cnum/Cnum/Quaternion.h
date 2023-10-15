#pragma once
//#include "ndArray.h"
#include <cmath>
#include <assert.h>

template<typename T>
class ndArray;

namespace Cnum
{

	template<typename T>
	class Quaternion
	{

	public:
		Quaternion(const ndArray<T>& arr)
			: m_real{ arr[0] }, m_imag{ arr.extract(1, -1) }
		{
			assert(arr.nDims() == 1);
			assert(arr.size() == 4);
		}

		Quaternion(T realPart, const ndArray<T>& imaginaryPart)
			: m_real{ realPart }, m_imag{ imaginaryPart }
		{
			assert(m_imag.nDims() == 1);
			assert(m_imag.size() == 3);
		}


		Quaternion<T> operator*(const ndArray<T>& rhs) {
			assert(rhs.size() == 3);
			assert(rhs.nDims() == 1);
			return *this * Quaternion<T>(0, rhs);
		}
		Quaternion<T> operator*(const Quaternion<T>& rhs)
		{
			ndArray<T>imag = ndArray<T>();
			T realPart = m_real * rhs.m_real - m_imag[0] * rhs.m_imag[0] - m_imag[0] * rhs.m_imag[0] - m_imag[0] * rhs.m_imag[0];
			imag.append(m_real * rhs.m_imag[0] + m_imag[0] * rhs.m_real + m_imag[1] * rhs.m_imag[2] - m_imag[2] * rhs.m_imag[1]);
			imag.append(m_real * rhs.m_imag[1] - m_imag[0] * rhs.m_imag[2] + m_imag[1] * rhs.m_real + m_imag[2] * rhs.m_imag[0]);
			imag.append(m_real * rhs.m_imag[2] + m_imag[0] * rhs.m_imag[1] - m_imag[1] * rhs.m_imag[0] + m_imag[2] * rhs.m_real);
			return Quaternion(realPart, imag);
		}

		static ndArray<T> Rotate(ndArray<T> pos, ndArray<T>&& axisOfRotation, T theta) {
			assert(pos.size() == 3);
			assert(pos.nDims() == 1);
			assert(axisOfRotation.size() == 3);
			assert(axisOfRotation.nDims() == 1);

			theta /= (180.0f / 3.14159265f);
			axisOfRotation.normalize();
			Quaternion q1(std::cos(theta / 2), axisOfRotation * std::sin(theta / 2));
			Quaternion qRes = q1 * pos * q1.inverse();
			return qRes.m_imag.normalize();
		}
		Quaternion inverse() {
			return Quaternion(m_real, m_imag * -1);
		}
		ndArray<T> to3D() {
			return m_imag;
		}

	private:
		T m_real;
		ndArray<T> m_imag;

	};
}


