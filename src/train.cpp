/****************************************************************
 * Program to train the perceptron. Enter the set of expected
 * input and output images. They must have the same resolution
 * and the output image must be the result of an edge detection
 * operation. The last parameter.
 * *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_image.h>

#define NET_INPUT_SIZE 10
#define EPOCHS 100

typedef unsigned char byte;

float weight[NET_INPUT_SIZE];
int input[NET_INPUT_SIZE];
float N = 0.01;

byte rgb_to_gray(SDL_Color color)
{
	return (byte)((0.299 * color.r) +
				  (0.587 * color.g) +
				  (0.114 * color.b));
}

byte *load_image(char *filename, int *weight, int *heigth, int level)
{
	SDL_Surface *surface = IMG_Load(filename);
	if (surface)
	{
		// convertint to 32 bits
		SDL_Surface *aux =
			SDL_ConvertSurfaceFormat(
				//surface, SDL_PIXELFORMAT_RGBA8888, 0);
				surface, SDL_PIXELFORMAT_RGB888, 0);
		SDL_FreeSurface(surface);
		surface = aux;

		// prepare output gray image vector
		byte *image = (byte *)malloc(surface->w * surface->h);

		// get pixel color of original image
		SDL_Color *pixels = (SDL_Color *)surface->pixels;
		for (int i = 0; i < (surface->w * surface->h); i++)
		{
			image[i] = rgb_to_gray(pixels[i]) < level ? 255 : 0;
		}
		SDL_FreeSurface(surface);
		*weight = surface->w;
		*heigth = surface->h;
		return image;
	}
	printf("Error loading image %s\n", filename);
	exit(1);
	return nullptr;
}

void init_weight()
{
	srand(time(NULL));
	for (int i = 0; i < NET_INPUT_SIZE; i++)
	{
		weight[i] = (float)(rand() / 100000000);
		weight[i] = (weight[i] * 4) / 100;
		printf("%.2f ", weight[i]);
	}
}

int run_perceptron()
{
	float y = 0.0;
	// Execute the NET, sum of inputs * weights
	for (int i = 0; i < NET_INPUT_SIZE; i++)
	{
		y += input[i] * weight[i];
	}
	// Simple transfer function
	return y > 0.0 ? 1 : 0;
}

void adjust_weights(int goal, int output)
{
	int i;
	for (i = 0; i < 10; i++)
	{
		weight[i] = weight[i] + (N * (goal - output) * input[i]);
	}
}

void save_weights()
{
	char line[80];
	FILE *fp;
	printf("Saving weights.net\n");

	if ((fp = fopen("weights.net", "w+")) == NULL)
	{
		printf("Error to open weights.net.\n");
		exit(1);
	}
	for (int i = 0; i < NET_INPUT_SIZE; i++)
	{
		sprintf(line, "%.4f\n", weight[i]);
		fputs(line, fp);
	}
	fclose(fp);
}

//  MAIN FUNCTION ********************************************************************

int main(int argc, char *argv[])
{
	byte *image_in, *image_out;
	int output, goal, level;
	clock_t time_begin, time_end;
	double time_run;

	if ((argc < 3) || (argc > 4))
	{
		printf("BACKPROPAGATION TRAINING                 \n");
		printf("usage:                                   \n");
		printf("./train in.ext output.ext level          \n");
		printf("  - in.ext: imput image. ex: image.jpg   \n");
		printf("  - out.ext: output image: ex: output.jpg\n");
		printf("  - level: num 0..255 (use same level to \n");
		printf("    create output image used to traininng\n");
	}
	level = 127;
	if (argc == 4)
	{
		sscanf(argv[3], "%d", &level);
	}
	int w, h, out_w, out_h;
	image_in = load_image(argv[1], &w, &h, level);
	image_out = load_image(argv[2], &out_w, &out_h, level);
	if (w != out_w || h != out_h)
	{
		printf("Images not corresponding\n");
		exit(1);
	}
	// Execute training
	time_begin = clock();
	int epochs = 0;
	for (epochs = 0; epochs < EPOCHS; epochs++)
	{
		bool error = false;
		for (int y = 0; y < h - 1; y++)
		{
			for (int x = 0; x < w - 1; x++)
			{
				int i = (y * (w - 1) + x);
				input[0] = -1; // bias
				input[1] = image_in[i - w - 1] > level ? 1 : 0;
				input[2] = image_in[i - w] > level ? 1 : 0;
				input[3] = image_in[i - w + 1] > level ? 1 : 0;
				input[4] = image_in[i - 1] > level ? 1 : 0;
				input[5] = image_in[i] > level ? 1 : 0;
				input[6] = image_in[i + 1] > level ? 1 : 0;
				input[7] = image_in[i + w - 1] > level ? 1 : 0;
				input[8] = image_in[i + w] > level ? 1 : 0;
				input[9] = image_in[i + w + 1] > level ? 1 : 0;

				goal = image_out[i] > level ? 1 : 0;

				output = run_perceptron();

				if (output != goal)
				{
					error = true;
					adjust_weights(goal, output);
				}
			}
		}
		printf(".");
		if (!error)
		{
			break;
		}
	}
	printf("\n");

	time_end = clock();
	time_run = ((double)(time_end - time_begin)) / CLOCKS_PER_SEC;

	printf("Trainning: %0.2lf seconds / %d epochs.", time_run, epochs);

	if (image_in)
	{
		free(image_in);
	}
	if (image_out)
	{
		free(image_out);
	}

	save_weights();

	return 0;
}
