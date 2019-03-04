
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Camera.h"
#include "Cube.h"
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
  
  sf::RenderWindow App(sf::VideoMode(screenwidth, screenheight), "My window");
  GuiInit(App);

  Cube cube;
  Object object("axis.obj");

  float last_time = 0;
  sf::Clock clock;

  PulsingMesh pulsing{1.5, 0.3, 1, false, {&cube}};

  Camera camera({.screen_width = screenwidth,
                 .screen_height = screenheight,
                 .aspect_ratio = screenwidth / screenheight,
                 .fov = 90.f,
                 .near = 0.1f,
                 .far = 1000.f});

  while (App.isOpen()) {
    const auto time = clock.restart();
    const auto delta_time = time.asSeconds();
    const auto fps = 1.0f / delta_time;

    last_time = delta_time;
    App.setTitle("Linal " + std::to_string(fps) + " fps");

    sf::Event Event;
    while (App.pollEvent(Event)) {
      ImGui::SFML::ProcessEvent(Event);
      if (Event.type == sf::Event::Closed)
        App.close();
    }

    ImGui::SFML::Update(App, time);

    ImGui::Begin("LINALG");

    GuiDrawCameraSettings(camera);

    ImGui::End();

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

    ImGui::SFML::Render(App);
    App.display();
  }
}