#pragma once
#include "../Cnum/Cnum.h"
// add concept to be only numeric

template<typename T>
class RigidBody {

public: 

	struct timeDifferentiatedState {
		DynamicArray<T> linearVelocity;
		DynamicArray<T> ddt_rotationMatrix;
		DynamicArray<T> ddt_linearMomentum;
		DynamicArray<T> ddt_angularMomentum;
	};
	struct State {
		DynamicArray<T> position;
		DynamicArray<T> rotationMatrix;
		DynamicArray<T> linearMomentum;
		DynamicArray<T> angularMomentum;
	};


	void update(const State& state) {
		m_globalPosition = state.position;
		m_rotationMatrix = state.rotationMatrix;
		m_linearMomentum = state.linearMomentum;
		m_angularMomentum = state.angularMomentum; 

		m_translationalVelocity = state.linearMomentum / m_mass; 
		m_globalInertiaTensorInverse = state.rotationMatrix * m_localInertiaTensorInverse * state.rotationMatrix.transpose(); 
		m_angualarVelocity = m_globalInertiaTensorInverse * m_angularMomentum;
	}



private: 

	// Contant properties
	const T m_mass; 
	const DynamicArray<T> m_localInertiaTensor; 
	const DynamicArray<T> m_localInertiaTensorInverse; 

	// State variables
	DynamicArray<T> m_globalPosition;
	DynamicArray<T> m_rotationMatrix;
	DynamicArray<T> m_linearMomentum; 
	DynamicArray<T> m_angularMomentum;

	// Derived quantatties
	DynamicArray<T> m_globalInertiaTensorInverse; 
	DynamicArray<T> m_translationalVelocity; 
	DynamicArray<T> m_angualarVelocity; 

	// Computed quantaties
	DynamicArray<T> m_force; 
	DynamicArray<T> m_torque; 

};
