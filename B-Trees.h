#pragma once
#ifndef BTREES_H
#define BTREES_H
#include <iostream>
#include<queue>
#include<iomanip>
using namespace std;

//Linked List class for duplicates insertion
class Node {
public:
    string value;
    Node* next;

    Node(const string& val) {
        value = val;
        next = nullptr;
    }
};

class LinkedList {
public:
    Node* head;

    LinkedList() {
        head = nullptr;
    }
    void insert(const string& val) {
        Node* newNode = new Node(val);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
};


class bTreeNode
{
public:
    LinkedList* list;
    string* keys;
    string* value;
    int order;
    bTreeNode** children;
    int keycount;

    bTreeNode(int order = 3) {
        keys = new string[order];
        value = new string[order];
        keycount = 0;
        this->order = order;
        children = new bTreeNode * [order + 1];
        for (int i = 0; i < order + 1; i++) {
            children[i] = nullptr;
        }
        list = new LinkedList[order];
        for (int i = 0; i < order; i++) {
            list[i].head = nullptr;
        }
    }

};

class bTree
{
protected:
    bTreeNode* root;
    bool flag;
    int order;
public:

    bTree();
    bTree(int order);
    Node* search(const string& searchItem, string& path);
    void insert(const string& item, const string& value);
    void inOrder();
    void levelOrderTraversal();
    void printTree();
    void Delete(const string& item, const string& value);
    void merge(bTreeNode*& parent, bTreeNode*& ptr, int loc, string key);
    bTreeNode* get_root();
    string get_top();

private:
    bTreeNode* insertion(const string& item, const string& value, bTreeNode* ptr, bTreeNode* parent, int i);
    bTreeNode* Split(bTreeNode* ptr, bTreeNode* parent, int i);
    void inOrder_Recursive(bTreeNode* root);
    void Sort(bTreeNode* ptr, string item, string value);
    int Mid(string arr[], int size);
    void remove(bTreeNode*, string num);
    void deletion(const string& key, const string& value, bTreeNode* ptr, bTreeNode* parent, int i, int loc);
    void print(bTreeNode* root, int indent, bool last);

};

#endif