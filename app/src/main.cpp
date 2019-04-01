
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Bullet.h"
#include "Camera.h"
#include "Cube.h"
#include "Engine.h"
#include "Game.h"
#include "Gui.h"
#include "Object.h"
#include "Spaceship.h"

#include <memory>
#include <random>

float random(int min, int max) {
  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(min, max);
  return dis(gen);
}

int main(int argc, char **argv) {
  float screenwidth = 1280;
  float screenheight = 720;

  srand(time(NULL));

  Game game("LINAL", screenwidth, screenheight);

  for (int i = 0; i < 20; ++i) {
    game.add_object(std::make_unique<Cube>(
        math::vector{random(-50, 50), random(-50, 50), random(-50, 50)}));
  }
  game.add_target(std::make_unique<Object>(
      "objs/sphere.obj",
      math::vector{random(-50, 50), random(-50, 50), random(-50, 50)}));
  game.add_target(std::make_unique<Object>(
      "objs/suzanne.obj",
      math::vector{random(-50, 50), random(-50, 50), random(-50, 50)}));

  game.run();
}
