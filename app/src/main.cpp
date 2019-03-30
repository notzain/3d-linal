
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Bullet.h"
#include "Camera.h"
#include "Cube.h"
#include "Engine.h"
#include "Gui.h"
#include "Object.h"
#include "Spaceship.h"

int main(int argc, char **argv) {
  float screenwidth = 1280;
  float screenheight = 720;

  auto &engine = Engine::get();
  engine.init("LINAL", screenwidth, screenheight);

  Cube cube;
  Object object("objs/axis.obj");
  Bullet bullet({}, {});
  Spaceship ship("objs/spaceship.obj");
  std::vector<Bullet> bullets;

  FreeCamera free_cam({screenwidth, screenheight, screenheight / screenheight,
                       90.f, 0.1f, 1000.f});
  FollowCamera follow_cam({screenwidth, screenheight,
                           screenheight / screenheight, 90.f, 0.1f, 1000.f},
                          &cube);
  BirdCamera bird_cam({screenwidth, screenheight, screenheight / screenheight,
                       90.f, 0.1f, 1000.f});

  Camera *cameras[] = {&free_cam, &follow_cam, &bird_cam};
  int current_cam = 0;
  // camera.follow(&cube);

  const auto has_pressed = [](sf::Keyboard::Key key) -> bool {
    return sf::Keyboard::isKeyPressed(key);
  };

  sf::Vector2i last_mouse_pos(screenwidth / 2, screenheight / 2);
  float yaw = 0.f;
  float pitch = 0.f;

  engine.run([&](float dt) {
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

    if (has_pressed(Key::Space)) {
      bullets.push_back(ship.shoot());
    }

    /*
        const auto mouse_pos = engine.mouse_position();
        float xoffset = last_mouse_pos.x - mouse_pos.x;
        float yoffset = last_mouse_pos.y - mouse_pos.y;

        xoffset *= engine.mouse_sensitivity();
        yoffset *= -engine.mouse_sensitivity();

        yaw += xoffset;
        pitch += yoffset;

        camera.look_at(yaw, pitch);

        last_mouse_pos = mouse_pos;
    */

    if (cube.checkAABB(bullet)) {
      cube.color[0] = 0;
    } else {
      cube.color[0] = 1;
    }

    for (auto &b : bullets) {
      b.update(dt);
      cameras[current_cam]->transform(b);
    }

    cameras[current_cam]->transform(cube);
    cameras[current_cam]->transform(object);
    cameras[current_cam]->transform(bullet);
    cameras[current_cam]->transform(ship);

    GUI::get().draw(current_cam, cameras, {&cube, &object, &bullet, &ship});

    for (auto &b : bullets) {
      engine.draw(b);
    }
    engine.draw(cube);
    // engine.draw(object);
    engine.draw(ship);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const Bullet &b) {
                                   if (b.origin().x > 10 || b.origin().x < -10)
                                     return true;
                                   if (b.origin().y > 10 || b.origin().y < -10)
                                     return true;
                                   if (b.origin().z > 10 || b.origin().z < -10)
                                     return true;

                                   return false;
                                 }),
                  bullets.end());
  });
}
