#include "image.h"
#include <string.h>


Image *load_image(char *filename) 
{    
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("Error: cannot open file");
    }

    
    char p3[3];
    fscanf(fp, "%s", p3);
    
    char checkForComment;
    while (1) 
    {
        fscanf(fp, " %c", &checkForComment);
        if (checkForComment == '#') 
        {
            while (fgetc(fp) != '\n');
        } 
        else 
        {
            ungetc(checkForComment, fp);
            break;
        }
    }

    unsigned short height, width;
    fscanf(fp, "%hu %hu", &width, &height);

    int maxIntensity;
    fscanf(fp, "%d", &maxIntensity);

    Image *image = (Image *)malloc(sizeof(Image));
    image->width = width;
    image->height = height;
    image->imageData = malloc(height * width * sizeof(unsigned char));

    for (unsigned int i = 0; i < width * height; i++) 
    {
        int pixelVal;
        fscanf(fp, "%d", &pixelVal);
        image->imageData[i] = (unsigned char)pixelVal;
        fscanf(fp, "%*d %*d"); 
    }

    fclose(fp);  
    return image;
}



void delete_image(Image *image) 
{
    free(image->imageData);
    free(image);
}

unsigned short get_image_width(Image *image) 
{
    return image->width;
}

unsigned short get_image_height(Image *image) 
{
    return image->height;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) 
{
    return image->imageData[row*image->width +col];
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) 
{
    FILE *inputFP = fopen(input_filename, "r");
    FILE *outputFP = fopen(output_filename, "w");
    
    char p3[3];
    fscanf(inputFP, "%s", p3);
    int width =0;
    int height =0;
    fscanf(inputFP, "%d %d", &width, &height);
    int maxIntensity= 0;
    fscanf(inputFP, "%d", &maxIntensity);
    fprintf(outputFP, "%s\n%d %d\n%d", p3, width, height, maxIntensity);

    int numOfPixels = height*width;
    int ch_index = 0;
    int bits_written = 0;

    // Process each pixel intensity
    for ( int i = 0; i < numOfPixels; i++) {
        int intensity;
        fscanf(inputFP, "%d", &intensity);

        int message_length = strlen(message);
        if (ch_index < message_length) {
            // Extract the current bit from the character's ASCII value
            char current_char = message[ch_index];
            int bit_position = bits_written % 8;
            int bit = (current_char >> (7 - bit_position)) & 1;

            // Modify LSB to encode the bit
            intensity = (intensity & ~1) | bit;
            bits_written++;

            // Move to the next character after writing 8 bits
            if (bits_written % 8 == 0) {
                ch_index++;
            }
        }

        fprintf(outputFP, "%d ", intensity);
    }

    fclose(inputFP);
    fclose(outputFP);
    (void)message;
    return 0;
}

char *reveal_message(char *input_filename) {
    (void)input_filename;
    return NULL;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) {
    (void)secret_image_filename;
    (void)input_filename;
    (void)output_filename;
    return 10;
}

void reveal_image(char *input_filename, char *output_filename) {
    (void)input_filename;
    (void)output_filename;
}
