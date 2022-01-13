#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>

#define WIDTH 800
#define HEIGHT 600

void sdl_dump_version()
{
    SDL_version ver;
    SDL_GetVersion(&ver);
    printf("SDL2 version: v%d.%d.%d\n", ver.major, ver.minor, ver.patch);
}

void sdl_draw_frame(const char *frame_path)
{
    SDL_Window *win;
    SDL_Renderer *render;
    SDL_Texture *frame;
    int width, height;
    SDL_Rect rect;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL2 init failed\n");
        exit(-1);
    }

    win = SDL_CreateWindow("Frame Viewer", 
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED, 
                            WIDTH, HEIGHT, 0);
    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    frame = IMG_LoadTexture(render, frame_path);
    SDL_QueryTexture(frame, NULL, NULL, &width, &height);
    rect.x = WIDTH / 2;
    rect.y = HEIGHT / 2;
    rect.w = width * 2;
    rect.h = height * 2;

    while (1) {
        SDL_Event e;
        
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                break;
            else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                break;
        }

        SDL_RenderClear(render);
        SDL_RenderCopy(render, frame, NULL, &rect);
        SDL_RenderPresent(render);
    }

    SDL_DestroyTexture(frame);
    SDL_DestroyRenderer(render);
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