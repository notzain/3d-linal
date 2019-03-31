#include "vector.hpp"
#include "math.hpp"

#include <cmath>

namespace math {
vector &vector::operator+=(const vector &other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}
vector &vector::operator-=(const vector &other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}
vector &vector::operator*=(float factor) {
  x *= factor;
  y *= factor;
  z *= factor;
  return *this;
}
vector &vector::operator/=(float factor) {
  x /= factor;
  y /= factor;
  z /= factor;
  return *this;
}

vector vector::operator+(const vector &rhs) const {
  auto other = *this;
  other += rhs;
  return other;
}
vector vector::operator-(const vector &rhs) const {
  auto other = *this;
  other -= rhs;
  return other;
}
vector vector::operator*(float scalar) const {
  auto other = *this;
  other *= scalar;
  return other;
}
vector vector::operator/(float scalar) const {
  auto other = *this;
  other /= scalar;
  return other;
}
float vector::dot_product(const vector &other) const {
  return x * other.x + y * other.y + z * other.z;
}

vector vector::cross_product(const vector &other) const {
  return {y * other.z - z * other.y, z * other.x - x * other.z,
          x * other.y - y * other.x};
}

float vector::length() const { return sqrtf(dot_product(*this)); }

void vector::normalize() {
  const auto len = length();
  *this /= len;
}
vector vector::normalized() const {
  auto other = *this;
  other.normalize();

  return other;
}

} // namespace math