// FinalProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[])

{
    std::cout << "Hello World!\n";
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    struct SDL_Rect rect = { 350, 250, 100, 100 };

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return -1;
    }

    window = SDL_CreateWindow("Your Window Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}


