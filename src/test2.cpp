#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <iostream>

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;
const int nMapHeight = 24;
const int nMapWidth = 24;

int nTileSize = 20;
const int mw = 24;
const int mh = 24;
const int nPlayerWidth = 10;
const int nPlayerHeight = 10;
int gMap[nMapHeight * nMapWidth] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

void inti() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {

    std::cerr << "Couldn't Initalize SDL\n";
    exit(1);
  }

  gWindow = SDL_CreateWindow("RayCast v1.2 ", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == nullptr) {
    std::cerr << "Couldn't create Window\n";
    exit(1);
  }

  gRenderer = SDL_CreateRenderer(
      gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!gRenderer) {
    std::cerr << "Couldn't Create Renderer\n";
    exit(1);
  }
}

void render_map(SDL_Rect &Wall) {

  for (int i = 0; i < nMapHeight * nMapWidth; i++) {

    int nPosX = i % nMapWidth;
    int nPosY = i / nMapHeight;
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0x00);
    if (gMap[i]) {
      Wall.x = nPosX * nTileSize;
      Wall.y = nPosY * nTileSize;
      SDL_RenderFillRect(gRenderer, &Wall);
    }
  }
}
void close_program() {

  SDL_DestroyWindow(gWindow);
  SDL_DestroyRenderer(gRenderer);

  SDL_Quit();
}
int main(int argc, char **argv) {

  std::printf("Program %s, running with %d argc", argv[0], argc);
  inti();

  // player rect
  SDL_FRect fPlayer;
  float fPlX = 12.0f;
  float fPlY = 20.f;
  fPlayer.x = fPlX * nPlayerWidth;
  fPlayer.y = fPlY * nPlayerHeight;
  fPlayer.w = nPlayerWidth;
  fPlayer.h = nPlayerHeight;

  float theta_spacing = 0.01;
  float fFov = M_PI / 2;
  float fFovMin = -fFov / 2;
  float fFovMax = fFov / 2;

  SDL_Rect Wall;
  Wall.w = nTileSize;
  float rayL = 1000;
  Wall.h = nTileSize;

  SDL_Event e;
  float fPA = 0;
  bool quit = false;

  while (!quit) {

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_w: {
          fPlX += cos(fPA) * 0.1;
          fPlY += sin(fPA) * 0.1;
          break;
        }
        case SDLK_s: {
          fPlX -= cos(fPA) * 0.1;
          fPlY -= sin(fPA) * 0.1;
          break;
        }
        case SDLK_LEFT: {

          fPA -= 0.1;
          if (fPA < 0)
            fPA = 2 * M_PI - 0.1;

          std::cout << "FPA: " << fPA << "\n";
          break;
        }
        case SDLK_RIGHT: {
          fPA += 0.1;
          if (fPA >= 2 * M_PI)
            fPA = 0;
          std::cout << "FPA: " << fPA << "\n";
          break;
        }
        }
      }
    }

    render_map(Wall);

    // rendering player
    //

    // vertical check
    //
    //
    float rx, ry;
    float xo, yo;
    float distH;
    int dof = 0;
    float Tan = tan(fPA);
    float vx, vy;
    if (cos(fPA) > 0.001) {
      SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0xff, 0xff);
      // looking to the right
      //
      rx = (((int)(fPlX)) / nTileSize) * nTileSize + nTileSize;

      ry = fPlY + (fPlX - rx) * Tan;

      xo = nTileSize;

      yo = -xo * Tan;

    } else if (cos(fPA) < 0.001) {
      // looking to the left
      //
      rx = (((int)(fPlX)) / nTileSize) * nTileSize - 0.001;
      ;

      ry = fPlY + (fPlX - rx) * Tan;

      xo = -nTileSize;

      yo = -xo * Tan;
    } else {
      dof = 10;
      vx = fPlX, vy = fPlY;
    }

    while (dof < 10) {
      int nMapX = (int)fPlX / nTileSize;
      int nMapY = (int)fPlY / nTileSize;
      int i = nMapX + nMapY * nMapHeight;

      if (i > 0 && i < nMapHeight * nMapWidth && gMap[i] == 1) {
        dof = 10;
      }

      else {
        rx += xo;
        ry += yo;
        dof++;
      }
    }
    vx = rx;
    vy = ry;

    fPlayer.x = (int)(nPlayerWidth * fPlX);
    fPlayer.y = (int)(nPlayerHeight * fPlY);
    vx = fPlayer.x + rayL * cos(fPA);
    vx = fPlayer.y + rayL * sin(fPA);
    SDL_RenderDrawLineF(gRenderer, fPlayer.x, fPlayer.y, vx, vy);
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderFillRectF(gRenderer, &fPlayer);

    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(gRenderer);
  }

  close_program();
  std::cout << "Hello Cruel World\n";
  return 0;
}
