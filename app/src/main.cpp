#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>
#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Camera.h"
#include "Cube.h"
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
  int screenwidth = 800;
  int screenheight = 600;
  sf::RenderWindow App(sf::VideoMode(800, 600), "My window");
  ImGui::SFML::Init(App);

  Cube cube;
  Object object("axis.obj");

  float last_time = 0;
  sf::Clock clock;

  PulsingMesh pulsing{1.5, 0.3, 1, false, {&cube}};

  Camera camera(CameraSettings{.screen_height = 800,
                               .screen_width = 600,
                               .aspect_ratio = 800 / 600,
                               .fov = 90.f,
                               .near = 0.1f,
                               .far = 1000.f});

  while (App.isOpen()) {
    const auto current_time = clock.restart().asSeconds();
    const auto fps = 1.0f / current_time;

    last_time = current_time;
    App.setTitle("Linal " + std::to_string(fps) + " fps");

    sf::Event Event;
    while (App.pollEvent(Event)) {
      if (Event.type == sf::Event::Closed)
        App.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    }

    App.clear();

    camera.transform(cube);

    App.draw(cube);
    App.display();
  }
}