#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// LIBRARY CODE ////////////////////////////////////////////////////////////////

#define MAX_CHILDREN 10

typedef struct Node {
    int value_;
    struct Node* children_[MAX_CHILDREN];
    int num_children_;
} Node;

void Node_ctor(Node *this, int value) {
    this->value_ = value;
    this->num_children_ = 0;
}

void Node_dtor(Node *this) {
    /* TODO: implement destructor such that no memory is leaked */
}

// Child node must be dynamically allocated.
// When adding a child, the parent node takes ownership of the child node.
void Node_add_child(Node *this, Node *child) {
    /* TODO: implement method */
}

Node *Node_get_child(Node *this, Node *child, int index) {
    return this->children_[index];
}


// APPLICATION CODE ////////////////////////////////////////////////////////////
// DO NOT MODIFY

int main(void) {
    Node root;
    Node_ctor(&root, 1);

    // Add child node. Root takes ownership of child1.
    Node *child1 = (Node*) malloc(sizeof(Node));
    Node_ctor(child1, 2);
    Node_add_child(&root, child1);

    // Add child node. Root takes ownership of child2.
    Node *child2 = (Node*) malloc(sizeof(Node));
    Node_ctor(child1, 2);
    Node_add_child(&root, child1);

    Node_dtor(&root);
    return 0;
}