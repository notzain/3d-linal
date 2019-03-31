#include "Engine.h"

void Engine::init(const std::string &title, int width, int height) {
  this->title = title;
  this->window.create(sf::VideoMode(width, height), title);

  GUI::get().init(window);
}

void Engine::show_fps(bool show) {
  show_fps_ = show;

  if (!show) {
    set_title(title);
  }
}

void Engine::update() {
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

void Engine::draw(const Mesh &mesh) {
  if (render_settings.render_type & RenderType::WIREFRAME &&
      render_settings.render_type & RenderType::SOLID) {
    WireframeAndSolidRenderer renderer(&window, render_settings.see_through);
    mesh.draw(renderer);
  } else if (render_settings.render_type & RenderType::WIREFRAME) {
    WireframeRenderer renderer(&window, render_settings.see_through);
    mesh.draw(renderer);
  }
  if (render_settings.render_type & RenderType::SOLID) {
    SolidRenderer renderer(&window);
    mesh.draw(renderer);
  }
}

void Engine::run(std::function<void(float)> onUpdate) {
  while (is_running()) {
    window.clear();

    update();

    onUpdate(delta_time);

    GUI::get().display(window);
    window.display();
  }
}
