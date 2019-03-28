
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <cmath>
#include <math/math.hpp>
#include <vector>

#include "Camera.h"
#include "Cube.h"
#include "Engine.h"
#include "Gui.h"
#include "Object.h"

int main(int argc, char **argv) {
  float screenwidth = 1280;
  float screenheight = 720;

  auto &engine = Engine::get();
  engine.init("LINAL", screenwidth, screenheight);

  Cube cube;
  Object object("objs/axis.obj");

  Camera camera({screenwidth, screenheight, screenheight / screenheight, 90.f,
                 0.1f, 1000.f});
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
    const auto LOOK = 50 * dt;

    // WASD - Look direction
    if (has_pressed(Key::W)) {
      camera.move_forward(D_MOVEMENT);
    }
    if (has_pressed(Key::S)) {
      camera.move_forward(-D_MOVEMENT);
    }
    if (has_pressed(Key::A)) {
      camera.move_sideways(D_MOVEMENT);
    }
    if (has_pressed(Key::D)) {
      camera.move_sideways(-D_MOVEMENT);
    }

    if (has_pressed(Key::Up)) {
      camera.look_vertical(-LOOK);
    }
    if (has_pressed(Key::Down)) {
      camera.look_vertical(LOOK);
    }
    if (has_pressed(Key::Left)) {
      camera.look_horizontal(LOOK);
    }
    if (has_pressed(Key::Right)) {
      camera.look_horizontal(-LOOK);
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

    camera.transform(cube);
    camera.transform(object);

    GUI::get().draw(&camera, {&cube, &object});

    engine.draw(cube);
    engine.draw(object);
  });
}
