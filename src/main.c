#include <SDL2/SDL.h>
#include "back.h"
#include "vec.h"

int main(void)
{
	SDL_Event event;
	SDL_Window *window = NULL;
	SDL_Surface *screen = NULL;
	SDL_Renderer *render = NULL;
	Uint32 background;

	init(window, screen, render, &background);

	Vec2 *points = malloc(sizeof(Vec2));
	Vec2 mousepos;
	int running = 1, npoints = 0, pointing = 0;

	while (running) {
		SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(render);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
						screen = SDL_GetWindowSurface(window);
						SDL_FillRect(screen, NULL, background);
						SDL_UpdateWindowSurface(window);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					points = realloc(points, ++npoints * sizeof(Vec2));
					pointing = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					pointing = 0;
					points[npoints - 1] = mousepos;
					if (mousepos.x < 0 || mousepos.y < 0) {
						npoints -= 1; // invalid, outside screen, redo
					}
					break;
			}
			// in the end, the mousepos pos of points is where the mouse as released
			if (pointing) {
				SDL_GetMouseState(&mousepos.x, &mousepos.y);
			}
			SDL_Color linecolor = {.r = 255, .b = 255, .g = 255, .a = SDL_ALPHA_OPAQUE};
			drawSegLine(render, linecolor, points, npoints);
		}
		SDL_Delay(16);
	}
	printf("total points:\n");
	for (int i = 0; i < npoints; ++i) {
		printf("(%4d, %4d)\n", points[i].x, points[i].y);
	}
	free(points);

	deinit();

	return 0;
}
