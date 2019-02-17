#pragma once
#include <cstdint>

namespace math {
struct matrix {
private:
  float matrix[4][4] = {0};

public:
  float &operator()(std::size_t x, std::size_t y);
  float operator()(std::size_t x, std::size_t y) const;
};
} // namespace math