#include "qtree.h"
#include "image.h"

#include "tests_utils.h"


int main() 
{
    struct stat st;
    if (stat("tests/output", &st) == -1)
        mkdir("tests/output", 0700);
    prepare_input_image_file("building1.ppm"); // copies the image to the images/ directory

    /******************************* create_quadtree *******************************/
    double max_rmse = 25;
    Image *image = load_image("images/building1.ppm");
    QTNode *root = create_quadtree(image, max_rmse);

    char intensity = get_node_intensity(root);
    printf("%c ", intensity);
 
    QTNode *child1 = get_child1(root);
    char intensity1 = get_node_intensity(child1);
    printf("%c", intensity1);
    // See tests/input/load_preorder_qt1_qtree.txt for the expected results
    // You will need to write your own code to verify that your quadtree was constructed properly
    delete_quadtree(root);
    delete_image(image);



//     /******************************* load_preorder_qt *******************************/
//     // tests/input/load_preorder_qt1_qtree.txt was generated by executing the following code:
//     //   QTNode *root = create_quadtree(load_image("images/building1.ppm"), 25);
//     root = load_preorder_qt("tests/input/load_preorder_qt1_qtree.txt");
//     // You will need to write your own code to verify that your quadtree was constructed properly
//     delete_quadtree(root); 

//     /******************************* save_preorder_qt *******************************/
//     image = load_image("images/building1.ppm"); 
//     root = create_quadtree(image, 25);
//     // See tests/input/load_preorder_qt1_qtree.txt for expected output
//     save_preorder_qt(root, "tests/output/save_preorder_qt1_qtree.txt");
//     delete_quadtree(root);
//     delete_image(image);

//     /******************************* save_qtree_as_ppm *******************************/
    image = load_image("images/building1.ppm"); 
    root = create_quadtree(image, 25);
    save_qtree_as_ppm(root, "tests/output/save_qtree_as_ppm1.ppm");
    // See tests/expected/save_qtree_as_ppm1.ppm for the expected file.
    // Visual inspection is generally not sufficient to determine if your output image is correct.
    // You will need to write code to more rigorously check your output image for correctness.
    delete_image(image); 
    delete_quadtree(root);

//     /******************************* hide_message and reveal_message *******************************/
//     prepare_input_image_file("wolfie-tiny.ppm");
//     hide_message("0000000000111111111122222222223333333333", "images/wolfie-tiny.ppm", "tests/output/hide_message1.ppm");
//     char *message = reveal_message("tests/output/hide_message1.ppm");
//     printf("Message: %s\n", message);
//     free(message);

//     /******************************* hide_image and reveal_image *******************************/
//     hide_image("images/wolfie-tiny.ppm", "images/building1.ppm", "tests/output/hide_image1.ppm");
//     reveal_image("tests/output/hide_image1.ppm", "tests/output/reveal_image1.ppm");

//     return 0;
}

// int main()
// {
//     struct stat st;
//     if (stat("tests/output", &st) == -1)
//         mkdir("tests/output", 0700);
//     prepare_input_image_file("dog.ppm"); // copies the image to the images/ directory   

//     Image *image = load_image("images/dog.ppm");
//     printf("Width: %hu\n", image->width);
//     printf("Height: %hu\n", image->height);


//     delete_image(image);
// }
