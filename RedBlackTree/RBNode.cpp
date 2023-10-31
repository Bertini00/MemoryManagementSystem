
#include "pch.h"

#include "RBNode.h"

RBNode::RBNode() {}

RBNode::RBNode(RBColor color, size_t key, void* value, RBNode* nullnode)
    : color(color)
    , key(key)
    , value(value)
    , parent(nullptr)
{
    this->left = nullnode;
    this->right = nullnode;
}
