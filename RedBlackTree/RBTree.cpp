
#include "pch.h"

#include <iostream>

#include "RBTree.h"

#define RED "\033[0;31m"
#define WHITE "\033[0m"

#undef max

RBTree::RBTree()
{
    nullnode = new RBNode();
    nullnode->color = RBColor::Black;
    nullnode->parent = nullptr;
    nullnode->left = nullptr;
    nullnode->right = nullptr;

    this->root = nullnode;
}

void RBTree::LeftRotation(RBNode* node)
{
    RBNode* parent = node->parent;
    RBNode* right = node->right;

    node->right = right->left;
    right->left = node;
    right->parent = node->parent;
    node->parent = right;

    if (parent == nullptr)
    {
        // node is the root
        this->root = right;
    }
    else if (parent->left == node)
    {
        // node is left child
        parent->left = right;
    }
    else
    {
        // node is right child
        parent->right = right;
    }
}

void RBTree::RightRotation(RBNode* node)
{
    RBNode* parent = node->parent;
    RBNode* left = node->left;

    node->left = left->right;
    left->right = node;
    left->parent = node->parent;
    node->parent = left;

    if (parent == nullptr)
    {
        // node is the root
        this->root = left;
    }
    else if (parent->left == node)
    {
        // node is left child
        parent->left = left;
    }
    else
    {
        // node is right child
        parent->right = left;
    }
}

void RBTree::Reconstruction(RBNode* newNode)
{
    RBNode* parent = newNode->parent;
    RBNode* grandparent = parent->parent;

    // ordering nodes basing on 4 cases
    if (newNode->key < parent->key && parent->key < grandparent->key)
    {
        // first case
        this->RightRotation(grandparent);

        // recolor
        parent->color = RBColor::Black;
        grandparent->color = RBColor::Red;
    }
    else if (newNode->key > parent->key && parent->key < grandparent->key)
    {
        // second case
        this->LeftRotation(parent);
        this->RightRotation(grandparent);

        // recolor
        newNode->color = RBColor::Black;
        grandparent->color = RBColor::Red;
    }
    else if (newNode->key > parent->key && parent->key > grandparent->key)
    {
        // third case
        this->LeftRotation(grandparent);

        // recolor
        parent->color = RBColor::Black;
        grandparent->color = RBColor::Red;
    }
    else
    {
        // fourth case
        this->RightRotation(parent);
        this->LeftRotation(grandparent);

        // recolor
        newNode->color = RBColor::Black;
        grandparent->color = RBColor::Red;
    }
}

void RBTree::Recoloring(RBNode* newNode)
{
    RBNode* parent = newNode->parent;
    RBNode* grandparent = parent->parent;
    RBNode* sibling = grandparent->left == parent ? grandparent->right : grandparent->left;

    parent->color = RBColor::Black;
    sibling->color = RBColor::Black;

    while (grandparent->parent != nullptr)
    {
        // grandparent is not root

        grandparent->color = RBColor::Red;

        if (grandparent->parent->color != RBColor::Red)
            // recoloring complete
            break;

        sibling = grandparent->key > parent->key ? grandparent->right : grandparent->left;
        parent = grandparent->parent;
        grandparent = parent->parent;

        parent->color = RBColor::Black;
        sibling->color = RBColor::Black;
    }
}

void RBTree::Transplant(RBNode* oldNode, RBNode* newNode)
{
    if (oldNode->parent == nullptr)
        this->root = newNode;
    else if (oldNode->parent->left == oldNode) // old is left child
        oldNode->parent->left = newNode;
    else // old is right child
        oldNode->parent->right = newNode;

    // if (newNode != nullptr) -> now using nullnode
    newNode->parent = oldNode->parent;
}

void RBTree::RBTreeInsuranceAtInsert(RBNode* newNode)
{
    // check red property violation

    RBNode* parent = newNode->parent;

    if (parent->color == RBColor::Black)
        // no red property violation
        return;

    // red property violation -> check the sibling of new node's parent

    // take sibling by comparing new node's parent P with children of P's parent: if it is the left node, take the right; take the left otherwise
    RBNode* grandparent = parent->parent;
    RBNode* sibling = grandparent->left == parent ? grandparent->right : grandparent->left;

    if (sibling == nullnode || sibling->color == RBColor::Black)
        this->Reconstruction(newNode);
    else
        this->Recoloring(newNode);
}

void RBTree::RBTreeInsuranceAtDelete(RBNode* node)
{
    RBNode* s;
    while (node != this->root && node->color == RBColor::Black)
    {
        if (node == node->parent->left)
        {
            s = node->parent->right;
            if (s->color == RBColor::Red)
            {
                s->color = RBColor::Black;
                node->parent->color = RBColor::Red;
                this->LeftRotation(node->parent);
                s = node->parent->right;
            }

            if (s->left->color == RBColor::Black && s->right->color == RBColor::Black)
            {
                s->color = RBColor::Red;
                node = node->parent;
            }
            else
            {
                if (s->right->color == RBColor::Black)
                {
                    s->left->color = RBColor::Black;
                    s->color = RBColor::Red;
                    this->RightRotation(s);
                    s = node->parent->right;
                }

                s->color = node->parent->color;
                node->parent->color = RBColor::Black;
                s->right->color = RBColor::Black;
                this->LeftRotation(node->parent);
                node = this->root;
            }
        }
        else
        {
            s = node->parent->left;
            if (s->color == RBColor::Red)
            {
                s->color = RBColor::Black;
                node->parent->color = RBColor::Red;
                this->RightRotation(node->parent);
                s = node->parent->left;
            }

            if (s->right->color == RBColor::Black && s->right->color == RBColor::Black)
            {
                s->color = RBColor::Red;
                node = node->parent;
            }
            else
            {
                if (s->left->color == RBColor::Black)
                {
                    s->right->color = RBColor::Black;
                    s->color = RBColor::Red;
                    this->LeftRotation(s);
                    s = node->parent->left;
                }

                s->color = node->parent->color;
                node->parent->color = RBColor::Black;
                s->left->color = RBColor::Black;
                this->RightRotation(node->parent);
                node = this->root;
            }
        }
    }
    node->color = RBColor::Black;
}

RBNode* RBTree::LookUp(size_t key)
{
    RBNode* node = this->root;
    while (node != nullnode)
    {
        if (node->key == key)
        {
            // node found
            return node;
        }

        // continue the search

        if (node->key < key)
            node = node->right;
        else
            node = node->left;
    }

    // key not found
    return nullptr;
}

RBNode* RBTree::LookUp(size_t key, void* value)
{
    RBNode* node = this->root;
    while (node != nullnode)
    {
        if (node->key == key && node->value == value)
        {
            // node found
            return node;
        }

        // continue the search

        if (node->key < key)
            node = node->right;
        else
            node = node->left;
    }

    // key not found
    return nullptr;
}

RBNode* RBTree::LookUpAtLeast(size_t key)
{
    if (this->root == nullnode)
        return nullptr; // void tree

    RBNode* node = this->root;
    RBNode* chosen = nullptr;
    size_t minKey = std::numeric_limits<size_t>::max();
    while (node != nullnode)
    {
        if (node->key == key)
        {
            // node found
            return node;
        }

        if (node->key < minKey && node->key >= key)
            chosen = node;

        // continue the search

        if (node->key < key)
            node = node->right;
        else
            node = node->left;
    }

    return chosen;
}

RBNode* RBTree::Min(RBNode* node)
{
    RBNode* min = node;
    while (min->left != nullnode)
    {
        min = min->left;
    }

    return min;
}

RBNode* RBTree::Max(RBNode* node)
{
    RBNode* max = node;
    while (max->right != nullnode)
    {
        max = max->right;
    }

    return max;
}

void RBTree::Insert(size_t key, void* value)
{
    if (this->root == nullnode)
    {
        // black node as first
        this->root = new RBNode(RBColor::Black, key, value, this->nullnode);

        return;
    }

    RBNode* node = this->root;
    RBNode* newNode = nullnode;

    if (node->key < key)
        newNode = node->right;
    else
        newNode = node->left;

    // search the next leaf of the tree
    while (newNode != nullnode)
    {
        node = newNode;

        if (node->key < key)
            newNode = node->right;
        else
            newNode = node->left;
    }

    // create new node
    if (node->key < key)
    {
        node->right = new RBNode(RBColor::Red, key, value, this->nullnode);
        newNode = node->right;
    }
    else
    {
        node->left = new RBNode(RBColor::Red, key, value, this->nullnode);
        newNode = node->left;
    }

    newNode->parent = node;

    // insure red property
    this->RBTreeInsuranceAtInsert(newNode);
}

void RBTree::Delete(size_t key)
{
    RBNode* toDelete = this->LookUp(key);

    if (toDelete == nullptr)
    {
        // node not found
        return;
    }

    this->DeleteNode(toDelete);
}

void RBTree::Delete(size_t key, void* value)
{
    RBNode* toDelete = this->LookUp(key, value);

    if (toDelete == nullptr)
    {
        // node not found
        return;
    }

    this->DeleteNode(toDelete);
}

void RBTree::DeleteNode(RBNode* node)
{
    RBNode* x;
    RBNode* y;

    y = node;
    RBColor y_original_color = y->color;

    if (node->left == nullnode)
    {
        x = node->right;
        this->Transplant(node, node->right);
    }
    else if (node->right == nullnode)
    {
        x = node->left;
        this->Transplant(node, node->left);
    }
    else
    {
        y = this->Min(node->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == node)
        {
            x->parent = y;
        }
        else
        {
            this->Transplant(y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }

        this->Transplant(node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    delete node;

    if (y_original_color == RBColor::Black)
    {
        this->RBTreeInsuranceAtDelete(x);
    }
}

void RBTree::RecursivePrint(const std::string& prefix, const RBNode* node, bool isLeft)
{
    if (node != nullnode)
    {
        std::cout << prefix;

        std::cout << (isLeft ? "|--" : "'--");

        // select color
        if (node->color == RBColor::Red)
            std::cout << RED;

        // print the value of the node
        std::cout << node->key << std::endl;

        std::cout << WHITE;

        // enter the next tree level - left and right branch
        RecursivePrint(prefix + (isLeft ? "|   " : "    "), node->left, true);
        RecursivePrint(prefix + (isLeft ? "|   " : "    "), node->right, false);
    }
}

void RBTree::Print()
{
    this->RecursivePrint("", this->root, false);
}

RBTree::~RBTree()
{
    RBNode* next = this->root;
    while (next != nullnode && next != nullptr)
    {
        if (next->left != nullnode)
        {
            next = next->left;
            continue;
        }

        if (next->right != nullnode)
        {
            next = next->right;
            continue;
        }

        // right and left are null -> leaf

        // save parent
        RBNode* parent = next->parent;

        if (parent != nullptr)
        {
            // null pointers on parent
            if (next == parent->left)
                parent->left = nullnode;
            else
                parent->right = nullnode;
        }

        // delete leaf
        delete next;

        // restart from parent
        next = parent;
    }

    delete this->nullnode;
}
