#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <Utils/FPSTimer.hpp>
#include <Utils/Texture.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>
SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
const int SCREEN_WIDTH = 640 * 2;
const int SCREEN_HEIGHT = 480;
TTF_Font *gFont = nullptr;
const int mw = 24;
const int mh = 24;

int gMap[mw * mh] = {
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
void init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Couldn't Initalize SDL\n";
    exit(1);
  }
  if (TTF_Init()) {
    std::cerr << "Couldn't Init TTf";
    exit(1);
  }
  gWindow = SDL_CreateWindow("RayCaster", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!gWindow) {

    std::cerr << "Couldn't Create Window\n";
    exit(1);
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

  if (!gRenderer) {
    std::cerr << "Couldn't Create Renderer\n";
    exit(1);
  }
  gFont = TTF_OpenFont("./assets/font/Pixeled.ttf", 16);
  if (!gFont) {
    std::cerr << TTF_GetError();
    exit(1);
  }
}

void quit_program() {

  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
}
int main(int argc, char **argv) {
  init();
  std::printf("Program %s running with %d args\n", argv[0], argc);
  int nWallW = 20;
  int nWallH = 20;
  float fPlayerY = 12.0f;
  float fPlayerX = 4.0f;
  int nPlW = 10;
  int nPlH = 10;

  SDL_Rect Player;
  Player.x = fPlayerX * nPlW;
  Player.y = fPlayerY * nPlH;
  Player.w = nPlW;
  Player.h = nPlH;
  std::printf("%d, %d, %d, %d\n", Player.x, Player.y, Player.w, Player.h);
  float fFov = 2 * M_PI;

  float thetaSpacing = 0.01;

  float fAngle = 0;
  SDL_Rect Wall;
  float fFovMin = -fFov / 2;
  float fFovMax = fFov / 2;
  float l = 20;
  bool quit = false;
  SDL_Color col = {255, 255, 255};
  ETexture *fpsTexture = new ETexture(gRenderer, "GAY", gFont, col);

  FPSTimer *fpsTimer = new FPSTimer(fpsTexture, gFont, col);

  SDL_Event event;

  while (!quit) {
    float l = 0;
    int r = 0;
    for (float theta = fFovMin; theta <= fFovMax; theta += thetaSpacing, r++) {
      l = 0;
      float fLineStartX = fPlayerX * nPlW + Player.w / 2.0;
      float fLineStartY = fPlayerY * nPlH + Player.h / 2.0;

      float fLineEndX;
      float fLineEndY;
      float tLineX;
      while (true) {

        fLineEndX = l * cos(theta) * nWallW;
        fLineEndY = l * sin(theta) * nWallH;

        float tLineX = fLineEndX;
        fLineEndX =
            fLineStartX + cos(fAngle) * tLineX - sin(fAngle) * fLineEndY;
        fLineEndY =
            fLineStartY + sin(fAngle) * tLineX + cos(fAngle) * fLineEndY;
        int nMapX = (int)(fLineEndX / nWallW);
        int nMapY = (int)(fLineEndY / nWallH);
        nMapX = nMapX < 0 ? 0 : nMapX;
        nMapX = nMapX >= mw ? mw - 1 : nMapX;
        nMapY = nMapY < 0 ? 0 : nMapY;
        nMapY = nMapY >= mh ? mh - 1 : nMapY;

        if (gMap[nMapX + nMapY * mw]) {
          break;
        }
        l += 0.1;
      }
      SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

      SDL_RenderDrawLineF(gRenderer, fLineStartX, fLineStartY, fLineEndX,
                          fLineEndY);

      // calculating line height
      //
      // float lineH = (float)(SCREEN_HEIGHT * mh) / (l * 32);
      // lineH = lineH >= SCREEN_HEIGHT ? SCREEN_HEIGHT : lineH;
      // float lineOff = SCREEN_HEIGHT / 2 - (lineH / 2);
      /* std::printf("%f,- %f and l- %f\n", lineOff, lineH, l); */
      // Uint8 color = 255 * (1 - (l / mh));
      // SDL_SetRenderDrawColor(gRenderer, color, 0x00, 0x0d, 0x00);

      // Wall.x = (r)*8 + 500;

      // Wall.w = 8;
      // Wall.y = lineOff;
      // Wall.h = (int)(lineH);
      // SDL_RenderFillRect(gRenderer, &Wall);
    }
    while (SDL_PollEvent(&event)) {

      if (event.type == SDL_QUIT)
        quit = true;

      if (event.type == SDL_KEYDOWN) {

        switch (event.key.keysym.sym) {

        case SDLK_w: {

          fPlayerY -= 0.2;
          break;
        }
        case SDLK_s: {
          fPlayerY += 0.2;
          break;
        }
        case SDLK_a: {
          fPlayerX -= 0.2;
          break;
        }
        case SDLK_d: {
          fPlayerX += 0.2;
          break;
        }
        case SDLK_RIGHT: {
          fAngle += 0.1;
          break;
        }
        case SDLK_LEFT: {
          fAngle -= 0.1;
          break;
        }
        }
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0xff);
    for (int i = 0; i < mw * mh; i++) {
      int nXPos = i % mw;
      int nYPos = i / mh;

      if (gMap[i]) {
        Wall.x = nXPos * nWallW;
        Wall.y = nYPos * nWallH;
        Wall.w = nWallW;
        Wall.h = nWallH;
        SDL_RenderFillRect(gRenderer, &Wall);
      }
    }
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0x00);
    Player.x = static_cast<int>(fPlayerX * nPlW);
    Player.y = static_cast<int>(fPlayerY * nPlH);
    SDL_RenderFillRect(gRenderer, &Player);
    fpsTimer->displayFPS(SCREEN_WIDTH / 2 + 30, 20);
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
  }
  quit_program();
  return 0;
}
