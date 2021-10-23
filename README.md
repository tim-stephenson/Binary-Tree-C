instructions:

Tree.h goes over most function with brief explanations

This is a implementation of a BST with AVl Balancing
each node in a tree stores and is sorted by a key and a value  (both stored as a void pointer for the sake of plymorphism in c)
In order for the tree knows how to compare keys, a function to compare two keys is inputted when a TREE is made
additionally functions to free keys and free value's are inputted unpon TREE initialization

Each "node" in the tree also stores Size and Height:
Storing size allowes for things such as Tree_nth and TREE_Position to be O(log n)
Storing Height allowes for AVL Balancing, which allowed the Height of the Tree to grow at O(log n), keeping Add and Delete at O( height ) (aka O(log n) ) time complexity

Implementation of AVL Balancing instructions largly came from:
https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-006-introduction-to-algorithms-fall-2011/lecture-videos/lecture-6-avl-trees-avl-sort/

Will look to add map, reduce, merge, filter in the future
