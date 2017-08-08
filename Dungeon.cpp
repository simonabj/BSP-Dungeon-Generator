#include "stdafx.h"

#include "randomizer.h"
#include "BTree.h"
#include "BTree.cpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

Randomizer globalRandomizer = Randomizer(420);

enum AXIS {
	VERT, HORIZ
};

struct Point {
	double x = 0, y = 0;
	Point(double x, double y) : x(x), y(y) {};
};

int random(int min, int max) {
	double range = (max - min);
	int rnd = min + int((range * rand()) / (RAND_MAX + 1.0));
	//printf("Random num: %d!\n", rnd);
	return rnd;
};



struct Container {
	int x, y, w, h, ID;
	Point center = Point(0,0);
	Container() : x(0), y(0), w(0), h(0) {};
	Container(int x, int y, int w, int h, int ID) : x(x), y(y), w(w), h(h), ID(ID) {
		center = Point(x + (w / 2), y + (h / 2));
	};
};

struct Room : public Container {
	Room(Container container) 
		: Container(
			container.x + random(0, floor(container.w/3)), 
			container.y + random(0, floor(container.h/3)), 
			container.w, 
			container.h,
			container.ID
		) {
		w = container.w - (x - container.x);
		w -= random(0, w / 3);

		h = container.h - (y - container.y);
		h -= random(0, h / 3);
	}
};



static int iter = 0;

std::vector<Container> random_split(Container container) {
	iter++;
	std::vector<Container> children = std::vector<Container>(2);
	
	double length = container.w;
	double height = container.h;
	double size_ratio = length / height;

	///////////////////////////
	//  Calculate Direction  //
	///////////////////////////

	int cut_direction = 0;
	srand(iter);

	// Determine which side is longest, and set cut direction, with favor to longest side
	if (size_ratio > 1) // Length > height
		cut_direction = (rand() % 100 < 90) ? AXIS::VERT : AXIS::HORIZ;

	else if (size_ratio < 1) // Length < height
		cut_direction = (rand() % 100 < 90) ? AXIS::HORIZ : AXIS::VERT;

	else // Length = height
		cut_direction = (rand() % 2) ? AXIS::HORIZ : AXIS::VERT;

	//////////////////////////
	//  Calculate Position  //
	//////////////////////////


	int cut_position = 0;
	// Sets the cut position to be in a random space 20~80% of length or height

	if (cut_direction == AXIS::VERT) {
		double min = ceil(length / 20), max = floor(length - (length / 20)), range = (max - min);
		int rnd = min + int((range * rand()) / (RAND_MAX + 1.0));
		cut_position = rnd;
	}
	if (cut_direction == AXIS::HORIZ) {
		double min = ceil(height / 20), max = floor(height - (height / 20)), range = (max - min);
		int rnd = min + int((range * rand()) / (RAND_MAX + 1.0));
		cut_position = rnd;
	}


	/////////////////////
	//  Cut Container  //
	/////////////////////

	if (cut_direction == AXIS::VERT) {
			// Vertical
		children[0] = Container(
			container.x, container.y,             // r1.x, r1.y
			cut_position, container.h,   // r1.w, r1.h
			container.ID << 1
		);
		children[1] = Container(
			container.x + children[0].w, container.y,      // r2.x, r2.y
			container.w - children[0].w, container.h,       // r2.w, r2.h
			(container.ID << 1) + 1
		);
	}
	else {
		// Horizontal
		children[0] = Container(
			container.x, container.y,             // r1.x, r1.y
			container.w, cut_position,   // r1.w, r1.h
			container.ID << 1
		);
		children[1] = Container(
			container.x, container.y + children[0].h,      // r2.x, r2.y
			container.w, container.h - children[0].h,       // r2.w, r2.h
			(container.ID << 1) + 1
		);

	}
	return children;
}


void DEBUG_TREE(Node<Container> * root) {
	// do something for each node
	
	printf("Center: (%f, %f)\n", root->data.center.x, root->data.center.y);

	// Repeat
	if (root->left != NULL) // Repeat for left child
		DEBUG_TREE(root->left);
	if (root->right != NULL) // Repeat for right child
		DEBUG_TREE(root->right);
	
}

Node<Container> * split_container(Container container, int iter) {
	Node<Container> * node = new Node<Container>();
	node->key_value = container.ID;
	node->data = container;
	if (iter != 0) {
		vector<Container> sr = random_split(container);
		node->left = split_container(sr[0], iter - 1);
		node->right = split_container(sr[1], iter - 1);
	}
	return node;
}

void output_ggb(Node<Room> * root) {
	
	ofstream fs;
	fs.open("BPSTreeGGB.txt", std::ios_base::app);

	printf("Rect[(%d,%d),(%d,%d)]\n", root->data.x, root->data.y, (root->data.x + root->data.w), (root->data.y + root->data.h));
	//fs << "Rect[(" << root->data.x + "," << root->data.y + "),(" << (root->data.x + root->data.w) + "," << (root->data.y + root->data.h) << ")]\n";

	if (root->left != NULL)
		output_ggb(root->left);
	if (root->right != NULL)
		output_ggb(root->right);


	fs.flush();
	fs.close();

}


int main()
{
	int MAP_SIZE = 50;
	int N_ITERATIONS = 4;
	int PATH_SIZE = 3;
	
	//btree<Container> tree = btree<Container>(split_container(Container(0, 0, MAP_SIZE, MAP_SIZE, 0), N_ITERATIONS));

	
	//std::vector<Node<Container>*> leafs = tree.getLeafs();

	//DEBUG_TREE(tree.getRoot());
	btree<int> iTree = btree<int>();
	iTree.insert(10);
	iTree.insert(5);
	iTree.insert(3);
	iTree.insert(6);
	iTree.insert(12);
	iTree.insert(11);
	iTree.insert(15);

	printf("BiSelect: %d\n", iTree.BiSelect(0b10, 2)->key_value);

	/*
	for (int i = 0; i < leafs.size(); i++) {
		Room new_room = Room(leafs.at(i)->data);
		//printf("Rect[(%d,%d),(%d,%d)]\n", new_room.x, new_room.y, (new_room.x + new_room.w), (new_room.y + new_room.h));
	}
	*/
    return 0;
}

