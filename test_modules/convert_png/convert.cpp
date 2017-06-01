#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>


Uint32 get_pixel32( SDL_Surface *surface, int x, int y ) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel ) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
			printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
			success = false;
		}
	}

	return success;
}

bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Load PNG surface
	gPNGSurface = loadSurface( "../../dist/media/victor2_terrain.png" );
	if( gPNGSurface == NULL ) {
		printf( "Failed to load PNG image!\n" );
		success = false;
	}

	return success;
}

void close() {
	//Free loaded image
	SDL_FreeSurface( gPNGSurface );
	gPNGSurface = NULL;


	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface( std::string path ) {
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );


	return loadedSurface;
}

int main( int argc, char* args[] ) {
	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	} else {
		//Load media
		if( !loadMedia() ) {
			printf( "Failed to load media!\n" );
		} else {	
			for (int i = 0; i < 2048; i++) {
				for (int j = 0; j < 2048; j++) {
					printf("%d\t", get_pixel32(gPNGSurface, i, j) & 0xff);
				}
				printf("\n");
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}