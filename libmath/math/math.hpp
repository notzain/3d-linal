#pragma once
#include "vector.hpp"
#include "matrix.hpp"
#include <cmath>

namespace math {
    vector multiply(const vector& vec, const matrix& mat) {
        vector temp{0,0,0};
        temp.x = vec.x * mat(0,0) + vec.y * mat(1,0) + vec.z * mat(2,0) + mat(3,0);
        temp.y = vec.x * mat(0,1) + vec.y * mat(1,1) + vec.z * mat(2,1) + mat(3,1);
        temp.z = vec.x * mat(0,2) + vec.y * mat(1,2) + vec.z * mat(2,2) + mat(3,2);

        float w = vec.x * mat(0,3) + vec.y * mat(1,3) + vec.z * mat(2,3) + mat(3,3);
        if (w != 0.0f) {
            temp /= w;
        }

        return temp;
    }

    matrix make_rotation_z(float theta) {
        matrix temp;
        temp(0,0) = cosf(theta);
        temp(0,1) = sinf(theta);
        temp(1,0) = -sinf(theta);
        temp(1,1) = cosf(theta);
        temp(2,2) = 1;
        temp(3,3) = 1;

        return temp;
    }


    matrix make_rotation_x(float theta) {
        matrix temp;
        temp(0,0) = 1;
        temp(1,1) = cosf(theta * 0.5f);
        temp(1,2) = sinf(theta * 0.5f);
        temp(2,1) = -sinf(theta * 0.5f);
        temp(2,2) = cosf(theta * 0.5f);
        temp(3,3) = 1;
        
        return temp;
    }
}