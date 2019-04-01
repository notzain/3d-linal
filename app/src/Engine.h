#pragma once

#include "Gui.h"
#include "MeshRenderer.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

using GuiCallback = std::function<void()>;

struct RenderSettings {
  uint8_t render_type = RenderType::WIREFRAME;
  bool see_through = true;
};

class Engine {
  std::string title;
  sf::RenderWindow window;
  sf::Clock clock;

  bool exit_ = false;

  float _mouse_sensitivity = .5f;
  float delta_time = 0;
  bool show_fps_ = true;
  RenderSettings render_settings;

  Engine() = default;
  ~Engine() = default;

public:
  static Engine &get() {
    static Engine engine;
    return engine;
  }

  void init(const std::string &title, int width, int height);

  float mouse_sensitivity() const { return _mouse_sensitivity / 10; }
  void set_mouse_sensitivyt(float value) { _mouse_sensitivity = value; }

  void set_title(const std::string &title) {
    this->title = title;
    window.setTitle(title);
  }

  void show_fps(bool show);

  void set_framerate(unsigned int framerate) {
    window.setFramerateLimit(framerate);
  }

  RenderSettings &get_render_settings() { return render_settings; }

  bool is_running() const { return window.isOpen() && !exit_; }
  void exit() { exit_ = true; }

  sf::Vector2i mouse_position() const { return sf::Mouse::getPosition(); }

  void update();

  void draw(const math::vector &cam, const Mesh &mesh);
  void draw_text(const std::string &text, int size, sf::Vector2f position);

  void run(std::function<void(float)> onUpdate);
};