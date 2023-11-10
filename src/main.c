#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FONT_SIZE = 18;
const int MAX_SQUARES = 1000000;
const int FPS = 144;
const float ANGLE_INCREMENT = 10.0f;
const float SHRINK_MODIFIER = 0.65f;
const float SPEED_MULTIPLIER = 1.01f;

typedef struct
{
    SDL_FRect rect;
    float dx, dy, r, g, b, speed;
} Square;
SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color);
void collisionHandler(Square *squares, int i, int *squareCount, float *angle);

int main(int argc, char *args[])
{
    int BANNER_HEIGHT;
    srand(time(NULL));
    Square *squares;
    squares = malloc(MAX_SQUARES * sizeof(Square));
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Renderer *renderer = NULL;
    int squareCount = 0;
    squares[0] = (Square){
        (SDL_FRect){
            SCREEN_WIDTH / 4,
            SCREEN_HEIGHT / 4,
            SCREEN_HEIGHT / 1.4,
            SCREEN_HEIGHT / 1.4,
        },
        0.35f, -0.2f, 0x00, 0x00, 0xFF, 1.0f};

    squareCount++;
    if (TTF_Init() < 0)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL FAILED TO INITIALIZE SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("Splitting Squares",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("SDL Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface,
                         NULL,
                         SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

            TTF_Font *font = TTF_OpenFont("./fonts/Roboto-Medium.ttf", FONT_SIZE);
            if (font == NULL)
            {
                printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
                return 1;
            }

            SDL_Event e;
            bool quit = false;
            float angle = 0.0f;
            Uint32 startTime = SDL_GetTicks();
            Uint32 lastFrameTime = startTime;
            int frames = 0;
            while (!quit)
            {
                if (squareCount >= MAX_SQUARES - 1)
                {
                    printf("MAX SQUARES REACHED. EXITING\n");
                    return 0;
                }
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                Uint32 currentTime = SDL_GetTicks();
                Uint32 elapsedTime = currentTime - lastFrameTime;

                float fps = 1000.0f / elapsedTime;
                lastFrameTime = currentTime;

                SDL_Color textColor = {255, 255, 255, 255};
                char text[256];
                sprintf(text, "SQUARE COUNT: %d\nFPS: %d", squareCount, (int)fps);
                SDL_Texture *textTexture = renderText(renderer, font, text, textColor);
                SDL_Rect textRect = {0, 0};
                SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
                BANNER_HEIGHT = textRect.h;
                SDL_Rect banner = {0, 0, SCREEN_WIDTH, BANNER_HEIGHT};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                SDL_RenderFillRect(renderer, &banner);
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);

                for (int i = 0; i < squareCount; i++)
                {
                    SDL_SetRenderDrawColor(renderer, squares[i].r, squares[i].g, squares[i].b, 0xFF);
                    SDL_RenderFillRectF(renderer, &squares[i].rect);

                    if (squares[i].rect.x <= 0 || squares[i].rect.x + squares[i].rect.w >= SCREEN_WIDTH)
                    {
                        collisionHandler(squares, i, &squareCount, &angle);
                        squares[i].dx *= -1;
                    }
                    if (squares[i].rect.y <= 0 + BANNER_HEIGHT || squares[i].rect.y + squares[i].rect.h >= SCREEN_HEIGHT)
                    {
                        collisionHandler(squares, i, &squareCount, &angle);
                        squares[i].dy *= -1;
                    }
                    squares[i].rect.x += squares[i].dx;
                    squares[i].rect.y += squares[i].dy;
                }
                frames++;
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(textTexture);
                if (currentTime - startTime >= 1000)
                {
                    frames = 0;
                    startTime = currentTime;
                }
                usleep(1000000 / FPS);
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    free(squares);

    return 0;
}

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid_Wrapped(font, text, color, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void collisionHandler(Square *squares, int i, int *squareCount, float *angle)
{
    float fixedRadius = 100.0f;
    squares[i].rect.w = (squares[i].rect.w * SHRINK_MODIFIER);
    squares[i].rect.h = (squares[i].rect.h * SHRINK_MODIFIER);
    squares[i].rect.x = SCREEN_WIDTH / 2 + cos(*angle * M_PI / 180.0f) * fixedRadius;
    squares[i].rect.y = SCREEN_HEIGHT / 2 + sin(*angle * M_PI / 180.0f) * fixedRadius;
    float randomOffset = (rand() % 11) - 5;
    float spiralAngle = *angle + randomOffset;
    squares[i].speed *= SPEED_MULTIPLIER;
    squares[i].dx = (cos(spiralAngle * M_PI / 180.0f)) * squares[i].speed;
    squares[i].dy = (sin(spiralAngle * M_PI / 180.0f)) * squares[i].speed;
    *angle += ANGLE_INCREMENT;
    randomOffset = (rand() % 11) - 5;
    spiralAngle = *angle + randomOffset;

    squares[*squareCount] = squares[i];

    squares[*squareCount].rect.x = SCREEN_WIDTH / 2 + cos(*angle * M_PI / 180.0f) * fixedRadius;
    squares[*squareCount].rect.y = SCREEN_HEIGHT / 2 + sin(*angle * M_PI / 180.0f) * fixedRadius;
    squares[*squareCount].dx = (cos(spiralAngle * M_PI / 180.0f)) * squares[i].speed;
    squares[*squareCount].dy = (sin(spiralAngle * M_PI / 180.0f)) * squares[i].speed;
    *angle += ANGLE_INCREMENT;

    if (rand() % 2 == 0)
    {
        squares[*squareCount].dx = -squares[*squareCount].dx;
    }
    if (rand() % 2 == 0)
    {
        squares[*squareCount].dy = -squares[*squareCount].dy;
    }
    do
    {
        squares[*squareCount].r = rand() % 256;
        squares[*squareCount].g = rand() % 256;
        squares[*squareCount].b = rand() % 256;
    } while (squares[i].r == 0xFF && squares[i].g == 0xFF && squares[i].b == 0xFF);

    (*squareCount)++;
}