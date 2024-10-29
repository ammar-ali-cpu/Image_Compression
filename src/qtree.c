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

// QTNode *load_preorder_helper(FILE *fp)
// {
//     char leafOfNode = 'X';
//     int intensity = 0;
//     int row = 0;
//     int height = 0;
//     int col = 0;
//     int width = 0;
//     fscanf(fp, "%c %d %d %d %d %d", &leafOfNode, &intensity, &row, &height, &col, &width);
//     if(leafOfNode == 'X')
//     {
//         return NULL;
//     }

//     QTNode *nodeToReturn = malloc(sizeof(QTNode));
//     nodeToReturn->intensity = intensity;
//     nodeToReturn->row = row;
//     nodeToReturn->height = height;
//     nodeToReturn->col = col;
//     nodeToReturn->width = width;
//     nodeToReturn->child1 = NULL;
//     nodeToReturn->child2 = NULL;
//     nodeToReturn->child3 = NULL;
//     nodeToReturn->child4 = NULL;

//     if(leafOfNode == 'N')
//     {
//         nodeToReturn->child1 = load_preorder_helper(fp);
//         nodeToReturn->child2 = load_preorder_helper(fp);
//         nodeToReturn->child3 = load_preorder_helper(fp);
//         nodeToReturn->child4 = load_preorder_helper(fp);
//     }
//     return nodeToReturn;
// }
QTNode *load_preorder_helper(FILE *fp) 
{
    char nodeType;  // To hold either 'N' for internal node or 'L' for leaf node
    int intensity = 0;
    int row = 0;
    int height = 0;
    int col = 0;
    int width = 0;

    // Read the node information from the file
    if (fscanf(fp, " %c %d %d %d %d %d", &nodeType, &intensity, &row, &height, &col, &width) != 6) 
    {
        return NULL;  // Return NULL if we don't read exactly 6 items
    }

    QTNode *nodeToReturn = malloc(sizeof(QTNode));
    if (nodeToReturn == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;  // Return NULL if memory allocation fails
    }

    // Set the properties of the current node
    nodeToReturn->intensity = intensity;
    nodeToReturn->row = row;
    nodeToReturn->height = height;
    nodeToReturn->col = col;
    nodeToReturn->width = width;

    // Check if this node is an internal node or a leaf node
    if (nodeType == 'N') 
    {
        // If it's an internal node, recursively load the children
        nodeToReturn->nodeOrLeaf = 'N';  // Set node type to internal
        nodeToReturn->child1 = load_preorder_helper(fp);
        nodeToReturn->child2 = load_preorder_helper(fp);
        nodeToReturn->child3 = load_preorder_helper(fp);
        nodeToReturn->child4 = load_preorder_helper(fp);
    } 
    else if (nodeType == 'L') 
    {
        // If it's a leaf node, set the node type to leaf and initialize children to NULL
        nodeToReturn->nodeOrLeaf = 'L';  // Set node type to leaf
        nodeToReturn->child1 = NULL;  // Leaf nodes do not have children
        nodeToReturn->child2 = NULL;
        nodeToReturn->child3 = NULL;
        nodeToReturn->child4 = NULL;
    }

    return nodeToReturn;  // Return the constructed node
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

void save_preorder_qt(QTNode *root, char *filename) 
{
    (void)root;
    (void)filename;
}

