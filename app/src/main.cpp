#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <cmath>
#include <math/math.hpp>
#include <vector>

struct triangle {
  math::vector vertices[3];
};

struct mesh {
  std::vector<triangle> tris;
};
int main(int argc, char **argv) {
  int screenwidth = 800;
  int screenheight = 600;
  sf::RenderWindow App(sf::VideoMode(800, 600), "My window");
  App.setFramerateLimit(15);

  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = 800 / 600;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

  mesh cube;
  cube.tris = {
      // SOUTH
      {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

      // EAST
      {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
      {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

      // NORTH
      {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
      {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

      // WEST
      {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

      // TOP
      {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
      {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

      // BOTTOM
      {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
      {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

  };

  math::matrix matProj;
  matProj(0, 0) = fAspectRatio * fFovRad;
  matProj(1, 1) = fFovRad;
  matProj(2, 2) = fFar / (fFar - fNear);
  matProj(3, 2) = (-fFar * fNear) / (fFar - fNear);
  matProj(2, 3) = 1.0f;
  matProj(3, 3) = 0.0f;

  float fTheta = 0;
  sf::Clock clock;

  while (App.isOpen()) {
    fTheta += 1 * clock.getElapsedTime().asSeconds();
    clock.restart();

    sf::Event Event;
    while (App.pollEvent(Event)) {
      if (Event.type == sf::Event::Closed)
        App.close();
    }
    App.clear();

    auto z_rotation = math::make_rotation_z(fTheta);
    auto x_rotation = math::make_rotation_x(fTheta);

    for (auto tri : cube.tris) {
      triangle rotated_z;
      rotated_z.vertices[0] = math::multiply(tri.vertices[0], z_rotation);
      rotated_z.vertices[1] = math::multiply(tri.vertices[1], z_rotation);
      rotated_z.vertices[2] = math::multiply(tri.vertices[2], z_rotation);

      triangle rotated_zx;
      rotated_zx.vertices[0] =
          math::multiply(rotated_z.vertices[0], x_rotation);
      rotated_zx.vertices[1] =
          math::multiply(rotated_z.vertices[1], x_rotation);
      rotated_zx.vertices[2] =
          math::multiply(rotated_z.vertices[2], x_rotation);

      auto translated = rotated_zx;
      translated.vertices[0].z += 3;
      translated.vertices[1].z += 3;
      translated.vertices[2].z += 3;

      triangle projected;
      projected.vertices[0] = math::multiply(translated.vertices[0], matProj);
      projected.vertices[1] = math::multiply(translated.vertices[1], matProj);
      projected.vertices[2] = math::multiply(translated.vertices[2], matProj);

      projected.vertices[0].x += 1;
      projected.vertices[0].y += 1;
      projected.vertices[1].x += 1;
      projected.vertices[1].y += 1;
      projected.vertices[2].x += 1;
      projected.vertices[2].y += 1;

      for (int i = 0; i < 3; ++i) {
        projected.vertices[i].x *= 0.5 * screenwidth;
        projected.vertices[i].y *= 0.5 * screenheight;
      }

      sf::Vertex a[] = {
          sf::Vector2f{projected.vertices[0].x, projected.vertices[0].y},
          sf::Vector2f{projected.vertices[1].x, projected.vertices[1].y},
      };
      sf::Vertex b[] = {
          sf::Vector2f{projected.vertices[1].x, projected.vertices[1].y},
          sf::Vector2f{projected.vertices[2].x, projected.vertices[2].y},
      };
      sf::Vertex c[] = {
          sf::Vector2f{projected.vertices[0].x, projected.vertices[0].y},
          sf::Vector2f{projected.vertices[2].x, projected.vertices[2].y},
      };

      App.draw(a, 2, sf::Lines);
      App.draw(b, 2, sf::Lines);
      App.draw(c, 2, sf::Lines);
    }


    App.display();
  }
}