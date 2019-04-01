#include "catch.hpp"
#include <math/math.hpp>
#include <sstream>

using namespace math;

namespace Catch {
template <> struct StringMaker<math::vector> {
  static std::string convert(const math::vector &v) {
    std::stringstream os;
    os << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
    return os.str();
  }
};
} // namespace Catch

TEST_CASE("Matrices can be multiplied", "[matrix]") {
  matrix m = make_identity();
  m(0, 0) = 2;
  m(1, 1) = 3;
  m(2, 2) = 4;
  m(3, 3) = 5;

  matrix n = make_identity();
  n(0, 0) = 5;
  n(1, 1) = 4;
  n(2, 2) = 3;
  n(3, 3) = 2;

  SECTION("m * n") {
    matrix o = m * n;
    // https://matrixcalc.org/en/#%7B%7B2,0,0,0%7D,%7B0,3,0,0%7D,%7B0,0,4,0%7D,%7B0,0,0,5%7D%7D%2A%7B%7B5,0,0,0%7D,%7B0,4,0,0%7D,%7B0,0,3,0%7D,%7B0,0,0,2%7D%7D
    REQUIRE(o(0, 0) == 10.f);
    REQUIRE(o(1, 1) == 12.f);
    REQUIRE(o(2, 2) == 12.f);
    REQUIRE(o(3, 3) == 10.f);
  }

  SECTION("n * m") {
    matrix o = n * m;
    // https://matrixcalc.org/en/#%7B%7B2,0,0,0%7D,%7B0,3,0,0%7D,%7B0,0,4,0%7D,%7B0,0,0,5%7D%7D%2A%7B%7B5,0,0,0%7D,%7B0,4,0,0%7D,%7B0,0,3,0%7D,%7B0,0,0,2%7D%7D
    REQUIRE(o(0, 0) == 10.f);
    REQUIRE(o(1, 1) == 12.f);
    REQUIRE(o(2, 2) == 12.f);
    REQUIRE(o(3, 3) == 10.f);
  }
}

TEST_CASE("Matrix can be multiplied with vector", "[matrix]") {
  matrix m = make_identity();
  matrix n = make_identity();
  n(0, 0) = 2;

  vector v{1, 2, 3, 4};

  // http://www.calcul.com/show/calculator/matrix-multiplication?matrix1=[[%222%22,%220%22,%220%22,%220%22],[%220%22,%221%22,%220%22,%220%22],[%220%22,%220%22,%221%22,%220%22],[%220%22,%220%22,%220%22,%221%22]]&matrix2=[[%221%22],[%222%22],[%223%22],[%224%22]]&operator=*
  REQUIRE(multiply(v, m) == vector{1, 2, 3, 4});
  REQUIRE(multiply(v, n) == vector{2, 2, 3, 4});
}

TEST_CASE("Rotation matrices can rotate a vector", "[matrix]") {
  vector v{1, 1, 1};

  SECTION("X axis") {
    // http://www.nh.cas.cz/people/lazar/celler/online_tools.php?start_vec=1,1,1&rot_ax=1,0,0&rot_ang=33
    auto result = multiply(v, make_rotation_x(to_radians(33)));
    REQUIRE(result.x == Approx(1.f));
    REQUIRE(result.y == Approx(0.29).epsilon(0.1));
    REQUIRE(result.z == Approx(1.38).epsilon(0.1));
  }

  SECTION("Y axis") {
    // http://www.nh.cas.cz/people/lazar/celler/online_tools.php?start_vec=1,1,1&rot_ax=0,1,0&rot_ang=33
    auto result = multiply(v, make_rotation_y(to_radians(33)));
    REQUIRE(result.x == Approx(0.29).epsilon(0.1));
    REQUIRE(result.y == Approx(1.f));
    REQUIRE(result.z == Approx(1.38).epsilon(0.1));
  }
  SECTION("Z axis") {
    // http://www.nh.cas.cz/people/lazar/celler/online_tools.php?start_vec=1,1,1&rot_ax=0,1,0&rot_ang=33
    auto result = multiply(v, make_rotation_z(to_radians(33)));
    REQUIRE(result.x == Approx(0.29).epsilon(0.1));
    REQUIRE(result.y == Approx(1.38).epsilon(0.1));
    REQUIRE(result.z == Approx(1.f));
  }
}
