#include "stdafx.h"
#include "BTree.h"
#include <iostream>

template<class T>
btree<T>::btree()
{
	root = NULL;
}

template<class T>
btree<T>::btree(Node<T> * node) 
{
	root = node;
}

template<class T>
btree<T>::~btree()
{
}

template<class T>
std::vector<Node<T>*> btree<T>::getLeafs()
{
	std::vector<Node<T>*> queue = std::vector<Node<T>*>();
	if (root != NULL)
		getLeafs(root, &queue);
	return queue;
}

template<class T>
std::vector<Node<T>*> btree<T>::getLevel(int level)
{
	return *getLevel(root, level, new std::vector<Node<T>*>);
}

template<class T>
void btree<T>::insert(int key)
{
	printf("Insert: Root");
	if (root != NULL)
		insert(key, root);
	else {
		root = new Node<T>;
		root->key_value = key;
		root->left = NULL;
		root->right = NULL;
		printf(": %d!\n", key);
	}
}

template<class T>
void btree<T>::insert(int key, T data)
{
	printf("Insert: Root");
	if (root != NULL)
		insert(key, data, root);
	else {
		root = new Node<T>;
		root->key_value = key;
		root->data = data;
		root->left = NULL;
		root->right = NULL;
		printf(": %d!\n", key);
	}
}

template <class T>
Node<T> * btree<T>::search(int key)
{
	return search(key, root);
}

template<class T>
Node<T>* btree<T>::BiSelect(int key, int depth)
{
	return BiSelect(key, depth, root);
}

template<class T>
Node<T>* btree<T>::getRoot()
{
	return root;
}

template<class T>
void btree<T>::destroy_tree()
{
	destroy_tree(root);
}

template<class T>
std::vector<Node<T>*> * btree<T>::getLeafs(Node<T> * leaf, std::vector<Node<T>*> * queue)
{
	if (leaf->left != NULL && leaf->right != NULL) {
		getLeafs(leaf->left, queue);
		getLeafs(leaf->right, queue);
	}	
	else
		queue->push_back(leaf);

	return queue;
}

template<class T>
std::vector<Node<T>*> * btree<T>::getLevel(Node<T> * leaf, int level, std::vector<Node<T>*> * queue)
{
	if (level == 1) {
		queue->push_back(leaf);
	}
	else {
		if (leaf->left != NULL)
			getLevel(leaf->left, level - 1, queue);
		if (leaf->right != NULL)
			getLevel(leaf->right, level - 1, queue);
	}
	return queue;
}

template<class T>
void btree<T>::destroy_tree(Node<T> * leaf)
{
	if (leaf != NULL) {
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}

template<class T>
void btree<T>::insert(int key, Node<T> * leaf)
{
	if (key < leaf->key_value) {
		printf(" left");
		if (leaf->left != NULL)
			insert(key, leaf->left);
		else {
			leaf->left = new Node<T>;
			leaf->left->key_value = key;
			leaf->left->parent = leaf;
			leaf->left->left = NULL;
			leaf->left->right = NULL;
			printf(": %d!\n", key);
		}
	}
	else if (key >= leaf->key_value) {
		printf(" right");
		if (leaf->right != NULL)
			insert(key, leaf->right);
		else {
			leaf->right = new Node<T>;
			leaf->right->key_value = key;
			leaf->right->parent = leaf;
			leaf->right->left = NULL;
			leaf->right->right = NULL;
			printf(": %d!\n", key);
		}
	}
}

template<class T>
void btree<T>::insert(int key, T data, Node<T>* leaf)
{
	if (key < leaf->key_value) {
		printf(" left");
		if (leaf->left != NULL)
			insert(key, data, leaf->left);
		else {
			leaf->left = new Node<T>;
			leaf->left->key_value = key;
			leaf->left->parent = leaf;
			leaf->left->data = data;
			leaf->left->left = NULL;
			leaf->left->right = NULL;
			printf(": %d!\n", key);
		}
		
	}
	else if (key >= leaf->key_value) {
		printf(" right");
		if (leaf->right != NULL)
			insert(key, leaf->right);
		else {
			leaf->right = new Node<T>;
			leaf->right->key_value = key;
			leaf->right->parent = leaf;
			leaf->right->data = data;
			leaf->right->left = NULL;
			leaf->right->right = NULL;
			printf(": %d!\n", key);
		}
		
	}
}

template<class T>
Node<T> * btree<T>::search(int key, Node<T> * leaf)
{
	if(leaf != NULL) {
		if (key == leaf->key_value)
			return leaf;
		if (key < leaf->key_value)
			return search(key, leaf->left);
		else
			return search(key, leaf->right);
	}
	else return NULL;
}

template<class T>
Node<T>* btree<T>::BiSelect(int key, int depth, Node<T>* leaf)
{
	if (leaf != NULL) {
		if (depth == 0) {
			printf("\n");
			return leaf;
		}
		if (key >> 1 % 2 == 0) {
			printf("left ");
			return BiSelect(key >> 1, depth - 1, leaf->left);
		}
		else {
			printf("right ");
			return BiSelect(key, depth - 1, leaf->right);
		}
	}
	else return NULL;
}
