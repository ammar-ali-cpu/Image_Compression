#include "qtree.h"
#include <math.h>

static double avg_intensity_and_rmse(Image *image, unsigned int height, unsigned int width, unsigned int row, unsigned int col, double *rmse)
{
    double sum = 0.0;
    for(unsigned int i = row; i < row + height; i++)
    {
        for(unsigned int j = col; j < col + width; j++)
        {
            sum += get_image_intensity(image, i, j);
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
        root->child2 = helper(image, max_rmse, height/2, width -(width/2), row, col + (width/2));// 
        root->child3 = helper(image, max_rmse, height-(height/2), width/2, row + (height/2), col);//
        root->child4 = helper(image, max_rmse, height- (height/2), width - (width/2), row + (height/2), col + (width/2));//
    }
    else if((rmse > max_rmse) && width > 1)
    {
        root->nodeOrLeaf = 'N';
        root->child1 = helper(image, max_rmse, height, width/2, row, col);
        root->child2 = helper(image, max_rmse, height, width -(width/2), row, col + (width/2));// 
    }
    else if((rmse > max_rmse) && height > 1)
    {
        root->nodeOrLeaf = 'N';
        root->child1 = helper(image, max_rmse, height/2, width, row, col);
        root->child3 = helper(image, max_rmse, height-(height/2), width, row + (height/2), col);//
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

void ppm_helper(QTNode *root, FILE *fp)
{
    if(root==NULL)
    {
        return;
    }
    if (root->nodeOrLeaf == 'L') 
    {
        for (unsigned int rows = 0; rows < root->height; rows++) 
        {
            for (unsigned int cols = 0; cols < root->width; cols++) 
            {
                int intensity = root->intensity;
                fprintf(fp, " %d %d %d\n", intensity, intensity, intensity);
            }
        }
    }
    else
    {
        ppm_helper(root->child1, fp);
        ppm_helper(root->child2, fp);
        ppm_helper(root->child3, fp);
        ppm_helper(root->child4, fp);
    }
}

void save_qtree_as_ppm(QTNode *root, char *filename) 
{
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("Error: cannot open file");
    }
    fprintf(fp, "P3");
    fclose(fp);
    fp = fopen(filename, "a");
    int height = root->height;
    int width = root->width;
    fprintf(fp, "\n%d %d", width, height);
    fprintf(fp, "\n255");
    ppm_helper(root, fp);
    fclose(fp); 
}


QTNode *load_preorder_helper(FILE *fp) 
{
    char leafOrNode;  
    int intensity = 0;
    int row = 0;
    int height = 0;
    int col = 0;
    int width = 0;

    if (fscanf(fp, " %c %d %d %d %d %d", &leafOrNode, &intensity, &row, &height, &col, &width) != 6) 
    {
        return NULL; 
    }

    QTNode *nodeToReturn = malloc(sizeof(QTNode));
    nodeToReturn->intensity = intensity;
    nodeToReturn->row = row;
    nodeToReturn->height = height;
    nodeToReturn->col = col;
    nodeToReturn->width = width;

    if(leafOrNode == 'N') 
    {
        nodeToReturn->nodeOrLeaf = leafOrNode; 
        if(height == 1)
        {
            nodeToReturn->child1 = load_preorder_helper(fp);
            nodeToReturn->child2 = load_preorder_helper(fp);
            nodeToReturn->child3 = NULL;
            nodeToReturn->child4 = NULL;
        }
        else if(width == 1)
        {
            nodeToReturn->child1 = load_preorder_helper(fp);
            nodeToReturn->child2 = NULL;
            nodeToReturn->child3 = load_preorder_helper(fp);
            nodeToReturn->child4 = NULL;
        }
        else
        {
            nodeToReturn->child1 = load_preorder_helper(fp);
            nodeToReturn->child2 = load_preorder_helper(fp);
            nodeToReturn->child3 = load_preorder_helper(fp);
            nodeToReturn->child4 = load_preorder_helper(fp);
        }
    } 
    else if (leafOrNode == 'L') 
    {
        nodeToReturn->nodeOrLeaf = leafOrNode;  
        nodeToReturn->child1 = NULL;  
        nodeToReturn->child2 = NULL;
        nodeToReturn->child3 = NULL;
        nodeToReturn->child4 = NULL;
    }
    return nodeToReturn;  
}


QTNode *load_preorder_qt(char *filename) 
{
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("Error: cannot open file");
    }
    QTNode *root = load_preorder_helper(fp);
    fclose(fp);
    return root;
}


void save_preorder_helper(QTNode *node, FILE *fp)
{
    if(node == NULL)
    {
        return;
    }
    char nodeOfLeaf = node->nodeOrLeaf;
    unsigned char intensity = node->intensity;
    unsigned short height = node->height;
    unsigned short width = node->width;
    unsigned int row = node->row;
    unsigned int col = node->col;
    fprintf(fp, "%c %c %d %d %d %d\n", nodeOfLeaf, intensity, row, height, col, width);
    if(node->child1 != NULL)
    {
        save_preorder_helper(node->child1, fp);
    }
    if(node->child2 != NULL)
    {
        save_preorder_helper(node->child2, fp);
    }
    if(node->child3 != NULL)
    {
        save_preorder_helper(node->child3, fp);
    }
    if(node->child4 != NULL)
    {
        save_preorder_helper(node->child4, fp);
    }
}

void save_preorder_qt(QTNode *root, char *filename) 
{
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("Error: cannot open file");
    }
    fclose(fp);
    fopen(filename, "a");
    save_preorder_helper(root, fp);

    fclose(fp);
    (void)root;
    (void)filename;
}

