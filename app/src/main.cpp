
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Camera.h"
#include "Cube.h"
#include "Engine.h"
#include "Gui.h"
#include "Object.h"

int main(int argc, char **argv) {
  float screenwidth = 1280;
  float screenheight = 720;

  auto &engine = Engine::get();
  engine.init("LINAL", screenwidth, screenheight);

  Cube cube;
  Object object("objs/spaceship_tri.obj");

  Camera camera({screenwidth, screenheight, screenheight / screenheight, 90.f,
                 0.1f, 1000.f});

  engine.run([&](float dt) {
    camera.transform(cube);
    camera.transform(object);

    GUI::get().draw(&camera, {&cube, &object});

    engine.draw(cube);
    engine.draw(object);
  });
}
