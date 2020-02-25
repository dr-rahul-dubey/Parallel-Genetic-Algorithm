

#include "individual.h"
#include <stdlib.h>
#include "random.h"
#include <assert.h>

using namespace std;
using namespace ga;

Individual::Individual(){
	length    = 0;
	setup();
}

Individual::Individual(int len, int criteria){
	length = len;
	setup();
}

void Individual::setup(){
	for(int i = 0; i < length; i++){
		this->chrom[i] = -1;
	}
}


void Individual::init(int len){
	assert(len <= MAX_CHROM_LENGTH);
	length = len;
	for(int i = 0; i < length; i++){
		this->chrom[i] = flip(0.5);
	}
}



void Individual::mutate(float prob){
	for(int i = 0; i < length; i++){
		if(flip(prob) == 1)
			chrom[i] = 1 - chrom[i];
	}
}

void Individual::copy(Individual *ip){
	assert(ip->length <= MAX_CHROM_LENGTH);
	this->length = ip->length;
	for(int i = 0; i < length; i++){
		this->chrom[i] = ip->chrom[i];
	}
	for(int i = 0; i < MAX_CRITERIA; i++){
		this->fitness[i] = ip->fitness[i];
		this->objectives[i] = ip->objectives[i];
	}
	this->fit = ip->fit;
	this->scaledFit = ip->scaledFit;
}

