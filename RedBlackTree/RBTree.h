
#pragma once

#ifndef LIB
#define LIB __declspec(dllexport)
#endif // !LIB

#include <string>

#include "RBNode.h"

class RBTree
{

public:

    /*
    @brief
    Default constructor: init the root and the nullnode object
    */
    LIB RBTree();

    /*
    @brief
    Print the red black tree in an understandable format (recursive)
    */
    LIB void Print();

    /*
    @brief
    Insert a new node inside the rbtree, maintaining rbtree properties
    @param key: the key of the node
    @param value: the pointer to the value held by the node
    */
    LIB void Insert(size_t key, void* value);

    /*
    @brief
    Look for a node with the corresponding key value
    @param key: the key to use in the search
    */
    LIB RBNode* LookUp(size_t key);

    /*
    @brief
    Look for a node with the minimum key equal or greater than the key parameter, nullptr otherwise
    @param key: the key to use in the search
    */
    LIB RBNode* LookUpAtLeast(size_t key);

    /*
    @brief
    Search for the node with minimum key in the subtree
    @param node: the root node of the subtree
    */
    LIB RBNode* Min(RBNode* node);

    /*
    @brief
    Search for the node with maximum key in the subtree
    @param node: the root node of the subtree
    */
    LIB RBNode* Max(RBNode* node);

    /*
    Delete the node with the corresponding key value from the rbtree, maintaining rbtree properties
    @param key: the key of the node to delete
    */
    LIB void Delete(size_t key);

    LIB ~RBTree();

private:

    void RecursivePrint(const std::string& prefix, const RBNode* node, bool isLeft);

    void RBTreeInsuranceAtInsert(RBNode* newNode);

    void RBTreeInsuranceAtDelete(RBNode* node);

    void Reconstruction(RBNode* newNode);

    void Recoloring(RBNode* newNode);

    void Transplant(RBNode* oldNode, RBNode* newNode);

    void LeftRotation(RBNode* node);

    void RightRotation(RBNode* node);

    RBNode* root;
    RBNode* nullnode;

};
