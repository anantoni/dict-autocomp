/****************** Recommendations *******************/
/******************************************************/
#include "class_definitions.hpp"

Recommendations::Recommendations() {
	isEmpty = true;
	totalRecommendations = 0;
}

void Recommendations::setRecommendation(int index, string word) {
	isEmpty = false;
	recommendations[index] = word;
}

void Recommendations::setFrequency(int index, int frequency) {
	frequencies[index] = frequency;
}

void Recommendations::incr() {
	totalRecommendations++;
}

int Recommendations::getNumberOfRecommendations() {
	return totalRecommendations;
}

string Recommendations::getRecommendation(int index) {
	return sortedRecommendations[index];
}

int Recommendations::printRecommendations() {
	string recs = "";
	if (isEmpty == false) {
		for (int i=0; i<N; i++)
			for (int j=i+1; j<N; j++) {
				if (recommendations[i] > recommendations[j]) {
					string temp1 = recommendations[i];
					recommendations[i] = recommendations[j];
					recommendations[j] = temp1;
					int temp2 = frequencies[i];
					frequencies[i] = frequencies[j];
					frequencies[j] = temp2;
				}
			}
		int counter = 0;
		for (int i=0; i<N; i++) {
			if (recommendations[i] != "") {
				recs += recommendations[i];
				stringstream convert;       //create stringstream
				convert << frequencies[i];  //add number to the stream
				recs += "{";
				recs += convert.str();
				recs += "}";
				recs += " ";
				sortedRecommendations[counter] = recommendations[i];
				counter++;
			}
		}
		if (totalRecommendations > 1)
			cerr << endl << recs;
		return 0;
	}
	else
		return -1;
}
