instructions:

Tree.h goes over most function with brief explanations

This is a implementation of a BST with AVl Balancing
Each node in a tree stores a key and a value and is sorted based on a keys (based on a inputed key comparison function)  (both stored as a void pointer for the sake of plymorphism in c)
In order for the tree knows how to compare keys, a function to compare two keys is inputted when a TREE is made
additionally functions to free keys and free value's are also inputted unpon TREE initialization
a key comparison function should be int keyComparison(void *x, void *y)
which returns a negative number if x < y,  0 if x = y,  a postive number if x > y  

Each "node" in the tree also stores Size and Height:
Storing size allowes for things such as Tree_nth and TREE_Position to be O(log n)
Storing Height allowes for AVL Balancing, which allowed the Height of the Tree to grow at O(log n), keeping Add and Delete at O( height ) time complexity

Implementation of AVL Balancing instructions largly came from:
https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-006-introduction-to-algorithms-fall-2011/lecture-videos/lecture-6-avl-trees-avl-sort/

Will look to add map, reduce, merge, filter in the future
