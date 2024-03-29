#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <Utils/FPSTimer.hpp>
#include <Utils/Texture.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
// TODO:: INCREASE ACURRACY
//
// FIX: the ray cast evolv doesn't cast rays on certain angles
#define degToRad(a) ((a * M_PI) / 180.0f)
#define radToDeg(r) ((r * 180) / M_PI)
#define dist(x1, y1, x2, y2)                                                   \
  ((sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
// Window and Renderer

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
TTF_Font *gFont = nullptr;

void cast_rays(SDL_Rect Player, float fPA, float fFov, float theta_spacing);
void cast_raysEvol(SDL_Rect Player, float fPA, float fFov);
// screen parameters
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
// player width and height
const int nPLW = 10;
const int nPLH = 10;
float forwardVel = 0.1;
float rotVel = 0.95;
const int nMapHeight = 24;
const int nMapWidth = 24;
const int MAX_DOF = nMapWidth;
int nTileSize = 20;
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
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
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
    std::cerr << "Couldn't Initalize Font\n";
    exit(1);
  }

  gWindow = SDL_CreateWindow(
      "RayCaster 1.3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH + SCREEN_HEIGHT, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!gWindow) {
    std::cerr << "Couldn't Create Window \n";
    exit(1);
  }
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

  if (!gRenderer) {
    std::cerr << "Coudn't Create Renderer\n";
    exit(1);
  }
  gFont = TTF_OpenFont("./assets/font/Pixeled.ttf", 16);
  if (!gFont) {
    std::cerr << "Error: " << TTF_GetError();
  }
}

void close_program() {

  SDL_DestroyWindow(gWindow);
  SDL_DestroyRenderer(gRenderer);
  TTF_CloseFont(gFont);
  TTF_Quit();
  SDL_Quit();
}

void render_map(SDL_Rect *wall) {

  if (wall == nullptr)
    return;

  SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0x00);
  for (size_t i = 0; i < nMapWidth * nMapHeight; i++) {
    if (gMap[i]) {
      int nXpos = i % nMapWidth;
      int nYPos = i / nMapHeight;

      wall->x = nXpos * nTileSize;
      wall->y = nYPos * nTileSize;

      SDL_RenderFillRect(gRenderer, wall);
    }
  }
}

int main(int argc, char **argv) {

  init();
  std::printf("Program %s running with %d args\n", argv[0], argc);
  std::cout << "Gmap: " << gMap[382] << "\n";
  float fPLX = 3.3f;
  float fPLY = 17.0f;

  SDL_Rect Player;
  Player.x = fPLX * nPLW;
  Player.y = fPLY * nPLH;
  Player.x = 33;
  Player.y = 170;
  Player.w = nPLW;
  Player.h = nPLH;

  float fPA = M_PI / 2;
  float fFov = M_PI / 2;
  float fFovMin = -fFov / 2;
  float fFovMax = fFov / 2;
  float theta_spacing = 0.01;

  SDL_Rect Wall;
  Wall.w = Wall.h = nTileSize;
  SDL_Event e;
  SDL_Color col = {255, 255, 255};
  ETexture *fpsTexture = new ETexture(gRenderer, "GAY", gFont, col);

  FPSTimer *fpsTimer = new FPSTimer(fpsTexture, gFont, col);

  bool quit = false;
  Uint32 elapsed_time = SDL_GetTicks();
  while (!quit) {
    float deltaTime = (SDL_GetTicks() - elapsed_time);
    elapsed_time = SDL_GetTicks();
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;

      if (e.type == SDL_KEYDOWN) {

        switch (e.key.keysym.sym) {

        case SDLK_w: {
          fPLX += deltaTime * forwardVel * cos(fPA);
          fPLY -= forwardVel * deltaTime * sin(fPA);
          int nPX = static_cast<int>((fPLX * nPLW / nTileSize));
          int nPY = static_cast<int>((fPLY * nPLW / nTileSize));
          if (nPX + nPY * nMapHeight < 0 ||
              nPX + nPY * nMapHeight > nMapHeight * nMapWidth ||
              gMap[nPX + nPY * nMapHeight]) {
            fPLX -= deltaTime * forwardVel * cos(fPA);
            fPLY += forwardVel * deltaTime * sin(fPA);
          }
          break;
        }

        case SDLK_s: {
          fPLX -= forwardVel * deltaTime * cos(fPA);
          fPLY += forwardVel * deltaTime * sin(fPA);
          int nPX = static_cast<int>((fPLX * nPLW / nTileSize));
          int nPY = static_cast<int>((fPLY * nPLW / nTileSize));
          if (nPX + nPY * nMapHeight < 0 ||
              nPX + nPY * nMapHeight > nMapHeight * nMapWidth ||
              gMap[nPX + nPY * nMapHeight]) {
            fPLX += deltaTime * forwardVel * cos(fPA);
            fPLY -= forwardVel * deltaTime * sin(fPA);
          }
          break;
        }
        case SDLK_RIGHT: {
          float deg = (radToDeg(fPA)) - rotVel * deltaTime;
          if (deg <= 0)
            deg = 360;
          fPA = degToRad(deg);
          /* std::cout << "deg " << deg << "\n"; */
          break;
        }
        case SDLK_LEFT: {
          float deg = (radToDeg(fPA)) + (rotVel * deltaTime);
          if (deg <= 0)
            deg = 360;
          fPA = degToRad(deg);
          break;
        }
        case SDLK_e: {
          std::printf("Posx: %f ,PosY: %f, fPA: %f\n", (float)Player.x,
                      (float)Player.y, fPA);
          break;
        }
        }
      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        int index = (mx / nTileSize) + nMapWidth * (my / nTileSize);
        std::printf("[%d, %d, %d]\n", index, mx / nTileSize, my / nTileSize);
      }
    }

    render_map(&Wall);

    // vertical check
    //
    //

    Player.x = fPLX * nPLW;
    Player.y = fPLY * nPLH;
    /* cast_raysEvol(Player, fPA, fFov); */
    cast_rays(Player, fPA, fFov, theta_spacing);

    /* vx = Player.x + rayL * cos(fPA); */
    /* vy = Player.y - rayL * sin(fPA); */
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(gRenderer, &Player);
    fpsTimer->displayFPS(SCREEN_HEIGHT + 10, 5);
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gRenderer);
  }

  close_program();
  return EXIT_SUCCESS;
}

void cast_rays(SDL_Rect Player, float fPA, float fFov, float theta_spacing) {
  float rx, ry; // ray start postion
  //
  //
  float fFovMin = -fFov / 2;
  float fFovMax = fFov / 2;
  int line_width;
  int num_rays = fFov / theta_spacing;
  line_width = SCREEN_HEIGHT;
  if (num_rays != 0)
    line_width = SCREEN_WIDTH / num_rays;
  int lineXoffset = 0;
  float hx = Player.x, hy = Player.y;

  float xo, yo;                       // x and y offsetn
  float vx = Player.x, vy = Player.y; // ray verticies
  for (float fRA = fFovMin + fPA; fRA < fPA + fFovMax; fRA += theta_spacing) {
    int deg = radToDeg(fRA);
    float fRayAngle = degToRad(deg % 360);
    //
    hx = Player.x;
    hy = Player.y;
    vx = Player.x;
    vy = Player.y;
    // casting rays
    float distH = std::numeric_limits<float>::max();
    float distV = std::numeric_limits<float>::max();

    float Tan = tan(fRayAngle);
    int dof = 0;
    if (cos(fRayAngle) > std::numeric_limits<float>::min()) {
      // looking to the right
      rx = (Player.x / nTileSize) * nTileSize + nTileSize;

      ry = Player.y + (Player.x - rx) * Tan;

      xo = nTileSize;
      yo = -xo * Tan;
    } else if (cos(fRayAngle) <
               std::numeric_limits<float>::min()) // looking to the left

    {

      rx = (Player.x / nTileSize) * nTileSize - 0.001;

      ry = Player.y + (Player.x - rx) * Tan;
      xo = -nTileSize;
      yo = -xo * Tan;

    } else {
      // looking up or down no vertical intersection

      hx = Player.x;
      hy = Player.y;
      dof = MAX_DOF;
    }
    while (dof < MAX_DOF) {
      int nMapX = ((int)(rx)) / nTileSize;
      int nMapY = ((int)(ry)) / nTileSize;
      int index = nMapX + nMapWidth * nMapY;
      if (index >= 0 && index < nMapHeight * nMapWidth && gMap[index]) {
        dof = MAX_DOF;
        hx = rx;
        hy = ry;
        distH = dist(Player.x, Player.y, hx, hy);
        distH =
            cos(fRayAngle) * (hx - Player.x) - sin(fRayAngle) * (hy - Player.y);
      } else {
        dof++;
        if (dof >= MAX_DOF)
          break;
        rx += xo;
        ry += yo;
      }
    }
    // horizontal check
    //
    //
    hx = rx;
    hy = ry;
    float invTan = 1 / Tan;

    dof = 0; // resetting debth of field
    if (sin(fRayAngle) > std::numeric_limits<float>::min()) {

      ry = (Player.y / nTileSize) * nTileSize - 0.001;
      rx = Player.x + (Player.y - ry) * invTan;
      yo = -nTileSize;
      xo = -yo * (invTan);
      // looking up
    } else if (sin(fRayAngle) < std::numeric_limits<float>::min()) {
      ry = (Player.y / nTileSize) * nTileSize + nTileSize;
      rx = Player.x + (Player.y - ry) * invTan;
      yo = nTileSize;
      xo = -yo * invTan;
    } else {
      vx = Player.x;
      vy = Player.y;
      dof = MAX_DOF;
    }

    while (dof < MAX_DOF) {
      int nMapX = (int)(rx) / nTileSize;
      int nMapY = (int)ry / nTileSize;
      int index = nMapX + nMapWidth * nMapY;
      if (index >= 0 && index < nMapWidth * nMapHeight && gMap[index]) {
        vx = rx;
        vy = ry;
        distV = dist(Player.x, Player.y, vx, vy);
        distV =
            cos(fRayAngle) * (vx - Player.x) - sin(fRayAngle) * (vy - Player.y);
        dof = MAX_DOF;

      } else {
        dof++;
        if (dof >= MAX_DOF)
          break;
        rx += xo;
        ry += yo;
      }
    }
    vx = rx;
    vy = ry;
    float d = 1;
    if (distV > distH) {
      vx = hx;
      vy = hy;
      d = 0.7;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderDrawLineF(gRenderer, Player.x + nPLW / 2, Player.y + nPLH / 2, vx,
                        vy);

    float min_line =
        std::min(distV, distH) * cos(fPA - fRayAngle); // fish eye effect fix

    float line_height = SCREEN_HEIGHT * nMapHeight / min_line;
    line_height = line_height >= SCREEN_HEIGHT ? SCREEN_HEIGHT : line_height;
    float lineOff = SCREEN_HEIGHT / 2.0f - line_height / 2.0f;
    SDL_FRect wall_line = {SCREEN_HEIGHT + 0.0f + lineXoffset, lineOff,
                           static_cast<float>(line_width * 2), line_height};
    lineXoffset += line_width;
    SDL_FRect celling = {SCREEN_HEIGHT + 0.0f + lineXoffset, 0.0f,
                         static_cast<float>(line_width), lineOff};
    SDL_FRect floor = {SCREEN_HEIGHT + 0.0f + lineXoffset,
                       lineOff + line_height, static_cast<float>(line_width),
                       SCREEN_HEIGHT - lineOff};
    // (1 - (min_line / SCREEN_HEIGHT));
    d = d < 0 ? 0 : d;
    SDL_SetRenderDrawColor(gRenderer, 135, 206, 250, 255);
    SDL_RenderFillRectF(gRenderer, &celling);
    SDL_SetRenderDrawColor(gRenderer, 178 * d, 34 * d, 34 * d, 255);
    SDL_RenderFillRectF(gRenderer, &wall_line);
    SDL_SetRenderDrawColor(gRenderer, 64, 64, 64, 0xff);
    SDL_RenderFillRectF(gRenderer, &floor);
  }
}
// BUG: THERE IS A GIANT FUCKING CHASM WHERE RAY ANGLES SHOULD BE PRESENT
void cast_raysEvol(SDL_Rect Player, float fPA, float fFov) {
  float rx, ry; // ray start postion
  float vx, vy; // ray verticies
  //
  //
  //
  float hx, hy;
  float xo, yo; // x and y offsetn
  const int MAX_DOF = 20;

  for (int i = 0; i < SCREEN_WIDTH; i++) {
    // do the calculations here
    //
    //
    //
    int deg = radToDeg(fPA);
    float fRayAngle = degToRad(deg % 360);

    fRayAngle = fPA - fFov / 2 + (float)i * fFov / (float)(SCREEN_WIDTH);
    float distH = std::numeric_limits<float>::max();
    float distV = std::numeric_limits<float>::max();

    float Tan = tan(fRayAngle);
    int dof = 0;
    if (cos(fRayAngle) > std::numeric_limits<float>::min()) {
      // looking to the right

      rx = (Player.x / nTileSize) * nTileSize + nTileSize;

      ry = Player.y + (Player.x - rx) * Tan;

      xo = nTileSize;
      yo = -xo * Tan;
    } else if (cos(fRayAngle) <
               std::numeric_limits<float>::min()) // looking to the left

    {

      rx = (Player.x / nTileSize) * nTileSize - 0.001;

      ry = Player.y + (Player.x - rx) * Tan;
      xo = -nTileSize;
      yo = -xo * Tan;

    } else {
      // looking up or down no vertical intersection

      hx = Player.x;
      hy = Player.y;
      dof = MAX_DOF;
    }
    while (dof < MAX_DOF) {
      int nMapX = ((int)(rx)) / nTileSize;
      int nMapY = ((int)(ry)) / nTileSize;
      int index = nMapX + nMapWidth * nMapY;

      if (index >= 0 && index < nMapHeight * nMapWidth && gMap[index]) {
        dof = MAX_DOF;
        hx = rx;
        hy = ry;
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xff, 0xff, 0x00);
        SDL_RenderDrawLineF(gRenderer, Player.x + nPLW / 2, Player.y + nPLH / 2,
                            hx, hy);
        distH = dist(Player.x, Player.y, hx, hy);
      } else {
        dof++;
        if (dof >= MAX_DOF)
          break;
        rx += xo;
        ry += yo;
        dof++;
      }
    }
    hx = rx;
    hy = ry;
    // if we riched depth of

    // horizontal check
    //
    //

    float invTan = 1 / Tan;

    dof = 0; // resetting debth of field
    if (sin(fRayAngle) > std::numeric_limits<float>::min()) {

      ry = (Player.y / nTileSize) * nTileSize - 0.001;
      rx = Player.x + (Player.y - ry) * invTan;
      yo = -nTileSize;
      xo = -yo * (invTan);
      // looking up
    } else if (sin(fRayAngle) < std::numeric_limits<float>::min()) {
      ry = (Player.y / nTileSize) * nTileSize + nTileSize;
      rx = Player.x + (Player.y - ry) * invTan;
      yo = nTileSize;
      xo = -yo * invTan;
    } else {
      vx = Player.x;
      vy = Player.y;
      dof = MAX_DOF;
    }

    while (dof < MAX_DOF) {
      int nMapX = (int)(rx) / nTileSize;
      int nMapY = (int)ry / nTileSize;
      int index = nMapX + nMapWidth * nMapY;

      if (index >= 0 && index < nMapWidth * nMapHeight && gMap[index]) {
        vx = rx;
        vy = ry;
        distV = dist(Player.x, Player.y, vx, vy);
        SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0xff, 0x00);
        SDL_RenderDrawLineF(gRenderer, Player.x + nPLW / 2, Player.y + nPLH / 2,
                            vx, vy);
        dof = MAX_DOF;
      } else {
        dof++;
        if (dof >= MAX_DOF)
          break;

        rx += xo;
        ry += yo;
      }
    }
    vx = rx;
    vy = ry;
    if (distV > distH) {
      vx = hx;
      vy = hy;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderDrawLineF(gRenderer, Player.x + nPLW / 2, Player.y + nPLH / 2, vx,
                        vy);
    deg = radToDeg(fPA);
    float fPAngle = degToRad(deg % 360);
    float minLine = std::min(distV, distH) * cos(fRayAngle - fPAngle);

    float LineH =
        (SCREEN_HEIGHT * nMapHeight) / minLine; // std::min(distV, distH);
    LineH = LineH > SCREEN_HEIGHT ? SCREEN_HEIGHT : LineH;
    float LineOff = SCREEN_HEIGHT / 2.0f - LineH / 2;
    Uint8 color = 255 * (1 - (minLine / SCREEN_HEIGHT));

    SDL_SetRenderDrawColor(gRenderer, color, 0x00, 0x00, 0x00);
    SDL_RenderDrawLineF(gRenderer, i + SCREEN_WIDTH + 10, LineOff,
                        i + SCREEN_WIDTH + 10, LineOff + LineH);
  }
}
