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
    fprintf(outputFP, "%s\n%d %d\n%d\n", p3, width, height, maxIntensity);

    unsigned int numOfPixels = height*width;
    int msgLength = strlen(message);
    printf("num pixels is %d and msg length is %d ", numOfPixels ,msgLength);
    unsigned int pixelsEncoded = 0;
    int charIndex = 0;
    int bitIndex = 0;
    //int ended = 0;
    int nuller =0;

    printf("%s", message);
    for(unsigned int i = 0; i < numOfPixels; i++)
    {
        int currIntensity;
        fscanf(inputFP, "%d", &currIntensity);
        int waste1 =0, waste2 =0;
        fscanf(inputFP, " %d %d", &waste1, &waste2);
        int bitToHide =0;

        if((charIndex < msgLength) && (pixelsEncoded < (numOfPixels/8)) && (i < numOfPixels-8))
        {
            unsigned char currChar = message[charIndex];
            bitToHide = (currChar >> (7 - bitIndex)) & 1;
            //printf("curr char is %c\n",currChar);
        }
        else if(/*(ended == 0) &&*/ (pixelsEncoded < (numOfPixels/8))&& nuller < 8) //maybe not 8
        {
            bitToHide = 0;
            //printf("condition reached");
            nuller++;
        }
        else
        {
            fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);
            if ((i + 1) % width == 0) 
            {
                fprintf(outputFP, "\n");
            }
            continue;
        }

        currIntensity = (currIntensity & ~0x1) | bitToHide;
        //printf("currIntensity: %x \n",currIntensity);
        bitIndex++;
        if(bitIndex == 8 && nuller<8)
        {
            bitIndex = 0;
            charIndex++;
            if (charIndex <= msgLength) 
            {
                pixelsEncoded++;
                //printf("pixels encoded: %d",pixelsEncoded);
            }
            if (charIndex >= msgLength) 
            {
                //ended = 1;  
            }
        }

        fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);

        if ((i + 1) % width == 0) 
        {
            fprintf(outputFP, "\n");
        }
    }
    fclose(inputFP);
    fclose(outputFP);
    printf("\n%d\n",pixelsEncoded);
    return pixelsEncoded;
}

char *reveal_message(char *input_filename) 
{
    FILE *inputFP = fopen(input_filename, "r");

    char p3[3];
    int height; 
    int width;
    int maxIntensity;
    fscanf(inputFP, "%s %d %d %d", p3, &width, &height, &maxIntensity);

    int numOfPixels = height * width;
    char *message = malloc((numOfPixels / 8) + 1); 
    int bitIndex = 0;
    int currChar = 0;
    message[currChar] = 0;

    for (int i = 0; i < numOfPixels; i++) 
    {
        int currPixel;
        fscanf(inputFP, "%d", &currPixel);
        int waste1 = 0;
        int waste2 = 0;
        fscanf(inputFP, " %d %d", &waste1, &waste2); 

        int bit = currPixel & 0x1;
        message[currChar] = (message[currChar] << 1) | bit;
        
        bitIndex++;
        if (bitIndex == 8) 
        {
            if (message[currChar] == '\0') 
            {
                break; 
            }
            currChar++;
            message[currChar] = 0;
            bitIndex = 0;
        }
    }

    fclose(inputFP);
    return message;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename)
{
    FILE *inputFP = fopen(input_filename, "r");
    // if(!inputFP)
    // {
    //     printf("Error opening input file %s\n", secret_image_filename);
    //     return 0;
    // }
    FILE *secretFP = fopen(secret_image_filename, "r");
    // if(!secretFP)
    // {
    //     printf("Error opening secret file %s\n", secret_image_filename);
    //     //fclose(inputFP);
    //     return 0;
    // }
    FILE *outputFP = fopen(output_filename, "w");

    char sp3[3];
    int swidth;
    int sheight;
    int smaxIntensity;
    fscanf(secretFP, "%s", sp3);
    fscanf(secretFP, "%d %d", &swidth, &sheight);
    fscanf(secretFP, "%d", &smaxIntensity);
    int snumOfPixels = swidth *sheight;
    printf("sp3: %s swidth: %d sheight: %d smaxIntensity: %d snumOfPixel: %d\n", sp3, swidth, sheight, smaxIntensity, snumOfPixels);

    char p3[3];
    int width=0;
    int height=0;
    int maxIntensity;
    fscanf(inputFP, "%s", p3);
    char checkForComment;
    while (1) 
    {
        fscanf(inputFP, " %c", &checkForComment);
        if (checkForComment == '#') 
        {
            while (fgetc(inputFP) != '\n');
        } 
        else 
        {
            ungetc(checkForComment, inputFP);
            break;
        }
    }
    fscanf(inputFP, "%d %d", &width, &height);
    fscanf(inputFP, "%d", &maxIntensity);
    int numOfPixels = width * height;
    printf("p3: %s width: %d height: %d maxIntensity: %d numOfPixels: %d\n", p3, width, height, maxIntensity, numOfPixels);

    if(numOfPixels < (snumOfPixels*8)+16)
    {
        fclose(inputFP);
        fclose(outputFP);
        fclose(secretFP);
        return 0;
    }
    fprintf(outputFP, "%s\n%d %d\n%d\n", p3, width, height, maxIntensity);

    int currIntensity;
    for(int w = 0; w < 8; w++)
    {
        int waste1=0;
        int waste2=0;
        fscanf(inputFP, "%d %d %d ", &currIntensity, &waste1, &waste2);
        int wbit = (swidth >> (7 - w)) & 0x1;
        currIntensity = (currIntensity & ~0x1) | wbit;
        fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);
    }

    for(int h = 0; h < 8; h++)
    {
        int waste1=0;
        int waste2=0;
        fscanf(inputFP, "%d %d %d ", &currIntensity, &waste1, &waste2);
        int hbit = (sheight >> (7 - h)) & 0x1;
        currIntensity = (currIntensity & ~0x1) | hbit;
        fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);
    }

    int toHide;
    for(int i = 0; i < snumOfPixels; i++)
    {
        int waste1=0;
        int waste2=0;
        fscanf(secretFP, "%d %d %d ", &toHide, &waste1, &waste2);
        for(int j = 0; j < 8; j++)
        {
            int waster1=0;
            int waster2=0;
            fscanf(inputFP, "%d %d %d ",&currIntensity, &waster1, &waster2);
            int bit = (toHide >> (7-j)) & 0x1;
            currIntensity = (currIntensity & ~0x1) | bit;
            fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);
        }
    }

    int waste11=0;
    int waste22=0;
    while (fscanf(inputFP, "%d %d %d ", &currIntensity, &waste11, &waste22) != EOF) 
    {
        fprintf(outputFP, "%d %d %d ", currIntensity, currIntensity, currIntensity);
    }

    fclose(inputFP);
    fclose(outputFP);
    fclose(secretFP);
    printf("done\n");
    return 1;
}

void reveal_image(char *input_filename, char *output_filename) 
{
    FILE *inputFP = fopen(input_filename, "r");
    FILE *outputFP = fopen(output_filename, "w");

    char p3[3];
    int width;
    int height;
    int maxIntensity;
    fscanf(inputFP, "%s", p3);
    fscanf(inputFP, "%d %d", &width, &height);
    fscanf(inputFP, "%d", &maxIntensity);
    int numOfPixels = width *height;
    printf("p3: %s width: %d height: %d maxIntensity: %d numOfPixels: %d\n", p3, width, height, maxIntensity, numOfPixels);

    int swidth=0; 
    int currNum =0;
    for(int i = 0; i < 8; i++)
    {
        int waste1;
        int waste2;
        fscanf(inputFP, "%d %d %d ", &currNum, &waste1, &waste2);
        swidth |= ((currNum & 1) << (7 - i));
    }
    
    int sheight =0;
    for(int i = 0; i < 8; i++)
    {
        int waste1;
        int waste2;
        fscanf(inputFP, "%d %d %d ", &currNum, &waste1, &waste2);
        sheight |= ((currNum & 1) << (7 - i));
    }
    fprintf(outputFP, "%s\n%d %d\n%d\n", p3, swidth, sheight, maxIntensity);

    int snumOfPixels = swidth * sheight;
    printf("swidth: %d shieght: %d snum: %d/n", swidth, sheight, snumOfPixels);

    for (int i = 0; i < (snumOfPixels); i++) 
    {
        int pixel = 0;
        for (int j = 0; j < 8; j++) 
        {
            int waster1;
            int waster2;
            fscanf(inputFP, "%d %d %d ", &currNum, &waster1, &waster2);
            pixel |= ((currNum & 1) << (7 - j));
        }
        printf("Decoded pixel %d intensity: %d\n", i, pixel);

        fprintf(outputFP, "%d %d %d ", pixel, pixel, pixel);
    }

    fclose(inputFP);
    fclose(outputFP);
    FILE *outputRead = fopen(output_filename, "r");
    char a[3];
    int b=0;
    int c=0;
    int d=0;
    fscanf(outputRead, "%s", a);
    fscanf(outputRead, "%d %d\n%d", &b, &c, &d);
    printf("%s\n%d %d\n%d ", a, b, c, d);

    fclose(outputRead);
}
