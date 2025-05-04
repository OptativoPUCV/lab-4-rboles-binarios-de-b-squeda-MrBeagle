#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

// Le pregunte a chatgp como solucionar el mismo problema del lab anterior 
// y me sugirio agregar las siguientes lineas de codigo para evitar el warning
#define _CRT_SECURE_NO_WARNINGS
#pragma GCC diagnostic ignored "-Wattributes"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;

    return NULL;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if ( tree == NULL) return;
    if ( tree->root == NULL) {
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
        return;
    }
    TreeNode * current = tree->root;
    TreeNode * parent = NULL;
    while (current != NULL){
        parent = current;
        if (tree->lower_than(key, current->pair->key)){
            current = current->left;    
        } else if (tree->lower_than(current ->pair->key, key)){
            current = current->right;
        } else {
            return;
        }
    }
    TreeNode * new = createTreeNode(key, value);
    new->parent = parent;
    if (tree->lower_than(key, parent->pair->key)){
        parent->left = new;
    } else {
        parent->right = new;
    }
    tree->current = new;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;
    if (node->left == NULL && node->right == NULL){
        if (node->parent != NULL){
            if (node->parent->left == node){
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else{
            tree->root = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }
    if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
        } else {
            tree->root = child; 
        }
        child->parent = node->parent;
        free(node->pair);
        free(node);
        return;
    }
    TreeNode* minNode = minimum(node->right);
    node->pair->key = minNode->pair->key;
    node->pair->value = minNode->pair->value;
    removeNode(tree, minNode);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * current = tree->root;
    while(current != NULL){
        if (tree->lower_than(key, current->pair->key) == 0 && 
            tree->lower_than(current->pair->key, key) == 0) {
            tree->current = current;
            return current->pair;
        } else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    if (tree->current->right != NULL) {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    TreeNode * parent = tree->current->parent;
    while (parent != NULL && tree->current == parent->right) {
        tree->current = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    if (tree->current != NULL) {
        return tree->current->pair;
    }
    return NULL;
}
