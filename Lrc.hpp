#ifndef LRC_H
#define LRC_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

class Lrc
{
	private:
		char* input;
		string sInput;
		string genRandomVels();
		bool isRandom;
		bool isCheating;
		bool neverLonelyFinal;
		int k;
		int maxP;
		int lonelyAt; // Used for sending to python script
		int lonelyAtV; // Used for sending to python script
		int getRandomInt(int min, int max);
		double kth;
		vector<double> vels;
		vector<double> dVels;
		vector<double> dvR;
		// Should create a class with different DVel that each contain resVec, notLonely etc...
		//map<double, vector<tuple<int, int>>> notLonely;
		vector<tuple<double, double>> resVec; // Save dvR and res when not lonely
		vector<tuple<int, int>> notLonely; // Save c and p when not lonely
		void startCalc();
		void initValues();
		void genDeltaVels(int curr);
		void genVRatios(int curr);
		void calcRunner(int curr);
		void checkLoneliness();

	public:
		Lrc(char* newInput, bool isRandom, bool isCheating);
};

#endif
