

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <iostream>
#include "const.h"

namespace ga {

	class Individual {
	private:
		void setup();

	public:
		int length;
		int chrom[MAX_CHROM_LENGTH];
		double fit, scaledFit;
		double fitness[MAX_CRITERIA];
		double objectives[MAX_CRITERIA];


		Individual();
		Individual(int len, int criteria);
		~Individual(){
		}
		void copy(Individual *i);
		void init(int len);
		void mutate(float prob);

		friend std::ostream &operator<<(std::ostream &output, const Individual &ip){
			for(int i = 0; i < ip.length; i++){
				output << ip.chrom[i];
			}
			output << " " ; //std::endl;

			output << "Fitness: " << ip.fit << /*" Scaled Fitness: " << ip.scaledFit << */ std::endl;
			return output;
		}
	};

}

#endif /* INDIVIDUAL_H_ */
