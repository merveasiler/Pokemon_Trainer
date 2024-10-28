#include "pokemon.h"

Pokemon::Pokemon(const string& name, const string& type, int evolutionBound) {
	int i;
	for (i=0; i<name.size(); i++)
		(this->name).push_back(name[i]);
	for (i=0; i<type.size(); i++)
		(this->type).push_back(type[i]);

	this->evolutionBound = evolutionBound;
	numberOfExperiences = 0;
	t_ref_cnt = 0;
}

Pokemon::Pokemon(const Pokemon& pokemon) {
	int i;
	for (i=0; i<pokemon.name.size(); i++)
		(this->name).push_back((pokemon.name)[i]);
	for (i=0; i<pokemon.type.size(); i++)
		(this->type).push_back((pokemon.type)[i]);

	this->evolutionBound = pokemon.evolutionBound;
	this->numberOfExperiences = pokemon.numberOfExperiences;
	t_ref_cnt = pokemon.t_ref_cnt;
}

Pokemon::~Pokemon() {
}

const string& Pokemon::getName() const {
	return name;
}

bool Pokemon::operator>>(const Pokemon& evolvedPokemon) {
	int numOfExp = this->numberOfExperiences;
	if (numOfExp >= this->evolutionBound) {
		(*this) = evolvedPokemon;
		this->numberOfExperiences = numOfExp;
		
		return true;
	}
	return false;	
}

Pokemon operator&(Pokemon& pokemon1, Pokemon& pokemon2) {
	pokemon1.numberOfExperiences++;
	pokemon2.numberOfExperiences++;
	if ( pokemon1.type.compare("electric") == 0 ) {
		if( pokemon2.type.compare("electric") == 0 )
			return pokemon1;				// throw exception
		if( pokemon2.type.compare("fire") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("water") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("grass") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("flying") == 0 )	
			return pokemon1;
		return pokemon1;					// throw exception
	}
	if ( pokemon1.type.compare("fire") == 0) {
		if( pokemon2.type.compare("electric") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("fire") == 0 )
			return pokemon1;				// throw exception
		if( pokemon2.type.compare("water") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("grass") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("flying") == 0 )
			return pokemon2;
		return pokemon1;					// throw exception
	}
	if ( pokemon1.type.compare("water") == 0) {
		if( pokemon2.type.compare("electric") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("fire") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("water") == 0 )
			return pokemon1;				// throw exception
		if( pokemon2.type.compare("grass") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("flying") == 0 )
			return pokemon1;
		return pokemon1;					// throw exception
	}
	if ( pokemon1.type.compare("grass") == 0) {
		if( pokemon2.type.compare("electric") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("fire") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("water") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("grass") == 0 )
			return pokemon1;				// throw exception
		if( pokemon2.type.compare("flying") == 0 )
			return pokemon2;
		return pokemon1;					// throw exception
	}
	if ( pokemon1.type.compare("flying") == 0) {
		if( pokemon2.type.compare("electric") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("fire") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("water") == 0 )
			return pokemon2;
		if( pokemon2.type.compare("grass") == 0 )
			return pokemon1;
		if( pokemon2.type.compare("flying") == 0 )
			return pokemon1;				// throw exception
		return pokemon1;					// throw exception
	}
	return pokemon1;
}

Pokemon& Pokemon::operator=(const Pokemon& pokemon) {
	int i;
	this->name = "";
	for (i=0; i<pokemon.name.size(); i++)
		(this->name).push_back((pokemon.name)[i]);
	this->type = "";
	for (i=0; i<pokemon.type.size(); i++)
		(this->type).push_back((pokemon.type)[i]);

	this->evolutionBound = pokemon.evolutionBound;
	this->numberOfExperiences = pokemon.numberOfExperiences;
	this->t_ref_cnt = pokemon.t_ref_cnt;
	return *(this);
}
