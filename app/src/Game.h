#pragma once

#include "Bullet.h"
#include "Camera.h"
#include "Engine.h"
#include "Mesh.h"
#include "Spaceship.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>

class GUI;

struct PulsingObjects {
  float current_size = 1;
  float max_size = 2.5;
  bool increase = true;
  std::vector<std::unique_ptr<Mesh>> objects;

  void update(float dt) {
    if (increase) {
      current_size *= 1 + dt / 2;
    } else {
      current_size *= 1 - dt / 2;
    }

    if (current_size >= max_size) {
      increase = false;
    }
    if (current_size <= 1) {
      increase = true;
    }

    for (auto &t : objects) {
      t->scaling() = {current_size, current_size, current_size};
    }
  }
};

class Game {
  PulsingObjects targets;
  std::vector<std::unique_ptr<Mesh>> objects;

  Spaceship ship;
  std::vector<Bullet> bullets;
  float reload_time = 1;
  float last_shot = 0;

  FreeCamera free_cam;
  FollowCamera follow_cam;
  BirdCamera bird_cam;

  Camera *cameras[3];
  int current_cam = 0;

  friend class GUI;

public:
  Game(std::string name, float screen_width, float screen_height)
      : ship("objs/spaceship.obj"),
        free_cam({screen_width, screen_height, screen_height / screen_height,
                  90.f, 0.1f, 1000.f}),
        follow_cam({screen_width, screen_height, screen_height / screen_height,
                    90.f, 0.1f, 1000.f},
                   &ship),
        bird_cam({screen_width, screen_height, screen_height / screen_height,
                  90.f, 0.1f, 1000.f}) {
    Engine::get().init(name, screen_width, screen_height);
    cameras[0] = &free_cam;
    cameras[1] = &follow_cam;
    cameras[2] = &bird_cam;
  }

  void add_object(std::unique_ptr<Mesh> mesh) {
    objects.push_back(std::move(mesh));
  }

  void add_target(std::unique_ptr<Mesh> mesh) {
    targets.objects.push_back(std::move(mesh));
  }

  void run() {

    const auto has_pressed = [](sf::Keyboard::Key key) -> bool {
      return sf::Keyboard::isKeyPressed(key);
    };

    auto &engine = Engine::get();

    engine.run([&](float dt) {
      auto &engine = Engine::get();

      using Key = sf::Keyboard;
      const auto MOVEMENT = 5;
      const auto D_MOVEMENT = MOVEMENT * dt;
      const auto LOOK = 100 * dt;

      if (has_pressed(Key::Tab)) {
        current_cam++;
        if (current_cam > 1)
          current_cam = 0;
      }

      // WASD - Look direction
      if (has_pressed(Key::W)) {
        cameras[current_cam]->move_forward(D_MOVEMENT);
      }
      if (has_pressed(Key::S)) {
        cameras[current_cam]->move_forward(-D_MOVEMENT);
      }
      if (has_pressed(Key::A)) {
        cameras[current_cam]->move_sideways(D_MOVEMENT);
      }
      if (has_pressed(Key::D)) {
        cameras[current_cam]->move_sideways(-D_MOVEMENT);
      }

      if (has_pressed(Key::Up)) {
        cameras[current_cam]->look_vertical(-LOOK);
      }
      if (has_pressed(Key::Down)) {
        cameras[current_cam]->look_vertical(LOOK);
      }
      if (has_pressed(Key::Left)) {
        cameras[current_cam]->look_horizontal(LOOK);
      }
      if (has_pressed(Key::Right)) {
        cameras[current_cam]->look_horizontal(-LOOK);
      }

      if (has_pressed(Key::LShift)) {
        ship.accelerate(D_MOVEMENT);
      }
      if (has_pressed(Key::RShift)) {
        ship.accelerate(-D_MOVEMENT);
      }

      if (has_pressed(Key::Space)) {
        if (last_shot >= reload_time) {
          bullets.push_back(ship.shoot());
          last_shot = 0;
        }
      }
      last_shot += dt;

      ship.update(dt);
      cameras[current_cam]->transform(ship);
      engine.draw(ship);

      targets.update(dt);

      for (auto &b : bullets) {
        b.update(dt);
        cameras[current_cam]->transform(b);
        for (auto &o : objects) {
          if (o->checkAABB(b)) {
            o->color[0] = 0;
          } else {
            o->color[0] = 1;
          }
        }
        for (auto &o : targets.objects) {
          if (o->checkAABB(b)) {
            o->color[0] = 0;
          } else {
            o->color[0] = 1;
          }
        }
      }

      for (auto &o : objects) {
        cameras[current_cam]->transform(*o);
        engine.draw(*o);
      }

      for (auto &o : targets.objects) {
        cameras[current_cam]->transform(*o);
        engine.draw(*o);
      }

      for (auto &b : bullets) {
        engine.draw(b);
      }

      GUI::get().draw(*this);

      bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                   [](const Bullet &b) {
                                     const auto VIEW_DISTANCE = 100;

                                     if (b.origin().x > VIEW_DISTANCE ||
                                         b.origin().x < -VIEW_DISTANCE)
                                       return true;
                                     if (b.origin().y > VIEW_DISTANCE ||
                                         b.origin().y < -VIEW_DISTANCE)
                                       return true;
                                     if (b.origin().z > VIEW_DISTANCE ||
                                         b.origin().z < -VIEW_DISTANCE)
                                       return true;

                                     return false;
                                   }),
                    bullets.end());
    });
  }
};