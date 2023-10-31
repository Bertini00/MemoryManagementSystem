#pragma once

#ifndef LIB
#define LIB __declspec(dllexport)
#endif // !LIB

enum RBColor { Red, Black };

class RBNode
{

public:

    LIB RBNode();

    /*
    @brief
    Constructor of a red black node
    @param color: the color of the node (red or black)
    @param key: the value used to perform operations on the node inside the rbtree: ordering, insert, delete, lookup, ...
    @param value: the pointer to the value held by the node
    @param nullnode: the pointer to an object representing a null node (null pointer by default)
    */
    LIB RBNode(RBColor color, size_t key, void* value, RBNode* nullnode = nullptr);

    RBNode* left;
    RBNode* right;
    RBNode* parent;
    RBColor color;
    size_t key;
    void* value;

};
