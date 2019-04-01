#include "Gui.h"

#include "Engine.h"
#include "Game.h"

static bool show_fps = true;

static struct frame_rate {
  int fps = 60;
  bool unlimited = true;
} frame_rate;

static float mouse_sensitivity = .5f;

static struct render_settings {

  bool wire_frame = true;
  bool solid = false;
  bool see_through = true;
} render_settings;

void GUI::init(sf::RenderWindow &window, float fontScale) {
  ImGui::SFML::Init(window);
  ImGui::GetIO().Fonts->Fonts[0]->Scale = fontScale;
}

void GUI::draw(Game &game) {
  ImGui::Begin("extra punten");

  if (ImGui::BeginTabBar("Test")) {
    draw_engine();
    draw_camera(game);
    draw_mesh(game);

    ImGui::EndTabBar();
  }

  ImGui::End();
}

void GUI::draw_engine() {
  if (ImGui::BeginTabItem("Engine")) {
    if (ImGui::Checkbox("Show FPS", &show_fps))
      Engine::get().show_fps(show_fps);

    if (ImGui::SliderInt("FPS", &frame_rate.fps, 10, 144)) {
      Engine::get().set_framerate(frame_rate.fps);
      frame_rate.unlimited = false;
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Unlimited", &frame_rate.unlimited)) {
      Engine::get().set_framerate(frame_rate.unlimited ? 0 : frame_rate.fps);
    }

    if (ImGui::SliderFloat("Mouse sensitivity", &mouse_sensitivity, 0.f, 1.f)) {
      Engine::get().set_mouse_sensitivyt(mouse_sensitivity);
    }

    if (ImGui::Checkbox("Wireframe", &render_settings.wire_frame)) {
      Engine::get().get_render_settings().render_type ^= RenderType::WIREFRAME;
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("SeeThrough", &render_settings.see_through)) {
      Engine::get().get_render_settings().see_through =
          render_settings.see_through;
    }
    if (ImGui::Checkbox("Solid", &render_settings.solid)) {
      Engine::get().get_render_settings().render_type ^= RenderType::SOLID;
    }

    ImGui::EndTabItem();
  }
}

void GUI::draw_camera(Game &game) {
  bool updated = false;
  if (ImGui::BeginTabItem("Camera")) {
    const char *text[] = {"Free", "Follow", "Bird"};
    ImGui::Combo("Cameras", &game.current_cam, text, 3);

    if (ImGui::CollapsingHeader("Settings")) {
      ImGui::Indent();
      if (ImGui::SliderFloat(
              "FOV", &game.cameras[game.current_cam]->settings.fov, 40, 200)) {
        updated = true;
      }
      if (ImGui::SliderFloat("Near",
                             &game.cameras[game.current_cam]->settings.near,
                             0.01f, 1000.f)) {
        updated = true;
      }
      if (ImGui::SliderFloat("Far",
                             &game.cameras[game.current_cam]->settings.far,
                             0.01f, 1000.f)) {
        updated = true;
      }
      ImGui::Unindent();
    }

    ImGui::DragFloat3("Camera Rotation (X, Y, Z)",
                      &game.cameras[game.current_cam]->yaw, 0.02f);

    ImGui::DragFloat3("Camera Pos (X, Y, Z)",
                      &game.cameras[game.current_cam]->position.x, 0.02f);

    ImGui::EndTabItem();
  }

  if (updated)
    game.cameras[game.current_cam]->reconfigure();
}

void GUI::draw_mesh(Game &game) {
  if (ImGui::BeginTabItem("Objects")) {
    if (ImGui::CollapsingHeader("Ship")) {
      ImGui::DragFloat3("Position (X, Y, Z)", &game.ship.origin().x, 0.02f);
      ImGui::DragFloat3("Rotation (X, Y, Z)", &game.ship.rotation().x, 0.02f);
      ImGui::DragFloat3("Scale (X, Y, Z)", &game.ship.scaling().x, 0.02f);

      ImGui::ColorEdit3("Color", game.ship.color);
    }

    if (ImGui::CollapsingHeader("Targets")) {
      for (int i = 0; i < game.targets.objects.size(); i++) {
        if (ImGui::TreeNode((void *)(intptr_t)i, "Target %d", i)) {
          ImGui::DragFloat3("Position (X, Y, Z)",
                            &game.targets.objects[i]->origin().x, 0.02f);
          ImGui::DragFloat3("Rotation (X, Y, Z)",
                            &game.targets.objects[i]->rotation().x, 0.02f);
          ImGui::DragFloat3("Scale (X, Y, Z)",
                            &game.targets.objects[i]->scaling().x, 0.02f);

          ImGui::ColorEdit3("Color", game.targets.objects[i]->color);
          ImGui::TreePop();
        }
      }
    }

    if (ImGui::CollapsingHeader("Objs")) {
      for (int i = 0; i < game.objects.size(); i++) {
        if (ImGui::TreeNode((void *)(intptr_t)i, "Object %d", i)) {
          ImGui::DragFloat3("Position (X, Y, Z)", &game.objects[i]->origin().x,
                            0.02f);
          ImGui::DragFloat3("Rotation (X, Y, Z)",
                            &game.objects[i]->rotation().x, 0.02f);
          ImGui::DragFloat3("Scale (X, Y, Z)", &game.objects[i]->scaling().x,
                            0.02f);

          ImGui::ColorEdit3("Color", game.objects[i]->color);
          ImGui::TreePop();
        }
      }
    }

    if (ImGui::CollapsingHeader("Bullets")) {
      for (int i = 0; i < game.bullets.size(); i++) {
        if (ImGui::TreeNode((void *)(intptr_t)i, "Bullet %d", i)) {
          ImGui::DragFloat3("Position (X, Y, Z)", &game.bullets[i].origin().x,
                            0.02f);
          ImGui::DragFloat3("Rotation (X, Y, Z)", &game.bullets[i].rotation().x,
                            0.02f);
          ImGui::DragFloat3("Scale (X, Y, Z)", &game.bullets[i].scaling().x,
                            0.02f);

          ImGui::ColorEdit3("Color", game.bullets[i].color);
          ImGui::TreePop();
        }
      }
    }

    ImGui::EndTabItem();
  }
}
