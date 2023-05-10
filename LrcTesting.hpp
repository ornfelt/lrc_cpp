#ifndef LRCTESTING_H
#define LRCTESTING_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

class LrcTesting
{
	private:
		char* input;
		string sInput;
		string genRandomVels();
		bool isRandom;
		bool neverLonelyFinal;
		bool compareDouble(double x, double y, double epsilon);
		int k;
		int maxP;
		int finalMaxP;
		int lonelyAt; // Used for sending to python script
		int lonelyAtV; // Used for sending to python script
		int getRandomInt(int min, int max);
		int maxRandom;
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
		LrcTesting(bool isRandom);
};

#endif
