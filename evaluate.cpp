
#include <evaluate.h>
#include <utils.h>
#include <const.h>
#include <vector>

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

void evaluate(ga::Individual *ent){
//	cout << *ent << endl;
	double sum = 0;
	for(int i = 0; i < ent->length; i++){
		sum += ent->chrom[i];
	}
	ent->fit = sum;
	for(int i = 0; i < ga::MAX_CRITERIA; i++){
		ent->fitness[i] = sum;
	}

//	cout << "----------------------------------------" << endl;

}


void readTwoFitnesses(double *fits, int nfits, string fname){
  std::ifstream ifs(fname.c_str(), std::ifstream::in);
  double f1,f2;
  //cout << "| " ;
  ifs >> f1 >> f2;// cout << f << " : ";
  fits[0] += f1;
  fits[1] += f2;
  ifs.close();
  //cout << "F[0]: " << fits[0] << ", F[1]" << fits[1] << endl;
}

void evalParetoScene(ga::Individual *ent, int scene, int rank, char *partialCommandFormat, char *fitnessFile){
	char command[1600];
	char batchmode[125];
	sprintf(batchmode , "-batchmode");
	sprintf(command, partialCommandFormat );
	cout << "command: " << command << endl;
	int err = system(command);
	assert(err == 0);
	readTwoFitnesses(ent->fitness, 2, fitnessFile);
}


void paretoEvalSim(ga::Individual *ent, int rank, int nScenes){
	char chrom[2*ga::MAX_CHROM_LENGTH];
	char partialCommandFormat[1550];
	//double real_chrom[12];
//	std::vector<double> real_chrom;
//	for(int i=0; i<12; i++){
//		real_chrom.push_back(0);
//	}
//	real_chrom[0] = -8192 + (decode(ent, 0, 14)/pow(2.0,13));
//	real_chrom[1] = -5.12 + (decode(ent, 14, 24)/pow(2.0, 9));
//	real_chrom[2] = -8192 + (decode(ent, 24, 38)/pow(2.0,13));
//	real_chrom[3] = -5.12 + (decode(ent, 38, 48)/pow(2.0, 9));
//	real_chrom[4] = -8192 + (decode(ent, 48, 62)/pow(2.0,13));
//	real_chrom[5] = -5.12 + (decode(ent, 62, 72)/pow(2.0, 9));
//	real_chrom[6] = -8192 + (decode(ent, 72, 86)/pow(2.0,13));
//	real_chrom[7] = -5.12 + (decode(ent, 86, 96)/pow(2.0, 9));
//	real_chrom[8] = -8192 + (decode(ent, 96, 110)/pow(2.0,13));
//	real_chrom[9] = -5.12 + (decode(ent, 110, 120)/pow(2.0, 9));
//	real_chrom[10] = -8192 + (decode(ent, 120, 134)/pow(2.0,13));
//	real_chrom[11] = -5.12 + (decode(ent, 134, 144)/pow(2.0, 9));
//	//std::cout<<real_chrom[0]<<std::endl;
	chromToString(ent->chrom, ent->length, chrom);
//	chromToString(real_chrom, 12, chrom);

	char fitnessFile[128];
	sprintf(fitnessFile, "fitness_%i", rank);

	char redirectFile[128];
	sprintf(redirectFile, "redirect_%i", rank);

	string commandFormat = string("./UAV_Linux.x86_64 %c%s%c %s");

	sprintf(partialCommandFormat, commandFormat.c_str(), '\"',chrom,'\"', fitnessFile);

	for(int i = 0; i < 2; i++){
		ent->fitness[i] = 0;
	}
	for(int i = 0; i < 1/*nScenes*/; i++){
		evalParetoScene(ent, i, rank, partialCommandFormat, fitnessFile);
	}
	ent->fit = 0;
	for(int i = 0; i < 2; i++){
		ent->fit += ent->fitness[i];
//		ent->fitness[i] /= nScenes;
		ent->fitness[i] /= 1;
		cout<<ent->fitness[i]<< " "<<i<<endl;
	}

}

double decode(ga::Individual *ent, int start, int end){
	double prod = 0;
	for(int i=start; i<end; i++){
		prod += (ent->chrom[i] == 0 ? 0 : pow(2.0,(double)(i-start)));
	}
	return prod;
}





