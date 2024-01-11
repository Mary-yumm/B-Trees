#pragma once
#include "pch.h"
#include<string>
#include"B-Trees.h"

// Constructors
bTree::bTree() {
    order = 3;
    root = nullptr;
    flag = false;
}

bTree::bTree(int order) {
    this->order = order;
    root = nullptr;
    flag = false;
}

// Returns the root node
bTreeNode* bTree::get_root() {
    return this->root;
}

// Returns the first key in root node
string bTree::get_top()
{
    return this->root->keys[0];
}

void bTree::insert(const string& item, const string& value) {

    bTreeNode* check_if_root = nullptr;
    check_if_root = insertion(item, value, root, root, 0);
    //Check if root is updated
    if (flag == false) {
        root = check_if_root;
    }
}

void bTree::inOrder() {
    inOrder_Recursive(root);
    cout << endl;
}

Node* bTree::search(const string& searchItem, string& path) {
    bTreeNode* temp = root;
    int i = 0;
    while (temp != nullptr) {

        if (temp->keys[i] == searchItem) {
            if (temp->list[i].head == nullptr) {
                // if duplicates don't exist 
                path = temp->value[i];
                return nullptr;
            }
            else {
                return temp->list[i].head; //return the list head
            }
        }
        else if (searchItem < temp->keys[i]) {
            temp = temp->children[i]; i = 0;
        }
        else if (i + 1 == temp->keycount) {
            temp = temp->children[i + 1]; i = 0;
        }
        else if (searchItem > temp->keys[i] && searchItem < temp->keys[i + 1]) {
            temp = temp->children[i + 1];
            i = 0;
        }
        else
            i++;
        if (temp == nullptr)
            return nullptr;
        if (i == temp->keycount)
            i = 0;
    }
    return nullptr;
}

void bTree::levelOrderTraversal() {
    if (root == nullptr) {
        cout << "Tree is empty." << endl;
        return;
    }

    queue<bTreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int nodesAtCurrentLevel = q.size();

        while (nodesAtCurrentLevel > 0) {
            bTreeNode* current = q.front();
            q.pop();

            for (int i = 0; i < current->keycount; ++i) {
                if (current->list[i].head == nullptr)
                    cout << current->keys[i] << " ";
                else {
                    Node* ptr = current->list[i].head;
                    while (ptr != nullptr) {
                        cout << current->keys[i] << " " << ptr->value << ",";
                        ptr = ptr->next;
                    }
                }
            }

            for (int i = 0; i <= current->keycount; ++i) {
                if (current->children[i] != nullptr) {
                    q.push(current->children[i]);
                }
            }

            nodesAtCurrentLevel--;
        }

        cout << endl;
    }
}

void bTree::printTree() {
    print(root, 0, true);
}

void bTree::Delete(const string& item, const string& value) {
    deletion(item, value, root, root, 0, 0);
}

///////////////////////////////////////////////////////////////// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////////////////////////

bTreeNode* bTree::insertion(const string& item, const string& value, bTreeNode* ptr, bTreeNode* parent, int i) {
    // If the current node is null (empty), create a new node and insert the key
    if (ptr == nullptr) {
        bTreeNode* newnode = new bTreeNode(order);
        ptr = newnode;
        ptr->keys[0] = item;
        ptr->value[0] = value;
        ptr->keycount++;
        return ptr;
    }
    else {
        if (item == ptr->keys[i]) { // If the item is equal to the current key , handle duplicates
            if (ptr->list[i].head == nullptr) {
                ptr->list[i].insert(ptr->value[i]);
            }
            ptr->list[i].insert(value);
            return ptr;
        }
        else if (ptr->keycount == i + 1) { // if next element doesn't exist 

            if (ptr->children[i]) { // If the current node has a child, recursively insert into the appropriate child
                if (item < ptr->keys[i]) {
                    insertion(item, value, ptr->children[i], ptr, 0); //left child
                }
                else {
                    insertion(item, value, ptr->children[i + 1], ptr, 0); // right child 
                }
            }
            else { // If the node doesn't have a child, increment the key count and insert the item
                ptr->keycount++;
                Sort(ptr, item, value);
            }
        }
        else if (item < ptr->keys[i] && ptr->keycount <= order) { // If the item is less than the current key and there's space in the current node
            if (ptr->children[i]) // If the current node has a child, move to the child
            {
                insertion(item, value, ptr->children[i], ptr, 0);
            }
            else {
                // If the node doesn't have a child, increment the key count and insert the item
                ptr->keycount++;
                Sort(ptr, item, value);
            }
        }
        else if (item > ptr->keys[i]) { // If the item is greater than the current key, recursively check the next element in the keys array
            insertion(item, value, ptr, parent, i + 1); // check next element in keys array
        }

    }
    if (ptr->keycount == order) {  // If the max order is reached, perform splitting
        bTreeNode* temp = Split(ptr, parent, i);
        if (ptr == parent) { // If the current node becomes the parent after splitting, update the root
            root = temp;
            flag = true;
            return root;
        }
        // Update the current node
        ptr = temp;
        return ptr;
    }
    return ptr;

}

bTreeNode* bTree::Split(bTreeNode* ptr, bTreeNode* parent, int i) {

    // Find the middle element of the current node
    int mid = Mid(ptr->keys, ptr->keycount);
    string middle_key = ptr->keys[mid];
    string middle_val = ptr->value[mid];
    LinkedList node(ptr->list[mid]);
    // Create a new left child node
    bTreeNode* leftChild = new bTreeNode(order);
    int j = 0, k = 0;
    while (ptr->keys[k] < ptr->keys[mid]) {
        leftChild->keys[j] = ptr->keys[k];
        leftChild->value[j] = ptr->value[k];
        leftChild->list[j] = ptr->list[k];
        leftChild->children[j] = ptr->children[k];
        k++; j++;
        leftChild->keycount++;
    }
    leftChild->children[j] = ptr->children[k];
    k++;

    // Create a new right child node
    bTreeNode* rightChild = new bTreeNode(order);
    j = 0;
    while (k < ptr->keycount) {
        rightChild->keys[j] = ptr->keys[k];
        rightChild->value[j] = ptr->value[k];
        rightChild->list[j] = ptr->list[k];
        rightChild->children[j] = ptr->children[k];
        k++; j++; rightChild->keycount++;
    }
    rightChild->children[j] = ptr->children[k];

    // Remove the middle element from the current node
    remove(ptr, ptr->keys[mid]);
    //ptr->keycount--;

    if (ptr == parent) {
        // Create a new parent node and set it as the root
        bTreeNode* temp = new bTreeNode(order);
        temp->keys[0] = middle_key;
        temp->value[0] = middle_val;
        temp->list[0] = node;
        temp->keycount++;
        temp->children[0] = leftChild;
        temp->children[1] = rightChild;
        return temp;
    }
    // If the current node is not the root
    parent->keycount++;
    Sort(parent, middle_key, middle_val);
    //Find the position of middle element
    for (i = 0; i < parent->keycount; i++) {
        if (parent->keys[i] == middle_key) {
            break;
        }
    }
    // Shift right after i + 1 to make space for the right child
    j = parent->keycount;
    while (j > i + 1) {
        parent->children[j] = parent->children[j - 1];
        j--;
    }
    // Update the parent's children pointers to include the left and right children
    parent->children[i] = leftChild;
    parent->children[i + 1] = rightChild;
    return parent;
}

void bTree::inOrder_Recursive(bTreeNode* root) {

    int i;
    if (root != nullptr) {
        for (i = 0; i < root->keycount; i++) {
            inOrder_Recursive(root->children[i]);
            if (root->list[i].head != nullptr) {
                Node* ptr = root->list[i].head;
                while (ptr != nullptr) {
                    cout << root->keys[i] << " " << ptr->value << " , ";
                    ptr = ptr->next;
                }
            }
            else
                cout << root->keys[i] << " " << root->value[i] << " , ";

        }

        inOrder_Recursive(root->children[i]);
    }
}

void bTree::Sort(bTreeNode* ptr, string item, string value) {
    int i = 0;
    bool inserted = false; int index = 0;
    // Find the correct position for the new key
    while (i < ptr->keycount) {
        // If we are at the last position and the key has not been inserted, place it at the end
        if (i + 1 == ptr->keycount && !inserted) {
            ptr->keys[i] = item;
            ptr->value[i] = value;
            break;
        }
        if (ptr->keys[i] > item) {
            if (!inserted) {
                index = i;
                inserted = true;
                break;
            }
        }
        i++;
    }
    // Right-shift elements after the determined index to make space for the new key
    if (inserted) {
        i = ptr->keycount - 2;
        while (i >= index) {
            ptr->keys[i + 1] = ptr->keys[i];
            ptr->value[i + 1] = ptr->value[i];
            ptr->list[i + 1] = ptr->list[i];
            i--;
        }
        if (inserted) {
            ptr->keys[index] = item;
            ptr->value[index] = value;
            ptr->list[index].head = nullptr;
        }
    }
}

int bTree::Mid(string arr[], int size) {
    int i = 0;
    if (size % 2 != 0) //odd
        i = size / 2;
    else
        i = size / 2 - 1; //even
    return i;
}

void bTree::remove(bTreeNode* ptr, string num) {
    bool flag = false;
    int i = 0;
    for (i = 0; i < ptr->keycount - 1; i++) {
        if (ptr->keys[i] == num) {
            flag = true;
        }
        if (flag) {
            ptr->keys[i] = ptr->keys[i + 1];
            ptr->value[i] = ptr->value[i + 1];
            ptr->list[i] = ptr->list[i + 1];
        }
    }
    if (ptr->keys[i] == num)
        flag = true;
    if (i == ptr->keycount - 1 && flag == true) {
        ptr->keys[i] = "";
        ptr->value[i] = "";
        ptr->list[i].head = nullptr;
        ptr->keycount--;
    }
}

void bTree::merge(bTreeNode*& parent, bTreeNode*& ptr, int loc, string key) {
    bool left = false;
    bool right = false;

    if (loc > 0)
        left = true;
    if (loc <= parent->keycount - 1)
        right = true;

    if (left) {
        bTreeNode* merged = new bTreeNode(order);
        int j = 0; int k = 0;
        //left child
        while (k < parent->children[loc - 1]->keycount) {
            merged->keys[j] = parent->children[loc - 1]->keys[k];
            merged->value[j] = parent->children[loc - 1]->value[k];
            merged->list[j] = parent->children[loc - 1]->list[k];
            merged->children[j] = parent->children[loc - 1]->children[k];
            k++; j++;
        }
        merged->children[j] = parent->children[loc - 1]->children[k];
        //parent key
        merged->keys[j] = parent->keys[loc - 1];
        merged->value[j] = parent->value[loc - 1];
        merged->list[j] = parent->list[loc - 1];
        j++;
        k = 0;
        //right child
        while (k < ptr->keycount) {
            merged->keys[j] = parent->children[loc]->keys[k];
            merged->value[j] = parent->children[loc]->value[k];
            merged->list[j] = parent->children[loc]->list[k];
            merged->children[j] = parent->children[loc]->children[k];
            k++; j++;
        }
        merged->children[j] = parent->children[loc]->children[k];
        merged->keycount = parent->children[loc - 1]->keycount + parent->children[loc]->keycount + 1;
        remove(merged, key);
        // remove the parent key
        remove(parent, parent->keys[loc - 1]);
        k = loc;
        parent->children[loc - 1] = merged;
        ptr = parent->children[loc - 1];
        if (parent->keycount != 0) {
            while (k < parent->keycount + 1) {
                parent->children[k] = parent->children[k + 1];
                k++;
            }
            parent->children[k] = nullptr;
        }
        else
            parent->children[1] = nullptr;

    }
    else if (right) {
        bTreeNode* merged = new bTreeNode(order);
        int j = 0; int k = 0;
        //left child
        while (k < ptr->keycount) {
            merged->keys[j] = ptr->keys[k];
            merged->value[j] = ptr->value[k];
            merged->list[j] = ptr->list[k];
            merged->children[j] = ptr->children[k];
            k++; j++;
        }
        merged->children[j] = ptr->children[k];
        //parent key
        merged->keys[j] = parent->keys[loc];
        merged->value[j] = parent->value[loc];
        merged->list[j] = parent->list[loc];
        j++;
        //right child
        k = 0;
        while (k < parent->children[loc + 1]->keycount) {
            merged->keys[j] = parent->children[loc + 1]->keys[k];
            merged->value[j] = parent->children[loc + 1]->value[k];
            merged->list[j] = parent->children[loc + 1]->list[k];
            merged->children[j] = parent->children[loc + 1]->children[k];
            k++; j++;
        }
        merged->children[j] = parent->children[loc + 1]->children[k];

        merged->keycount = parent->children[loc + 1]->keycount + ptr->keycount + 1;
        remove(merged, key);
        // remove the parent key
        remove(parent, parent->keys[loc]);
        k = loc + 1;
        parent->children[loc] = merged;
        ptr = parent->children[loc];
        if (parent->keycount != 0) {
            while (k < parent->keycount + 1) {
                parent->children[k] = parent->children[k + 1];
                k++;
            }
            parent->children[k] = nullptr;
        }
        else {
            parent->children[1] = nullptr;
        }

    }
}

void bTree::print(bTreeNode* root, int indent, bool last) {
    if (root == nullptr)
        return;

    cout << setw(indent);
    if (last) {
        cout << "\\-";
        indent += 4;
    }
    else {
        cout << "|-";
        indent += 4;
    }

    for (int i = 0; i < root->keycount; i++) {
        cout << root->keys[i] << " ";
    }
    cout << endl;

    for (int i = 0; i <= root->keycount; i++) {
        print(root->children[i], indent, i == root->keycount);
    }
}

void bTree::deletion(const string& key, const string& value, bTreeNode* ptr, bTreeNode* parent, int i, int loc) {

    bool flag = false;
    bool check = false;
    if (ptr == nullptr) { // if key doesn't exist
        return;
    }
    else if (ptr->keys[i] == key) {
        if (ptr->children[i] == nullptr) {
            flag = true;
            if (ptr == root && ptr->keycount == 1) {
                root = nullptr;
                return;
            }
        }
        else {
            //Predecessor
            bool left = false;
            bool right = false;
            if (loc >= 0)
                left = true;
            if (loc <= ptr->keycount - 1)
                right = true;
            if (left) {
                bTreeNode* temp;
                temp = ptr;
                bTreeNode* temp2(ptr);
                temp2 = temp2->children[i];
                string max;
                while (temp2->children[i] != nullptr) {
                    temp2 = temp2->children[temp2->keycount];
                }
                max = temp2->keys[temp2->keycount - 1];
                string val = temp2->value[temp2->keycount - 1];
                LinkedList node(temp2->list[temp2->keycount - 1]);
                temp2->keys[temp2->keycount - 1] = temp->keys[i];
                temp2->list[temp2->keycount - 1] = temp->list[i];
                temp2->value[temp2->keycount - 1] = temp->value[i];
                temp->keys[i] = max;
                temp->list[i] = node;
                temp->value[i] = val;
                check = true;
                deletion(key, value, ptr->children[i], ptr, 0, i);
            }
            if (right && check == false) {
                //Successor
                bTreeNode* temp;
                temp = ptr;
                bTreeNode* temp2(ptr);
                temp2 = temp2->children[i + 1];
                string min;
                while (temp2->children[i] != nullptr) {
                    temp2 = temp2->children[0];
                }
                min = temp2->keys[0];
                string val = temp2->value[0];
                LinkedList node(temp2->list[0]);
                temp2->keys[0] = temp->keys[i];
                temp2->list[0] = temp->list[0];
                temp2->value[0] = temp->value[0];
                temp->keys[i] = min;
                temp->list[i] = node;
                temp->value[i] = val;
                deletion(key, value, ptr->children[i + 1], ptr, 0, i + 1);
            }
        }
    }
    else if (key < ptr->keys[i]) {
        deletion(key, value, ptr->children[i], ptr, 0, i);
    }
    else if (i + 1 == ptr->keycount) {
        deletion(key, value, ptr->children[i + 1], ptr, 0, i + 1);
    }
    else if (key > ptr->keys[i] && key < ptr->keys[i + 1]) {
        deletion(key, value, ptr->children[i + 1], ptr, 0, i + 1);
    }
    else {
        deletion(key, value, ptr, parent, i + 1, i + 1);
    }

    //if key is in leaf node
       // if leaf node contains more than min no of keys 
    if (flag == true || (ptr->keycount < (order + 1) / 2 - 1 && ptr != root)) {
        if (ptr->keycount > (order + 1) / 2 - 1 || ptr == root) {
            remove(ptr, key);
        }
        // if leaf node contains min no of keys
        else if (ptr->keycount <= (order + 1) / 2 - 1) {
            //borrow from left sibling if left sibling contains more than min keys
            bool left = false;
            bool right = false;
            bool cont = true;
            bTreeNode* location(parent);
            int j = 0;
            while (location->children[j] != ptr) {
                j++;
                if (location->children[j] == nullptr) {
                    j = loc;
                    cont = false;
                    break;
                }
            }
            loc = j;
            if (loc > 0 && cont)
                left = true;
            if (loc <= parent->keycount - 1 && cont)
                right = true;
            if (left == true && parent->children[loc - 1]->keycount > (order + 1) / 2 - 1) {
                //Transfer max value of sibling to its parent and value of parent to ptr and then remove key
                string max_c = parent->children[loc - 1]->keys[(parent->children[loc - 1]->keycount) - 1];
                string max_cv = parent->children[loc - 1]->value[(parent->children[loc - 1]->keycount) - 1];
                string max_p = parent->keys[loc - 1];
                string max_pv = parent->value[loc - 1];
                bTreeNode* right = parent->children[loc - 1]->children[parent->children[loc - 1]->keycount];
                parent->children[loc - 1]->children[parent->children[loc - 1]->keycount] = nullptr;

                remove(parent->children[loc - 1], max_c);
                remove(parent, max_p);

                parent->keycount++;
                Sort(parent, max_c, max_cv);
                ptr->keycount++;
                Sort(ptr, max_p, max_pv);
                remove(ptr, key);
                // relocate
                int j = ptr->keycount;
                while (j > 0) {
                    ptr->children[j] = ptr->children[j - 1];
                    j--;
                }
                ptr->children[0] = right;
            }

            else if (right && parent->children[loc + 1]->keycount > (order + 1) / 2 - 1) {
                //Transfer min value of sibling to its parent and value of parent to ptr and then remove key
                string min_c = parent->children[loc + 1]->keys[0];
                string min_cv = parent->children[loc + 1]->value[0];
                string min_p = parent->keys[loc];
                string min_pv = parent->value[loc];
                bTreeNode* left = parent->children[loc + 1]->children[0];

                remove(parent->children[loc + 1], min_c);
                remove(parent, min_p);

                parent->keycount++;
                Sort(parent, min_c, min_cv);
                ptr->keycount++;
                Sort(ptr, min_p, min_pv);
                remove(ptr, key);
                // relocate
                ptr->children[ptr->keycount] = left;
                int j = 0;
                while (j < parent->children[loc + 1]->keycount + 1) {
                    parent->children[loc + 1]->children[j] = parent->children[loc + 1]->children[j + 1];
                    j++;
                }
                parent->children[loc + 1]->children[j] = nullptr;
            }
            else if (cont) {
                //Merge the node with left sibling if exists
                merge(parent, ptr, loc, key);

                if (parent == root && parent->keycount == 0) {
                    root = parent->children[0];
                }

            }
        }
    }

}




