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

  Cube cube;
  Object object("axis.obj");

  float last_time = 0;
  sf::Clock clock;

  Camera cam(90, 800 / 600, 0.1, 1000);

  PulsingMesh pulsing{1.5, 0.3, 1, false, {&cube}};

  math::matrix projection =
      math::make_projection(90.f, 800 / 600, 0.1f, 1000.f);
  math::vector camera;
  math::vector lookDir;
  float yaw{};
  float theta{};

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
    const auto forward = lookDir * (movement_speed * current_time);
    math::vector target{0, 0, 1};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      camera.x -= movement_speed * current_time;

      cam.move({-movement_speed * current_time, 0, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      camera.x += movement_speed * current_time;

      cam.move({movement_speed * current_time, 0, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      camera.y -= movement_speed * current_time;

      cam.move({0, -movement_speed * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      camera.y += movement_speed * current_time;

      cam.move({0, movement_speed * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
      camera += forward;

      cam.move({0, 0, movement_speed * current_time});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
      camera -= forward;

      cam.move({0, 0, -movement_speed * current_time});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      target.y += movement_speed * current_time;
      cam.look({0, 1 * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      target.y -= movement_speed * current_time;

      cam.look({0, -1 * current_time, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      yaw -= movement_speed / 4 * current_time;
      cam.look({-1 * current_time, 0, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      yaw += movement_speed / 4 * current_time;
      cam.look({1 * current_time, 0, 0});
    }

    App.clear();

    const auto rotZ = math::make_rotation_z(theta * 0.5f);
    const auto rotX = math::make_rotation_x(theta);

    const auto translation = math::make_translation({0.f, 0.f, 5.f});

    auto world = rotZ * rotX;
    world *= translation;

    math::vector up{0, 1, 0};
    const auto cameraRot = math::make_rotation_y(yaw);

    lookDir = math::multiply(target, cameraRot);
    target = camera + lookDir;

    const auto cameraMat = math::point_at(camera, target, up);
    const auto view = math::inverse(cameraMat);

    auto quads = cube.vertices();
    std::vector<Quad> to_draw;

    for (auto &quad : quads) {
      for (auto &vertex : quad.vertices) {
        vertex = math::multiply(vertex, world);
        vertex = math::multiply(vertex, view);
        vertex = math::multiply(vertex, projection);

        vertex = vertex / vertex.w;

        vertex += math::vector{1, 1, 0};
        vertex.x *= (0.5 * screenwidth);
        vertex.y *= (0.5 * screenheight);
      }
    }

    for (auto &line : Mesh::make_lines(quads)) {
      for (auto &vertex : line) {
        vertex.color = sf::Color::White;
      }
      App.draw(line.data(), 2, sf::Lines);
    }

    App.display();
  }
}