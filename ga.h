
#ifndef GA_H_
#define GA_H_

#include "population.h"

namespace ga {

	class GA {

	private:
		void configure();

	public:
		Population *parent;
		Population *child;
		Options options;

		unsigned int maxFitGen;
		Individual *bestIndividualSoFar;
		double     bestFitnessSoFar;

		//GA(){}
		GA(int argc, char *argv[]);
		~GA(){}

		void setupOptions(int argc, char *argv[]);
		void init();
		void parEval(Population *p, int start, int end);
		void run();
		//virtual void CHC(Population *p, Population *c);
		void report();
		void updateProgress(unsigned int gen, Population *p);
		int  sendInitialEnts(Population *p, int start, int end, int numProcs);
		void extractFitnesses(double *fitness, Individual *ent);

		void readFitnessFromFile(Population *parent, int gen);
		void readFitness(Population *p, int j, int gen);
		void writeChromToFile(Population *parent, int gen);
		void writeChrom(Population *p, int j, int gen);
		bool fexists();



	};

}



#endif /* GA_H_ */
