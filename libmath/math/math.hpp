#pragma once

namespace math {
    struct vector;
    class matrix;
    vector multiply(const vector& vec, const matrix& mat);
    matrix make_rotation_x(float angle);

    matrix make_rotation_z(float angle);
    matrix make_rotation_y(float angle);
    float dot_product(const vector& a, const vector& b);
}