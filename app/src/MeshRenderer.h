#pragma once

#include "Polygon.h"
#include "math/math.hpp"
#include <SFML/Graphics.hpp>

struct RenderType {
  static const uint8_t WIREFRAME = 1 << 0;
  static const uint8_t SOLID = 1 << 1;
};

class MeshRenderer {
public:
  MeshRenderer() = default;
  virtual ~MeshRenderer() = default;

  virtual void draw(const std::vector<Polygon> &mesh, sf::Color color) = 0;
};

class WireframeRenderer : public MeshRenderer {
  math::vector cam_pos;

  sf::RenderWindow *window;

  bool see_through = false;

public:
  WireframeRenderer(const math::vector &cam_pos, sf::RenderWindow *window,
                    bool see_through)
      : cam_pos(cam_pos), window(window), see_through(see_through) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override;
};

class SolidRenderer : public MeshRenderer {
  math::vector cam_pos;
  sf::RenderWindow *window;

public:
  SolidRenderer(const math::vector &cam_pos, sf::RenderWindow *window)
      : cam_pos(cam_pos), window(window) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override;
};

class WireframeAndSolidRenderer : public MeshRenderer {
  WireframeRenderer wireframe;
  SolidRenderer solid;

public:
  WireframeAndSolidRenderer(const math::vector &cam_pos,
                            sf::RenderWindow *window, bool see_through)
      : wireframe(cam_pos, window, see_through), solid(cam_pos, window) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override {
    solid.draw(mesh, color);
    wireframe.draw(mesh, sf::Color::Black);
  }
};
