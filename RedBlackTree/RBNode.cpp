
#include "pch.h"

#include "RBNode.h"

RBNode::RBNode() {}

RBNode::RBNode(RBColor color, unsigned int key, void* value, RBNode* nullnode)
    : color(color)
    , key(key)
    , value(value)
    , parent(nullptr)
{
    this->left = nullnode;
    this->right = nullnode;
}
