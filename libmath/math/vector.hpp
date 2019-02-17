#pragma once

namespace math {
struct vector {
  float x{0};
  float y{0};

  float z{0};
  //float w{1};

  vector &operator+=(const vector &other);
  vector &operator-=(const vector &other);
  vector &operator*=(float factor);
  vector &operator/=(float factor);

  vector operator+(const vector &rhs) const;
  vector operator-(const vector &rhs) const;
  vector operator*(float scalar) const;
  vector operator/(float scalar) const;
};
} // end namespace math
