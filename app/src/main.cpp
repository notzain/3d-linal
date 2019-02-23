#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>
#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Cube.h"
#include "Object.h"

int main(int argc, char **argv) {
  int screenwidth = 800;
  int screenheight = 600;
  sf::RenderWindow App(sf::VideoMode(800, 600), "My window");
  App.setFramerateLimit(144);

  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = 800 / 600;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

  Cube cube;
  Object object("spaceship.obj");

  math::matrix matProj;
  matProj(0, 0) = fAspectRatio * fFovRad;
  matProj(1, 1) = fFovRad;
  matProj(2, 2) = fFar / (fFar - fNear);
  matProj(3, 2) = (-fFar * fNear) / (fFar - fNear);
  matProj(2, 3) = 1.0f;
  matProj(3, 3) = 0.0f;

  float fTheta = 0;

  float last_time = 0;
  sf::Clock clock;

  float rot_x = 0;
  float rot_y = 0;
  float rot_z = 0;

  float scale = 1;

  while (App.isOpen()) {
    const auto current_time = clock.restart().asSeconds();
    const auto fps = 1.0f / current_time;

    last_time = current_time;
    App.setTitle("Linal " + std::to_string(fps) + " fps");

    sf::Event Event;
    while (App.pollEvent(Event)) {
      if (Event.type == sf::Event::Closed)
        App.close();

      if (Event.type == sf::Event::KeyPressed) {
        switch (Event.key.code) {
        case sf::Keyboard::Left:
          rot_x -= 5 * current_time;
          break;
        case sf::Keyboard::Right:
          rot_x += 5 * current_time;
          break;
        case sf::Keyboard::Up:
          rot_y += 5 * current_time;
          break;
        case sf::Keyboard::Down:
          rot_y -= 5 * current_time;
          break;
        case sf::Keyboard::Equal:
          scale += 5 * current_time;
          break;
        case sf::Keyboard::Dash:
          scale -= 5 * current_time;
          break;
        }
      }
    }
    App.clear();

    const auto z_rotation = math::make_rotation_z(rot_z);
    const auto x_rotation = math::make_rotation_x(rot_x);
    const auto y_rotation = math::make_rotation_y(rot_y);

    cube.rotate(z_rotation);
    cube.rotate(x_rotation);
    cube.translate({0, 0, 3});
    cube.rotate(matProj);
    cube.translate({1, 1, 0});
    cube.scale(Dimension::X, 0.5 * screenwidth);
    cube.scale(Dimension::Y, 0.5 * screenheight);

    object.scale(scale);

    object.rotate(z_rotation);
    object.rotate(x_rotation);
    object.rotate(y_rotation);
    object.translate({0, 0, 10});
    object.rotate(matProj);
    object.translate({1, 1, 0});
    object.scale(Dimension::X, 0.5 * screenwidth);
    object.scale(Dimension::Y, 0.5 * screenheight);

    // App.draw(object);
    App.draw(cube);

    App.display();
  }
}