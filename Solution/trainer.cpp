#include "trainer.h"
#include <iostream>
Trainer::Trainer(const string& name, const Pokemon& firstPokemon, const int mins[3], const int maxs[3]) {
	int i;
	for (i=0; i<name.size(); i++)
		(this->name).push_back(name[i]);

	Pokemon* pokemon = new Pokemon(firstPokemon);
	pokemons.push_back(pokemon);
	minFieldOfView[0] = mins[0];	maxFieldOfView[0] = maxs[0];
	minFieldOfView[1] = mins[1];	maxFieldOfView[1] = maxs[1];
	minFieldOfView[2] = mins[2];	maxFieldOfView[2] = maxs[2];
	index = 0;
}

Trainer::Trainer(const Trainer& trainer) {
	int i;
	for (i=0; i<trainer.name.size(); i++)
		(this->name).push_back(trainer.name[i]);

	for (i=0; i<pokemons.size(); i++) {
		Pokemon* p = new Pokemon(*(trainer.pokemons[i]));
		this->pokemons.push_back(p);
	}
	minFieldOfView[0] = trainer.minFieldOfView[0];	maxFieldOfView[0] = trainer.maxFieldOfView[0];
	minFieldOfView[1] = trainer.minFieldOfView[1];	maxFieldOfView[1] = trainer.maxFieldOfView[1];
	minFieldOfView[2] = trainer.minFieldOfView[2];	maxFieldOfView[2] = trainer.maxFieldOfView[2];
	index = trainer.index;
}

Trainer::~Trainer() {
	int i;
	for (i=0; i<pokemons.size(); i++)
		delete pokemons[i];
	pokemons.clear();
}

void Trainer::helperScanRegion(Region& region, int index, const int minBorders[3], const int maxBorders[3]) {
	int mins[3], maxs[3];
	mins[0] = minBorders[0];	maxs[0] = maxBorders[0];
	mins[1] = minBorders[1];	maxs[1] = maxBorders[1];
	mins[2] = minBorders[2];	maxs[2] = maxBorders[2];

	if (mins[0] == maxs[0] && mins[1] == maxs[1] && mins[2] == maxs[2]) {
		Pokemon& pokemon = region(mins[0], mins[1], mins[2]);
		catchPokemon(pokemon);
	}

	if (maxs[index] - mins[index] == 0) {
		index = (index+1)%3;
		if (maxs[index] - mins[index] == 0)
			index = (index+1)%3;
	}
	int border = (maxs[index] - mins[index]) / 2 + mins[index];
	int temp = maxs[index];
	maxs[index] = border;
	if (region.getPokemonCount(mins, maxs) > 0 && isLegalToScan(mins, maxs))
		helperScanRegion(region, (index+1)%3, mins, maxs);

	maxs[index] = temp;
	mins[index] = border+1;
	if (region.getPokemonCount(mins, maxs) > 0 && isLegalToScan(mins, maxs))
		helperScanRegion(region, (index+1)%3, mins, maxs);
}

void Trainer::scanRegion(Region& region) {
	int mins[3], maxs[3];
	mins[0] = region.getMinBorder('x');	maxs[0] = region.getMaxBorder('x');
	mins[1] = region.getMinBorder('y');	maxs[1] = region.getMaxBorder('y');
	mins[2] = region.getMinBorder('z');	maxs[2] = region.getMaxBorder('z');
	
	if (region.getPokemonCount(mins, maxs) > 0 && isLegalToScan(mins, maxs))
		helperScanRegion(region, 0, mins, maxs);	
}

bool Trainer::isLegalToScan(const int mins[3], const int maxs[3]) const {
	if (mins[0] > maxFieldOfView[0])
		return false;
	else if (minFieldOfView[0] > maxs[0])
		return false;
	else if (mins[1] > maxFieldOfView[1])
		return false;
	else if (minFieldOfView[1] > maxs[1])
		return false;
	else if (mins[2] > maxFieldOfView[2])
		return false;
	else if (minFieldOfView[2] > maxs[2])
		return false;
	else
		return true;
}

ostream& showPokemons(ostream& outputStream, const Trainer& trainer) {
	unsigned int i;
	for (i=0; i<trainer.pokemons.size(); i++)
		outputStream << trainer.pokemons[i]->getName() << "\n";
	return outputStream;
}
