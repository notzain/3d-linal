#include "catch.hpp"
#include <math/vector.hpp>
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

TEST_CASE("Vectors can be made", "[vector]") {
  const vector vec{1.f, 1.f, 1.f};
  REQUIRE(vec.x == 1.f);
  REQUIRE(vec.y == 1.f);
  REQUIRE(vec.z == 1.f);
}

TEST_CASE("Vectors can be compared", "[vector]") {
  vector a{5, 6, 7};
  vector b{1, 2, 3};
  vector c{1, 2, 3};

  SECTION("Equals") { REQUIRE(b == c); };
  SECTION("NEquals") { REQUIRE(a != b); };
}

TEST_CASE("2 vectors can be added", "[vector]") {
  vector vec{1.f, 1.f, 1.f};
  vector to_add{2.f, 3.f, 4.f};

  SECTION("As compound assignment") {
    vec += to_add;
    REQUIRE(vec.x == 3.f);
    REQUIRE(vec.y == 4.f);
    REQUIRE(vec.z == 5.f);
  }
  SECTION("As new vector") {
    auto added = vec + to_add;
    REQUIRE(added.x == 3.f);
    REQUIRE(added.y == 4.f);
    REQUIRE(added.z == 5.f);
  }
}

TEST_CASE("2 vectors can be subtracted", "[vector]") {
  vector vec{2.f, 3.f, 4.f};
  vector to_subtract{1.f, 1.f, 1.f};

  SECTION("As compound assignment") {
    vec -= to_subtract;
    REQUIRE(vec.x == 1.f);
    REQUIRE(vec.y == 2.f);
    REQUIRE(vec.z == 3.f);
  }
  SECTION("As new vector") {
    auto added = vec - to_subtract;
    REQUIRE(added.x == 1.f);
    REQUIRE(added.y == 2.f);
    REQUIRE(added.z == 3.f);
  }
}

TEST_CASE("Vector can be multiplied by scalar", "[vector]") {
  vector vec{1.f, 2.f, 3.f};
  float scalar = 5.f;

  SECTION("As compound assignment") {
    vec *= scalar;
    REQUIRE(vec.x == 5.f);
    REQUIRE(vec.y == 10.f);
    REQUIRE(vec.z == 15.f);
  }
  SECTION("As new vector") {
    auto added = vec * scalar;
    REQUIRE(added.x == 5.f);
    REQUIRE(added.y == 10.f);
    REQUIRE(added.z == 15.f);
  }
}

TEST_CASE("Vector can be divided by scalar", "[vector]") {
  vector vec{5.f, 10.f, 15.f};
  float scalar = 5.f;

  SECTION("As compound assignment") {
    vec /= scalar;
    REQUIRE(vec.x == 1.f);
    REQUIRE(vec.y == 2.f);
    REQUIRE(vec.z == 3.f);
  }
  SECTION("As new vector") {
    auto added = vec / scalar;
    REQUIRE(added.x == 1.f);
    REQUIRE(added.y == 2.f);
    REQUIRE(added.z == 3.f);
  }
}

TEST_CASE("Dot product of 2 vectors", "[vector") {
  vector a{5, 6, 7};
  vector b{1, 2, 3};

  // https://onlinemschool.com/math/assistance/vector/multiply/
  SECTION("a dot b") { REQUIRE(a.dot_product(b) == 38); }

  SECTION("b dot a") { REQUIRE(b.dot_product(a) == 38); }
}

TEST_CASE("Cross product of 2 vectors", "[vector") {
  vector a{5, 6, 7};
  vector b{1, 2, 3};

  // https://onlinemschool.com/math/assistance/vector/multiply1/
  SECTION("a cross b") { REQUIRE(a.cross_product(b) == vector{4, -8, 4}); }

  SECTION("b cross a") { REQUIRE(b.cross_product(a) == vector{-4, 8, -4}); }
}

TEST_CASE("Length/Mag of a vector", "[vector]") {
  vector a{1, 2, 3};
  // https://onlinemschool.com/math/assistance/vector/length/
  REQUIRE(a.length() == Approx(3.74165f).epsilon(0.01));
}
