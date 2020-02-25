

#ifndef CONST_H_
#define CONST_H_

namespace ga {

	const int MAX_POPSIZE = 200;
	const int MAX_CHROM_LENGTH = 250;
	const int MAX_CRITERIA = 3;
	const int MAX_INPUT_CHROMOSOMES = 3;

	enum Xover {
		OnePoint = 0,
		TwoPoint = 1,
		NPoint   = 2,
		UX       = 3,
		PMX      = 4
	};

	enum Mutator {
		Flip = 0,
		Swap = 1
	};

	enum Selector {
		Proportionate = 0,
		Ranked        = 1,
		Random        = 2,
	};




}

#endif /* CONST_H_ */
