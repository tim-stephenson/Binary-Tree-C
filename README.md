instructions:

Tree.h goes over most function with brief explanations

This is a implementation of a sorted binary tree with unique string sorted by the strcmp function.
Each string (aka key) also stores a void pointer (value)
Each "node" in the tree also stores Size allowing for things such as Tree_nth to be O(log n) and Tree_Size to be O(1)