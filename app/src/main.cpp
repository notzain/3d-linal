
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

struct PulsingMesh {
  float maxPulse;
  float pulseStep;
  float currentPulse;
  bool isExpanding{false};
  std::vector<Mesh *> meshes;

  void pulse(float elapsedTime) {
    if (isExpanding) {
      currentPulse += pulseStep * elapsedTime;
      if (currentPulse >= maxPulse) {
        isExpanding = false;
      }
    } else {
      currentPulse -= pulseStep * elapsedTime;
      if (currentPulse <= 1.f) {
        isExpanding = true;
      }
    }
  }
};

int main(int argc, char **argv) {
  float screenwidth = 1280;
  float screenheight = 720;

  auto &engine = Engine::get();
  engine.init("LINAL", 1270, 720);

  Cube cube;
  Object object("objs/axis.obj");

  PulsingMesh pulsing{1.5, 0.3, 1, false, {&cube}};

  Camera camera({screenwidth, screenheight, screenheight / screenheight, 90.f,
                 0.1f, 1000.f});

  engine.run([&](float dt) {
    camera.transform(cube);
    camera.transform(object);

    GUI::get().draw(&camera, {&cube, &object});

    engine.draw(cube);
    //engine.draw(object);
  });
}
