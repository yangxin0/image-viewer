#include <cstdio>
#include <cstring>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int bgr888_width;
int bgr888_height;

typedef SDL_Surface *(*surface_loader)(const char *);

void sdl_dump_version()
{
    SDL_version ver;
    SDL_GetVersion(&ver);
    printf("SDL2 version: v%d.%d.%d\n", ver.major, ver.minor, ver.patch);
}

SDL_Surface *opencv_load_surface(const char *image_path)
{
    printf("use: opencv image decoder\n");
    cv::Mat mat = cv::imread(image_path, cv::IMREAD_COLOR);
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 
                                                mat.cols, mat.rows, 24, 
                                                0xFF0000, 0x00FF00, 0x0000FF,
                                                0);
    memcpy(surface->pixels, mat.data, mat.cols * mat.rows * 3);

    return surface;
}

SDL_Surface *bgr888_load_surface(const char *image_path)
{
    printf("use: raw bgr888 image is used\n");
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 
                                                bgr888_width, bgr888_height, 24, 
                                                0xFF0000, 0x00FF00, 0x0000FF,
                                                0);
    int nread = 0;
    int total = bgr888_width * bgr888_height * 3;
    int rc;
    FILE *fin = fopen(image_path, "rb");
    while (nread < total && (rc = fread((unsigned char *) surface->pixels + nread, 1, 1024, fin)) != EOF) {
        nread += rc;
    }
    
    if (nread != total) {
        SDL_FreeSurface(surface);
        surface = NULL;
    }

    fclose(fin);
    return surface;
}

void sdl_draw_frame(surface_loader loader, const char *image_path)
{
    SDL_Window *win;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL2 init failed\n");
        exit(-1);
    }

    surface = loader(image_path);
    if (surface == NULL) {
        fprintf(stderr,"create surface failed\n");
        return;
    }
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
    fprintf(stderr, "Usage: FrameViewer decoder image_path [width, height]\n"
                    "       decoder: default, opencv, bgr888\n"
                    "       Note: bgr888 requires image width and height\n");
    exit(-1);
}

int main(int argc, char *argv[]) 
{
    sdl_dump_version();
    if (argc < 3) usage();

    surface_loader loader;
    if (!strcmp(argv[1], "opencv")) {
        loader = opencv_load_surface;
    } else if (!strcmp(argv[1], "bgr888")) {
        if (argc < 5) usage();
        sscanf(argv[3], "%d", &bgr888_width);
        sscanf(argv[4], "%d", &bgr888_height);
        loader = bgr888_load_surface;
    } else {
        loader = IMG_Load;
    }

    sdl_draw_frame(loader, argv[2]);

    return 0;
}
