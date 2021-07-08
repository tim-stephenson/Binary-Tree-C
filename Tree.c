// Timothy Stephenson June 14th 2021

// Implementation for binary Tree

// all keys must be "strings", in C this means a '\0' terminated char* must end them
// all values must be void pointers 

// max number of elements in a given TREE is the max value of a unsigned int (4,294,967,295)
// though I wouldn't recommend going past a few million

#include <string.h>

#include <stdlib.h>

#include <assert.h>

#include <stdbool.h>

#include "Tree.h"


typedef struct node node;
struct node{
    unsigned int size;
    char *key;
    void *val;
    node *l;
    node *r;
};
 
void node_Free(node *n){
    if(n!=NULL){
        node_Free(n->l);
        node_Free(n->r);
        free(n->key);
        free(n);
    }
    return;
}

node *node_Leaf(char *k , void *v ){
    node *n = (node*) malloc( sizeof(node) );
    n->size = 1;
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


// assuming childrens nodes sizes are correct, fix the size of node *n
void node_FixSize(node *n){
    n->size =  1 + node_Size(n->l) + node_Size(n->r);
    return;
}

// returns the max depth that should occur in a balanced tree
// is functionally just math.floor( log2(size) )
unsigned int max_depth(unsigned int size){
    if(size==0) return 0;
    return 1 + max_depth( (unsigned int) size/2 ); 
}

// to call function, call node_BalanceScore(n , n->size, 1)
double node_AverageDepth(node *n, unsigned int TotalSize, unsigned short int depth){
    if(n==NULL) return 0;
    return ( (double) depth / TotalSize ) + node_AverageDepth(n->l,TotalSize,depth+1) + node_AverageDepth(n->r,TotalSize,depth+1) ;
}

unsigned node_MaxDepth(node *n){
    if (n==NULL) return 0;

    unsigned left_depth = node_MaxDepth(n->l) , right_depth = node_MaxDepth(n->r) ;
    if (left_depth>right_depth) return left_depth+1;
    return right_depth +1;
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

node* node_Merge(node *left , node *right){
    if (left==NULL) return right;
    if (right==NULL) return left;
    if (left->r ==NULL){
        left->r = right;
        left->size += right->size;
        return left;
    }
    if (right->l==NULL){
        right->l = left;
        right->size += left->size;
        return right;
    }

    node *ptr, *temp;
    bool go_right; //true to go right, false to go left
    if(left->size > right->size){
        ptr = left;
        go_right = true;
    }
    else{
        ptr = right;
        go_right = false;
    }
    while(true){
        ptr->size += -1;
        if(go_right){
            if(ptr->r->r==NULL){
                temp = ptr->r;
                ptr->r = ptr->r->l;
                break;
            }
            ptr = ptr->r;
        }
        else{
            if(ptr->l->l==NULL){
                temp = ptr->l;
                ptr->l = ptr->l->r;
                break;
            }
            ptr = ptr->l;
        }
    }
    temp->l = left;
    temp->r = right;
    node_FixSize(temp);
    return temp;
}


// returns an reblanced tree with the node with the key = char *k as the head 
// precondition, k is a key in the tree
// O(logn ^2) where n is size of treee
node *node_SplitAt(node *n , char *k){
    assert(n!=NULL);

    int i = strcmp(k,n->key);
    // Equal
    if(i==0) return n;
    node *recursion_node = NULL;
    // Less
    if(i<0){
        recursion_node = node_SplitAt(n->l , k);
        n->l = NULL;
        node_FixSize(n);
        recursion_node->r = node_Merge(recursion_node->r , n);
        node_FixSize(recursion_node); 
        return recursion_node;
    }
    // greater (i>0):
        recursion_node = node_SplitAt(n->r , k);
        n->r = NULL;
        node_FixSize(n);
        recursion_node->l = node_Merge(n, recursion_node->l);
        node_FixSize(recursion_node); 
        return recursion_node;
}


// a call should look like node_SplitAtMiddle(n , 0 , n->size)
node *node_SplitAtMiddle(node *n, unsigned int items_before , unsigned int TotalSize){
    assert(n!=NULL);
    if(n->size==1) return n; // only thing to return if only one node

    unsigned int constant_size = n->size;
    unsigned int all_leftward = items_before + node_Size(n->l);

    // middle
    if(all_leftward == TotalSize / 2) return n;
    node *recursion_node = NULL;
    // too many (go left)
    if(all_leftward > TotalSize / 2){
        recursion_node = node_SplitAtMiddle(n->l , items_before , TotalSize);
        n->l = NULL;
        node_FixSize(n);
        recursion_node->r = node_Merge(recursion_node->r , n);
        node_FixSize(recursion_node);
        assert(constant_size==recursion_node->size);
        return recursion_node;
    }
    // too few (go right)
    if(all_leftward < TotalSize / 2){
        recursion_node = node_SplitAtMiddle(n->r , items_before + node_Size(n->l) + 1 , TotalSize);
        n->r = NULL;
        node_FixSize(n);
        recursion_node->l = node_Merge(n , recursion_node->l);
        node_FixSize(recursion_node);
        assert(constant_size==recursion_node->size);
        return recursion_node;
    }

    assert(false);
    return NULL;
}

bool node_uneven(node *n){
    assert(n!=NULL);
    // make sure a theoritical max depth of a child node + 1 is not greater than the theoritical max depth of the parent node
    return( max_depth( n->size ) < 1 + max_depth( node_Size(n->l) ) || max_depth( n->size ) < 1 + max_depth( node_Size(n->r) ) );
}

node *node_RebalanceALL(node *n ){
    if(n==NULL) return NULL;
    node *to_return = n;
    if( node_uneven(n) ){
        to_return = node_SplitAtMiddle(n , 0, n->size);
    }
    to_return->l = node_RebalanceALL(to_return->l);
    to_return->r = node_RebalanceALL(to_return->r);
    return to_return;
}

//rebalances all that needs such in the direction of char *k
node *node_RebalanceDirection(node *n , char *k){
    if(n==NULL) return NULL;

    node *to_return = n;
    if (node_uneven(n)){
        to_return = node_SplitAtMiddle(n , 0, n->size);
    }
    int i = strcmp(k,n->key);
    // k is to the left:
    if(i<0){
        to_return->l = node_RebalanceDirection(to_return->l, k);
    }
    //if k is to the right
    if(i>0){
        to_return->r = node_RebalanceDirection(to_return->r, k);
    }
    return to_return;
}

// inserts the key val pair to the node tree
// precondition: n is a valid node, char *k does not exist in the tree
void node_Insert(node *n, char *k, void *v){
    assert(n!=NULL);
    int i = strcmp(k,n->key);
    n->size +=1;
    // Less
    if(i<0){
        if(n->l==NULL) n->l = node_Leaf(k,v);
        else node_Insert(n->l , k , v);
        return;
    }
    // greater
    if(i>0){
        if(n->r==NULL) n->r = node_Leaf(k,v);
        else node_Insert(n->r , k , v);
        return;
    }
    // equal (i==0)
    assert(false);
    return;
}

//remove the node with the char *k from the tree
//precondition: node_Search(n , k) != NULL
node *node_Remove(node *n, char *k){
    assert(n!=NULL);
    int i = strcmp(k,n->key);

    //at the node to remove
    if(i==0){
        node *temp = node_Merge(n->l,n->r);
        free(n->key);
        free(n);
        return temp;
    }
    n->size += -1;
    if(i<0) n->l = node_Remove(n->l , k);
    if(i>0) n->r = node_Remove(n->r , k);

    return n;
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
    //too few nodes leftward (before index i)
    if(i>true_nodes_leftward) return node_nth(n->r , i, true_nodes_leftward+1 );

    assert(false);
    return NULL;
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
    // assert(no de_uneven(n) == false);
    assert(n->key!=NULL);
    assert(n->val!=NULL);
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
    node_Valid(d->head);
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

// free's a given dictionary
// everything besides whatever the void pointers were pointing at
void TREE_Free(TREE *d){ node_Free(d->head);  free(d);  return; }

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
            d->head = node_RebalanceDirection(d->head , k);
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


// removes the key val pair associated with the inputed char* does not free whatever the void pointer was pointing to
// will free the key in the dictorary, but not the callers char *k
void TREE_Remove(TREE *d , char *k){
    if(node_Search(d->head , k)==NULL) return;
    d->size += -1;
    d->head = node_Remove(d->head, k);
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
unsigned TREE_MaxDepth(TREE *d){ return node_MaxDepth(d->head); }


void TREE_RebalanceAll(TREE *d){
    d->head = node_RebalanceALL(d->head);
    TREE_Valid(d);
    return;
}

// returns the nth member in sorted order of the dicttonary, returns NULL if invalid n
//returns tthe actual key currently being used by the dictionary
char *TREE_nth(TREE *d , unsigned n){
    if( n >= d->size ) return NULL;
    return node_nth(d->head , n, 0);
}

// returns the number of elements that come before (BUT NOT AT) the inputed char
// the given char does not have to exist in the TREE and is not freed or mutated durning runtime
unsigned TREE_Position( TREE *d , char *k ){ return node_Position(d->head, k); }
