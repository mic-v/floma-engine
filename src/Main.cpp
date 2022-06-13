#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>

// Create git ignore for tmrw


int main(int argc, char* argv[]) {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    SDL_Init(SDL_INIT_EVERYTHING);

    glm::vec2 velocity = glm::vec2(0.0,-1.0);

    std::cout << "Hello, world!" << std::endl;
    return 0;
}
