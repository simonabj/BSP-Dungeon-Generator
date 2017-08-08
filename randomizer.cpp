#include "stdafx.h"
#include "randomizer.h"
#include "xxHash32.h"
#include <iostream>

using namespace std;

Randomizer::Randomizer(uint32_t seed): hash(seed) {
	this->seed = seed;
}
Randomizer::~Randomizer() {
}
uint32_t Randomizer::randomizeAtPos(uint32_t x, uint32_t y) {
	this->hash.add(&x, 4);
	this->hash.add(&y, 4);
	this->result = this->hash.hash();
	this->updateSeed(); //used to undo add()
	return this->result;
}
//Used for the global randomizer only
uint32_t Randomizer::randomizeFromKey(uint32_t key){
	this->hash.add(&key, 4);
	this->result = this->hash.hash();
	this->updateSeed(); //used to undo add()
	return this->result;
}

uint32_t Randomizer::random() {
	this->hash.add(nullptr, 4);
	this->result = this->hash.hash();
	this->updateSeed();
	return this->result;
}

//int chance is the percentage chance of returning 1.
bool Randomizer::randomizeFromChance(int chance, uint32_t key){
	this->hash.add(&key, 4);
	this->result = this->hash.hash();
	this->updateSeed();
	result %= 100;
	if (result < chance) return 1;
	else return 0;
	cout << result % 2 << endl;
}
uint32_t Randomizer::randomizeInRange(uint32_t min, uint32_t max, uint32_t key) {
	return (this->randomizeFromKey(key) + min) % max;
}
void Randomizer::setSeed(uint32_t seed) {
	this->seed = seed;
	this->updateSeed();
}
uint32_t Randomizer::getSeed() {
	return this->seed;
}
void Randomizer::updateSeed() {
	this->hash = XXHash32(this->seed);
}
