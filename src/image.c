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

// unsigned int hide_message(char *message, char *input_filename, char *output_filename) 
// {
//     FILE *inputFP = fopen(input_filename, "r");
//     FILE *outputFP = fopen(output_filename, "w");
    
//     char p3[3];
//     fscanf(inputFP, "%s", p3);
//     int width =0;
//     int height =0;
//     fscanf(inputFP, "%d %d", &width, &height);
//     int maxIntensity= 0;
//     fscanf(inputFP, "%d", &maxIntensity);
//     fprintf(outputFP, "%s\n%d %d\n%d\n", p3, width, height, maxIntensity);

//     unsigned int numOfPixels = height*width;
//     int msgLength = strlen(message);
//     //printf("%d %d", numOfPixels ,msgLength);
//     unsigned int pixelsEncoded = 0;
//     int charIndex = 0;
//     int bitIndex = 0;
//     //int ended = 0;
//     int nuller =0;

//     printf("%s", message);
//     for(unsigned int i = 0; i < numOfPixels; i++)
//     {
//         int currIntensity;
//         fscanf(inputFP, "%d", &currIntensity);
//         int waste1 =0, waste2 =0;
//         fscanf(inputFP, " %d %d", &waste1, &waste2);
//         int bitToHide =0;

//         if((charIndex < msgLength) && (pixelsEncoded < (numOfPixels/8)) && (i < numOfPixels-8))
//         {
//             unsigned char currChar = message[charIndex];
//             bitToHide = (currChar >> (7 - bitIndex)) & 1;
//             printf("curr char is %c\n",currChar);
//         }
//         else if(/*(ended == 0) &&*/ (pixelsEncoded < (numOfPixels/8))&& nuller < 8) //maybe not 8
//         {
//             bitToHide = 0;
//             //printf("condition reached");
//             nuller++;
//         }
//         else
//         {
//             fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);
//             if ((i + 1) % width == 0) 
//             {
//                 fprintf(outputFP, "\n");
//             }
//             continue;
//         }

//         currIntensity = (currIntensity & ~0x1) | bitToHide;
//         //printf("currIntensity: %x \n",currIntensity);
//         bitIndex++;
//         if(bitIndex == 8)
//         {
//             bitIndex = 0;
//             charIndex++;
//             if (charIndex <= msgLength) 
//             {
//                 pixelsEncoded++;
//                 printf("pixels encoded: %d",pixelsEncoded);
//             }
//             if (charIndex >= msgLength) 
//             {
//                 //ended = 1;  
//             }
//         }

//         fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);

//         if ((i + 1) % width == 0) 
//         {
//             fprintf(outputFP, "\n");
//         }
//     }
//     fclose(inputFP);
//     fclose(outputFP);
//     //pixelsEncoded++;
//     //if((unsigned)msgLength > (numOfPixels/8))
//     //{
//     //    pixelsEncoded-=2;
//     //}
//     printf("\n%d\n",pixelsEncoded);
//     return pixelsEncoded;
// }

unsigned int hide_message(char *message, char *input_filename, char *output_filename) 
{
    FILE *inputFP = fopen(input_filename, "r");
    FILE *outputFP = fopen(output_filename, "w");
    
    char p3[3];
    fscanf(inputFP, "%s", p3);
    int width = 0;
    int height = 0;
    fscanf(inputFP, "%d %d", &width, &height);
    int maxIntensity = 0;
    fscanf(inputFP, "%d", &maxIntensity);
    fprintf(outputFP, "%s\n%d %d\n%d\n", p3, width, height, maxIntensity);

    unsigned int numOfPixels = width * height;
    unsigned int max_chars = numOfPixels / 8;   // Maximum characters that can be encoded (including null)
    unsigned int pixelsEncoded = 0;

    int msgLength = strlen(message);
    int charIndex = 0;
    int bitIndex = 0;

    // Encode each character in the message, up to max_chars - 1 if there's space for a null terminator
    for (unsigned int i = 0; i < numOfPixels; i++) 
    {
        int currIntensity;
        fscanf(inputFP, "%d", &currIntensity);
        int waste1 = 0, waste2 = 0;
        fscanf(inputFP, " %d %d", &waste1, &waste2);

        int bitToHide = 0;

        if (charIndex < msgLength && pixelsEncoded < max_chars - 1) 
        {
            // Get current character and retrieve the next bit to encode
            unsigned char currChar = message[charIndex];
            bitToHide = (currChar >> (7 - bitIndex)) & 1;
        } 
        else if (charIndex == msgLength && pixelsEncoded < max_chars) 
        {
            // Encoding null character after last character of message
            bitToHide = 0;
        } 
        else 
        {
            // If finished encoding, write the unmodified pixel
            fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);
            if ((i + 1) % width == 0) 
                fprintf(outputFP, "\n");
            continue;
        }

        // Set the LSB of currIntensity with the current bit
        currIntensity = (currIntensity & ~0x1) | bitToHide;
        fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);

        bitIndex++;
        if (bitIndex == 8) 
        {
            bitIndex = 0;
            charIndex++;
            pixelsEncoded++;
        }

        if ((i + 1) % width == 0) 
            fprintf(outputFP, "\n");
    }

    fclose(inputFP);
    fclose(outputFP);
    return pixelsEncoded;  // Number of characters encoded, excluding the null terminator
}

char *reveal_message(char *input_filename) 
{
    (void)input_filename;
    return NULL;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) 
{
    (void)secret_image_filename;
    (void)input_filename;
    (void)output_filename;
    return 10;
}

void reveal_image(char *input_filename, char *output_filename) 
{
    (void)input_filename;
    (void)output_filename;
}
