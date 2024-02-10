#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO_FILE "/tmp/fifo_twoway"


struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TreeNode* createNode(int value) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct TreeNode* findMin(struct TreeNode* node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}


struct TreeNode* insert(struct TreeNode* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }

    return root;
}

struct TreeNode* delete(struct TreeNode* root, int value) {
    if (root == NULL) {
        return root;
    }

    if (value < root->data) {
        root->left = delete(root->left, value);
    } else if (value > root->data) {
        root->right = delete(root->right, value);
    } else {
        if (root->left == NULL) {
            struct TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        struct TreeNode* temp = findMin(root->right);

        root->data = temp->data;

        root->right = delete(root->right, temp->data);
    }

    return root;
}


void inOrderTraversal(struct TreeNode* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->data);
        inOrderTraversal(root->right);
    }
}

void destroyTree(struct TreeNode* root) {
    if (root != NULL) {
        destroyTree(root->left);
        destroyTree(root->right);
        free(root);
    }
}

int main() {
    char end[10];
    int to_end;
    int read_bytes;
    struct TreeNode* root = NULL;

    root = insert(root, 2);
    root = insert(root, 4);
    root = insert(root, 5);
    root = insert(root, 6);

    inOrderTraversal(root);
    fflush(stdout);

    int x;
    int fd = open(FIFO_FILE, O_RDWR);

    if(fd == -1){
        return 1;
    }
    
    while(1){
        if(read(fd, &x, sizeof(int)) == -1){
            return 2;
        }

        if(x != 1){
            break;
        }

        printf("Command Recieved: %d\n", x);

        fflush(stdout);
        struct TreeNode *miniNode = findMin(root);
        int mini = -1;

        if(miniNode != NULL){
            mini = miniNode->data;
        }

        printf("mini: %d \n", mini);
        root = delete(root, mini);
        fflush(stdout);
        if(write(fd, &mini, sizeof(int)) == -1){
            return 2;
        }

        sleep(2);
    }

    destroyTree(root);
    return 0;
}

