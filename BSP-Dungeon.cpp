#include "stdafx.h"
#include "randomizer.h"

#include "Cell.h"
#include "SMath.h"
#include "Room.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace SMath;

struct Map {
	int size_x = 0, size_y = 0;
	std::vector<std::vector<Cell>> map = std::vector<std::vector<Cell>>();

	Map() {}

	Map(int x, int y) :
		size_x(x), size_y(y)
	{
		map = std::vector<std::vector<Cell>>(size_x, std::vector<Cell>(size_y));
	}

	Cell * getCell(int x, int y) {
		return &(map.at(x).at(y));
	}

	Cell * getCell(Vector2D pos) {
		return &(map.at(pos.x).at(pos.y));
	}
};

struct MapRegion : Region {
	int RegionId = 0;
	int depth = 0;

	MapRegion() : Region() {}
	MapRegion(Region reg) : Region(reg) {}
	MapRegion(int depth) : Region(), depth(depth) {}
	MapRegion(Region reg, int depth) : Region(reg), depth(depth) {}
};

Map grid;
MapRegion region;

int size_x, size_y = 100;
const int min_region_size = 4;
Randomizer randomizer = Randomizer(0);

int depth = 2;

void division_loop(vector<MapRegion> RegionStack, vector<MapRegion> * OutputStack, int depth) {
	///////////////////////////
	//  Division Loop Start  //
	///////////////////////////

	printf("Starting division loop...");
	
	unsigned int iter = 0;
division_loop_start:
	iter++;
	MapRegion region = RegionStack.at(RegionStack.size() - 1);

	double length = (double)region.getSize().x;
	double height = (double)region.getSize().y;

	double size_ratio = length / height;

	///////////////////////////
	//  Calculate Direction  //
	///////////////////////////

	int cut_direction = 0;
	srand(iter);

	// Determine which side is longest, and set cut direction, with favor to longest side
	if (size_ratio > 1) // Length > height
		cut_direction = (rand() % 100 < 75) ? AXIS::VERT : AXIS::HORIZ;

	else if (size_ratio < 1) // Length < height
		cut_direction = (rand() % 100 < 75) ? AXIS::HORIZ : AXIS::VERT;

	else // Length = height
		cut_direction = (rand() % 2) ? AXIS::HORIZ : AXIS::VERT;


	//////////////////////////
	//  Calculate Position  //
	//////////////////////////

	int cut_position = 0;
	// Sets the cut position to be in a random space 20~80% of length or height

	if (cut_direction == AXIS::VERT) {
		double min = ceil(length / 10), max = floor(length - (length / 10)), range = (max - min);
		int rnd = min + int((range * rand()) / (RAND_MAX + 1.0));
		cut_position = rnd;
	}
	if (cut_direction == AXIS::HORIZ) {
		double min = ceil(height / 10), max = floor(height - (height / 10)), range = (max - min);
		int rnd = min + int((range * rand()) / (RAND_MAX + 1.0));
		cut_position = rnd;
	}

	////////////////////
	//  Split region  //
	////////////////////

	Region A1 = Region();
	Region A2 = Region();
	MapRegion B1 = MapRegion();
	MapRegion B2 = MapRegion();
	Vector2D upperLeft = Vector2D();
	Vector2D bottomRight = Vector2D();

	if (cut_direction == AXIS::VERT) {
		//printf("\nCutting: Verticaly!\n");
		// Set positions for A1
		upperLeft = Vector2D(region.min);
		bottomRight = Vector2D(region.min.x + cut_position, region.max.y);
		A1 = Region(upperLeft, bottomRight);

		// Set positions for A2
		upperLeft = Vector2D(region.min.x + cut_position + 1, region.min.y);
		bottomRight = Vector2D(region.max);
		A2 = Region(upperLeft, bottomRight);

		// Set the equivelent map regions with id and depth
		B1 = MapRegion(A1, region.depth + 1);
		B2 = MapRegion(A2, region.depth + 1);

		B1.RegionId = (region.RegionId << 1);
		B2.RegionId = (region.RegionId << 1) + 1;
	}
	else if (cut_direction == AXIS::HORIZ) {
		//printf("\nCutting: Horizontaly!\n");
		// Set positions for A1
		upperLeft = Vector2D(region.min);
		bottomRight = Vector2D(region.max.x, region.min.y + cut_position);
		A1 = Region(upperLeft, bottomRight);

		// Set positions for A2
		upperLeft = Vector2D(region.min.x, region.min.y + cut_position + 1);
		bottomRight = Vector2D(region.max);
		A2 = Region(upperLeft, bottomRight);

		// Set the equivelent map regions with id and depth
		B1 = MapRegion(A1, region.depth + 1);
		B2 = MapRegion(A2, region.depth + 1);

		B1.RegionId = (region.RegionId << 1);
		B2.RegionId = (region.RegionId << 1) + 1;
	}
	else {
		printf("What happened??!?!?!?\n");
	}

	// Check if new regions comply with size requirements
	if (
		B1.getSize().x <= min_region_size ||
		B1.getSize().y <= min_region_size ||
		B2.getSize().x <= min_region_size ||
		B2.getSize().y <= min_region_size
		) 
	{
		MapRegion DUP = MapRegion(region, region.depth + 1);
		DUP.RegionId = (DUP.RegionId << 1) + 1;

		RegionStack.pop_back();

		if (DUP.depth == depth)
			OutputStack->push_back(DUP);
		else
			RegionStack.push_back(DUP);
	} 
	else {
		// Remove used region
		//printf("Removing stack element...\n");
		RegionStack.pop_back();
		if (B1.depth == depth)
			// Region have reached given depth
			OutputStack->push_back(B1);
		else
			// Put the region back into stack for further processing
			RegionStack.push_back(B1);

		// Do the same with B2
		if (B2.depth == depth)
			OutputStack->push_back(B2);
		else
			RegionStack.push_back(B2);
	}


	if (RegionStack.size() > 0) {
		goto division_loop_start;
	}
	printf(" Done!\n");
}

void register_cell_id(vector<MapRegion> * OutputStack) {
	printf("Scanning cells...");
	int i = 0;
	vector<MapRegion>::iterator it = OutputStack->begin();
	vector<Vector2D> temp_pos = vector<Vector2D>();
	vector<Vector2D> unset_pos = vector<Vector2D>();

	for (int x = 0; x < size_x; x++) {
		for (int y = 0; y < size_y; y++) {
			unset_pos.push_back(Vector2D(x, y));
		}
	}

	printf(" Done!\n");
	printf("Registering cell ids...");
	for (; it < OutputStack->end(); it++) {
		i++;
		// Loop postitions:
		if ((depth >= 10 && i % 100 && size_x*size_y > 1000000) || (depth < 10 && i % 2))
			printf("\nCells left to set: %d...", unset_pos.size());
		 
		vector<Vector2D>::iterator pos_it = unset_pos.begin();

		for (; pos_it < unset_pos.end(); pos_it++) {
			if (pos_it->inside(*it)) {
				grid.getCell(*pos_it)->SetIdentifier(it->RegionId);
			}
			else {
				temp_pos.push_back(*pos_it);
			}
		}

		unset_pos.clear();
		unset_pos = temp_pos;
		temp_pos.clear();

	}

	printf(" Done!\n");
}

void create_rooms(vector<MapRegion> * RegionStack) {
	vector<MapRegion>::iterator it = RegionStack->begin();
	printf("Creating rooms...");
	for (; it < RegionStack->end(); it++) {
		Region room_area = Region(it->min+1, it->max-1);
		Room room = Room(room_area);
	}
	printf(" Done!\n");
}

void draw_map() {
	for (int y = 0; y < size_y; ++y) {
		for (int x = 0; x < size_y; ++x) {
			std::cout << grid.getCell(x, y)->to_c();
		}
		printf("\n");
	}
	printf("\n\n");
	/*
	for (int y = 0; y < size_y; ++y) {
	for (int x = 0; x < size_y; ++x) {
	std::cout << grid.getCell(x, y)->to_s();
	}
	printf("\n");
	}
	*/
}

void draw_rooms() {
	for (int y = 0; y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
		
			vector<Room *>::iterator it = Room::rooms.begin();

			for (; it < Room::rooms.end(); it++) {
				char temp = ' ';
				if (Vector2D(x, y).inside((*it)->area)) {
					switch ((*it)->type)
					{
					case(Room_Type::BOSS_ROOM):
						temp = 'B';
						break;
					case(Room_Type::DEFAULT_ROOM):
						temp = '-';
						break;
					case(Room_Type::TRAP_ROOM):
						temp = '!';
						break;
					case(Room_Type::TREASURE_ROOM):
						temp = 'T';
						break;
					}
				}
				std::cout << temp;
			}

		}
		std::cout << std::endl;
	}
}

void output_map(string filename, Map map, bool bDebugFile) {
	ofstream outfile;
	printf("\nPrinting map to file...");
	outfile.open(filename + ".txt", std::ios_base::trunc);

	int mapX, mapY = 0;
	for (mapY = 0; mapY < map.size_y; ++mapY) {
		for (mapX = 0; mapX < map.size_x; mapX++) {
			outfile << map.getCell(mapX, mapY)->to_c();
		}
		outfile << '\n';
	}
	printf(" Done!\n");
	outfile.flush();
	outfile.close();

	if (bDebugFile) {
		outfile.open(filename + "_debug" + ".txt", std::ios_base::trunc);
		int mapX, mapY = 0;
		printf("Printing map data to file...");
		for (mapY = 0; mapY < map.size_y; ++mapY) {
			for (mapX = 0; mapX < map.size_x; mapX++) {
				outfile << map.getCell(mapX, mapY)->to_s(depth);
			}
			outfile << '\n';
		}

		outfile.flush();
		outfile.close();
	}
	printf(" Done!\n");

}

void output_to_GeoGebra(vector<MapRegion> OutputStack) {
	vector<MapRegion>::iterator it = OutputStack.begin();
	printf("_________________________________________________\n");
	printf("Copy the folowing code into a button in GeoGebra: \n\n");
	for (; it < OutputStack.end(); ++it) {
		printf("Rect[(%d, %d),(%d, %d)]\n", it->min.x, it->min.y, it->max.x, it->max.y);
	}
	printf("\n_________________________________________________\n");
}

void output_to_GeoGebra(vector<MapRegion> OutputStack, string filename) {
	cout << "Pringting GeoGebra script to " << filename << ".txt...  ";
	ofstream fs;
	fs.open(filename + ".txt", std::ios_base::trunc);

	vector<MapRegion>::iterator it = OutputStack.begin();
	for (; it < OutputStack.end(); ++it) {
		string ggbCommand = "Rect[(";
			ggbCommand += it->min.x + ",";
			ggbCommand += it->min.y + "),(";
			ggbCommand += it->max.x + ",";
			ggbCommand += it->max.y + ")]\n";

			fs << "Rect[(" << it->min.x << "," << it->min.y << "),(" << it->max.x << "," << it->max.y << ")]\n";
	}
	fs.flush();
	fs.close();
	printf("Done!\n\n");
}

int main()
{
	/////////////////////
	//  Program start  //
	/////////////////////

	int seed = 39268;
	randomizer.setSeed(seed);
	vector<MapRegion> RegionStack = vector<MapRegion>();
	vector<MapRegion> OutputStack = vector<MapRegion>();

	printf("Please enter width: ");
	scanf_s("%d", &size_x);
	printf("Please enter height: ");
	scanf_s("%d", &size_y);
	
	printf("The map size is %dx%d, and contains %d cells!\n", size_x, size_y, size_x*size_y);

	printf("Please enter depth: ");
	scanf_s("%d", &depth);

	if (depth >= 10) {
		printf("\nWith depth %d, there will be created %d regions, and will take some time...\nAre you sure you want to continue? (y/N)", depth, (int)pow(2,depth));
		string conf;
		std::cin >> conf;

		if(conf.at(0) != 'y' && conf.at(0) != 'Y') {
			printf("Aborted!\n");
			return 1;
		}
	}

	//////////////////////
	//  Initialize Map  //
	//////////////////////

	grid = Map(size_x, size_y);
	region = MapRegion(Region(Vector2D(0, 0), Vector2D(size_x, size_y)));
	RegionStack.push_back(region);
	printf("\nMap created!\n");

	/////////////////////
	//  Division Loop  //
	/////////////////////

	division_loop(RegionStack, &OutputStack, depth);

	////////////////////////
	//  Apply IDs to Map  //
	////////////////////////

	register_cell_id(&OutputStack);

	///////////////////////
	//  Construct rooms  //
	///////////////////////

	create_rooms(&OutputStack);

	///////////////
	// Draw Map  //
	///////////////
	
	//draw_map();
	//output_map("map", grid, false);
	//output_to_GeoGebra(OutputStack);
	//output_to_GeoGebra(OutputStack, "ggb");

	// Debug

	//fix_regions(&OutputStack);

	return 0;
}