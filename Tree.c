// Timothy Stephenson June 14th 2021

// Implementation for binary Tree

// all keys must be "strings", in C this means a '\0' terminated char* must end them
// all values must be void pointers 

// max number of elements in a given TREE is the max value of a unsigned int (4,294,967,295)


//Will sure AVL Rebalancing to keep the Binary Search Tree balanced
// the term height refers to the the longest path from a given node to a empty node
// ie a "NULL" node has a hieght of 0, a leaf node has a height of 1, and the height of any given node is
// the max height of its two children + 1 

#include <string.h>

#include <stdlib.h>

#include <assert.h>

#include <stdbool.h>

#include "Tree.h"


typedef struct node node;
struct node{
    unsigned int size;
    unsigned short int height;
    char *key;
    void *val;
    node *l;
    node *r;
};
 
void node_Free(node *n , void (*freeing_function)(void*) ){
    if(n!=NULL){
        node_Free(n->l , freeing_function);
        node_Free(n->r , freeing_function);
        free(n->key);
        freeing_function(n->val);
        free(n);
    }
    return;
}

node *node_Leaf(char *k , void *v ){
    node *n = (node*) malloc( sizeof(node) );
    n->size = 1;
    n->height = 1;
    n->key = k;
    n->val = v;
    n->l = NULL;
    n->r = NULL;
    return n;
}

unsigned int node_Size(node *n){
    if (n==NULL) return 0;
    return n->size;
}

unsigned short int node_Height(node *n){
    if(n==NULL) return 0;
    return n->height;
}

// assuming childrens nodes sizes are correct, fix the size of node *n
void node_FixSize(node *n){
    n->size =  1 + node_Size(n->l) + node_Size(n->r);
    return;
}

//assuming childrens heights are correct, fix the height of node *n 
void node_FixHeight(node *n){
    unsigned short int left_height = node_Height(n->l) , right_height = node_Height(n->r) ;
    // left child's node has a greater height:
    if(left_height > right_height) n->height = left_height + 1;
    // right child's node has a greater or equal height:
    else n->height = right_height + 1;
    return;
}

//postive for left-heavy , negative for right-heavy
short int node_HeightDiffernce(node *n){
    assert(n!=NULL);
    return node_Height(n->l) - node_Height(n->r);
}


void node_FillKeys(node *n, char **string_list){
    if(n==NULL) return;
    // element itself
    unsigned items_leftward = node_Size(n->l);

    string_list[items_leftward] = n->key;

    // fill left
    node_FillKeys(n->l,string_list);

    // fill right
    node_FillKeys(n->r, &string_list[items_leftward+1] );

    return;
}


void *node_Search(node *n, char *k){
    if(n==NULL) return NULL;
    int i = strcmp(k,n->key);
    //Equal
    if(i==0) return n->val;

    // Less
    if(i<0) return node_Search(n->l , k);

    // Greater (i>0)
    return node_Search(n->r , k);
}

// to call function, call node_AverageDepth(n , n->size, 1)
double node_AverageDepth(node *n, unsigned int TotalSize, unsigned short int depth){
    if(n==NULL) return 0;
    return ( (double) depth / TotalSize ) + node_AverageDepth(n->l,TotalSize,depth+1) + node_AverageDepth(n->r,TotalSize,depth+1) ;
}


void node_Assign(node *n , char *k , void *v){
    assert(n!=NULL);
    int i = strcmp(k,n->key);
    // found the key value pair
    if(i==0) n->val = v;
    // Less tthan
    if(i<0) node_Assign(n->l , k , v);
    // greater than
    if(i>0) node_Assign(n->r , k , v);

    return;
}

//precondition n->r != NULL
node* node_LeftRotate(node *n){
    assert(n->r != NULL);
    node *n_right = n->r;

    n->r = n_right->l ;
    node_FixSize(n);
    node_FixHeight(n);

    n_right->l = n;
    node_FixSize(n_right);
    node_FixHeight(n_right);
    return n_right;
}

//precondition n->l != NULL
node* node_RightRotate(node *n){
    assert(n->l != NULL);
    node *n_left = n->l;

    n->l = n_left->r ;
    node_FixSize(n);
    node_FixHeight(n);

    n_left->r = n;
    node_FixSize(n_left);
    node_FixHeight(n_left);
    return n_left;
}

node* node_ChildDirection(node *n, bool go_right){ assert(n!=NULL); if(go_right) return(n->r); return(n->l); }

//allocate space for min_max_child to store one node pointer, and free it afterwards
//precondition:
// the mix or max will not be the head of the intial function call
node *node_RemoveMinMax(node *n, bool go_right, char **to_fix, node **min_max_child){
    assert(n!=NULL);
    if(go_right && n->r == NULL){
        *(min_max_child) = n->l;
        n->l = NULL;
        node_FixSize(n);
        node_FixHeight(n);
        return n;
    }
    if(!go_right && n->l == NULL){
        *(min_max_child) = n->r;
        n->r = NULL;
        node_FixSize(n);
        node_FixHeight(n);
        return n;
    }


    node *min_max = node_RemoveMinMax(node_ChildDirection(n,go_right) , go_right , to_fix , min_max_child);

    //the child of n is the min/max
    if(min_max == node_ChildDirection(n,go_right) ){

        if(go_right) n->r = *(min_max_child);
        else n->l = *(min_max_child);

        *(to_fix) = n->key;
    }
    node_FixSize(n);
    node_FixHeight(n);
    return min_max;
}

//will merge two nodes who's heights differ by no more than +=1 (ie two previous children of a now deleted parent)
// the outputed out will be <= 1 + max( node_Height(left) , node_Height(right) )
//Precondition:
// ( left !=NULL || right != NULL ) aka do not call merge on two empty nodes, as there is nothing for to_fix to be pointed at
node* node_Merge(node *left , node *right, char **to_fix){
    assert( left !=NULL || right != NULL );
    if (left==NULL){
        *(to_fix) = right->key;
        return right;
    } 
    if (right==NULL){
        *(to_fix) = left->key;
        return left; 
    }

    if (left->r ==NULL){
        left->r = right;
        node_FixSize(left);
        node_FixHeight(left);
        *(to_fix) = left->key;
        return left;
    }
    if (right->l==NULL){
        right->l = left;
        node_FixSize(right);
        node_FixHeight(right);
        *(to_fix) = right->key;
        return right;
    }

    node *new_root;
    node **temp = (node**) malloc(1 * sizeof(node*) );
    bool random_bool = (bool) rand()%2 ;
    if(left->height > right->height || (left->height==right->height && random_bool) ){
        new_root = node_RemoveMinMax(left , true , to_fix , temp ); 
    }
    else{
        new_root = node_RemoveMinMax(right , false , to_fix , temp ); 
    }
    free(temp);

    new_root->l = left;
    new_root->r = right;
    node_FixSize(new_root);
    node_FixHeight(new_root);
    return new_root;
}

//prconditions:
// a node with char *k exists in the tree with n at the root
// prior to adding the node with char *k, it was a balanced AVL tree
node* node_RebalanceDirectionAVL(node *n, char *k){
    if(n==NULL) return NULL;
    int i = strcmp(k,n->key);
    // k is to the left
    if(i<0) n->l = node_RebalanceDirectionAVL(n->l , k);
    // k is to the right
    if(i>0) n->r = node_RebalanceDirectionAVL(n->r , k);
    node_FixHeight(n);

    if( node_HeightDiffernce(n) < -1 || 1 < node_HeightDiffernce(n) ){
        assert( node_HeightDiffernce(n) == -2 || node_HeightDiffernce(n) == 2 ); //for testing, should only be possible to get messed up by one height differnce

        //doubly left heavy:
        if(node_HeightDiffernce(n)==2){

            //n->l is balanced or left heavy:
            if( 0 <= node_HeightDiffernce(n->l) ) return node_RightRotate(n);
            //n->l is right heavy:
            else{
                n->l = node_LeftRotate(n->l);
                node_FixHeight(n);
                return node_RightRotate(n);
            }
        }

        //doubly right heavy:
        if(node_HeightDiffernce(n)==-2){
            //n->r is balanced or right heavy:
            if( node_HeightDiffernce(n->r) <= 0 ) return node_LeftRotate(n);
            //n->r is left heavy:
            else{
                n->r = node_RightRotate(n->r);
                node_FixHeight(n);
                return node_LeftRotate(n);
            }
        }


    }

    return n;
}


// inserts the key val pair to the node tree
// precondition: n is a valid node, char *k does not exist in the tree
void node_Insert(node *n, char *k, void *v){
    assert(n!=NULL);
    int i = strcmp(k,n->key);

    // Less
    if(i<0){
        if(n->l==NULL) n->l = node_Leaf(k,v);
        else node_Insert(n->l , k , v);
    }
    // greater
    if(i>0){
        if(n->r==NULL) n->r = node_Leaf(k,v);
        else node_Insert(n->r , k , v);
    }
    // precondition assures i==0 is not a possiblity 
    node_FixSize(n);
    node_FixHeight(n);
    return;
}

//remove the node with the char *k from the tree
//precondition: 
// node_Search(n , k) != NULL
// on the intial call on the root of the tree, n->size >= 2  (there must be a node left once the specified node is deleted)
//points the string pointer to_fix to the node to call node_RebalanceDirectionAVL on in order to maintain AVL property
// the only nodes that have potentially changed heightts are "to_fix" and its parents
node *node_Remove(node *n, char *k, char **to_fix){
    assert(n!=NULL);
    int i = strcmp(k,n->key);
    node *to_return = n;

    //at the node to remove
    if(i==0){
        if(n->l==NULL && n->r==NULL) to_return = NULL;
        else to_return = node_Merge(n->l,n->r , to_fix);
        free(n->key);
        free(n);
    }
    else{
        // in case n->l or n->r is a leaf of only the node we want to delete, we save its parents key as the node to starting fixing from
        *(to_fix) = n->key;
        if(i<0) n->l = node_Remove(n->l , k , to_fix);
        if(i>0) n->r = node_Remove(n->r , k , to_fix);
        node_FixSize(n);
        node_FixHeight(n);
    }

    return to_return;
}

// precondition 0<= i < n->size
//to call from head node, give 0 as nodes_leftward
char *node_nth(node *n, unsigned i, unsigned nodes_leftward){
    assert(n!=NULL);
    unsigned true_nodes_leftward = nodes_leftward + node_Size(n->l);

    //are at nodes where exactly i nodes are "left" of it, thus 0-indexed, it is in the i'th spot
    if(i==true_nodes_leftward) return n->key;

    // too many nodes leftward (past index i)
    if(i<true_nodes_leftward) return node_nth(n->l , i , nodes_leftward);

    //too few nodes leftward (before index i) (i>true_nodes_leftward)
    return node_nth(n->r , i, true_nodes_leftward+1 );

}

unsigned node_Position( node *n , char *k ){
    if(n==NULL) return 0;
    int i = strcmp(k , n->key);
    // are at the given node
    if (i==0) return node_Size(n->l);

    //go left
    if(i<0) return node_Position(n->l , k);

    //go right (i>0)
    return 1 + node_Size(n->l) + node_Position(n->r , k);
} 



void node_ValidHelper(node*n){
    if(n==NULL) return;
    assert(n->size == 1 + node_Size(n->l) + node_Size(n->r) );
    assert(n->key!=NULL);
    assert(n->val!=NULL);
    assert( (node_Height(n->l) > node_Height(n->r) && n->height == node_Height(n->l) +1 ) ||  (node_Height(n->l) <= node_Height(n->r) && n->height == node_Height(n->r) +1 )  );

    //AVL Property:
    assert( -1 <= node_HeightDiffernce(n) && node_HeightDiffernce(n) <= 1 );

    node_ValidHelper(n->l);
    node_ValidHelper(n->r);
    return;
}


void node_Valid(node *n){
    if(n==NULL) return;
    node_ValidHelper(n);

    //make sure strings are in correct sorted order, and tests out some functions
    char **string_list = (char**) malloc(n->size *  sizeof(char*)  );
    node_FillKeys(n , string_list);
    int i = 0;
    for(i=0; i<n->size; i+=1){
        if (i!=n->size-1) assert( strcmp(string_list[i] , string_list[i+1]) < 0 );
        assert(node_Position(n,string_list[i]) == i );
        assert(node_nth(n,i,0)==string_list[i]);
    }
    free(string_list);

    return;
}











struct TREE{
    unsigned int size;
    node *head;
};

void TREE_Valid(TREE *d){
    // Goes through a O(n) validation checker, would make O(log n) function like Add and delete O(n) if they validated the tree every time
    // only comment in for testing with smaller datasets
    // node_Valid(d->head); 
    assert(d->size == node_Size(d->head));
}


// returns a empty dictionary
TREE* TREE_Empty(){
    TREE *d = (TREE*) malloc( sizeof(TREE) );
    d->head = NULL;
    d->size = 0;
    TREE_Valid(d);
    return d;
}

// TODO: MAKE THIS A HIGHER ORDER FUNCTION THAT TAKES A FUNTION AS INPUT TO FREE THE VOID POINTERS
// Free's a given Tree
// the second field is for a function that takes the "values" of the tree, stored as void pointers and frees them
void TREE_Free(TREE *d , void (*freeing_function)(void*) ){ 
    node_Free(d->head , freeing_function);  
    free(d);  
    return; 
}

// if the key already exists, changes the value to the inputed void pointer and frees the callers char *k
// if the key does not exist, adds the key value pair to the dictionary (using the callers char *k)
// either way, do not free char *k after calling
void TREE_Add(TREE *d , char *k , void *v){
    assert(v!=NULL);
    assert(k!=NULL);
    // currently empty
    if(d->head==NULL){
        d->head = node_Leaf(k,v);
        d->size +=1;
    }
    else{
        // key does not exist already
        if( node_Search(d->head ,k)==NULL ){
            node_Insert(d->head , k , v);
            d->size +=1;
            d->head = node_RebalanceDirectionAVL(d->head , k);
        }
        //key does already exist
        else{
            node_Assign(d->head , k , v);
            free(k);
        }
    }

    TREE_Valid(d);
    return;
}

// returns the void pointer ascociated with the given char* in the TREE if it existts, returns NULL if it doesn't
void* TREE_Search(TREE *d , char *k){ return node_Search(d->head , k);  }


// removes the key val pair associated with the inputted char* does not free whatever the void pointer was pointing to
// will free the key in the tree, but not the callers char *k
// will also free the void pointer at char *k using the third field inputed function.
void TREE_Remove(TREE *d , char *k , void (*freeing_function)(void*) ){
    void *value = node_Search(d->head , k);
    if(value==NULL) return;
    

    if(d->size==1){
        node_Free(d->head, freeing_function);
        d->head = NULL;
    }
    else{
        freeing_function(value);
        char **to_fix = (char**) malloc( 1 * sizeof(char*) );
        d->head = node_Remove(d->head, k, to_fix);
        d->head = node_RebalanceDirectionAVL(d->head , *(to_fix) );
        free(to_fix);
    }

    d->size += -1;
    TREE_Valid(d);
    return;
}

// returns the number of elements in the given TREE
unsigned TREE_Size(TREE *d){ return d->size; }

// returns a sorted list of the keys in the given TREE
// contains pointers to the actual keys used in the TREE, so do not modify/free tthem
char** TREE_KeyList(TREE *d){
    if(d->head==NULL) return NULL;

    char **string_list = (char**) malloc(d->size *  sizeof(char*)  );
    node_FillKeys(d->head , string_list);

    return string_list;
}


// gives a score of how balanced a tree is
// sum of all nodes depth above max depth  / total_nodes
double TREE_AverageDepth(TREE *d){ return node_AverageDepth(d->head, d->size, 1); }

// returns the max depth of a given node
unsigned TREE_Height(TREE *d){ return node_Height(d->head); }

// returns the nth member in sorted order of the dicttonary, returns NULL if invalid n
//returns tthe actual key currently being used by the dictionary
char *TREE_nth(TREE *d , unsigned n){
    if( n >= d->size ) return NULL;
    return node_nth(d->head , n, 0);
}

// returns the number of elements that come before (BUT NOT AT) the inputed char
// the given char does not have to exist in the TREE and is not freed or mutated durning runtime
unsigned TREE_Position( TREE *d , char *k ){ return node_Position(d->head, k); }
