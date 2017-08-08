#pragma once
/*
 * Devoloped by Simon Andreas Bjørn and Henrik Kvamme
 * API extending the XXHash32 API.
 * 
 * You are free to use this API in anyway imagenable, but do not reupload it, without permission 
 * form the origional creators!
 * Copyright(c) 2016 - Simon Andreas Bjørn and Henrik Kvamme. All rights reserved.
 */
 
 /* HOW TO USE:
  * 
  * 0. Include the randomizer.h where the object is to be used
  * 1. Create a new Randomizer object with a new seed.
  * 2. Call the functions from the new object
  * 
  * You can set new seeds with the .setSeed(uint32_t) method!
  */

#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "stdafx.h"
#include "xxHash32.h"
#include <iostream>

using namespace std;

class Randomizer {
private:
	uint32_t seed;
	XXHash32 hash;
	uint32_t result;
public:
	Randomizer::Randomizer(uint32_t seed); //initialize with seed
	Randomizer::~Randomizer();
	uint32_t Randomizer::randomizeAtPos(uint32_t x, uint32_t y); //Returns a random number for the specified position
	uint32_t Randomizer::randomizeFromKey(uint32_t key);
	uint32_t Randomizer::random();
	bool Randomizer::randomizeFromChance(int chance, uint32_t key);
	uint32_t Randomizer::randomizeInRange(uint32_t min, uint32_t max, uint32_t key);
	void Randomizer::setSeed(uint32_t seed);
	uint32_t Randomizer::getSeed();
	void Randomizer::updateSeed();
};

#endif // !RANDOMIZER_H