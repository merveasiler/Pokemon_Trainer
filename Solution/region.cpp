#include "region.h"

Region::Region(const int corner1[3], const int corner2[3]) {
	minx = corner1[0];
	miny = corner1[1];
	minz = corner1[2];

	maxx = corner2[0];
	maxy = corner2[1];
	maxz = corner2[2];

	firstHalf = NULL;
	secondHalf = NULL;
	parent = NULL;
	pokemon = NULL;
	divisionCoordinate = 'x';
	numberOfPokemons = 0;
}

Region::Region(const Region& region) {
	this->minx = region.minx;
	this->miny = region.miny;
	this->minz = region.minz;
	this->maxx = region.maxx;
	this->maxy = region.maxy;
	this->maxz = region.maxz;
	this->parent = NULL;

	if (region.firstHalf) {
		this->firstHalf = new Region( *(region.firstHalf) );
		this->firstHalf->parent = this;
	}
	else
		this->firstHalf = NULL;
	if (region.secondHalf) {
		this->secondHalf = new Region( *(region.secondHalf) );
		this->secondHalf->parent = this;
	}
	else
		this->secondHalf = NULL;
	if (region.pokemon)
		this->pokemon = new Pokemon(*(region.pokemon));
	else
		this->pokemon = NULL;
	numberOfPokemons = region.numberOfPokemons;
	divisionCoordinate = region.divisionCoordinate;
}

void Region::deleteRegion() {
	if (this->firstHalf != NULL) {
		this->firstHalf->deleteRegion();
		delete this->firstHalf;
		this->firstHalf = NULL;
	}
	if (this->secondHalf != NULL) {
		this->secondHalf->deleteRegion();
		delete this->secondHalf;
		this->secondHalf = NULL;
	}
	if (this->parent != NULL)
		this->parent = NULL;
	if (this->pokemon != NULL) {
		delete this->pokemon;
		this->pokemon = NULL;
	}
}

Region::~Region() {
	deleteRegion();
}

int Region::getMinBorder(char type) const {
	if (type == 'x')
		return minx;
	if (type == 'y')
		return miny;
	if (type == 'z')
		return minz;
}

int Region::getMaxBorder(char type) const {
	if (type == 'x')
		return maxx;
	if (type == 'y')
		return maxy;
	if (type == 'z')
		return maxz;
}

void Region::placePokemon(const Pokemon& pokemon, int x, int y, int z) {
	Region* pointedRegion = this;
	int mins[3];
	int maxs[3];
	int coordinates[3] = {x, y, z};
	int index, border, coordinate;

	while(true) {
		mins[0] = pointedRegion->minx;	maxs[0] = pointedRegion->maxx;
		mins[1] = pointedRegion->miny;	maxs[1] = pointedRegion->maxy;
		mins[2] = pointedRegion->minz;	maxs[2] = pointedRegion->maxz;

		pointedRegion->numberOfPokemons = pointedRegion->numberOfPokemons + 1;
		if ( maxs[0] == mins[0] && maxs[1] == mins[1] && maxs[2] == mins[2]) {	// unit cell
			pointedRegion->pokemon = new Pokemon(pokemon);
			return;
		}

		index = pointedRegion->divisionCoordinate -'x';
		if (maxs[index] - mins[index] == 0) {
			index = (index+1)%3;
			if (maxs[index] - mins[index] == 0)
				index = (index+1)%3;
			pointedRegion->divisionCoordinate = 'x' + index;
		}

		border = (maxs[index] - mins[index]) / 2 + mins[index];

		if (coordinates[index] <= border) {
			maxs[index] = border;
			if (pointedRegion->firstHalf == NULL) {
				pointedRegion->firstHalf = new Region(mins, maxs);
				pointedRegion->firstHalf->parent = pointedRegion;
			}
			pointedRegion = pointedRegion->firstHalf;
		}
		else {
			mins[index] = border+1;
			if (pointedRegion->secondHalf == NULL) {
				pointedRegion->secondHalf = new Region(mins, maxs);
				pointedRegion->secondHalf->parent = pointedRegion;
			}
			pointedRegion = pointedRegion->secondHalf;
		}
		pointedRegion->divisionCoordinate = 'x' + ((index+1)%3);
	}
}

Pokemon& Region::operator()(int x, int y, int z) {
	int coordinates[3] = {x, y, z};
	int index, tempBorder;
	char tempIndex;
	bool isFirstHalf = false, isSecondHalf = false;
	pokemonException pokemon_exception;
	Region* pointedRegion = this;
	Region* previousRegion = NULL;
	Pokemon* pokemon;
	while(true) {
		if (pointedRegion->firstHalf == NULL && pointedRegion->secondHalf == NULL) {
			if (pointedRegion->pokemon) {
				pokemon = pointedRegion->pokemon;
				pointedRegion->pokemon = NULL;
				previousRegion = pointedRegion->parent;
				
				if (previousRegion) {
					tempIndex = previousRegion->divisionCoordinate;
					tempBorder = (previousRegion->getMaxBorder(tempIndex) - previousRegion->getMinBorder(tempIndex)) / 2 +previousRegion->getMinBorder(tempIndex);
					if (pointedRegion->getMaxBorder(tempIndex) <= tempBorder)
						isFirstHalf = true;
					else
						isSecondHalf = true;
					if (isFirstHalf)
						previousRegion->firstHalf = NULL;
					else
						previousRegion->secondHalf = NULL;
					delete pointedRegion;
				}

				pointedRegion = previousRegion;
				break;
			}
			else
				throw pokemon_exception;
		}

		index = pointedRegion->divisionCoordinate - 'x';
	
		if (pointedRegion->firstHalf != NULL && pointedRegion->firstHalf->getMaxBorder('x'+index) >= coordinates[index])
			pointedRegion = pointedRegion->firstHalf;
		else if (pointedRegion->secondHalf != NULL && pointedRegion->secondHalf->getMinBorder('x'+index) <= coordinates[index])
			pointedRegion = pointedRegion->secondHalf;
		else
			throw pokemon_exception;
	}

	while(pointedRegion) {
		isFirstHalf = false; isSecondHalf = false;
		if (pointedRegion->firstHalf == NULL && pointedRegion->secondHalf == NULL) {
			previousRegion = pointedRegion->parent;

			if (previousRegion) {
				tempIndex = previousRegion->divisionCoordinate;
				tempBorder = (previousRegion->getMaxBorder(tempIndex) - previousRegion->getMinBorder(tempIndex)) / 2 +previousRegion->getMinBorder(tempIndex);
				if (pointedRegion->getMaxBorder(tempIndex) <= tempBorder)
					isFirstHalf = true;
				else
					isSecondHalf = true;
				if (isFirstHalf)
					previousRegion->firstHalf = NULL;
				else
					previousRegion->secondHalf = NULL;
				delete pointedRegion;
				pointedRegion = previousRegion;
			}
			else
				break;
		}
		else {
			pointedRegion->numberOfPokemons = pointedRegion->numberOfPokemons - 1;
			pointedRegion = pointedRegion->parent;
		}
	}
	return *pokemon;
}


Region Region::crop(const int mins[3], const int maxs[3]) const {
	int index;
	const Region* pointedRegion = this;

	while(true) {
		if (pointedRegion->minx == mins[0] && pointedRegion->miny == mins[1] && pointedRegion->minz == mins[2] &&
		    pointedRegion->maxx == maxs[0] && pointedRegion->maxy == maxs[1] && pointedRegion->maxz == maxs[2])
			return *pointedRegion;
		index = pointedRegion->divisionCoordinate - 'x';
		if (pointedRegion->firstHalf != NULL && pointedRegion->firstHalf->getMaxBorder('x'+index) >= maxs[index])
			pointedRegion = pointedRegion->firstHalf;
		else if (pointedRegion->secondHalf != NULL && pointedRegion->secondHalf->getMinBorder('x'+index) <= mins[index])
			pointedRegion = pointedRegion->secondHalf;
	}
}

void Region::patch(Region region) {
	if (minx == maxx && miny == maxy && minz == maxz) {
		if (this->pokemon) {
			delete this->pokemon;
			this->pokemon = NULL;
		}
		this->pokemon = new Pokemon(*(region.pokemon));
		if (this->parent)
			this->parent->updatePokemonCount();
		return;
	}

	int minCoordinates[3] = {minx, miny, minz};
	int maxCoordinates[3] = {maxx, maxy, maxz};

	int index = this->divisionCoordinate - 'x';
	if (maxCoordinates[index] == minCoordinates[index]) {
		index = (index+1)%3;
		if (maxCoordinates[index] == minCoordinates[index])
			index = (index+1)%3;
		this->divisionCoordinate = 'x' + index;
	}

	int border = (maxCoordinates[index] - minCoordinates[index]) / 2 + minCoordinates[index];
	int temp = maxCoordinates[index];
	maxCoordinates[index] = border;
	
	// Totally patch onto firstHalf
	if (maxCoordinates[index] >= region.getMaxBorder(this->divisionCoordinate)) {
		if (this->firstHalf == NULL) {
			this->firstHalf = new Region(minCoordinates, maxCoordinates);
			this->firstHalf->parent = this;
			this->firstHalf->divisionCoordinate = 'x' + (index+1)%3;
			this->firstHalf->numberOfPokemons = region.numberOfPokemons;;
		}
		this->firstHalf->patch(region);
	}

	// Patch onto both halves
	else if (maxCoordinates[index] >= region.getMinBorder(this->divisionCoordinate)) {
		if (region.firstHalf) {
			if (this->firstHalf == NULL) {
				this->firstHalf = new Region(minCoordinates, maxCoordinates);
				this->firstHalf->parent = this;
				this->firstHalf->divisionCoordinate = (region.firstHalf)->divisionCoordinate;
				this->firstHalf->numberOfPokemons = (region.firstHalf)->numberOfPokemons;
			}
			this->firstHalf->patch(*(region.firstHalf));
		}
		else {
			if (this->firstHalf)
				delete this->firstHalf;
			this->firstHalf = NULL;
		}
		maxCoordinates[index] = temp;
		minCoordinates[index] = border+1;
		if (region.secondHalf) {
			if (this->secondHalf == NULL) {
				this->secondHalf = new Region(minCoordinates, maxCoordinates);
				this->secondHalf->parent = this;
				this->secondHalf->divisionCoordinate = (region.secondHalf)->divisionCoordinate;
				this->secondHalf->numberOfPokemons = (region.secondHalf)->numberOfPokemons;
			}
			this->secondHalf->patch(*(region.secondHalf));
		}
		else {
			if (this->secondHalf)
				delete this->secondHalf;
			this->secondHalf = NULL;
		}
	}
	// Totally patch onto secondHalf
	else {
		if (this->secondHalf == NULL) {
			maxCoordinates[index] = temp;
			minCoordinates[index] = border+1;
			this->secondHalf = new Region(minCoordinates, maxCoordinates);
			this->secondHalf->parent = this;
			this->secondHalf->divisionCoordinate = 'x' + (index+1)%3;
			this->secondHalf->numberOfPokemons = region.numberOfPokemons;
		}
		this->secondHalf->patch(region);
	}
}


Region& Region::operator=(const Region& region) {
	this->minx = region.minx;	this->maxx = region.maxx;
	this->miny = region.miny;	this->maxy = region.maxy;
	this->minz = region.minz;	this->maxz = region.maxz;
	this->numberOfPokemons = region.numberOfPokemons;
	this->divisionCoordinate = region.divisionCoordinate;
	
	if (this->firstHalf) {
		delete this->firstHalf;
		this->firstHalf = NULL;
	}
	if (this->secondHalf) {
		delete this->secondHalf;
		this->secondHalf = NULL;
	}

	if (region.firstHalf) {
		this->firstHalf = new Region( *(region.firstHalf) );
		this->firstHalf->parent = this;
	}
	else
		this->firstHalf = NULL;
	if (region.secondHalf) {
		this->secondHalf = new Region( *(region.secondHalf) );
		this->secondHalf->parent = this;
	}
	else
		this->secondHalf = NULL;
	if (region.pokemon)
		this->pokemon = new Pokemon(*(region.pokemon));
	else
		this->pokemon = NULL;
	return *this;
}

int Region::getPokemonCount(const int mins[3], const int maxs[3]) const {
	if (	this->minx == mins[0] && this->miny == mins[1] && this->minz == mins[2] && 
		this->maxx == maxs[0] && this->maxy == maxs[1] && this->maxz == maxs[2]	 )
		return this->numberOfPokemons;

	int pokemonCount = 0;
	char c = this->divisionCoordinate;

	if (this->firstHalf != NULL) {
		if (this->firstHalf->getMaxBorder(c) >= maxs[c-'x'])
			return firstHalf->getPokemonCount(mins, maxs);
		else if (this->firstHalf->getMaxBorder(c) >= mins[c-'x']) {
			int border1[3] = {maxs[0], maxs[1], maxs[2]};
			border1[c-'x'] = firstHalf->getMaxBorder(c);
			pokemonCount += firstHalf->getPokemonCount(mins, border1);
		}
	}
	if (this->secondHalf != NULL) {
		if (this->secondHalf->getMinBorder(c) <= mins[c-'x'])
			return secondHalf->getPokemonCount(mins, maxs);
		else if (this->secondHalf->getMinBorder(c) <= maxs[c-'x']) {
			int border2[3] = {mins[0], mins[1], mins[2]};
			border2[c-'x'] = secondHalf->getMinBorder(c);
			pokemonCount += secondHalf->getPokemonCount(border2, maxs);
		}
	}
	return pokemonCount;
}

bool Region::intersects(const int mins[3], const int maxs[3]) const {
	if (mins[0] > maxx)
		return false;
	else if (minx > maxs[0])
		return false;
	else if (mins[1] > maxy)
		return false;
	else if (miny > maxs[1])
		return false;
	else if (mins[2] > maxz)
		return false;
	else if (minz > maxs[2])
		return false;
	else
		return true;
}


void Region::updatePokemonCount() {
	int count = 0;
	if (this->firstHalf)
		count += firstHalf->numberOfPokemons;
	if (this->secondHalf)
		count += secondHalf->numberOfPokemons;
	this->numberOfPokemons = count;
	if (this->parent)
		this->parent->updatePokemonCount();
}

