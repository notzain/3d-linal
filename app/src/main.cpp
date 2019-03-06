
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

    for (auto *mesh : meshes) {
      mesh->scale(currentPulse);
    }
  }
};

int main(int argc, char **argv) {
  float screenwidth = 800;
  float screenheight = 600;

  auto &engine = Engine::create("Linal", screenwidth, screenheight);

  Cube cube;
  //Object object("axis.obj");

  PulsingMesh pulsing{1.5, 0.3, 1, false, {&cube}};

  Camera camera({screenwidth,
                 screenheight,
                 screenwidth / screenheight,
                 90.f,
                 0.1f,
                 1000.f});

  engine.register_gui([&camera] { GuiDrawCameraSettings(camera); });

  engine.run([&](float dt) {
    pulsing.pulse(dt);

    camera.transform(cube);

    engine.draw(cube);
  });
}