#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>

void sdl_dump_version()
{
    SDL_version ver;
    SDL_GetVersion(&ver);
    printf("SDL2 version: v%d.%d.%d\n", ver.major, ver.minor, ver.patch);
}

void sdl_draw_frame(const char *image_path)
{
    SDL_Window *win;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL2 init failed\n");
        exit(-1);
    }

    surface = IMG_Load(image_path);
    // show image path in window title
    win = SDL_CreateWindow(image_path,
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           surface->w, surface->h, 0);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    while (1) {
        SDL_Event e;
        
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                break;
            else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
}

void usage()
{
    fprintf(stderr, "Usage: FrameViewer BGR888_FRAME_PATH\n");
    exit(-1);
}

int main(int argc, char *argv[]) 
{
    sdl_dump_version();
    if (argc < 2) usage();
    sdl_draw_frame(argv[1]);

    return 0;
}