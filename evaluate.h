

#ifndef EVALUATE_H_
#define EVALUATE_H_

#include <individual.h>
#include <cstring>

void evaluate(ga::Individual *ent);
void convertToString(int *vec, int size, char *chrom);
void EvalSim(ga::Individual *ent, int mpiRank, int nScenes);
double decode(ga::Individual *ent, int start, int end);


#endif /* EVALUATE_H_ */
