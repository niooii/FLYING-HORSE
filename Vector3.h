#pragma once

class Vector3 {

public:
	double x{}, y{}, z{};
	Vector3();
	Vector3(double x, double y, double z);

	void normalize();
	void rotate(double rX, double rY, double rZ);
	void setRotation(double rX, double rY, double rZ);

	double magnitude();

	Vector3 operator/(double d);
	Vector3 operator-(Vector3 const& vec);
	Vector3 operator+(Vector3 const& vec);
	Vector3 operator*(double d);
};