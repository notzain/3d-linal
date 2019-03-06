#pragma once

#include "Gui.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

using GuiCallback = std::function<void()>;

class Engine {
  std::string title;
  sf::RenderWindow window;
  sf::Clock clock;

  float delta_time = 0;
  bool show_fps_ = true;

  bool show_gui_ = true;

  std::vector<GuiCallback> gui_callbacks;

  Engine() = default;
  ~Engine() = default;

public:
  static Engine &get() {
    static Engine engine;
    return engine;
  }

  static Engine &create(const std::string &title, int width, int height) {
    Engine::get().title = title;
    Engine::get().window.create(sf::VideoMode(width, height), title);
    GuiInit(Engine::get().window);

    return Engine::get();
  }

  void register_gui(GuiCallback &&callback) {
    gui_callbacks.push_back(std::move(callback));
  }
  void set_title(const std::string &title) {
    this->title = title;
    window.setTitle(title);
  }
  void show_fps(bool show) {
    show_fps_ = show;
    set_title(title);
  }

  void show_gui(bool show) { show_gui_ = show; }

  bool is_running() const { return window.isOpen(); }

  void update() {
    const auto time = clock.restart();
    delta_time = time.asSeconds();
    const auto fps = 1.f / delta_time;

    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed)
        window.close();
    }

    ImGui::SFML::Update(window, time);

    if (show_fps_) {
      window.setTitle(title + std::to_string(fps));
    }
  }

  void draw(sf::Drawable &drawable) { window.draw(drawable); }

  void show() {
    if (show_gui_) {
      ImGui::Begin("extra punten");
      for (const auto &cb : gui_callbacks) {
        cb();
      }
      ImGui::End();
      ImGui::SFML::Render(window);
    }

    window.display();
  }

  template <typename Func> void run(Func &&func) {
    while (is_running()) {
      window.clear();

      update();

      func(delta_time);

      show();
    }
  }
};