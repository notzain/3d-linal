#pragma once

#include "matrix.hpp"
#include "vector.hpp"

namespace math {
    vector multiply(const vector& vec, const matrix& mat);

    matrix make_identity();
    matrix make_rotation_x(float angle);

    matrix make_rotation_z(float angle);
    matrix make_rotation_y(float angle);

    matrix make_translation(const vector& translation);

    matrix inverse(const matrix& matrix);

    float dot_product(const vector& a, const vector& b);
}