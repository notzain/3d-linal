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

  Engine() = default;
  ~Engine() = default;

public:
  static Engine &get() {
    static Engine engine;
    return engine;
  }

  void init(const std::string &title, int width, int height) {
    this->title = title;
    this->window.create(sf::VideoMode(width, height), title);

	GUI::get().init(window);
  }

  void set_title(const std::string &title) {
    this->title = title;
    window.setTitle(title);
  }

  void show_fps(bool show) {
    show_fps_ = show;

	if (!show) {
		set_title(title);
	}
  }

  void set_framerate(unsigned int framerate) {
	  window.setFramerateLimit(framerate);
  }

  bool is_running() const { return window.isOpen(); }

  void update() {
    const auto time = clock.restart();
    delta_time = time.asSeconds();
    const auto fps = 1.f / delta_time;

    sf::Event event;
    while (window.pollEvent(event)) {
		GUI::get().poll(event);
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (show_fps_) {
      window.setTitle(title + " - " + std::to_string(fps));
    }

	GUI::get().update(window, time);
  }

  void draw(sf::Drawable &drawable) { window.draw(drawable); }

  template <typename Func> void run(Func &&func) {
    while (is_running()) {
      window.clear();

      update();

      func(delta_time);

	  GUI::get().display(window);
	  window.display();
    }
  }
};