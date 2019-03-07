#pragma once

namespace math {
struct vector {
  float x{0};
  float y{0};
  float z{0};
  float w{1};

  vector() = default;
  vector(float x, float y, float z) : x(x), y(y), z(z) {}
  vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  vector &operator+=(const vector &other);
  vector &operator-=(const vector &other);
  vector &operator*=(float factor);
  vector &operator/=(float factor);

  vector operator+(const vector &rhs) const;
  vector operator-(const vector &rhs) const;
  vector operator*(float scalar) const;
  vector operator/(float scalar) const;

  float dot_product(const vector &other) const;
  vector cross_product(const vector &other) const;
  float length() const;
  void normalize();
  vector normalized() const;
};
} // end namespace math
