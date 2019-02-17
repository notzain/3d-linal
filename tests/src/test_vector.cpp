#include "catch.hpp"
#include <math/vector.hpp>

using namespace math;
TEST_CASE("Vectors can be made", "[vector]") {
  const vector vec{1.f, 1.f, 1.f};
  REQUIRE(vec.x == 1.f);
  REQUIRE(vec.y == 1.f);
  REQUIRE(vec.z == 1.f);
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