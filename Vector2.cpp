#include <iostream>
#include "Vector2.h"

Vector2::Vector2() {

}

Vector2::Vector2(double x, double y) {
    this->x = x;
    this->y = y;
}

void Vector2::normalize() {
    using std::pow, std::sqrt;
    //magnitude of vec
    double m = magnitude();
    if (m != 0)
    {
        x /= m;
        y /= m;
    }
}

void Vector2::rotate(double deg) {
    double x2 = x * cos(deg) - y * sin(deg);
    double y2 = x * sin(deg) + y * cos(deg);
    x = x2;
    y = y2;
}

double Vector2::magnitude() {
    return sqrt(pow(x, 2) + pow(y, 2));
}

Vector2 Vector2::operator/(double d) {
    Vector2 vec(x / d, y / d);
    return vec;
}

Vector2 Vector2::operator-(Vector2 const& vec) {
    Vector2 vect(x - vec.x, y - vec.y);
    return vect;
}

Vector2 Vector2::operator+(Vector2 const& vec) {
    Vector2 vect(x + vec.x, y + vec.y);
    return vect;
}

Vector2 Vector2::operator*(double d) {
    Vector2 vect(x * d, y * d);
    return vect;
}