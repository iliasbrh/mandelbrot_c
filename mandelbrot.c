#include <stdio.h>
#include "types.h"
#include <complex.h>
#include <string.h>


typedef double complex c;
typedef struct {
		u32 width, height;
		c* data;
} map;
typedef struct {
		u32 width, height;
		u32* data;
} conv_map;

void print_complex(c z);
void print_map(map* cmap);

map* create_map(u32 width, u32 height);
conv_map* create_conv(u32 width, u32 height);

void iterate(map* initial_map, map* map, conv_map* conv, u32 iterations);
void draw_plane(conv_map* conv, u32 iterations);


int main() {
		u32 w = 360;
		u32 h = 240;

		u32 iterations = 200;

		map* cmap = create_map(w, h);
		map* initial_map = create_map(w, h);
		conv_map* conv = create_conv(w, h);

		iterate(initial_map, cmap, conv, iterations);

		draw_plane(conv, iterations);

		return 0;
}


void print_complex(c z) {
		printf("%f + i*%f || ", creal(z), cimag(z));
}

void print_map(map* cmap) {
		for (u32 y=0; y<cmap->height; y++) {
				for (u32 x=0; x<cmap->width; x++)
						print_complex(cmap->data[y*(cmap->width)+x]);
				printf("\n");
		}
}

map* create_map(u32 width, u32 height) {
		map* cmap = (map*)malloc(sizeof(map));

		cmap->width = width;
		cmap->height = height;
		cmap->data = (c*)malloc(width*height*sizeof(c));

		// x ranges from -2 to 1
		// y ranges from -1 to 1
		for (u32 ix=0; ix<width; ix++)
				for (u32 iy=0; iy<height; iy++) {
						f64 x = -2. + ix*3./(f64)width;
						f64 y = 1. - iy*2./(f64)height;
						cmap->data[iy*width+ix] = x + I*y;
				}
		
		return cmap;
}

conv_map* create_conv(u32 width, u32 height) {
		conv_map* conv = (conv_map*)malloc(sizeof(conv_map));

		conv->width = width;
		conv->height = height;
		conv->data = (u32*)malloc(width*height*sizeof(u32));

		memset(conv->data, 0, width*height*sizeof(u32));

		return conv;
}

void iterate(map* initial_map, map* cmap, conv_map* conv, u32 iterations) {
		for (u32 i=0; i<iterations; i++)
				for (u32 x=0; x<cmap->width; x++)
						for (u32 y=0; y<cmap->height; y++)
								if (conv->data[y*cmap->width+x] == 0) {
										cmap->data[y*cmap->width+x] = cpow(cmap->data[y*cmap->width+x], (c)2.) + initial_map->data[y*cmap->width+x];
										if (cabs(cmap->data[y*cmap->width+x]) > 2.)
												conv->data[y*cmap->width+x] = i;
								}
}

void draw_plane(conv_map* conv, u32 iterations) {
		for (u32 y=0; y<conv->height; y++) {
				for (u32 x=0; x<conv->width; x++)
						printf("\x1b[48;5;%dm  ", 232+conv->data[y*conv->width+x]*256/iterations);
						//printf("\x1b[48;2;%d;%d;%dm  ", conv->data[y*conv->width+x]*256/iterations, g, b);
				printf("\n");
		}
}
