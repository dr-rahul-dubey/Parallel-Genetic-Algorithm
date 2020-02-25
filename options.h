

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
#include "const.h"
namespace ga {

	typedef struct {
		std::string infile;
		std::string outfile;
		std::string paretoFrontFile;
		std::string phenotypeFile;

		long int randomSeed;
		int popSize;
		int chromLength;
		unsigned int maxgens;
		float px;
		float pm;
		float scaler;
		int lambda;
        Xover xover;
        Mutator mutator;
        Selector selector;

        int nCriteria;
        bool maximize;
        bool nsga;

        bool Dummy = false;
        std::string file;
        std::string Fit;
        std::string fitness = "fitness";
        std::string chrom = "chrom";

	}Options;

}



#endif /* OPTIONS_H_ */
