#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <iostream>
#include <limits>
// TODO:: the movement is screwed
SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int nMAP_WIDTH = 24;
const int nMAP_HEIGHT = 24;
int worldMap[nMAP_WIDTH][nMAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

void rotateVec(SDL_FRect &rect, float theta) {

  float x = rect.x;
  float y = rect.y;

  float xtemp = x * cos(theta) - y * sin(theta);
  y = x * sin(theta) + y * cos(theta);

  rect.x = xtemp;
  rect.y = y;
}
void init() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Couldn't Initalize SDL";
    exit(1);
  }

  gWindow = SDL_CreateWindow("RayCaster v1.4 ", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!gWindow) {
    std::cerr << "Couldn't Craete Window\n";
    exit(1);
  }

  gRenderer = SDL_CreateRenderer(
      gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!gRenderer) {
    std::cerr << "Couldn't Create Renderer\n";
    exit(1);
  }
}

void close_program() {

  SDL_DestroyWindow(gWindow);
  SDL_DestroyRenderer(gRenderer);

  SDL_Quit();
}
int main(int argc, char **argv) {

  init();

  SDL_FRect fPlayer;
  fPlayer.x = 3.0f;
  fPlayer.y = 22.0f;
  float forwardVel = 40;
  float rotVel = 2;
  SDL_FRect fCameraPlane = {0, 0.66, 0, 0}; // setting the camera plane
  SDL_FRect fPlayerDir = {
      -1, 0, 0, 0}; // player direct rependicular to  the player direction

  bool quit = false;

  SDL_Event e;
  Uint32 elapsed_time = SDL_GetTicks();
  while (!quit) {
    float deltaTime = (SDL_GetTicks() - elapsed_time) / 1000.0f;
    elapsed_time = SDL_GetTicks();
    // casting a ray for each pixel
    //
    //
    for (int x = 0; x < SCREEN_WIDTH; x++) {

      float cameraX = (2.0f * x) / SCREEN_WIDTH - 1;
      SDL_FPoint fRay;
      fRay.x = fPlayerDir.x + fCameraPlane.x * cameraX;

      fRay.y =
          fPlayerDir.y +
          fCameraPlane.y * cameraX; // projecting the rays into the screen view
      //
      int nMapX = static_cast<int>(fPlayer.x);
      int nMapY = static_cast<int>(fPlayer.y);

      int nSide;

      double fDeltaX = fRay.x == 0 ? std::numeric_limits<double>::max()
                                   : std::abs(1 / fRay.x);
      double fDeltaY = fRay.y == 0 ? std::numeric_limits<double>::max()
                                   : std::abs(1 / fRay.y);
      int xStep, yStep;
      double fSideX, fSideY;
      bool wallHit = false;

      if (fRay.x < 0) {

        // vertical intersection looking to the left
        //
        xStep = -1;

        fSideX = (fPlayer.x - nMapX) * fDeltaX;
      } else {
        // looking to the right
        //
        xStep = 1;

        fSideX = (nMapX + 1.0 - fPlayer.x) * fDeltaX;
      }

      if (fRay.y < 0) {
        yStep = -1;
        fSideY = (-nMapY + fPlayer.y) * fDeltaY;
      } else {
        yStep = 1;
        fSideY = (nMapY + 1.0 - fPlayer.y) * fDeltaY;
      }
      while (!wallHit) {

        if (fSideX < fSideY) {
          fSideX += fDeltaX;
          nMapX += xStep;
          nSide = 0;
        } else {
          fSideY += fDeltaY;
          nMapY += yStep;
          nSide = 1;
        }

        if (nMapX < 0 || nMapY < 0 || nMapX >= nMAP_WIDTH ||
            nMapY >= nMAP_HEIGHT || worldMap[nMapX][nMapY]) {
          wallHit = true;
        }
      }

      float fPrepDistance;
      if (nSide == 0) {
        fPrepDistance = fSideX - fDeltaX;
      } else
        fPrepDistance = fSideY - fDeltaY;

      double line_height = SCREEN_HEIGHT / fPrepDistance;

      float line_start = -line_height / 2.0 + SCREEN_HEIGHT / 2.0;
      line_start = line_start < 0 ? 0 : line_start;
      float line_end = line_height / 2.0 + SCREEN_HEIGHT / 2.0;

      line_end = line_end >= SCREEN_HEIGHT ? SCREEN_HEIGHT : line_end;

      Uint8 color = 0xff;
      if (nSide == 1)
        color /= 2;

      switch (worldMap[nMapX][nMapY]) {
      case 1: {
        SDL_SetRenderDrawColor(gRenderer, color, 0x00, 0x00, 0x00);
        break;
      }
      case 2: {
        SDL_SetRenderDrawColor(gRenderer, 0x00, color, 0x00, 0x00);
        break;
      }
      case 3: {
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, color, 0x00);
        break;
      }
      case 4: {
        SDL_SetRenderDrawColor(gRenderer, color, 0x00, color, 0x00);
        break;
      }
      case 5: {
        SDL_SetRenderDrawColor(gRenderer, 0x00, color, color, 0x00);
        break;
      }
      }
      SDL_RenderDrawLineF(gRenderer, x, line_start, x, line_end);
      SDL_SetRenderDrawColor(gRenderer, 135, 206, 250, 255);
      SDL_RenderDrawLineF(gRenderer, x, 0, x, line_start);

      SDL_SetRenderDrawColor(gRenderer, 64, 64, 64, 0xff);
      SDL_RenderDrawLineF(gRenderer, x, line_end, x, SCREEN_HEIGHT);
    }

    float moveSpeed = deltaTime * forwardVel;
    float rotate = deltaTime * rotVel;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;

      // TODO: other input handling

      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {

        case SDLK_w: {
          // move forward
          //
          if (worldMap[int(fPlayer.x + fPlayerDir.x * moveSpeed)]
                      [int(fPlayer.y)] == false) {
            fPlayer.x += fPlayerDir.x * moveSpeed;
            std::cout << "PL: " << fPlayer.x << " movevel: " << moveSpeed

                      << std::endl;
          }
          if (worldMap[int(fPlayer.x)]
                      [int(fPlayer.y - fPlayerDir.y * moveSpeed)] == false) {
            fPlayer.y += fPlayerDir.x * moveSpeed;
          }
        }
        case SDLK_s: {
          if (worldMap[int(fPlayer.x - fPlayerDir.x * moveSpeed)]
                      [int(fPlayer.y)] == false) {
            fPlayer.x -= fPlayerDir.x * moveSpeed;
          }
          if (worldMap[int(fPlayer.x)]
                      [int(fPlayer.y + fPlayerDir.y * moveSpeed)] == false) {
            fPlayer.y += fPlayerDir.x * moveSpeed;
          }
          break;
        }
        case SDLK_a: {
          rotateVec(fPlayerDir, rotVel);
          rotateVec(fCameraPlane, rotVel);
          break;
        }
        case SDLK_d: {
          rotateVec(fPlayerDir, -rotVel);
          rotateVec(fCameraPlane, -rotVel);
          std::cout << "rot vel: " << rotate << " movevel: " << moveSpeed
                    << std::endl;
          break;
        }
        }
      }
    }

    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);

    SDL_RenderClear(gRenderer);
    // Render some bullshit
  }

  std::printf("%s program running with %d args\n", argv[0], argc);
  std::cout << "Hello Cruel World\n";

  close_program();
  return 0;
}
