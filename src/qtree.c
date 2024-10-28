#include "qtree.h"
#include <math.h>

static double avg_intensity_and_rmse(Image *image, unsigned int height, unsigned int width, unsigned int row, unsigned int col, double *rmse)
{
    double sum = 0.0;
    //double sum_square = 0.0;
    for(unsigned int i = row; i < row + height; i++)
    {
        for(unsigned int j = col; j < col + width; j++)
        {
            sum += get_image_intensity(image, i, j);
            //sum_square += (get_image_intensity(image, i, j)) * (get_image_intensity(image, i, j));
        }
    }
    double numOfPixels = height * width;
    double avg = sum / numOfPixels;

    double squaredDifference = 0;
    for(unsigned int i = row; i < row + height; i++)
    {
        for(unsigned int j = col; j < col + width; j++)
        {
            double difference = avg - get_image_intensity(image, i, j);
            squaredDifference += (difference*difference);
        }
    }
    double avgOfSquaredDifference = squaredDifference/numOfPixels;
    *rmse = sqrt(avgOfSquaredDifference);

    //*rmse = sqrt((sum_square/numOfPixels) - (avg*avg));
    //printf("%f ", avg);

    //unsigned char avgChar = (unsigned char)avg;

    //printf("%d ", avgChar);


    return avg;
}

QTNode *helper(Image *image, double max_rmse, unsigned int height, unsigned int width, unsigned int row, unsigned int col)
{
    double rmse = 0;
    QTNode *root = (QTNode*)malloc(sizeof(QTNode));

    root->intensity = (unsigned char)avg_intensity_and_rmse(image, height, width, row, col, &rmse);
    root->height = height;
    root->width = width;
    root->row = row;
    root->col = col;
    root->child1 = NULL;
    root->child2 = NULL;
    root->child3 = NULL;
    root->child4 = NULL;
    root->nodeOrLeaf = 'L';

    if((rmse > max_rmse) && height>1 && width >1)
    {
        root->nodeOrLeaf = 'N';
        root->child1 = helper(image, max_rmse, height/2, width/2, row, col);
        root->child2 = helper(image, max_rmse, height/2, (width/2), row, col + (width/2));//width - 
        root->child3 = helper(image, max_rmse, (height/2), width/2, row + (height/2), col);//height-
        root->child4 = helper(image, max_rmse, (height/2), (width/2), row + (height/2), col + (width/2));///height- width - 
    }
    return root;
}


QTNode *create_quadtree(Image *image, double max_rmse) 
{
    QTNode *root = helper(image, max_rmse, get_image_height(image), get_image_width(image), 0, 0);
    return root;
}

QTNode *get_child1(QTNode *node) 
{
    return node->child1;
}

QTNode *get_child2(QTNode *node) 
{
    return node->child2;
}

QTNode *get_child3(QTNode *node) 
{
    return node->child3;
}

QTNode *get_child4(QTNode *node) 
{
    return node->child4;
}

unsigned char get_node_intensity(QTNode *node) 
{
    return node->intensity;
}

void delete_quadtree(QTNode *root) 
{
    if (root == NULL)
    {
        return;
    }
    delete_quadtree(root->child1);
    delete_quadtree(root->child2);
    delete_quadtree(root->child3);
    delete_quadtree(root->child4);
    free(root);
}

void save_qtree_as_ppm(QTNode *root, char *filename) 
{
    (void)root;
    (void)filename;
}

QTNode *load_preorder_qt(char *filename) 
{
    (void)filename;
    return NULL;
}

void save_preorder_qt(QTNode *root, char *filename) 
{
    (void)root;
    (void)filename;
}

