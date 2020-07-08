#include "btreelib.h"
#include <iostream>
#include <string>
#include <queue>
using namespace std;

BNode::BNode() {};

BTree::BTree() { root = nullptr; }


BTree::~BTree() {  
    if (root != nullptr) {
        deletenode(root); // delete all info before destroying an instance
    }
}


void BTree::deletenode(BNode* node) {
    if (node != nullptr) {
        for (int i = 0; i <= (2 * t - 1); i++) { // delete all children of the given root, using recursion
            if (node->children[i] != nullptr)
                deletenode(node->children[i]);
            else {
                delete(node);
                break;
            }
        }
    }
}


void BTree::insert_to_node(int key, BNode* node) {
    node->keys[node->valuesNum] = key;
    node->valuesNum += 1;
    sort(node);
}

void BTree::sort(BNode* node) {
    int m;
    for (int i = 0; i < (2 * t - 1); i++) {
        for (int j = i + 1; j <= (2 * t - 1); j++) {
            if ((node->keys[i] != 0) && (node->keys[j] != 0)) {
                if ((node->keys[i]) > (node->keys[j])) {
                    m = node->keys[i];
                    node->keys[i] = node->keys[j];
                    node->keys[j] = m;
                }
            }
            else break;
        }
    }
}



void BTree::insert(int key) {
    if (search(key) == true)
        return;
    if (root == nullptr) { // simply create root if tree is empty
        BNode* newRoot = new BNode;
        newRoot->keys[0] = key;
        for (int j = 1; j <= (2 * t - 1); j++)
            newRoot->keys[j] = 0;
        for (int i = 0; i <= 2 * t; i++)
            newRoot->children[i] = nullptr;
        newRoot->valuesNum = 1;
        newRoot->childrenNum = 0;
        newRoot->leaf = true;
        newRoot->parent = nullptr;
        root = newRoot;
    }
    else {               // go to children untill we get a  leaf
        BNode* ptr = root;
        while (ptr->leaf == false) { // ptr->leaf == true
            for (int i = 0; i <= (2 * t - 1); i++) { // checking all keys
                if (ptr->keys[i] != 0) {
                    if (key < ptr->keys[i]) {
                        ptr = ptr->children[i];
                        break;
                    }
                    if ((ptr->keys[i + 1] == 0) && (key > ptr->keys[i])) {
                        ptr = ptr->children[i + 1]; // go to the last child if key wasn't found
                        break;
                    }
                }
                else break;
            }
        }
        insert_to_node(key, ptr);

        while (ptr->valuesNum == 2 * t) {
            if (ptr == root) {
                restruct(ptr);
                break;
            }
            else {
                BNode* restr = ptr;
                ptr = ptr->parent;
                restruct(restr);
            }
        }
    }
}

void BTree::restruct(BNode* node) {
    if (node->valuesNum < 2 * t - 1)
        return;

    BNode* child1 = new BNode; // new left node
    int j;
    for (j = 0; j <= t - 2; j++)
        child1->keys[j] = node->keys[j];
    for (j = t - 1; j <= (2 * t - 1); j++)
        child1->keys[j] = 0;
    child1->valuesNum = t - 1;
    if (node->childrenNum != 0) {
        for (int i = 0; i <= (t - 1); i++) {
            child1->children[i] = node->children[i];
            child1->children[i]->parent = child1;
        }
        for (int i = t; i <= (2 * t); i++)
            child1->children[i] = nullptr;
        child1->leaf = false;
        child1->childrenNum = t - 1;
    }
    else {
        child1->leaf = true;
        child1->childrenNum = 0; 
        for (int i = 0; i <= (2 * t); i++)
            child1->children[i] = nullptr;
    }

    BNode* child2 = new BNode;  // new right node
    for (int j = 0; j <= (t - 1); j++)
        child2->keys[j] = node->keys[j + t];
    for (int j = t; j <= (2 * t - 1); j++)
        child2->keys[j] = 0;
    child2->valuesNum = t;
    if (node->childrenNum != 0) {
        for (int i = 0; i <= t; i++) {
            child2->children[i] = node->children[i + t];
            child2->children[i]->parent = child2;
        }
        for (int i = t + 1; i <= (2 * t); i++)
            child2->children[i] = nullptr;
        child2->leaf = false;
        child2->childrenNum = t;
    }
    else {
        child2->leaf = true;
        child2->childrenNum = 0;
        for (int i = 0; i <= (2 * t); i++)
            child2->children[i] = nullptr;
    }


    if (node->parent == nullptr) { // new parent
        node->keys[0] = node->keys[t - 1];
        for (int j = 1; j <= (2 * t - 1); j++)
            node->keys[j] = 0;
        node->children[0] = child1;
        node->children[1] = child2;
        for (int i = 2; i <= (2 * t); i++)
            node->children[i] = nullptr;
        node->parent = nullptr;
        node->leaf = false;
        node->valuesNum = 1;
        node->childrenNum = 2;
        child1->parent = node;
        child2->parent = node;
    }
    else {
        insert_to_node(node->keys[t - 1], node->parent);
        for (int i = 0; i <= (2 * t); i++)
            if (node->parent->children[i] == node)
                node->parent->children[i] = nullptr;
        for (int i = 0; i <= (2 * t); i++) {
            if (node->parent->children[i] == nullptr) {
                for (int j = (2 * t); j > (i + 1); j--)
                    node->parent->children[j] = node->parent->children[j - 1];
                node->parent->children[i + 1] = child2;
                node->parent->children[i] = child1;
                break;
            }
        }
        child1->parent = node->parent;
        child2->parent = node->parent;
        node->parent->leaf = false;
        delete node;
    }
}

bool BTree::search(int key) {
    return searchKey(key, this->root);
}

bool BTree::searchKey(int key, BNode* node) {
    if (node != nullptr) {
        if (node->leaf == false) {
            int i = 0;
            for (i = 0; i <= (2 * t - 1); i++) {
                if (node->keys[i] != 0) {
                    if (node->keys[i] == key)
                        return true;
                    if (node->keys[i] > key) {
                        return searchKey(key, node->children[i]);
                        break;
                    }
                }
                else break;
            }
            return searchKey(key, node->children[i]);
        }
        else {
            for (int j = 0; j <= (2 * t - 1); j++)
                if (key == node->keys[j]) return true;
            return false;
        }
    }
    else return false;
}

void BTree::removeFromNode(int key, BNode* node) {
    for (int i = 0; i < node->valuesNum; i++) {
        if (node->keys[i] == key) {
            for (int j = i; j < node->valuesNum; j++) {
                node->keys[j] = node->keys[j + 1];
                node->children[j] = node->children[j + 1];
            }
            node->keys[node->valuesNum - 1] = 0;
            node->children[node->valuesNum - 1] = node->children[node->valuesNum];
            node->children[node->valuesNum] = nullptr;
            break;
        }
    }
    node->valuesNum--;
}


void BTree::lconnect(BNode* node, BNode* othernode) {
    if (node == nullptr) 
        return;
    for (int i = 0; i <= (othernode->valuesNum - 1); i++) {
        node->keys[node->valuesNum] = othernode->keys[i];
        node->children[node->valuesNum] = othernode->children[i];
        node->valuesNum++;
    }
    node->children[node->valuesNum] = othernode->children[othernode->valuesNum];
    for (int j = 0; j <= node->valuesNum; j++) {
        if (node->children[j] == nullptr)
            break;
        node->children[j]->parent = node;
    }
    delete othernode;
}

void BTree::rconnect(BNode* node, BNode* othernode) {
    if (node == nullptr)
        return;
    for (int i = 0; i <= (othernode->valuesNum - 1); i++) {
        node->keys[node->valuesNum] = othernode->keys[i];
        node->children[node->valuesNum + 1] = othernode->children[i + 1];
        node->valuesNum++;
    }
    for (int j = 0; j <= node->valuesNum; j++) {
        if (node->children[j] == nullptr)
            break;
        node->children[j]->parent = node;
    }
    delete othernode;
}

void BTree::repair(BNode* node) {
    if ((node == root) && (node->valuesNum == 0)) {
        if (root->children[0] != nullptr) {
            root->children[0]->parent = nullptr;
            root = root->children[0];
        }
        else {
            delete root;
        }
        return;
    }
    BNode* ptr = node;
    int k1;
    int k2;
    int positionChild;
    BNode* parent = ptr->parent;
    for (int j = 0; j <= parent->valuesNum; j++) {
        if (parent->children[j] == ptr) {
            positionChild = j; // position of node towards the parent
            break;
        }
    }
    // if our node is the FIRST child:
    if (positionChild == 0) {
        insert_to_node(parent->keys[positionChild], ptr);
        lconnect(ptr, parent->children[positionChild + 1]);
        parent->children[positionChild + 1] = ptr;
        parent->children[positionChild] = nullptr;
        removeFromNode(parent->keys[positionChild], parent);
        if (ptr->valuesNum == 2 * t) {
            while (ptr->valuesNum == 2 * t) {
                if (ptr == root) {
                    restruct(ptr);
                    break;
                }
                else {
                    restruct(ptr);
                    ptr = ptr->parent;
                }
            }
        }
        else
            if (parent->valuesNum <= (t - 2))
                repair(parent);
    }
    else {
        // if our node is the LAST child:
        if (positionChild == parent->valuesNum) {
            insert_to_node(parent->keys[positionChild - 1], parent->children[positionChild - 1]);
            lconnect(parent->children[positionChild - 1], ptr);
            parent->children[positionChild] = parent->children[positionChild - 1];
            parent->children[positionChild - 1] = nullptr;
            removeFromNode(parent->keys[positionChild - 1], parent);
            BNode* temp = parent->children[positionChild];
            if (ptr->valuesNum == 2 * t) {
                while (temp->valuesNum == 2 * t) {
                    if (temp == root) {
                        restruct(temp);
                        break;
                    }
                    else {
                        restruct(temp);
                        temp = temp->parent;
                    }
                }
            }
            else
                if (parent->valuesNum <= (t - 2))
                    repair(parent);
        }
        else { // if node is neither FIRST or LAST child
            insert_to_node(parent->keys[positionChild], ptr);
            lconnect(ptr, parent->children[positionChild + 1]);
            parent->children[positionChild + 1] = ptr;
            parent->children[positionChild] = nullptr;
            removeFromNode(parent->keys[positionChild], parent);
            if (ptr->valuesNum == 2 * t) {
                while (ptr->valuesNum == 2 * t) {
                    if (ptr == root) {
                        restruct(ptr);
                        break;
                    }
                    else {
                        restruct(ptr);
                        ptr = ptr->parent;
                    }
                }
            }
            else
                if (parent->valuesNum <= (t - 2))
                    repair(parent);
        }
    }
}

void BTree::removeLeaf(int key, BNode* node) {
    if ((node == root) && (node->valuesNum == 1)) {
        removeFromNode(key, node);
        root->children[0] = nullptr;
        delete root;
        root = nullptr;
        return;
    }
    if (node == root) {
        removeFromNode(key, node);
        return;
    }
    if (node->valuesNum > (t - 1)) {
        removeFromNode(key, node);
        return;
    }
    BNode* ptr = node;
    int k1;
    int k2;
    int position;
    int positionChild;
    int i;
    for (int i = 0; i <= node->valuesNum - 1; i++) {
        if (key == node->keys[i]) {
            position = i; // saving the position of the key in the node
            break;
        }
    }
    BNode* parent = ptr->parent;
    for (int j = 0; j <= parent->valuesNum; j++) {
        if (parent->children[j] == ptr) {
            positionChild = j;
            break;
        }
    }
    // if node is the FIRST child:
    if (positionChild == 0) {
        if (parent->children[positionChild + 1]->valuesNum > (t - 1)) { // if the right brother has more than (t - 1) keys:
            k1 = parent->children[positionChild + 1]->keys[0]; // k1 - the min key in the right brother
            k2 = parent->keys[positionChild]; // k2 less than k1, but greater than the value to be deleted
            insert_to_node(k2, ptr);
            removeFromNode(key, ptr);
            parent->keys[positionChild] = k1; // swap k1 and k2
            removeFromNode(k1, parent->children[positionChild + 1]); // delete k1 from the right brother
        }
        else { // if the right brother has equal or less than (t -1) keys:
            removeFromNode(key, ptr);
            if (ptr->valuesNum <= (t - 2))
                repair(ptr);
        }
    }
    else { // if the child is the LAST child:
        if (positionChild == parent->valuesNum) {
            if (parent->children[positionChild - 1]->valuesNum > (t - 1)) { // if left brother has more than (t - 1) keys:
                BNode* temp = parent->children[positionChild - 1];
                k1 = temp->keys[temp->valuesNum - 1]; // k1 is the max key of the left brother
                k2 = parent->keys[positionChild - 1]; // k2 is the key of parent that is less than the one to be deleted and greater than k1
                insert_to_node(k2, ptr);
                removeFromNode(key, ptr);
                parent->keys[positionChild - 1] = k1;
                removeFromNode(k1, temp);
            }
            else { // if left brother has <= (t -1) keys
                removeFromNode(key, ptr);
                if (ptr->valuesNum <= (t - 2))
                    repair(ptr);
            }
        }
        else { // if the node is neither FIRST or LAST child:
            if (parent->children[positionChild + 1]->valuesNum > (t - 1)) { // if right brother has more than (t - 1) keys
                k1 = parent->children[positionChild + 1]->keys[0]; // k1 - min key of the right brother
                k2 = parent->keys[positionChild]; // k2 - parent's key,  key < k2 < k1
                insert_to_node(k2, ptr);
                removeFromNode(key, ptr);
                parent->keys[positionChild] = k1;
                removeFromNode(k1, parent->children[positionChild + 1]);
            }
            else { // if left brother has more than (t - 1) keys:
                if (parent->children[positionChild - 1]->valuesNum > (t - 1)) {
                    BNode* temp = parent->children[positionChild - 1];
                    k1 = temp->keys[temp->valuesNum - 1]; // k1 - max key of left brother
                    k2 = parent->keys[positionChild - 1]; // k1 < k2 < key
                    insert_to_node(k2, ptr);
                    removeFromNode(key, ptr);
                    parent->keys[positionChild - 1] = k1;
                    removeFromNode(k1, temp);
                }
                else { // if both brothers have equal or less than (t-1) keys
                    removeFromNode(key, ptr);
                    if (ptr->valuesNum <= (t - 2))
                        repair(ptr);
                }
            }
        }
    }
}

void BTree::remove(int key, BNode* node) {
    BNode* ptr = node;
    int position; // number of key
    int i;
    for (int i = 0; i <= node->valuesNum - 1; i++) {
        if (key == node->keys[i]) {
            position = i;
            break;
        }
    }
    int positionChild; // number of child towards parent
    if (ptr->parent != nullptr) {
        for (int i = 0; i <= ptr->parent->valuesNum; i++) {
            if (ptr->parent->children[i] == ptr) {
                positionChild = i;
                break;
            }
        }
    }
    ptr = ptr->children[position + 1]; // find the min key of right subtree
    int newkey = ptr->keys[0]; 
    while (ptr->leaf == false)
        ptr = ptr->children[0];
    // if there is only 1 key in found leaf, search for max key in left subtree
    // else - change our key and the found key and delete the key from leaf
    if (ptr->valuesNum > (t - 1)) {
        newkey = ptr->keys[0];
        removeFromNode(newkey, ptr);
        node->keys[position] = newkey;
    }
    else {
        ptr = node;
        ptr = ptr->children[position]; // find the max key in left subtree
        newkey = ptr->keys[ptr->valuesNum - 1];
        while (ptr->leaf == false)
            ptr = ptr->children[ptr->valuesNum];
        newkey = ptr->keys[ptr->valuesNum - 1];
        node->keys[position] = newkey;
        if (ptr->valuesNum > (t - 1))
            removeFromNode(newkey, ptr);
        else { // if there is only 1 key in leaf - restruct it
            removeLeaf(newkey, ptr);
        }
    }
}

void BTree::remove(int key) {
    BNode* ptr = this->root;
    int position;
    int positionChild;
    int i;
    if (searchKey(key, ptr) == false) {
        return;
    }
    else {
        for (i = 0; i <= ptr->valuesNum - 1; i++) { // searching for node where the key is
            if (ptr->keys[i] != 0) {
                if (key == ptr->keys[i]) {
                    position = i;
                    break;
                }
                else {
                    if ((key < ptr->keys[i])) {
                        ptr = ptr->children[i];
                        positionChild = i;
                        i = -1;
                    }
                    else {
                        if (i == (ptr->valuesNum - 1)) {
                            ptr = ptr->children[i + 1];
                            positionChild = i + 1;
                            i = -1;
                        }
                    }
                }
            }
            else
                break;
        }
    }
    if (ptr->leaf == true) {
        if (ptr->valuesNum > (t - 1))
            removeFromNode(key, ptr);
        else
            removeLeaf(key, ptr);
    }
    else remove(key, ptr);
}



void BTree::view() {
    if (root == nullptr)
        return;
    queue <BNode*> queue;
    queue.push(root);
    while (!queue.empty()) {
        BNode* current = queue.front();
        int childrencount = queue.size();
        queue.pop();
        cout << '\n';
        for (int j = 0; j < childrencount; j++) {
            if (j != 0) {
                current = queue.front();
                queue.pop();
            }
            for (int i = 0; i < current->valuesNum; i++) {
                cout << to_string(current->keys[i]);
                if (current->children[i])
                    queue.push(current->children[i]);
                cout << ",";
            }
            cout << "||";
            if(current->children[current->valuesNum])
                queue.push(current->children[current->valuesNum]);
        }
    }
    cout << '\n';
}