#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <Utils/FPSTimer.hpp>
#include <Utils/Texture.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
SDL_Window *gWindow = nullptr;

SDL_Renderer *gRenderer = nullptr;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font *gFont = nullptr;
const int nMapWidth = 24;
SDL_Texture *gTexture = nullptr;
const int nMapHeight = 24;
Uint32 *screenBuff;
const int nTextureHeight = 64;
const int nTextureWidth = 64;
int worldMap[nMapWidth][nMapHeight] = {
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 4, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 0, 7, 7, 7, 7, 7},
    {4, 0, 5, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 6, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
    {4, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 8, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
    {4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 1},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 6, 0, 6, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
    {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 2},
    {4, 0, 0, 5, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
    {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3}};
std::vector<std::vector<Uint32>> TextureArray;
void rotateVec(SDL_FPoint &rect, float theta) {

  float x = rect.x;
  float y = rect.y;

  float xtemp = x * std::cos(theta) - y * std::sin(theta);
  y = x * std::sin(theta) + y * std::cos(theta);

  rect.x = xtemp;
  rect.y = y;
}
void setDrawColor(SDL_Renderer *render, Uint32 rgb) {

  Uint8 r = rgb >> 16;
  Uint8 g = (rgb >> 8) & 0xff;
  Uint8 b = (rgb)&0xff;

  SDL_SetRenderDrawColor(render, r, g, b, 0xff);
}
void init() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Error: " << SDL_GetError() << std::endl;
    exit(1);
  }
  if (TTF_Init()) {
    std::cerr << "Couldn't Initalize Font\n";
    exit(1);
  }

  gWindow = SDL_CreateWindow("RayCast v1.4.1", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!gWindow) {
    std::cerr << "Error: " << SDL_GetError() << std::endl;
    exit(1);
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (!gRenderer) {
    std::cerr << "Error " << SDL_GetError() << std::endl;
    exit(1);
  }
  Uint8 img_flags = IMG_INIT_PNG;

  if (!(IMG_Init(img_flags) & img_flags)) {
    std::cerr << "couldn't init img\n";
    exit(1);
  }
  gFont = TTF_OpenFont("./assets/font/Pixeled.ttf", 16);
  if (!gFont) {
    std::cerr << "Error: " << TTF_GetError();
    exit(1);
  }

  gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888,
                               SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                               SCREEN_HEIGHT);

  if (!gTexture) {
    std::cerr << "Error: " << TTF_GetError() << std::endl;
    exit(1);
  }
}

void close_program() {

  SDL_DestroyWindow(gWindow);
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyTexture(gTexture);
  TTF_CloseFont(gFont);
  TTF_Quit();
  SDL_Quit();
}

void addTexture(std::string path) {

  SDL_Surface *testSurface = IMG_Load(path.c_str());
  if (!testSurface) {

    std::cout << SDL_GetError() << std::endl;
    exit(1);
  }

  SDL_Surface *optimizedSurface =
      SDL_ConvertSurfaceFormat(testSurface, SDL_PIXELFORMAT_RGBA8888, 0);

  int pitch = optimizedSurface->pitch / 4;

  Uint32 *pixels = static_cast<Uint32 *>(optimizedSurface->pixels);

  std::vector<Uint32> texels;
  texels.resize(nTextureWidth * nTextureHeight);

  for (int x = 0; x < nTextureWidth; x++) {
    for (int y = 0; y < nTextureHeight; y++) {
      texels[x + nTextureWidth * y] = pixels[x + y * pitch];
    }
  }

  TextureArray.push_back(std::move(texels));
}
void clean_buffer(Uint32 *buffer, int width, int height, Uint32 clean) {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {

      buffer[x + y * width] = clean;
    }
  }
}
int main(int argc, char **argv) {
  init();

  std::cout << SDL_GetError() << std::endl;
  SDL_FPoint fPlayer;
  fPlayer.x = 10;
  fPlayer.y = 2.0f;
  addTexture("./assets/texture/tex.png");

  addTexture("./assets/texture/greystone.png");
  addTexture("./assets/texture/tex1.png");
  addTexture("./assets/texture/tex2.png");
  addTexture("./assets/texture/tex3.png");
  /* addTexture("./assets/texture/mossy.png"); */
  /* addTexture("./assets/texture/purplestone.png"); */

  float fSpeed = 10;
  float fRotSpeed = 5;
  SDL_FPoint fPlayerDir;
  fPlayerDir.x = -1;
  fPlayerDir.y = 0;
  SDL_FPoint fCameraPlane = {0, 0.5};

  SDL_Event e;
  SDL_Color col = {255, 255, 255};
  ETexture *fpsTexture = new ETexture(gRenderer, "GAY", gFont, col);

  FPSTimer *fpsTimer = new FPSTimer(fpsTexture, gFont, col);

  bool quit = false;
  Uint32 elapsed_time = SDL_GetTicks();
  while (!quit) {

    float delta = (SDL_GetTicks() - elapsed_time) / 1000.0f;
    elapsed_time = SDL_GetTicks();
    // begin the ray casting
    //
    //
    int pitch;

    SDL_LockTexture(gTexture, nullptr, (void **)&screenBuff, &pitch);
    pitch /= 4;
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      float fCameraX = 2.0f * x / SCREEN_WIDTH - 1.0;

      float fRayDirX = fPlayerDir.x + fCameraPlane.x * fCameraX;
      float fRayDirY = fPlayerDir.y + fCameraPlane.y * fCameraX;

      int nMapX = static_cast<int>(fPlayer.x);
      int nMapY = static_cast<int>(fPlayer.y);

      float fSideX, fSideY; // the leanth of horizotal and vertical ray cast
      int nSide;            // which side was hit
      //

      int nXStep, nYStep;
      float fDeltaX = fRayDirX == 0 ? std::numeric_limits<float>::max()
                                    : 1 / std::abs(fRayDirX);
      float fDeltaY = fRayDirY == 0 ? std::numeric_limits<float>::max()
                                    : 1 / std::abs(fRayDirY);

      // vertical intersection
      //
      if (fRayDirX < 0) {
        nXStep = -1;
        // looking to the left
        //
        //
        fSideX = (fPlayer.x - nMapX) * fDeltaX;
      } else {
        nXStep = 1;

        fSideX = (nMapX + 1 - fPlayer.x) * fDeltaX;
      }

      if (fRayDirY > 0) {
        nYStep = -1;

        fSideY = (fPlayer.y - nMapY) * fDeltaY;
      } else {
        nYStep = 1;

        fSideY = (nMapY + 1 - fPlayer.y) * fDeltaY;
      }

      bool hitWall = false;

      while (!hitWall) {

        if (fSideX < fSideY) {
          fSideX += fDeltaX;
          nMapX += nXStep;
          nSide = 0;
        } else {
          fSideY += fDeltaY;
          nMapY += nYStep;
          nSide = 1;
        }
        if (nMapX < 0 || nMapY < 0 || nMapX > nMapWidth || nMapY > nMapHeight ||
            worldMap[nMapX][nMapY])
          hitWall = true;
      }

      float fPerpDistance;
      if (nSide == 0)
        fPerpDistance = fSideX - fDeltaX;
      else
        fPerpDistance = fSideY - fDeltaY;

      float line_height = SCREEN_HEIGHT / fPerpDistance;

      float line_start = -line_height / 2 + SCREEN_HEIGHT / 2.0;
      line_start = line_start < 0 ? 0 : line_start;
      float line_end = line_height / 2 + SCREEN_HEIGHT / 2.0;

      line_end = line_end >= SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : line_end;

      int nTextureNum = worldMap[nMapX][nMapY] - 1;
      if (nTextureNum >= (int)TextureArray.size())
        nTextureNum = TextureArray.size() - 1;
      float fWallX;
      if (nSide == 0) {
        fWallX = fPlayer.y + fPerpDistance * fRayDirY;
      } else
        fWallX = fPlayer.x + fPerpDistance * fRayDirX;

      fWallX = fWallX - std::floor(fWallX);

      int nTetX =
          fWallX * nTextureWidth; // the x postion of the starting texture
      if (nSide == 0 && fRayDirX > 0)
        nTetX = nTextureWidth - nTetX - 1;
      if (nSide == 1 && fRayDirY > 0)
        nTetX = nTextureWidth - nTetX - 1;
      float fStep = 1.0 * nTextureHeight / line_height;
      float fTetY =
          (line_start - SCREEN_HEIGHT / 2.0 + line_height / 2.0) * fStep;

      for (int y = line_start; y < line_end; y++) {
        int nTetY = static_cast<int>(fTetY) & (nTextureHeight - 1);
        fTetY += fStep;

        Uint32 color =
            TextureArray[nTextureNum][nTetX + nTextureHeight * nTetY];

        screenBuff[x + y * pitch] = color;
      }

      SDL_RenderDrawLineF(gRenderer, x, line_start, x, line_end);
      SDL_RenderDrawLineF(gRenderer, x, line_start, x, line_end);
      SDL_SetRenderDrawColor(gRenderer, 135, 206, 250, 255);
      SDL_RenderDrawLineF(gRenderer, x, 0, x, line_start);

      SDL_SetRenderDrawColor(gRenderer, 64, 64, 64, 0xff);
      SDL_RenderDrawLineF(gRenderer, x, line_end, x, SCREEN_HEIGHT);
    }

    SDL_UnlockTexture(gTexture);
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;

      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {

        case SDLK_w: {
          float fTempX = fPlayer.x + delta * fSpeed * fPlayerDir.x;
          float fTempY = fPlayer.y - delta * fSpeed * fPlayerDir.y;
          if (!worldMap[(int)fTempX][(int)fPlayer.y]) {
            fPlayer.x = fTempX;
          }
          if (!worldMap[(int)fPlayer.x][(int)fTempY])
            fPlayer.y = fTempY;
          break;
        }
        case SDLK_s: {
          float fTempX = fPlayer.x - delta * fSpeed * fPlayerDir.x;
          float fTempY = fPlayer.y + delta * fSpeed * fPlayerDir.y;
          if (!worldMap[(int)fTempX][(int)fPlayer.y]) {
            fPlayer.x = fTempX;
          }
          if (!worldMap[(int)fPlayer.x][(int)fTempY])
            fPlayer.y = fTempY;
          break;
        }
        case SDLK_a: {
          rotateVec(fCameraPlane, delta * fRotSpeed);
          rotateVec(fPlayerDir, delta * fRotSpeed);
          break;
        }
        case SDLK_d: {
          rotateVec(fCameraPlane, -delta * fRotSpeed);
          rotateVec(fPlayerDir, -delta * fRotSpeed);

          break;
        }
        }
      }
    }

    SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);
    fpsTimer->displayFPS();
    SDL_RenderPresent(gRenderer);
    clean_buffer(screenBuff, pitch, SCREEN_HEIGHT, 0x00);
    setDrawColor(gRenderer, 0x00);

    SDL_RenderClear(gRenderer);
  }

  std::printf("Program %s running with %d argc\n", argv[0], argc);
  std::cout << "Hello Cruel World\n";

  close_program();
  return 0;
}
