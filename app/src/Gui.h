#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include "Camera.h"
#include "Mesh.h"

class Game;

class GUI {
  bool draw_gui_;

  GUI() = default;
  ~GUI() = default;

public:
  static GUI &get() {
    static GUI gui;
    return gui;
  }

  void init(sf::RenderWindow &window, float fontScale = 1.f);

  void poll(const sf::Event &event) { ImGui::SFML::ProcessEvent(event); }

  void update(sf::RenderWindow &window, const sf::Time &time) {
    ImGui::SFML::Update(window, time);
  }

  void display(sf::RenderWindow &window) { ImGui::SFML::Render(window); }

  void draw(Game& game);

private:
  void draw_engine();

  void draw_camera(Game &game);

  void draw_mesh(Game &game);
};
