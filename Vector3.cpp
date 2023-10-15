#include <iostream>
#include "Vector3.h"

Vector3::Vector3() {

}

Vector3::Vector3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::normalize() {
    using std::pow, std::sqrt;
    //magnitude of vec
    double m = magnitude();
    x /= m;
    y /= m;
    z /= m;
}

//rX = rotation around X axis in rads, etc.
//WHAT THE FUCK IS QUARTERNION BASED ROTATION!
void Vector3::rotate(double rX = 0.2, double rY = 0.2, double rZ = 0.2) {
    using std::sin, std::cos;

    double halfThetaX = rX * 0.5;
    double halfThetaY = rY * 0.5;
    double halfThetaZ = rZ * 0.5;

    double sinHalfThetaX = sin(halfThetaX);
    double cosHalfThetaX = cos(halfThetaX);
    double sinHalfThetaY = sin(halfThetaY);
    double cosHalfThetaY = cos(halfThetaY);
    double sinHalfThetaZ = sin(halfThetaZ);
    double cosHalfThetaZ = cos(halfThetaZ);

    double qx = sinHalfThetaX * cosHalfThetaY * cosHalfThetaZ - cosHalfThetaX * sinHalfThetaY * sinHalfThetaZ;
    double qy = cosHalfThetaX * sinHalfThetaY * cosHalfThetaZ + sinHalfThetaX * cosHalfThetaY * sinHalfThetaZ;
    double qz = cosHalfThetaX * cosHalfThetaY * sinHalfThetaZ - sinHalfThetaX * sinHalfThetaY * cosHalfThetaZ;
    double qw = cosHalfThetaX * cosHalfThetaY * cosHalfThetaZ + sinHalfThetaX * sinHalfThetaY * sinHalfThetaZ;

    double newX = x * (qw * qw + qx * qx - qy * qy - qz * qz) +
        y * (2 * qx * qy - 2 * qw * qz) +
        z * (2 * qx * qz + 2 * qw * qy);
    double newY = x * (2 * qx * qy + 2 * qw * qz) +
        y * (qw * qw - qx * qx + qy * qy - qz * qz) +
        z * (2 * qy * qz - 2 * qw * qx);
    double newZ = x * (2 * qx * qz - 2 * qw * qy) +
        y * (2 * qy * qz + 2 * qw * qx) +
        z * (qw * qw - qx * qx - qy * qy + qz * qz);

    x = newX;
    y = newY;
    z = newZ;
}

void Vector3::setRotation(double rX, double rY, double rZ) {
    double halfThetaX = rX * 0.5;
    double halfThetaY = rY * 0.5;
    double halfThetaZ = rZ * 0.5;

    double sinHalfThetaX = sin(halfThetaX);
    double cosHalfThetaX = cos(halfThetaX);
    double sinHalfThetaY = sin(halfThetaY);
    double cosHalfThetaY = cos(halfThetaY);
    double sinHalfThetaZ = sin(halfThetaZ);
    double cosHalfThetaZ = cos(halfThetaZ);

    // Calculate the quaternion components for the desired rotation angles
    double qx = sinHalfThetaX * cosHalfThetaY * cosHalfThetaZ - cosHalfThetaX * sinHalfThetaY * sinHalfThetaZ;
    double qy = cosHalfThetaX * sinHalfThetaY * cosHalfThetaZ + sinHalfThetaX * cosHalfThetaY * sinHalfThetaZ;
    double qz = cosHalfThetaX * cosHalfThetaY * sinHalfThetaZ - sinHalfThetaX * sinHalfThetaY * cosHalfThetaZ;
    double qw = cosHalfThetaX * cosHalfThetaY * cosHalfThetaZ + sinHalfThetaX * sinHalfThetaY * sinHalfThetaZ;

    // Normalize the quaternion
    double magnitude = sqrt(qw * qw + qx * qx + qy * qy + qz * qz);
    qw /= magnitude;
    qx /= magnitude;
    qy /= magnitude;
    qz /= magnitude;

    // Apply the rotation directly using the conjugate quaternion
    double newX = x * (qw * qw + qx * qx - qy * qy - qz * qz) +
        y * (2 * qx * qy - 2 * qw * qz) +
        z * (2 * qx * qz + 2 * qw * qy);
    double newY = x * (2 * qx * qy + 2 * qw * qz) +
        y * (qw * qw - qx * qx + qy * qy - qz * qz) +
        z * (2 * qy * qz - 2 * qw * qx);
    double newZ = x * (2 * qx * qz - 2 * qw * qy) +
        y * (2 * qy * qz + 2 * qw * qx) +
        z * (qw * qw - qx * qx - qy * qy + qz * qz);

    // Update the vector's components with the rotated values
    x = newX;
    y = newY;
    z = newZ;
}

//void Vector3::rotate(Vector3 u, double rad) {
//    using std::cos, std::sin, std::pow;
//    double tX = x, tY = y, tZ = z;
//    x = tX * (cos(rad) + pow(u.x, 2) * (1 - cos(rad))) + tY * (u.x * u.y * (1 - cos(rad) - u.z * sin(rad))) + tZ * (u.x * u.z * (1 - cos(rad)) + u.y * sin(rad));
//    y = tX * (u.y * u.x * (1 - cos(rad)) + u.z * sin(rad)) + tY * (cos(rad) + u.y * u.y * (1 - cos(rad))) + tZ * (u.y * u.z * (1 - cos(rad)) - u.x * sin(rad));
//    z = tX * (u.z * u.x * (1 - cos(rad)) - u.y * sin(rad)) + tY * (u.z * u.y * (1 - cos(rad)) + u.x * sin(rad)) + tZ * (cos(rad) + u.z * u.z * (1 - cos(rad)));
//}

double Vector3::magnitude() {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3 Vector3::operator/(double d) {
	Vector3 vec(x/d, y/d, z/d);
	return vec;
}

Vector3 Vector3::operator-(Vector3 const& vec) {
    Vector3 vect(x - vec.x, y - vec.y, z - vec.z);
    return vect;
}

Vector3 Vector3::operator+(Vector3 const& vec) {
    Vector3 vect(x + vec.x, y + vec.y, z + vec.z);
    return vect;
}

Vector3 Vector3::operator*(double d) {
    Vector3 vect(x * d, y * d, z * d);
    return vect;
}