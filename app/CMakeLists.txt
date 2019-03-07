add_executable(linalg-app
    src/main.cpp
	src/Gui.cpp
	)

target_link_libraries(linalg-app math imgui-sfml sfml-window sfml-graphics)