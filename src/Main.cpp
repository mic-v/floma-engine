#include <iostream>



#ifdef WINDOW_BUILD
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif


#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>

#include "Game.h"

// Create git ignore for tmrw


int main(int argc, char* argv[]) {
    // TODO: Do some magic!
    Game game;


    glm::vec2 test = glm::vec2(10,10);
    std::cout << glm::to_string(test) << std::endl;
    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}
