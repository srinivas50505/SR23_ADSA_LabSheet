#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key, height;
    struct Node *left, *right;
};

int height(struct Node *n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

struct Node* create(int key) {
    struct Node* n = malloc(sizeof(struct Node));
    n->key = key; n->left = n->right = NULL; n->height = 1;
    return n;
}

struct Node* rightRotate(struct Node *y) {
    struct Node *x = y->left;
    y->left = x->right; x->right = y;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

struct Node* leftRotate(struct Node *x) {
    struct Node *y = x->right;
    x->right = y->left; y->left = x;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(struct Node *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

// Insert node
struct Node* insert(struct Node *root, int key) {
    if (!root) return create(key);
    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    // LL & RR rotations
    if (balance > 1 && key < root->left->key) return rightRotate(root);
    if (balance < -1 && key > root->right->key) return leftRotate(root);
    // LR & RL rotations
    if (balance > 1 && key > root->left->key) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && key < root->right->key) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// Inorder traversal
void inorder(struct Node* root, FILE* fp) {
    if (root) {
        inorder(root->left, fp);
        fprintf(fp, "%d ", root->key);
        inorder(root->right, fp);
    }
}

// Main program
int main() {
    struct Node* root = NULL;
    FILE *fin = fopen("input.txt", "r");
    int val;

    while (fscanf(fin, "%d", &val) != EOF)
        root = insert(root, val);
    fclose(fin);

    // Write inorder to output
    FILE *fout = fopen("output.txt", "w");
    inorder(root, fout);
    fclose(fout);

    return 0;
}
