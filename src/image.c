#include "image.h"

Image *load_image(char *filename) 
{    
    FILE *fp = fopen(filename, "r");
    
    char p3[3];
    fscanf(fp, "%s", p3);

    int checkForComment = fgetc(fp);
    if (checkForComment == '#') 
    {
        while (fgetc(fp) != '\n' && !feof(fp));
    } 
    else 
    {
        ungetc(checkForComment, fp);
    }

    unsigned int height, width;
    fscanf(fp, "%u %u", &width, &height);

    Image *image = (Image *)malloc(sizeof(Image));
    image->width = width;
    image->height = height;
    image->imageData = malloc(width * height * sizeof(int));

    for (unsigned int i = 0; i < width * height; i++) 
    {
        int pixel_value;
        fscanf(fp, "%d", &pixel_value);
        image->imageData[i] = pixel_value;
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

unsigned short get_image_width(Image *image) {
    (void)image;
    return 0;
}

unsigned short get_image_height(Image *image) {
    (void)image;
    return 0;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) {
    (void)image;
    (void)row;
    (void)col;
    return 0;
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
