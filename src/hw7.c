// Jason Cumbe-Ramon
// SBUID: 112730509
#include "../include/hw7.h"
#include <string.h>
#include <ctype.h>
// Character struct implementation to help the infix-postfix functions
typedef struct {
    char items[MAX_LINE_LEN];
    int top;
} CharStack;

void initializeCharStack(CharStack *stack){
    stack->top = -1;
}
int isEmpty(CharStack *stack){
    return stack->top == -1;
}
void Cpush(CharStack *stack, char item){
    stack->items[++stack->top] = item;
}
char Cpop(CharStack *stack){
    char poppedItem = stack->items[stack->top--];
    return poppedItem;
}
char peek(CharStack *stack){
    return stack->items[stack->top];
}

//Matrix stack implementation to use when evaluating an expression
typedef struct {
    matrix_sf *items[MAX_LINE_LEN]; 
    int top;
} MatrixStack;

void initializeMatrixStack(MatrixStack *stack) {
    stack->top = -1;
}

int isEmptyMatrix(MatrixStack *stack) {
    return stack->top == -1;
}

void Mpush(MatrixStack *stack, matrix_sf *m) {
    stack->items[++stack->top] = m;
}

matrix_sf* Mpop(MatrixStack *stack) {
    return stack->items[stack->top--];
}

matrix_sf* Mpeek(MatrixStack *stack) {
    return stack->items[stack->top];
}


//recursively inserts node into correct location in bst
bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
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

//recursively finds node in a bst structure
matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if (root == NULL)
        return NULL;
    if(root->mat->name == name)
        return root->mat;
    if(name < root->mat->name)
        return find_bst_sf(name, root->left_child);
    if(name > root->mat->name)
        return find_bst_sf(name,root->right_child);
    return NULL;
}

//frees each part of bst and its corresponding matrix nodes
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
    for(unsigned int i = 0; i < rows; i++){
        for(unsigned int j = 0; j < cols; j++){
            m->values[(i*cols) + j] = mat1->values[(i*cols) + j] + mat2->values[(i*cols) + j];
        }
    }
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
    for(unsigned int i = 0; i < rows;i++ ){
        for(unsigned int j = 0; j < cols; j++){
            sum = 0;
            for(unsigned int k=0; k <mid_size; k++){
                sum += (mat1->values[(i*mid_size) + k]) * (mat2->values[( k* cols) + j]); 
            }
             m->values[i*cols + j] = sum; 
        }
        
    }
   return m;
}


matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    unsigned int rows= mat->num_cols;
    unsigned int cols = mat->num_rows;
    matrix_sf *m = malloc(sizeof(matrix_sf) + rows*cols*sizeof(int));
    m->num_rows = rows;
    m->num_cols = cols;
    for(unsigned int i = 0; i < rows;i++){
        for(unsigned int j = 0; j < cols; j++){
            m->values[i*cols + j] = mat->values[j*rows + i];
        }
    }
    return m;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    char const *temp = expr;
    unsigned int rows = 0;
    unsigned int cols = 0;
    int index = 0;
    char *endptr;
    //Finding rows and cols values
    while(*temp){
        if(isdigit(*temp)){
            rows = (unsigned int) strtol((char*)temp,&endptr,10);
            temp = endptr;
            break;
        }
        else
            temp++;
    }
    while(*temp){
        if(isdigit(*temp)){
            cols = (unsigned int) strtol((char*)temp,&endptr,10);
            temp = endptr;
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
    
    
    return m;
}

//helper functions for infix2postfix
int precedence(char c) {
    if (c == '\'')
        return 3;
    else if (c == '*')
        return 2;
    else if (c == '+')
        return 1;
    return -1;    
}

char* infix2postfix_sf(char *infix) {
    int size = strlen(infix);
    CharStack *operator = malloc(sizeof(CharStack));
    initializeCharStack(operator);

    char *result = malloc(size + 1);
    int pos = 0;

    for(int i = 0; i < size; i++){
        char c = infix[i];
        if(isalpha(c)){
            result[pos] = c;
            pos++;
        }
        else if(c == '(')
            Cpush(operator,c);
        else if(c == ')'){
            while(!isEmpty(operator) && peek(operator) != '('){
                result[pos] = Cpop(operator);
                pos++;
            }
            Cpop(operator);
        }
        else if (c == '+' || c == '*' || c == '\'') {
            while (!isEmpty(operator) && precedence(peek(operator)) >= precedence(c) && peek(operator) != '(') {
                result[pos] = Cpop(operator);
                pos++;
            }
            Cpush(operator, c);
        }
        
    }
    while (!isEmpty(operator)){
            result[pos++] = Cpop(operator);
        }
        result[pos] = '\0';
    free(operator);
    return result;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    //create postfix string to be evaluated
    char *postfix = infix2postfix_sf(expr);
    int size = strlen(postfix);

    MatrixStack *stack = malloc(sizeof(MatrixStack));
    initializeMatrixStack(stack);
    //evaluating expression 
    for (int i = 0; i < size; i++) {
        
        char c = postfix[i];

        //we find an operand
        if (isalpha(c)) {
            matrix_sf *m = find_bst_sf(c, root);
            Mpush(stack, m);
        }

        else if (c == '\'') {
            matrix_sf *m1 = Mpop(stack);
            matrix_sf *res = transpose_mat_sf(m1);
            res->name = '?';  
            Mpush(stack, res);
            if (!isalpha(m1->name))
                 free(m1);
        }
        else if (c == '+') {
            matrix_sf *m2 = Mpop(stack);
            matrix_sf *m1 = Mpop(stack);
            matrix_sf *res = add_mats_sf(m1, m2);
            res->name = '?'; //temp matrix with a nonalpha value

            Mpush(stack, res);

            // free only temp operands
            if (!isalpha(m1->name))
                free(m1);
            if (!isalpha(m2->name))
                free(m2);
        }

        else if (c == '*') {

            matrix_sf *m2 = Mpop(stack);
            matrix_sf *m1 = Mpop(stack);

            matrix_sf *res = mult_mats_sf(m1, m2);
            res->name = '?';

            Mpush(stack, res);

            if (!isalpha(m1->name))
                free(m1);
            if (!isalpha(m2->name))
                free(m2);
        }
    }

    matrix_sf *result = Mpop(stack);
    result->name = name;
    free(postfix);
    free(stack);
    return result;
}

matrix_sf* execute_script_sf(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) 
        return NULL;

    bst_sf *root = NULL;
    char *line = NULL;
    size_t max_line_size = MAX_LINE_LEN;
    matrix_sf *result_matrix = NULL;

    while (getline(&line, &max_line_size, fp) != -1) {
        // handling empty spaces or file with only empty lines
        int only_spaces = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isspace(line[i])) {
                only_spaces = 0;
                break;
            }
        }
        if (only_spaces) 
            continue;

        char *eq = strchr(line, '=');

        // finding the matrix name before '='
        char name = '\0';
        char *ptr = line;
        while (ptr < eq) {
            if (isalpha(*ptr)) {
                name = *ptr;
                break;
            }
            ptr++;
        }

        // looking for the RHS of equation skipping whitespaces
        char *expr = eq + 1;
        while (*expr && isspace(*expr)) 
                expr++;

        // checking for matrix literal
        if (strchr(expr, '[')) {
            matrix_sf *m = create_matrix_sf(name, expr);
            root = insert_bst_sf(m, root);
            result_matrix = m;
        } else {
            // Otherwise, evaluate expression
            matrix_sf *m = evaluate_expr_sf(name, expr, root);
            root = insert_bst_sf(m, root);
            result_matrix = m;
        }
    }
    //need to copy matrix to free BST
    matrix_sf *result = NULL;

    // create deep copy if matrix exist
    if (result_matrix) {
        result = copy_matrix(result_matrix->num_rows, result_matrix->num_cols,result_matrix->values);
        result->name = result_matrix->name; 
    }

    free_bst_sf(root);
    free(line);
    fclose(fp);
    return result;
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
