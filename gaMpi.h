

#ifndef GAMPI_H_
#define GAMPI_H_

#include <mpi.h>

//------------------------------------------
typedef struct {
	int numProcs;
	std::string processorName;
	int myId;
} MPIInfo;

const int MAX_PROCS = 8;
//------------------------------------------
namespace ga {
	int RankToPopIndexMap[MAX_PROCS];
	MPIInfo info;
}

#endif /* GAMPI_H_ */
