//
// Created by Agamjeet Singh on 21/09/25.
//

#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <cmath>

namespace eng {
    class Vector3f {
    public:
        Vector3f(const float x, const float y, const float z): x(x), y(y), z(z) {}

        Vector3f(): x(0.0f), y(0.0f), z(0.0f) {}

        float x, y, z;

        Vector3f operator+(const Vector3f& other) const { return {x+other.x, y+other.y, z+other.z}; }
        Vector3f operator-(const Vector3f& other) const { return {x-other.x, y-other.y, z-other.z}; }
        Vector3f operator*(const float scalar) const { return {x*scalar, y*scalar, z*scalar}; }
        Vector3f operator/(const float scalar) const { return {x/scalar, y/scalar, z/scalar}; }

        friend Vector3f operator*(const float scalar, const Vector3f& v) { return v * scalar; }

        Vector3f operator+() const { return *this; }
        Vector3f operator-() const { return {-x, -y, -z}; }

        Vector3f& operator+=(const Vector3f& other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
        Vector3f& operator-=(const Vector3f& other) { x-=other.x; y-=other.y; z-=other.z; return *this; }
        Vector3f& operator*=(float scalar) { x*=scalar; y*=scalar; z*=scalar; return *this; }
        Vector3f& operator/=(float scalar) { x/=scalar; y/=scalar; z/=scalar; return *this; }

        bool operator==(const Vector3f& other) const { return x==other.x && y==other.y && z==other.z; }

        float dot(const Vector3f& other) const { return x*other.x + y*other.y + z*other.z; }

        float length() const { return std::sqrt(x*x + y*y + z*z); }

        Vector3f normalized() const { const float len = length(); return len>0 ? *this/len : Vector3f(); }
    };
}


#endif //VECTOR3F_H
