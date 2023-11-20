project(snake)
add_executable(${PROJECT_NAME} main.cpp window.cpp game.cpp snake.cpp apple.cpp ground.cpp camera.cpp util.cpp)
enable_abcg(${PROJECT_NAME})
