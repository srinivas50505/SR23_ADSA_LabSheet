#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ORDER 5
#define MAX_KEYS (ORDER - 1)
#define MIN_KEYS (ORDER / 2 - 1)

typedef struct BTreeNode {
    int keys[MAX_KEYS];
    struct BTreeNode* children[ORDER];
    int numKeys;
    bool leaf;
} BTreeNode;

// Create a new B-Tree node
BTreeNode* createNode(bool leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->leaf = leaf;
    node->numKeys = 0;
    for (int i = 0; i < ORDER; i++)
        node->children[i] = NULL;
    return node;
}

// Function to search a key in B-Tree
bool search(BTreeNode* root, int key) {
    int i = 0;
    while (i < root->numKeys && key > root->keys[i])
        i++;
    if (i < root->numKeys && root->keys[i] == key)
        return true;
    if (root->leaf)
        return false;
    return search(root->children[i], key);
}

// Split child
void splitChild(BTreeNode* x, int i, BTreeNode* y) {
    BTreeNode* z = createNode(y->leaf);
    z->numKeys = MIN_KEYS;
    for (int j = 0; j < MIN_KEYS; j++)
        z->keys[j] = y->keys[j + MIN_KEYS + 1];
    if (!y->leaf) {
        for (int j = 0; j <= MIN_KEYS; j++)
            z->children[j] = y->children[j + MIN_KEYS + 1];
    }
    y->numKeys = MIN_KEYS;
    for (int j = x->numKeys; j >= i + 1; j--)
        x->children[j + 1] = x->children[j];
    x->children[i + 1] = z;
    for (int j = x->numKeys - 1; j >= i; j--)
        x->keys[j + 1] = x->keys[j];
    x->keys[i] = y->keys[MIN_KEYS];
    x->numKeys++;
}

// Insert when node is not full
void insertNonFull(BTreeNode* x, int key) {
    int i = x->numKeys - 1;
    if (x->leaf) {
        while (i >= 0 && x->keys[i] > key) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = key;
        x->numKeys++;
    } else {
        while (i >= 0 && x->keys[i] > key)
            i--;
        i++;
        if (x->children[i]->numKeys == MAX_KEYS) {
            splitChild(x, i, x->children[i]);
            if (key > x->keys[i])
                i++;
        }
        insertNonFull(x->children[i], key);
    }
}

// Insert at root
void insert(BTreeNode** rootRef, int key) {
    BTreeNode* root = *rootRef;
    if (root->numKeys == MAX_KEYS) {
        BTreeNode* s = createNode(false);
        s->children[0] = root;
        splitChild(s, 0, root);
        int i = 0;
        if (s->keys[0] < key)
            i++;
        insertNonFull(s->children[i], key);
        *rootRef = s;
    } else {
        insertNonFull(root, key);
    }
}

// Print in-order traversal
void traverse(BTreeNode* root) {
    if (root) {
        int i;
        for (i = 0; i < root->numKeys; i++) {
            if (!root->leaf)
                traverse(root->children[i]);
            printf("%d ", root->keys[i]);
        }
        if (!root->leaf)
            traverse(root->children[i]);
    }
}

// Find index of key in node
int findKey(BTreeNode* node, int key) {
    int idx = 0;
    while (idx < node->numKeys && node->keys[idx] < key)
        ++idx;
    return idx;
}

// Remove from a leaf node
void removeFromLeaf(BTreeNode* node, int idx) {
    for (int i = idx + 1; i < node->numKeys; ++i)
        node->keys[i - 1] = node->keys[i];
    node->numKeys--;
}

// Get predecessor
int getPred(BTreeNode* node, int idx) {
    BTreeNode* cur = node->children[idx];
    while (!cur->leaf)
        cur = cur->children[cur->numKeys];
    return cur->keys[cur->numKeys - 1];
}

// Get successor
int getSucc(BTreeNode* node, int idx) {
    BTreeNode* cur = node->children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];
    return cur->keys[0];
}

// Merge
void merge(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];
    child->keys[MIN_KEYS] = node->keys[idx];
    for (int i = 0; i < sibling->numKeys; ++i)
        child->keys[i + MIN_KEYS + 1] = sibling->keys[i];
    if (!child->leaf) {
        for (int i = 0; i <= sibling->numKeys; ++i)
            child->children[i + MIN_KEYS + 1] = sibling->children[i];
    }
    for (int i = idx + 1; i < node->numKeys; ++i)
        node->keys[i - 1] = node->keys[i];
    for (int i = idx + 2; i <= node->numKeys; ++i)
        node->children[i - 1] = node->children[i];
    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;
    free(sibling);
}

// Remove a key from the subtree rooted with node
void removeFromNode(BTreeNode* node, int key);

// Remove from non-leaf
void removeFromNonLeaf(BTreeNode* node, int idx) {
    int k = node->keys[idx];
    if (node->children[idx]->numKeys >= MIN_KEYS + 1) {
        int pred = getPred(node, idx);
        node->keys[idx] = pred;
        removeFromNode(node->children[idx], pred);
    } else if (node->children[idx + 1]->numKeys >= MIN_KEYS + 1) {
        int succ = getSucc(node, idx);
        node->keys[idx] = succ;
        removeFromNode(node->children[idx + 1], succ);
    } else {
        merge(node, idx);
        removeFromNode(node->children[idx], k);
    }
}

void fill(BTreeNode* node, int idx) {
    if (idx != 0 && node->children[idx - 1]->numKeys >= MIN_KEYS + 1)
        ; // borrow from prev can be added here
    else if (idx != node->numKeys && node->children[idx + 1]->numKeys >= MIN_KEYS + 1)
        ; // borrow from next can be added here
    else {
        if (idx != node->numKeys)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

void removeFromNode(BTreeNode* node, int key) {
    int idx = findKey(node, key);
    if (idx < node->numKeys && node->keys[idx] == key) {
        if (node->leaf)
            removeFromLeaf(node, idx);
        else
            removeFromNonLeaf(node, idx);
    } else {
        if (node->leaf)
            return; // The key is not present in the tree
        bool flag = ( (idx == node->numKeys) ? true : false );
        if (node->children[idx]->numKeys < MIN_KEYS + 1)
            fill(node, idx);
        if (flag && idx > node->numKeys)
            removeFromNode(node->children[idx - 1], key);
        else
            removeFromNode(node->children[idx], key);
    }
}

void deleteKey(BTreeNode** root_ref, int key) {
    if (!(*root_ref)) {
        printf("Tree is empty\n");
        return;
    }
    removeFromNode(*root_ref, key);
    if ((*root_ref)->numKeys == 0) {
        BTreeNode* tmp = *root_ref;
        if ((*root_ref)->leaf)
            *root_ref = NULL;
        else
            *root_ref = (*root_ref)->children[0];
        free(tmp);
    }
}

// Usage Example
int main() {
    BTreeNode* root = createNode(true);
    int elements[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(elements) / sizeof(elements[0]);
    for (int i = 0; i < n; i++)
        insert(&root, elements[i]);
    printf("B-Tree traversal: ");
    traverse(root);
    printf("\n");

    int keyToDelete = 6;
    printf("Deleting %d...\n", keyToDelete);
    deleteKey(&root, keyToDelete);

    printf("B-Tree after deletion: ");
    traverse(root);
    printf("\n");

    return 0;
}
