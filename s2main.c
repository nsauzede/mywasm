#define CONF_IMPL
#include "conf.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDL_Surface *screen = 0;
SDL_Window *sdlWindow = 0;
SDL_Renderer *sdlRenderer = 0;
SDL_Texture *sdlTexture = 0;
int quit = 0;
int cur = 0, last = -1;
conf_t c;
void loop() {
  SDL_Event e;
  // printf("polling events..\n");
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      quit = 1;
      break;
    }
    if (e.type == SDL_KEYDOWN) {
      // printf("key down!\n");
      if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q) {
        quit = 1;
        break;
      } else if (e.key.keysym.sym == SDLK_LEFT) {
        if (cur == 0)
          cur = napis;
        cur--;
      } else if (e.key.keysym.sym == SDLK_RIGHT) {
        cur = (cur + 1) % napis;
      }
    }
  }
  // printf("done\n");
  if (quit) {
    printf("QUIT!!!!\n");
    SDL_Quit();
#ifdef __EMSCRIPTEN__
    emscripten_force_exit(0);
#endif
    return;
  }
  if (cur != last) {
    int r, g, b;
    last = cur;
    const int image_width = IMG_WIDTH(c);
    const int image_height = IMG_HEIGHT(c);
    printf("calling api %d\n", cur);
    apis[cur](c);
    uint8_t *p = PIXELS(c);
    if (SDL_MUSTLOCK(screen))
      SDL_LockSurface(screen);
    for (int j = image_height - 1; j >= 0; --j) {
      for (int i = 0; i < image_width; ++i) {
        int r = *p++;
        int g = *p++;
        int b = *p++;
        *((Uint32 *)screen->pixels + j * image_width + i) =
            SDL_MapRGBA(screen->format, r, g, b, 255);
      }
    }
    if (SDL_MUSTLOCK(screen))
      SDL_UnlockSurface(screen);
    SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
  }
}

int main(int argc, char *argv[]) {
  int n = 0;
  int arg = 1;
  if (arg < argc) {
    sscanf(argv[arg++], "%d", &n);
  }
  if (n < 0 || n >= napis) {
    printf("Usage: %s [n] (max n=%d)\n", argv[0], napis - 1);
    exit(1);
  }
  // Image
  const int image_width = 400;
  const int image_height = 225;
  const int bpp = 32;
  IMG_WIDTH(c) = image_width;
  IMG_HEIGHT(c) = image_height;
  PIXELS(c) = calloc(image_width * image_height, 3);

  // SDL2
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(image_width, image_height, 0, &sdlWindow,
                              &sdlRenderer);
  screen = SDL_CreateRGBSurface(0, image_width, image_height, bpp, 0x00FF0000,
                                0x0000FF00, 0x000000FF, 0xFF000000);
  sdlTexture =
      SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, image_width, image_height);

#ifdef TEST_SDL_LOCK_OPTS
  EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; "
         "SDL.defaults.opaqueFrontBuffer = false;");
#endif

#ifdef __EMSCRIPTEN__
  printf("set main loop..\n");
  emscripten_set_main_loop(loop, 0, 1);
#else
  printf("looping..\n");
  while (!quit) {
    loop();
    SDL_Delay(100);
  }
#endif
  printf("bye\n");

  SDL_Quit();
  return 0;
}
