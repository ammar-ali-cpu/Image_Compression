#include "image.h"


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

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    (void)message;
    (void)input_filename;
    (void)output_filename;
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
