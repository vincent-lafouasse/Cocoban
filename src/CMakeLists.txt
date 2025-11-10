add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
PRIVATE
        main.cpp
	game/Board.cpp
	game/Game.cpp
	render/Renderer.cpp
	geometry.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ./)
target_include_directories(${PROJECT_NAME} PUBLIC ./common/)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if (APPLE)
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit")
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif()
