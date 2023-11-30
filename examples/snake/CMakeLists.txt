project(snake)
add_executable(${PROJECT_NAME} main.cpp window.cpp game.cpp camera.cpp snake.cpp apple.cpp ground.cpp skybox.cpp util.cpp)
enable_abcg(${PROJECT_NAME})
