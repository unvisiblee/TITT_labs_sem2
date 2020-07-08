#pragma once
#ifndef btreelib_H
#define btreelib_H

const int t = 2;

struct BNode {
    int keys[2 * t];
    BNode* children[2 * t + 1];
    BNode* parent;
    int valuesNum;    // number of values in the node
    int childrenNum; // number of children of the node
    bool leaf;        // true if the node is a leaf

    BNode();
};


class BTree {
public:
    BTree();
    ~BTree();
    void insert(int key);
    bool search(int key);
    void remove(int key);
    void view();

private:
    BNode* root;
    void insert_to_node(int key, BNode* node);
    void sort(BNode* node);
    void restruct(BNode* node);
    void deletenode(BNode* node);
    bool searchKey(int key, BNode* node);
    void remove(int key, BNode* node);
    void removeFromNode(int key, BNode* node);
    void removeLeaf(int key, BNode* node);
    void lconnect(BNode* node, BNode* othernode);
    void rconnect(BNode* node, BNode* othernode);
    void repair(BNode* node);
};

#endif // !btreelib_H