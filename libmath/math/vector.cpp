#include "vector.hpp"

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
} // namespace math