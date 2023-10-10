#pragma once
//#include "DynamicArray.h"
#include <cmath>
#include <assert.h>

template<typename T>
class DynamicArray;

namespace Cnum
{

	template<typename T>
	class Quaternion
	{

	public:
		Quaternion(const DynamicArray<T>& arr)
			: m_real{ arr[0] }, m_imag{ arr.extract(1, -1) }
		{
			assert(arr.nDims() == 1);
			assert(arr.size() == 4);
		}

		Quaternion(T realPart, const DynamicArray<T>& imaginaryPart)
			: m_real{ realPart }, m_imag{ imaginaryPart }
		{
			assert(m_imag.nDims() == 1);
			assert(m_imag.size() == 3);
		}


		Quaternion<T> operator*(const DynamicArray<T>& rhs) {
			assert(rhs.size() == 3);
			assert(rhs.nDims() == 1);
			return *this * Quaternion<T>(0, rhs);
		}
		Quaternion<T> operator*(const Quaternion<T>& rhs)
		{
			DynamicArray<T>imag = DynamicArray<T>();
			T realPart = m_real * rhs.m_real - m_imag[0] * rhs.m_imag[0] - m_imag[0] * rhs.m_imag[0] - m_imag[0] * rhs.m_imag[0];
			imag.append(m_real * rhs.m_imag[0] + m_imag[0] * rhs.m_real + m_imag[1] * rhs.m_imag[2] - m_imag[2] * rhs.m_imag[1]);
			imag.append(m_real * rhs.m_imag[1] - m_imag[0] * rhs.m_imag[2] + m_imag[1] * rhs.m_real + m_imag[2] * rhs.m_imag[0]);
			imag.append(m_real * rhs.m_imag[2] + m_imag[0] * rhs.m_imag[1] - m_imag[1] * rhs.m_imag[0] + m_imag[2] * rhs.m_real);
			return Quaternion(realPart, imag);
		}

		static DynamicArray<T> Rotate(DynamicArray<T> pos, DynamicArray<T>&& axisOfRotation, T theta) {
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
		DynamicArray<T> to3D() {
			return m_imag;
		}

	private:
		T m_real;
		DynamicArray<T> m_imag;

	};
}


