#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include "Camera.h"

void GuiInit(sf::RenderWindow &window, float fontScale = 1.f) {
  ImGui::SFML::Init(window);
  ImGui::GetIO().Fonts->Fonts[0]->Scale = fontScale;
}

void GuiDrawCameraSettings(Camera &camera) {
  bool updated = false;
  if (ImGui::CollapsingHeader("CameraSettings")) {
    if (ImGui::SliderFloat("FOV", &camera.settings.fov, 80, 110)) {
      updated = true;
    }
    if (ImGui::SliderFloat("Near", &camera.settings.near, 0.1f, 1000.f)) {
      updated = true;
    }
    if (ImGui::SliderFloat("Far", &camera.settings.far, 0.1f, 1000.f)) {
      updated = true;
    }
    if (ImGui::DragFloat("Yaw", &camera.yaw, 0.02f)) {
      updated = true;
    }
    if (ImGui::DragFloat("Theta", &camera.pitch, 0.02f)) {
      updated = true;
    }
  }

  ImGui::DragFloat3("Camera Pos (X, Y, Z)", &camera.camera_pos.x, 0.02f);

  if (updated)
    camera.reconfigure();
}