//transformations.h
#pragma once
#include "../ew/ewMath/ewMath.h"

namespace myLib {
	//Identity matrix
	inline ew::Mat4 Identity() {
		ew::Mat4 ident(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return ident;
	};
	//Scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s) {
		ew::Mat4 scale(
			s.x, 0.0f, 0.0f, 0.0f,
			0.0f, s.y, 0.0f, 0.0f,
			0.0f, 0.0f, s.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return scale;
	};
	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		ew::Mat4 rotationX(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos(rad), -sin(rad), 0.0f,
			0.0f, sin(rad), cos(rad), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return rotationX;
	};
	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		ew::Mat4 rotationY(
			cos(rad), 0.0f, sin(rad), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(rad), 0.0f, cos(rad), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return rotationY;
	};
	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		ew::Mat4 rotationZ(
			cos(rad), -sin(rad), 0.0f, 0.0f,
			sin(rad), cos(rad), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return rotationZ;
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t) {
		ew::Mat4 transform(
			1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return transform;
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		
		ew::Mat4 getModelMatrix() const {
			ew::Mat4 modelMat = Translate(position) * RotateY(ew::Radians(rotation.y)) * RotateX(ew::Radians(rotation.x)) * RotateZ(ew::Radians(rotation.z)) * Scale(scale) * Identity();
			return modelMat;
		}
	};

}
