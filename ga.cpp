
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <assert.h>
#include <gaMpi.h>
#include <stdlib.h>
#include "utils.h"
#include "population.h"
#include "individual.h"
#include <evaluate.h>
#include "ga.h"
#include "random.h"

using namespace std;
using namespace ga;


MPIInfo initMPI(int argc, char *argv[]){

	MPIInfo info;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &(info.numProcs));
	MPI_Comm_rank(MPI_COMM_WORLD, &(info.myId));
	int namelen;
	char processorName[MPI_MAX_PROCESSOR_NAME];
	MPI_Get_processor_name(processorName, &namelen);
	info.processorName = string(processorName);

	return info;
}

void evalWithRank(int rank, Individual *ent){
	MPI_Request request;
	MPI_Status status;
	MPI_Isend(ent->chrom, ent->length, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, &request);
	//MPI_Request_free(&request); // Does this need to be MPI_Wait?
	MPI_Wait(&request,&status);
}


const int DIE_TAG = 101;

void SetupSlaves(int chromLength, int nCriteria){
	cout << "Hello world from " << info.processorName << " with rank " << info.myId << " out of " << info.numProcs << endl;
	flush(cout);

	Individual *ent = new Individual(chromLength, nCriteria);
	MPI_Status status;
	while(1 == 1){
		MPI_Recv(ent->chrom, ent->length, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if(status.MPI_TAG == DIE_TAG) break;
		paretoEvalSim(ent, info.myId, 5);
		//paretoEval(ent, info.myId);
		usleep(10);
		//MPI_Send(ent->fitness, MAX_CRITERIA, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		MPI_Send(ent->fitness, nCriteria, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
}

void KillSlaves(int n){
	for (int rank = 1; rank < n; rank++){
		MPI_Request req;
		MPI_Isend(&rank, 0, MPI_INT, rank, DIE_TAG, MPI_COMM_WORLD, &req);
		MPI_Request_free(&req);
	}
}

int GA::sendInitialEnts(Population *p, int start, int end, int numProcs){
	int rank = 1;
	int nsent = 0;
	for(int i = start; i < start + numProcs - 1 && i < end && rank < numProcs; i++){
		RankToPopIndexMap[rank] = i;
		evalWithRank(rank, p->pop[i]);
		rank++;
		nsent++;
	}
	return nsent;
}

void GA::extractFitnesses(double *fitness, Individual *ent){
	ent->fit = 0;
	for(int i = 0; i < options.nCriteria; i++){
		ent->fitness[i] =  fitness[i];
		ent->fit        += fitness[i];
	}
	ent->fit = ent->fit/options.nCriteria;
}

void GA::parEval(Population *p, int start, int end){

	assert(start >= 0 && start < end);

	int nreceived = 0;
	int nsent = sendInitialEnts(p, start, end, info.numProcs);
	int sendIndex = start + nsent;
	double fitness[options.nCriteria];
	MPI_Status status;
	int entIndex = -1;
	while(nreceived < options.popSize) {
		MPI_Recv(&fitness, options.nCriteria, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		entIndex = RankToPopIndexMap[status.MPI_SOURCE]; //slave with rank status.MPI_SOURCE finished AND is free for more work
		extractFitnesses(fitness, p->pop[entIndex]);
		//cout << info.myId << ": Index: " << entIndex << " Fitness: " << p->pop[entIndex]->fit << endl;
		nreceived++;
		if(nsent < options.popSize && sendIndex < end){
			RankToPopIndexMap[status.MPI_SOURCE] = sendIndex;
			evalWithRank(status.MPI_SOURCE, p->pop[sendIndex]); //send free slave work
			sendIndex++; // next member of pop to send
			nsent++;
		}
	}
}

int main(int argc, char *argv[]) {
	srand(atoi(argv[3]));

	info = initMPI(argc, argv);
	GA pga = GA(argc, argv);
	cout<<atoi(argv[3])<<"  -------------- "<<endl;
	if(info.myId != 0)
		SetupSlaves(pga.options.chromLength, pga.options.nCriteria);

	if(info.myId == 0){
		pga.init();
		pga.run();
		pga.report();

		KillSlaves(info.numProcs);
	}

	MPI_Finalize();

	return 0;
}

GA::GA(int argc, char *argv[]){

	setupOptions(argc, argv);
	srand(options.randomSeed);
	ofstream ofs(options.outfile, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofstream pofs(options.phenotypeFile, std::ofstream::out | std::ofstream::trunc);
	pofs.close();
	maxFitGen = 0;
	this->bestIndividualSoFar = new Individual(options.chromLength, 1);
	bestFitnessSoFar = -1;


}


void GA::init(){
	bool done = false;
	parent = new Population(options);
	child  = new Population(options);
	parEval(parent, 0, options.popSize);
	parent->statistics(parent);
	parent->report(0, parent);
	updateProgress(0, parent);
}


void GA::run(){//chc

	Population *tmp;
	for (unsigned int i = 1; i < options.maxgens; i++){
	        parent->generation(parent);
	        parEval(parent, options.popSize, options.popSize * options.lambda);
	        parent->copyChild(parent, child);

	        child->statistics(child);
	        child->report(i, child);

	        updateProgress(i, child);
			for(int j=0; j<options.popSize; j++ ){
				cout<<child->pop[j]->fit <<" ";
			}
			cout<<endl;

		tmp = parent;
		parent = child;
		child = tmp;
	}

}

/**
 * Update and save the best ever individual
 */
void GA::updateProgress(unsigned int gen, Population *p){

  if (p->max > bestFitnessSoFar){
    bestFitnessSoFar = p->max;
    maxFitGen = gen;
    bestIndividualSoFar->copy(p->pop[p->maxi]);
    
    char printbuf[1024];
    char chromString[MAX_CHROM_LENGTH+1];
    chromToString(bestIndividualSoFar->chrom, bestIndividualSoFar->length, chromString);
    sprintf(printbuf, "%4i \t %f \t %s\n", maxFitGen, bestFitnessSoFar, chromString);
    writeBufToFile(printbuf, options.phenotypeFile);
  }
  
}


void GA::report(){
  //parent->report(options.maxgens);
  cout << *(parent->pop[parent->maxi]) << endl;
}

void GA::configure(){
	ifstream ifs(options.infile);
	if(ifs.good()){
		ifs >> options.popSize;
		ifs >> options.chromLength;
		ifs >> options.maxgens;
		ifs >> options.px;
		ifs >> options.pm;
		ifs >> options.scaler;
		ifs >> options.lambda;
	}
	ifs.close();
}

void GA::setupOptions(int argc, char *argv[]){

	options.randomSeed = 189;
	options.infile = string("infile");
	options.outfile = string("outfile_189");// append randomseed to output file names

	options.popSize = 10;
	options.chromLength = 10;
	options.maxgens = 10;
	options.px = 0.7f;
	options.pm = 0.001f;
	options.scaler = 1.05;
	options.lambda = 2;
	options.nCriteria   = 1;

	options.mutator = Mutator::Flip;
	options.xover = Xover::UX;
	options.selector = Selector::Proportionate;

	if(argc == 4){
		options.infile = string(argv[1]);
		options.outfile = string(argv[2]);
		options.randomSeed = atoi(argv[3]);
		configure();
	}
	//derived values go after configure() above
	options.phenotypeFile   = string(options.outfile + ".pheno"); //derived from options.outfile
	options.outfile = string(options.outfile);// append randomseed to output file names
	//options.maxgens = options.popSize * 1.5;


}


