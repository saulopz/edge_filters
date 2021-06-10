#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include "net.h"

SDL_PixelFormat *fmt;
int level = 127;

typedef unsigned char byte;

#define NET_INPUT_SIZE 10
float weight[NET_INPUT_SIZE];
int net_input[NET_INPUT_SIZE];
float N;

// PROTOTYPES ***************************************************************

byte rgb_to_gray(SDL_Color color);
SDL_Surface *load_image(char *filename, byte *&image, int *size, int *height, int level);
void save_image(char *filename, SDL_Surface *surface, byte *image, int width, int height);
void load_weights();
int run_perceptron(const int *entrada);

// ALGORITHMS
void edge_if(byte *image_in, byte *&image_out, int width, int height);
void edge_double_if(byte *image_in, byte *&image_out, int width, int height);
void edge_roberts(byte *image_in, byte *&image_out, int width, int height, bool variant);
void edge_sobel(byte *image_in, byte *&image_out, int width, int height, bool variant);
void edge_prewitt(byte *image_in, byte *&image_out, int width, int height);
void edge_perceptron(byte *image_in, byte *&image_out, int width, int height);
void edge_backpropagation(byte *image_in, byte *&image_out, int width, int height);

// MAIN FUNCTION ************************************************************

int main(int argc, char *argv[])
{
    SDL_Surface *surface;
    int width, height;
    clock_t time_begin, time_end;
    double run_time;
    byte *image_in = nullptr;
    byte *image_out = nullptr;

    if ((argc < 4) || (argc > 5))
    {
        printf("Parametros Incorretos\n");
        printf("imagem_entrada imagem_saida <nivel>\n");
        return 0;
    }
    if (argc == 5)
    {
        sscanf(argv[4], "%d", &level);
    }

    // load input image vector
    surface = load_image(argv[2], image_in, &width, &height, level);
    if (!surface || !image_in)
    {
        printf("Could not create image surface...\n");
        exit(1);
    }

    // prepare output image vector
    image_out = (byte *)malloc(width * height);
    if (!image_out)
    {
        printf("Could not allocate memory to image output.\n");
        exit(1);
    }

    // load network weights
    load_weights();

    time_begin = clock();

    bool valid = true;
    char *algorithm = argv[1];
    if (strcmp(algorithm, "if") == 0)
    {
        edge_if(image_in, image_out, width, height);
    }
    else if (strcmp(algorithm, "doubleif") == 0)
    {
        edge_double_if(image_in, image_out, width, height);
    }
    else if (strcmp(algorithm, "roberts") == 0)
    {
        edge_roberts(image_in, image_out, width, height, false);
    }
    else if (strcmp(algorithm, "roberts2") == 0)
    {
        edge_roberts(image_in, image_out, width, height, true);
    }
    else if (strcmp(algorithm, "sobel") == 0)
    {
        edge_sobel(image_in, image_out, width, height, false);
    }
    else if (strcmp(algorithm, "sobel2") == 0)
    {
        edge_sobel(image_in, image_out, width, height, true);
    }
    else if (strcmp(algorithm, "prewitt") == 0)
    {
        edge_prewitt(image_in, image_out, width, height);
    }
    else if (strcmp(algorithm, "perceptron") == 0)
    {
        edge_perceptron(image_in, image_out, width, height);
    }
    else if (strcmp(algorithm, "net") == 0)
    {
        edge_backpropagation(image_in, image_out, width, height);
    }
    else
    {
        printf("Algorithm %s invalid...\n", algorithm);
        valid = false;
    }
    if (valid)
    {
        time_end = clock();
        run_time = ((double)(time_end - time_begin)) / CLOCKS_PER_SEC;
        printf("Algorithm %s at level %d\n", algorithm, level);
        printf("Runtime: %lf seconds.\n", run_time);

        // save output image
        save_image(argv[3], surface, image_out, width, height);
    }
    if (image_in)
    {
        free(image_in);
    }
    if (image_out)
    {
        free(image_out);
    }
    return 0;
}
//END_OF_MAIN();

// IMPLEMENTATION **************************************************************

byte rgb_to_gray(SDL_Color color)
{
    return (byte)((0.299 * color.r) +
                  (0.587 * color.g) +
                  (0.114 * color.b));
}

SDL_Surface *load_image(char *filename, byte *&image, int *width, int *height, int level)
{
    SDL_Surface *surface = IMG_Load(filename);
    if (surface)
    {
        *width = surface->w;
        *height = surface->h;

        // save original format
        fmt = surface->format;

        // convertint to 32 bits
        SDL_Surface *aux =
            SDL_ConvertSurfaceFormat(
                //surface, SDL_PIXELFORMAT_RGBA8888, 0);
                surface, SDL_PIXELFORMAT_RGB888, 0);
        SDL_FreeSurface(surface);
        surface = aux;

        // prepare output gray image vector
        image = (byte *)malloc(*width * *height);

        // get pixel color of original image
        SDL_Color *pixels = (SDL_Color *)surface->pixels;
        for (int i = 0; i < (*width * *height); i++)
        {
            image[i] = rgb_to_gray(pixels[i]) < level ? 255 : 0;
        }
        return surface;
    }
    printf("Error to load image %s\n", filename);
    return nullptr;
}

void save_image(char *filename, SDL_Surface *surface, byte *image, int width, int height)
{
    if (surface)
    {
        SDL_Color *pixels = (SDL_Color *)surface->pixels;
        for (int i = 0; i < surface->w * surface->h; i++)
        {
            pixels[i].r = image[i];
            pixels[i].g = image[i];
            pixels[i].b = image[i];
            pixels[i].a = 255;
        }
        // Convert original format
        //SDL_Surface *aux = SDL_ConvertSurfaceFormat(surface, fmt, 0);
        if (SDL_SaveBMP(surface, filename) != 0)
        {
            printf("Save image failed: %s\n", SDL_GetError());
        }
        return;
    }
    printf("error in surface...\n");
}

//  EDGE DETECTION BY IF *************************************************

void edge_if(byte *image_in, byte *&image_out, int width, int height)
{
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int i = (y * (width - 1)) + x;
            image_out[i] = (image_in[i] != image_in[i + width + 1]) ? 0 : 255;
        }
    }
}

//  EDGE DETECTION BY DOUBLE IF **********************************************

void edge_double_if(byte *image_in, byte *&image_out, int width, int height)
{
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int i = (y * (width - 1)) + x;
            image_out[i] = (image_in[i] != image_in[i + width + 1] ||
                            image_in[i + 1] != image_in[i + width])
                               ? 0
                               : 255;
        }
    }
}

//  EDGE DETECTION BY ROBERTS *****************************************************

void edge_roberts(byte *image_in, byte *&image_out, int width, int height, bool variant)
{
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int i = (y * (width - 1) + x);
            int gx = image_in[i] - image_in[i + width + 1];
            int gy = image_in[i + 1] - image_in[i + width];

            int g = variant ? abs(abs(gx) + abs(gy)) : (int)sqrt((gx * gx) + (gy * gy));

            image_out[i] = g == 0 ? 255 : 0;
        }
    }
}

//  EDGE DETECTION BY SOBEL ********************************************************

void edge_sobel(byte *image_in, byte *&image_out, int width, int height, bool variant)
{
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int i = (y * (width - 1) + x);
            //      |-1  0  1|
            // GX = |-2  0  2|  * A
            //      |-1  0  1|
            int gx = -(image_in[i - width - 1]) +
                     -(2 * image_in[i - 1]) +
                     -(image_in[i + width - 1]) +
                     +(image_in[i - width + 1]) +
                     +(2 * image_in[i + 1]) +
                     +(image_in[i + width + 1]);

            //      |-1 -2 -1|
            // GY = | 0  0  0|  * A
            //      | 1  2  1|
            int gy = -(image_in[i - width - 1]) +
                     -(2 * image_in[i - width]) +
                     -(image_in[i - width + 1]) +
                     +(image_in[i + width - 1]) +
                     +(2 * image_in[i + width]) +
                     +(image_in[i + width + 1]);

            int g = variant ? g = abs(gx) + abs(gy) : (int)sqrt(((gx * gx) + (gy * gy)));
            image_out[i] = g == 0 ? 255 : 0;
        }
    }
}

// EDGE DETECTION BY PREWITT **************************************************

void edge_prewitt(byte *image_in, byte *&image_out, int width, int height)
{
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int i = (y * (width - 1) + x);
            //      |+1  0 -1|
            // GX = |+1  0 -1|  * A
            //      |+1  0 -1|
            int gx = +(image_in[i - width - 1]) +
                     +(image_in[i - 1]) +
                     +(image_in[i + width - 1]) +
                     -(image_in[i - width + 1]) +
                     -(image_in[i + 1]) +
                     -(image_in[i + width + 1]);

            //      |+1 +1 +1|
            // GY = | 0  0  0|  * A
            //      |-1 -1 -1|
            int gy = +(image_in[i - width - 1]) +
                     +(image_in[i - width]) +
                     +(image_in[i - width + 1]) +
                     -(image_in[i + width - 1]) +
                     -(image_in[i + width]) +
                     -(image_in[i + width + 1]);

            int g = (int)sqrt(((gx * gx) + (gy * gy)));
            image_out[i] = g == 0 ? 255 : 0;
        }
    }
}

// EDGE DETECTION BY PERCEPTRON ************************************************

// Open the file with the weights of the trained inputs.
void load_weights()
{
    FILE *fp;
    if ((fp = fopen("weights.net", "r")) == NULL)
    {
        printf("You need training perceptron first to create weights.net file.\n");
        exit(0);
    }
    char line[80];
    for (int i = 0; i < NET_INPUT_SIZE; i++)
    {
        fgets(line, 10, fp);
        sscanf(line, "%f", &weight[i]);
    }
    printf("\n");
    fclose(fp);
}

int run_perceptron()
{
    float y = 0;
    // run net. Sum of inputs * weights
    for (int i = 0; i < NET_INPUT_SIZE; i++)
    {
        y += net_input[i] * weight[i];
    }
    // Simple transfer function
    return y > 0 ? 0 : 255;
}

void edge_perceptron(byte *image_in, byte *&image_out, int width, int height)
{
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int i = (y * (width - 1) + x);
            net_input[0] = -1; // bias
            net_input[1] = image_in[i - width - 1] > level ? 1 : 0;
            net_input[2] = image_in[i - width] > level ? 1 : 0;
            net_input[3] = image_in[i - width + 1] > level ? 1 : 0;
            net_input[4] = image_in[i - 1] > level ? 1 : 0;
            net_input[5] = image_in[i] > level ? 1 : 0;
            net_input[6] = image_in[i + 1] > level ? 1 : 0;
            net_input[7] = image_in[i + width - 1] > level ? 1 : 0;
            net_input[8] = image_in[i + width] > level ? 1 : 0;
            net_input[9] = image_in[i + width + 1] > level ? 1 : 0;
            image_out[i] = run_perceptron();
        }
    }
}

// EDGE DETECTION BY ARTIFICIAL NEURAL NETWORK USING BACKPROPAGATION **************

void edge_backpropagation(byte *image_in, byte *&image_out, int width, int height)
{
    double out;
    double *input = (double *)malloc(9 * (sizeof(double)));
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int i = (y * (width - 1) + x);
            input[0] = image_in[i - width - 1];
            input[1] = image_in[i - width];
            input[2] = image_in[i - width + 1];
            input[3] = image_in[i - 1];
            input[4] = image_in[i];
            input[5] = image_in[i + 1];
            input[6] = image_in[i + width - 1];
            input[7] = image_in[i + width];
            input[8] = image_in[i + width + 1];

            if (!eval_net(input, &out))
            {
                printf("Neural Network error.\n");
                exit(2);
            }
            image_out[i] = (byte)(out > 0.5 ? 255 : 0);
        }
    }
    free(input);
}