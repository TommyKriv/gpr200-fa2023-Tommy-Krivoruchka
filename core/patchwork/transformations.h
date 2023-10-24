//transformations.h
#pragma once
#include "../ew/ewMath/ewMath.h"

namespace patchwork {
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

	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) {
		//...
		ew::Vec3 forwardVector = ew::Normalize(eye - target);
		ew::Vec3 rightVector = ew::Normalize(ew::Cross(up, forwardVector));
		ew::Vec3 cameraUp = ew::Cross(forwardVector, rightVector);

		ew::Mat4 lookVec(
			rightVector.x, rightVector.y, rightVector.z, -ew::Dot(rightVector, eye),
			cameraUp.x, cameraUp.y, cameraUp.z, -ew::Dot(cameraUp, eye),
			forwardVector.x, forwardVector.y, forwardVector.z, -ew::Dot(forwardVector, eye),
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return lookVec;
	};
	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) {
		float right = (aspect*height) / 2.0f;
		float top = height / 2.0f;
		float left = -right;
		float bottom = -top;

		ew::Mat4 ortho(
			2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
			0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
			0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near),
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return ortho;
	};
	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
		float tangent = tanf(fov / 2.0f);
		
		ew::Mat4 perspective(
			1.0f / (aspect * tangent), 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / tangent, 0.0f, 0.0f,
			0.0f, 0.0f, (far + near) / (near - far), (2 * far * near) / (near - far),
			0.0f, 0.0f, -1.0f, 0.0f
		);

		return perspective;
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		
		ew::Mat4 getModelMatrix() const {
			ew::Mat4 modelMat = patchwork::Translate(position) * RotateY(ew::Radians(rotation.y)) * RotateX(ew::Radians(rotation.x)) * RotateZ(ew::Radians(rotation.z)) * patchwork::Scale(scale) * Identity();
			return modelMat;
		}
	};

}
