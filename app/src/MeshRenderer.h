#pragma once

#include "Polygon.h"

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
  sf::RenderWindow *window;

  bool see_through = false;

public:
  WireframeRenderer(sf::RenderWindow *window, bool see_through)
      : window(window), see_through(see_through) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override;
};

class SolidRenderer : public MeshRenderer {
  sf::RenderWindow *window;

public:
  SolidRenderer(sf::RenderWindow *window) : window(window) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override;
};

class WireframeAndSolidRenderer : public MeshRenderer {
  WireframeRenderer wireframe;
  SolidRenderer solid;

public:
  WireframeAndSolidRenderer(sf::RenderWindow *window, bool see_through)
      : wireframe(window, see_through), solid(window) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override {
    solid.draw(mesh, color);
    wireframe.draw(mesh, sf::Color::Black);
  }
};
