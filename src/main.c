#include "include/library.h"

int             width = 1280;
int             height = 720;

SDL_Window     *mainwindow;
SDL_Renderer   *renderer;
SDL_Event       event;

SDL_KeyboardEvent *keyboardevent;
SDL_MouseMotionEvent *motionevent;
SDL_MouseButtonEvent *buttonevent;

Mix_Chunk      *bgm;
int             channel;

SDL_Texture    *menu1;
SDL_Texture    *menu2;

SDL_Texture    *startimgtexture;
SDL_Texture    *blocktexture;
SDL_Texture    *darkblocktexture;

TTF_Font       *menuttf;

SDL_Rect        blockrect = { 32, 32, 28, 28 };
SDL_Rect        currentblock = { 0, 0, 0, 0 };

__flags         mainflags = { 0, 0 };

int             currentshape = 0;
int             speed = 0;
int             speedcount = 1;

Uint8 map[23][13] =
    {{1,1,1,1,1,1,1,1,1,1,1,1,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,0,0,0,0,0,0,0,0,0,0,0,1},
     {1,1,1,1,1,1,1,1,1,1,1,1,1}};
int
main(int __attribute__ ((unused)) argc, char **
     __attribute__ ((unused)) argv) {
  mainloop:
    while (mainflags.running) {

	while (SDL_PollEvent(&event)) {
	    __printlog("\n");
	    switch (event.type) {
	    case SDL_KEYDOWN:
		__handlekey(&event, &mainflags, &(event.key));
		break;
	    case SDL_MOUSEBUTTONDOWN:
		__printmousebutton(&event, &(event.button));
		break;
	    case SDL_MOUSEMOTION:
		__printmousemotion(&event, &(event.motion));
		break;
	    case SDL_QUIT:
		__printquit(&event);
		__handlequit(&event, &mainflags);
		break;
	    default:
		break;
	    }
	}

	if (Mix_Playing(channel) != 0) {}

	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, startimgtexture, NULL, NULL);

	__drawmap(renderer, blocktexture, darkblocktexture, &blockrect, &map);


	
	SDL_RenderPresent(renderer);
	SDL_Delay(10);
    }

    if (!mainflags.init) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG);

	__initwindow(&mainwindow, "debug");
	__initrenderer(&renderer);
	__inithandler(&mainflags);
	__initprinter();

	keyboardevent = &event.key;
	buttonevent = &event.button;
	motionevent = &event.motion;

	__loadttf(&menuttf, "font/LM-Regular.ttf", 20);


	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
	bgm = Mix_LoadWAV("sound/bgm.wav");

	channel = Mix_PlayChannel(-1, bgm, 0);

	startimgtexture = IMG_LoadTexture(renderer, "img/white2.png");
	blocktexture = IMG_LoadTexture(renderer, "img/block.png");
	darkblocktexture = IMG_LoadTexture(renderer, "img/darkblock.png");

	mainflags.init = 1;
	goto mainloop;
    }
    __destroyttf(&menuttf);
    __destroywindow(&mainwindow);
    __destroyrenderer(&renderer);
    __destroyprinter();
    Mix_FreeChunk(bgm);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void
__initwindow(SDL_Window ** windowptr, const char *restrict text) {
    *windowptr =
	SDL_CreateWindow(text, SDL_WINDOWPOS_UNDEFINED,
			 SDL_WINDOWPOS_UNDEFINED, width, height,
			 SDL_WINDOW_OPENGL);
    __errorcheck(*windowptr, "initwindow");
}

void
__initrenderer(SDL_Renderer ** rendererptr) {
    *rendererptr = NULL;
    *rendererptr = SDL_CreateRenderer(mainwindow, -1,
				      SDL_RENDERER_ACCELERATED);
    __errorcheck(*rendererptr, "renderer");
}

