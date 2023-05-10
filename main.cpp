#include <iostream>
#include <cstring>
#include "Lrc.hpp"
#include "LrcTesting.hpp"

using namespace std;

// Check if w2 is part of w1
bool isPartOf(char* w1, char* w2)
{
	int i=0;
	int j=0;

	while(w1[i] != '\0'){
		if(w1[i] == w2[j])
		{
			int init = i;
			while (w1[i] == w2[j] && w2[j] != '\0')
			{
				j++;
				i++;
			}
			if(w2[j] == '\0'){
				return true;
			}
			j=0;
		}
		i++;
	}
	return false;
}

int main(int argc, char** argv) {
	//cout << "This program has " << argc << " arguments:" << endl;
	bool isRandom = false;
	bool isCheating = false;
	bool isTesting = false;

	if (argc == 1) {
		cout << "No input from argument. Using standard input." << endl;
		//Lrc start("1,2,3,4,5,6,7,8", isRandom, isCheating);
		// Using pointer
		Lrc *start = new Lrc("1,2,3,4,5,6,7,8", isRandom, isCheating);
		delete start;

	} else {
		// else means inputs are given
		//for (int i = 0; i < argc; ++i) {
		//	cout << "input: " << argv[i] << endl;
		//}

		char* argCheck = "r"; // Check for isRandom
		if (argc > 2) {
			isRandom = isPartOf(argv[2], argCheck);
			argCheck = "c"; // Check for isCheating
			isCheating = isPartOf(argv[2], argCheck);
			//cout << "isRandom: " << isRandom << endl;
			//cout << "isCheating: " << isCheating << endl;
		} else if (argc == 2) {
			isRandom = isPartOf(argv[1], argCheck);
			argCheck = "t"; // Check for isTesting
			isTesting = isPartOf(argv[1], argCheck);
		}
		// Use isPartOf or:
		//char *sent = "this is my sample example";
		//char *word = "sample";
		//char *pch = strstr(sent, word);
		
		argCheck = NULL;
		/*
		if (isTesting)
			LrcTesting start(isRandom);
		else
			Lrc start(argv[1], isRandom, isCheating);
		*/
		// Using pointer
		if (isTesting) {
			LrcTesting *start = new LrcTesting(isRandom);
			delete start;
		}
		else {
			Lrc *start = new Lrc(argv[1], isRandom, isCheating);
			delete start;
		}
	}

	cout << "END!" << endl;
    return 0;
}
