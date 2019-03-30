#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include "Camera.h"
#include "Mesh.h"

class GUI {
  bool draw_gui_;

  GUI() = default;
  ~GUI() = default;

public:
  static GUI &get() {
    static GUI gui;
    return gui;
  }

  void init(sf::RenderWindow &window, float fontScale = 1.f) {
    ImGui::SFML::Init(window);
    ImGui::GetIO().Fonts->Fonts[0]->Scale = fontScale;
  }

  void poll(const sf::Event &event) { ImGui::SFML::ProcessEvent(event); }

  void update(sf::RenderWindow &window, const sf::Time &time) {
    ImGui::SFML::Update(window, time);
  }

  void display(sf::RenderWindow &window) { ImGui::SFML::Render(window); }

  void draw(int &currentcam, Camera **camera, const std::vector<Mesh *> &mesh) {
    ImGui::Begin("extra punten");

    if (ImGui::BeginTabBar("Test")) {
      draw_engine();
      draw_camera(currentcam, camera, 3);
      draw_mesh(mesh);

      ImGui::EndTabBar();
    }

    ImGui::End();
  }

private:
  void draw_engine();

  void draw_camera(int &current, Camera **camera, int size) {
    if (camera == nullptr)
      return;

    bool updated = false;
    if (ImGui::BeginTabItem("Camera")) {
      const char *text[] = {"Free", "Follow", "Bird"};
      ImGui::Combo("Cameras", &current, text, size);

      if (ImGui::CollapsingHeader("Settings")) {
        ImGui::Indent();
        if (ImGui::SliderFloat("FOV", &camera[current]->settings.fov, 40,
                               200)) {
          updated = true;
        }
        if (ImGui::SliderFloat("Near", &camera[current]->settings.near, 0.01f,
                               1000.f)) {
          updated = true;
        }
        if (ImGui::SliderFloat("Far", &camera[current]->settings.far, 0.01f,
                               1000.f)) {
          updated = true;
        }
        ImGui::Unindent();
      }

      ImGui::DragFloat2("Camera Rotation (X, Y)", &camera[current]->yaw, 0.02f);

      ImGui::DragFloat3("Camera Pos (X, Y, Z)", &camera[current]->position.x,
                        0.02f);

      ImGui::EndTabItem();
    }

    // ImGui::DragFloat3("Camera LookAt (X, Y, Z)", &camera.target.x,
    // 0.02f);

    if (updated)
      camera[current]->reconfigure();
  }

  void draw_mesh(const std::vector<Mesh *> &meshes) {
    if (ImGui::BeginTabItem("Objects")) {
      for (int i = 0; i < meshes.size(); i++) {
        if (ImGui::TreeNode((void *)(intptr_t)i, "%d", i)) {
          ImGui::DragFloat3("Position (X, Y, Z)", &meshes[i]->origin().x,
                            0.02f);
          ImGui::DragFloat3("Rotation (X, Y, Z)", &meshes[i]->rotation().x,
                            0.02f);
          ImGui::DragFloat3("Scale (X, Y, Z)", &meshes[i]->scaling().x, 0.02f);

          ImGui::ColorEdit3("Color", meshes[i]->color);
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }
};
