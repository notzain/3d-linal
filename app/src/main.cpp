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
  App.setFramerateLimit(144);

  Cube cube;
  Object object("axis.obj");

  float last_time = 0;
  sf::Clock clock;

  Camera cam(90, 800 / 600, 0.1, 1000);

  PulsingMesh pulsing{1.5, 0.3, 1, false, {&cube}};

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

    const auto movement_speed = 10;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      cam.move({-movement_speed * current_time, 0, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      cam.move({movement_speed * current_time, 0, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      cam.move({0, -movement_speed * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      cam.move({0, movement_speed * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
      cam.move({0, 0, movement_speed * current_time});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
      cam.move({0, 0, -movement_speed * current_time});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      cam.look({0, 1 * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      cam.look({0, -1 * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      cam.look({-1 * current_time, 0, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      cam.look({1 * current_time, 0, 0});
    }

    App.clear();

    pulsing.pulse(current_time);

    cam.transform(object);
    cam.transform(cube);

    App.draw(object);
    App.draw(cube);

    App.display();
  }
}