instructions:

Tree.h goes over most function with brief explanations

This is a implementation of a BST with AVl Balancing
each node in a tree stores and is sorted by a char list/string (aka key) and a void pointer to whatever data the cleint disires
comparisons of char lists are done via strcmp()

Each "node" in the tree also stores Size and Height:
Storing size allowes for things such as Tree_nth and TREE_Position to be O(log n)
Storing Height allowes for AVL Balancing

Implementation of AVL Balancing instructions largly came from:
https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-006-introduction-to-algorithms-fall-2011/lecture-videos/lecture-6-avl-trees-avl-sort/
