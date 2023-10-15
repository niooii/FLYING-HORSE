#pragma once

class Vector2 {

public:
	double x{}, y{};
	Vector2();
	Vector2(double x, double y);

	void normalize();
	void rotate(double deg);

	double magnitude();

	Vector2 operator/(double d);
	Vector2 operator-(Vector2 const& vec);
	Vector2 operator+(Vector2 const& vec);
	Vector2 operator*(double d);
};