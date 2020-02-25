
#include "random.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <string.h>

using namespace std;

void swap(int *x, int i, int j){
	int tmp = x[i];
	x[i] = x[j];
	x[j] = tmp;
}

void shuffle(int *array, int size){
	for(int i = 0; i < size; i++){
		array[i] = i;
	}
	int a, b;
	for(int i = 0; i < size/2; i++){
		a = intInRange(0, size);
		b = intInRange(0, size);
		swap(array, a, b);
	}
}

void printIntArray(int *x, int size){
	for(int i = 0; i < size; i++){
		std::cout << x[i] << " ";
	}
	std::cout << std::endl;
}

void writeBufToFile(std::string buf, std::string filename){
	std::ofstream ofs(filename, std::ofstream::app);
	if(ofs.good()){
		ofs << buf;
	}
	ofs.flush();
	ofs.close();
}

void chromToString(int *chrom, int len, char *s){
	for(int i = 0; i < len; i++){
		s[i] = (chrom[i] == 0 ? '0' : '1');
	}
	s[len] = '\0';
	assert(strlen(s) == (unsigned int) len);
}

void stringToChrom(char *chromChar, int len, int *chromInt){
	for(int i = 0; i < len; i++){
		chromInt[i] = (chromChar[i] == '0' ? 0 : 1);
	}
}

float decode (int *chrom, int start, int end, float min, float max){
	int sum = 0;
	int powerOfTwo = 0;
	for(int i = start; i < end; i++){
		sum += (chrom[i] == 1 ? pow(2.0, powerOfTwo) : 0);
		powerOfTwo++;
	}
	float precision = (max - min)/pow(2.0, (double)(end-start));
	return min + (precision * sum);
}

float decode(const char* chrom, int start, int end, float min, float max){
	int sum = 0;
	for(int i = 0; i < end - start; i++){
		sum += (chrom[start + i] == '0' ? 0 : pow(2.0, (double) i));
	}
	float precision = (max - min)/pow(2.0, (double)(end-start));
	return min + (precision * sum);
}



