
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

int main(int argc, char **argv) {
  float screenwidth = 1280;
  float screenheight = 720;

  Game game("LINAL", screenwidth, screenheight);

  game.add_object(std::make_unique<Cube>());
  game.add_target(std::make_unique<Object>("objs/sphere.obj"));

  game.run();
}
