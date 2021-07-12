// Timothy Stephenson June 14th 2021

// Interface / Specifications for binary Tree

// all keys must be "strings", in C this means a '\0' terminated char* must end them
// all values must be void pointers 

// max number of elements in a given TREE is the max value of a unsigned int (4,294,967,295)
// though I wouldn't recommend going past a few million



struct TREE;

typedef struct TREE TREE;




// returns a empty Tree
TREE* TREE_Empty();


// Free's a given Tree
// will NOT free any underlying structures the void pointers were pointing at
void TREE_Free(TREE*);

// adds a given key val pair, with the char* being the key and the void* being the val
// if a the char already exists, the val (void pointer) will be overwritten and it is the callers responsibility to ensure this overwrite does not create memory leaks
// uses the callers char *k in the TREE (DO NOT FREE IT AFTER CALLING)
void TREE_Add(TREE* , char* , void*);

// returns the void pointer associated with the given char* in the TREE if it exists, returns NULL if it doesn't
// does not free in inputted char *k
void* TREE_Search(TREE* , char*);

// removes the key val pair associated with the inputted char* does not free whatever the void pointer was pointing to
// will free the key in the tree, but not the callers char *k
void TREE_Remove(TREE* , char*);

// returns the number of elements in the given TREE
unsigned TREE_Size(TREE*);

// returns a sorted list of the keys in the given TREE
char** TREE_KeyList(TREE*);

// returns the average depth of all nodes
double TREE_AverageDepth(TREE*);

// returns the max depth of a given node
unsigned TREE_Height(TREE*);

// returns the nth member in sorted order of the TREE, returns NULL if invalid n
//returns the actual key currently being used by the TREE
char *TREE_nth(TREE * , unsigned);

//returns the number of elements that come before (BUT NOT AT) the inputted char
// the given char does not have to exist in the TREE and is not freed or mutated during runtime
// ie: if the inputted char is in the TREE, it is returning the index
//     if the inputted char is NOT in the TREE, returns numbers of elements that come before
unsigned TREE_Position( TREE * , char * );


