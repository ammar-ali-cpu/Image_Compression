#include "qtree.h"
#include <math.h>

double avg_intensity_and_rmse(Image *image, unsigned int height, unsigned int width, unsigned int row, unsigned int col, double *rmse)
{
    double sum = 0.0;
    double sum_square = 0.0;
    for(unsigned int i = row; i < row + height; i++)
    {
        for(unsigned int j = col; j < col + width; j++)
        {
            sum += get_image_intensity(image, i, j);
            sum_square += (get_image_intensity(image, i, j)) * (get_image_intensity(image, i, j));
        }
    }
    double numOfPixels = height * width;
    double avg = sum / numOfPixels;

    *rmse = sqrt((sum_square/numOfPixels) - (avg * avg));
    return avg;
}

QTNode *helper(Image *image, double max_rmse, unsigned int height, unsigned int width, unsigned int row, unsigned int col)
{
    QTNode *root = (QTNode*)malloc(sizeof(QTNode));
    double rmse = 0;
    root->intensity = (unsigned char)avg_intensity_and_rmse(image, get_image_height(image), get_image_width(image), 0, 0, &rmse);
    root->height = height;
    root->width = width;
    root->row = row;
    root->col = col;
    root->child1 = NULL;
    root->child2 = NULL;
    root->child3 = NULL;
    root->child4 = NULL;
    root->nodeOrLeaf = 'L';

    if(rmse >= max_rmse)
    {
        root->nodeOrLeaf = 'N';
        root->child1 = helper(image, max_rmse, height/2, width/2, row, col);
        root->child2 = helper(image, max_rmse, height/2, width - (width/2), row, col + (width/2));
        root->child3 = helper(image, max_rmse, height-(height/2), width/2, row + (height/2), col);
        root->child4 = helper(image, max_rmse, height-(height/2), width - (width/2), row + (height/2), col + (width/2));
    }
    return root;
}


QTNode *create_quadtree(Image *image, double max_rmse) 
{
    QTNode *root = helper(image, max_rmse, get_image_height(image), get_image_width(image), 0, 0);
    return root;
}

QTNode *get_child1(QTNode *node) {
    (void)node;
    return NULL;
}

QTNode *get_child2(QTNode *node) {
    (void)node;
    return NULL;
}

QTNode *get_child3(QTNode *node) {
    (void)node;
    return NULL;
}

QTNode *get_child4(QTNode *node) {
    (void)node;
    return NULL;
}

unsigned char get_node_intensity(QTNode *node) {
    (void)node;
    return 0;
}

void delete_quadtree(QTNode *root) {
    (void)root;
}

void save_qtree_as_ppm(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}

QTNode *load_preorder_qt(char *filename) {
    (void)filename;
    return NULL;
}

void save_preorder_qt(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}

