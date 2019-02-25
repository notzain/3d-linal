#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>
#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Cube.h"
#include "Object.h"
#include "Camera.h"

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
  Object object("axis.obj");

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

  Camera cam(90, 800/600, 0.1, 1000);

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
          cam.move({-5 * current_time, 0, 0});
          rot_x -= 5 * current_time;
          break;
        case sf::Keyboard::Right:
          cam.move({5 * current_time, 0, 0});
          rot_x += 5 * current_time;
          break;
        case sf::Keyboard::Up:
          cam.move({0, 5 * current_time, 0});
          rot_y += 5 * current_time;
          break;
        case sf::Keyboard::Down:
          cam.move({0, -5 * current_time, 0});
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

    cam.transform(object);
    
    App.draw(object);
    App.display();
  }
}