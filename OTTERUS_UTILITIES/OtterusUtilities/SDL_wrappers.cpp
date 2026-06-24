#include "SDL_wrappers.h"
#include <iostream>

void otterus_utils::SDLDestroyer::operator()(SDL_Window* window) const
{
    SDL_DestroyWindow(window);
    std::cout << "Window Destroyed.";
}

void otterus_utils::SDLDestroyer::operator()(SDL_GameController * controller) const
{}

void otterus_utils::SDLDestroyer::operator()(SDL_Cursor * cursor) const
{}

Controller make_shared_controller(SDL_GameController* controller)
{
    return Controller();
}

Cursor make_shared_cursor(SDL_Cursor* cursor)
{
    return Cursor();
}
