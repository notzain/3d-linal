add_executable(linalg-app
    src/main.cpp
	src/Gui.cpp
	src/Engine.cpp
	src/Mesh.cpp
	src/MeshRenderer.cpp
	src/Cube.cpp
	src/Bullet.cpp
	src/Object.cpp
	src/Spaceship.cpp
	)

target_link_libraries(linalg-app math imgui-sfml sfml-window sfml-graphics)