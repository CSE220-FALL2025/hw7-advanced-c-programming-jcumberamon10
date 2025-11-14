#include "../include/hw7.h"
#include <string.h>
#include <ctype.h>
// Allocate memory for a matrix 'c' 
//matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));

//Deallocate 
//free(m)
bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    // base case 
    /*
    typedef struct bst_sf {
    matrix_sf *mat;
    struct bst_sf *left_child;
    struct bst_sf *right_child;
    } bst_sf;

    */
    if(root == NULL){
        bst_sf *node = malloc(sizeof(bst_sf));
        node->mat = mat;
        node->left_child = NULL;
        node->right_child = NULL;
        return node;
    }
    if(mat->name < root->mat->name){
        root->left_child = insert_bst_sf(mat, root->left_child);
    }
    else if(mat->name > root->mat->name){
        root->right_child = insert_bst_sf(mat, root->right_child);
    }
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if (root == NULL)
        return NULL;
    if(root->mat->name == name)
        return root->mat;
    if(name < root->mat->name)
        return find_bst_sf(name, root->left_child);
    if(name > root->mat->name)
        return find_bst_sf(name,root->right_child);
}


void free_bst_sf(bst_sf *root) {
    if(root == NULL)
        return;
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);
    free(root->mat);
    free(root);
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    unsigned int rows = mat1->num_rows;
    unsigned int cols = mat1->num_cols;
    matrix_sf *m = malloc(sizeof(matrix_sf) + rows*cols*sizeof(int));
    m->num_rows = rows;
    m->num_cols = cols; 
    //m->name = 'm'; do i need a name for this matrix or no ?
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            //Number of colums or j ?
            //Somethign at the end of start of the matrix is not being calculated (need to cjeck edge)
            //{6, 27, 4, -26, 32, 30, 39, 20, 93, -47, -88, 24, 21, 16, -18}
            // 6 27 4 -26 32 0 39 0 93 -47 0 0 21 0 0
            //printf("%d\n",mat1->values[(i*cols) + j]);
            //printf("%d\n",mat2->values[(i*cols) + j]);
            //printf("%d\n",mat1->values[(i*cols) + j] + mat2->values[(i*cols) + j]);
            m->values[(i*cols) + j] = mat1->values[(i*cols) + j] + mat2->values[(i*cols) + j];
        }
    }
    //print_matrix_sf(m);
    return m;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    unsigned int rows = mat1->num_rows;
    unsigned int cols = mat2->num_cols;
    unsigned int mid_size = mat1->num_cols;
    matrix_sf *m = malloc(sizeof(matrix_sf) + rows*cols*sizeof(int));
    m->num_rows = rows;
    m->num_cols = cols; 
    int sum = 0;
    for(int i = 0; i < rows;i++ ){
        for(int j = 0; j < cols; j++){
            sum = 0;
            for(int k=0; k <mid_size; k++){
                sum += (mat1->values[(i*mid_size) + k]) * (mat2->values[( k* cols) + j]); 
            }
             m->values[i*cols + j] = sum; 
        }
        
    }
    
    //print_matrix_sf(m);

   return m;
}


matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    unsigned int rows= mat->num_cols;
    unsigned int cols = mat->num_rows;
    matrix_sf *m = malloc(sizeof(matrix_sf) + rows*cols*sizeof(int));
    m->num_rows = rows;
    m->num_cols = cols;
    for(int i = 0; i < rows;i++){
        for(int j = 0; j < cols; j++){
            m->values[i*cols + j] = mat->values[j*rows + i];
        }
    }
    return m;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    //idea parse string, find rows, find cols, loop to find array of ints, allocate approciate memory
    int len = strlen(expr);
    const char  *temp = expr;
    unsigned int rows = 0;
    unsigned int cols = 0;
    int index = 0;
    const char *endptr;
    //Finding rows and cols values
    while(*temp){
        if(isdigit(*temp)){
            rows = (unsigned int) strtol(temp,&temp,10);
            break;
        }
        else
            temp++;
    }
    while(*temp){
        if(isdigit(*temp)){
            cols = (unsigned int) strtol(temp,&temp,10);
            break;
        }
        else
            temp++;
    }  
    //going past left braket
    temp = strchr(temp, '[');
    if (!temp) return NULL;
    temp++;

    //allocate mmemory for matrix
    matrix_sf *m = malloc(sizeof(matrix_sf) + rows*cols*sizeof(int));
    m->name = name;
    m->num_rows =rows;
    m->num_cols = cols;
    //assign values to appropriate index using strol to skip over spaces and take care of '-' values aswell. skipping over ';'
    while(*temp && *temp != ']'){
        if (*temp == ';' || *temp == '[') {
            temp++;
            continue;
        }
        //skipping over spaces
        if(*temp == '-' || isdigit(*temp)){
            m->values[index] = (int) strtol(temp, &endptr,10);
            index++;
            temp = endptr;
        }
        else
            temp++;
        }   
    
    
    //print_matrix_sf(m);
    return m;
}

char* infix2postfix_sf(char *infix) {
    return NULL;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    return NULL;
}

matrix_sf *execute_script_sf(char *filename) {
   return NULL;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
