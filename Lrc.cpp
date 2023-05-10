#include "Lrc.hpp"
// Examples:
// make; ./lrc.exe 1,2,3,4,5,6,7,8 
// make; ./lrc.exe 4,7,11,12,13,21,22,9
// make; ./lrc.exe 11,1,4,12,13,15,16,9
// make; ./lrc.exe 1,2,3,4,5,6,7,9
// make; ./lrc.exe 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.9
// Random:
// make; ./lrc.exe -r
// Cheating:
// make; ./lrc.exe 1,2,3,4,5,6,7,8,9 -c
// Random and cheating
// make; ./lrc.exe -rc

Lrc::Lrc(char* newInput, bool isRandom, bool isCheating)
{
	//input = newInput;
	this->isRandom = isRandom;
	this->isCheating = isCheating;
	if (isRandom)
		sInput = genRandomVels();
	else
		sInput = newInput;
	startCalc();
}

// Get random int in range (min, max)
int Lrc::getRandomInt(int min, int max)
{
	static bool first = true;
	if (first) 
	{  
		srand(time(NULL)); // Seed randomness for the first time only
		cout << "Seeding!" << endl;
		first = false;
	}
	return min + (rand() % ( max - min + 1 ));
}

// Get the random vels to string, example: 54,42,7,62,33,45,24,96
string Lrc::genRandomVels()
{
	string randomVels, randomInt;
	for(int i = 0; i < 8; i++){
		//randomInt = to_string(getRandomInt(1, 1000));
		//randomInt = to_string(getRandomInt(1, 100));
		randomInt = to_string(getRandomInt(1, 50));
		if (i == 0) randomVels = randomInt + ",";
		else {
			if (randomVels.find(randomInt) == string::npos) {
				if (i == 7) randomVels += randomInt;
				else randomVels += randomInt + ",";
			} else i--;
		}
	}
	return randomVels;
}


// Start the calculations
void Lrc::startCalc()
{
	initValues();

	int runnerToCheck = 0;
	cout << "Checking runner (vel): " << vels.at(runnerToCheck) << endl;
	genDeltaVels(runnerToCheck);
	int loopLen = k-1;
	if (isCheating) loopLen++;

	for(int i = 0; i < loopLen; i++) {
		genVRatios(i);
		cout << endl;
		calcRunner(i);
		dvR.clear();
	}
	checkLoneliness();
	cout << "neverLonelyFinal: " << neverLonelyFinal << endl;
	cout << endl;
	dVels.clear();
	resVec.clear();
	notLonely.clear();
	vels.clear();

	string lonelyAtStr = to_string(lonelyAt);
	string lonelyAtVStr = to_string(lonelyAtV);
	if (neverLonelyFinal) 
		lonelyAtStr = "x"; // Never lonely
		
	//cout << "lonelyAt: " << lonelyAtStr << endl;
	//cout << "lonelyAtV: " << lonelyAtVStr << endl;
	// Feed the vels as argument to python script
	string pyScriptString = "python lrc_game.py " + sInput + " " + lonelyAtStr + " " + lonelyAtVStr;
	const char* pyScript = pyScriptString.c_str();
	int result = system(pyScript);
	cout << "Result from py script: " << result << endl;
	cout << "End..." << endl;
}

// Generate delta vels
// This function assumes that the velocities are unique...
void Lrc::genDeltaVels(int curr)
{
	// Example:
	// runnerToCheck: 1
	// Input: 1,2,3,4,5,6,7,8
	// Output: 1,2,3,4,5,6,7
	int loopLen = k;
	if (isCheating) loopLen++;

	double currVal = vels.at(curr);
	for(int i = 0; i < loopLen; i++) {
		if (i != curr)
			dVels.push_back((vels.at(i) > currVal ? (vels.at(i)-currVal) : (currVal - vels.at(i))));
	}
	//sort(dVels.begin(), dVels.end());
	for(int i = 0; i < dVels.size(); i++)
		cout << "dVels: " << dVels.at(i) << endl;
}

// Calculate ratios of delta velocities (Δv/Δv)
void Lrc::genVRatios(int curr)
{
	// Example:
	// runnerToCheck: 0.1 (or 1)
	// dvRatio i: 2.0 (0.2 / 0.1)
	// dvRatio i: 3.0 (0.3 / 0.1)
	// dvRatio i: 4.0 (0.4 / 0.1)
	// dvRatio i: 5.0 (0.5 / 0.1)
	// dvRatio i: 6.0 (0.6 / 0.1)
	// dvRatio i: 7.0 (0.7 / 0.1)

	for(int i = 0; i < dVels.size(); i++){
		if (i != curr)
			dvR.push_back(dVels.at(i) / dVels.at(curr));
	}
	for(int i = 0; i < dvR.size(); i++)
		cout << "dvR: " << dvR.at(i) << endl;
}

// Compare two double values
//bool compare_double(double x, double y, double epsilon = 0.01f){
//bool compare_double(double x, double y, double epsilon = 0.001f){
bool compare_double(double x, double y, double epsilon = 0.00001d){
   //return (fabs(x - y) < epsilon);
   return (abs(x - y) < epsilon);
}

// Here, the Δv/Δv ratio is being checked against the conjectures rules
// calcRunner(k, dvRatio.length, dvRatio);
// Start with slowest runner?
void Lrc::calcRunner(int curr)
{
	kth = static_cast<double> (1.0 / k); // (1/k)
	double res; // Result of kth + c * dvR
	double dec; // Decimal values
	double decP; // Decimals of first period
	int p; // Period counter
	int c; // Integer counter
	maxP = 0; // Save the highest p value

	for(int i = 0; i < dvR.size(); i++){
		p = -1; // Reset period counter
		c = 0;
		bool pReached = false;
		while (!pReached) {
			res = (kth + c++) * dvR.at(i);
			//cout << "res: " << res << endl;

			// Check if lonely
			dec = res - (int)res;
			//dec = roundf(dec * 10) / 10;

			dec = res - (int)res;
			if (p == -1)
				decP = dec;
			p++;

			//cout << "dec: " << dec << endl;
			if (dec < kth || dec > (kth * (k-1))) {
				notLonely.push_back(make_tuple(c-1, 0));
				resVec.push_back(make_tuple(dvR.at(i), res));
			}

			// Check if pReached
			if (compare_double(dec, decP) && p > 0)
				pReached = true;
		}
		if (p > maxP) maxP = p;
		//cout << "pReached : " << p << endl;

		// This could be slightly faster if just checking new values added to vec
		for(int j = 0; j < notLonely.size(); j++){
			if (get<1>(notLonely.at(j)) == 0) {
				get<1>(notLonely.at(j)) = p;
				//cout << "notLonely: " << get<0>(notLonely.at(j)) << ", " << get<1>(notLonely.at(j)) << " - dvR: " << dvR.at(i) << endl;
			}
		}
		// Add separator to keep track of different dv
		notLonely.push_back(make_tuple(-1, -1));
		resVec.push_back(make_tuple(-1, -1));
	}
}

// Check runner loneliness based on notLonely vector: c, p
void Lrc::checkLoneliness()
{
	cout << "checking loneliness..." << endl;
	int vCounter = 0; // Keep track of current dVel
	int currDv = 0; // Increments if new dVel
	int maxV = 6; // When to switch to new dVel
	if (isCheating) maxV++;

	// This could be improved by using a map from the beginning instead of using notLonely for all dvR...
	// resVec isn't necessarily required and can be disregarded when improving speed (it's more relevant for printing info)
	vector<int> pCheck; // Used to check period values (if p = 3, then check 1,2,3,4,5,6)
	map<double, vector<int>> pCheckMap; // Map for pCheck

	for(int i = 0; i < notLonely.size(); i++){
		// Keep track of current dVel
		if (vCounter == maxV) {
			sort(pCheck.begin(), pCheck.end()); // Sort vector
			pCheck.erase(unique( pCheck.begin(), pCheck.end()), pCheck.end()); // Clean vector from duplicates
			pCheckMap.insert({dVels.at(currDv), pCheck}); // Add vector to map
			pCheck.clear(); // Clear vector

			cout << endl; // Print line separator
			currDv++; // Mark new dVel
			vCounter = 0; // Reset counter
		} 

		// Print information and check loneliness
		if (get<0>(resVec.at(i)) != -1) {
			cout << "notLonely: " << get<0>(notLonely.at(i)) << ", " << get<1>(notLonely.at(i));
			cout << " - dv: " << dVels.at(currDv);
			cout << " - dvR: " << get<0>(resVec.at(i)) << ", res: " << get<1>(resVec.at(i)) << endl;

			// Check loneliness (loop until double period?)
			int pCount = get<0>(notLonely.at(i));
			while (pCount < maxP*2+1) {
				pCheck.push_back(pCount);
				pCount += get<1>(notLonely.at(i));
			}
		} else
			vCounter++;
	}

	// Add last dv to map
	sort(pCheck.begin(), pCheck.end()); // Sort vector
	pCheck.erase(unique( pCheck.begin(), pCheck.end()), pCheck.end()); // Clean vector from duplicates
	pCheckMap.insert({dVels.at(currDv), pCheck}); // Add vector to map
	pCheck.clear(); // Clear vector

	// TODO: Find first time t when lonely (right now all times are calculated)
	vector<int> pController;
	for(int i = 0; i < maxP*2+1; i++) pController.push_back(i);

	cout << "\npCheckMap (maxP = " << maxP << "):" << endl;
	for (auto entry : pCheckMap) {
		auto k = entry.first;
		auto x = entry.second;
		cout << k << ":" << endl;
		if (x.size() == 0){
			cout << "Runner lonely at dv: " << k << ", p not found: 0" << endl;
			cout << "Time: " << ((0 + kth) / k) << endl;
			lonelyAt = 0;
			//lonelyAtDv = k; // Value of dv
			lonelyAtV = find(dVels.begin(), dVels.end(), k) - dVels.begin() + 1; // Index value of the vel that dv relates to
		}
		for(int i = 0; i < x.size(); i++){
			cout << x.at(i) << ", ";
			if (x.at(i) != pController.at(i)){
				neverLonelyFinal = false;
				// Save this into some var / vec
				cout << "Runner lonely at dv: " << k << ", p not found: " << pController.at(i) << endl;
				cout << "Time: " << ((pController.at(i) + kth) / k) << endl;
				lonelyAt = pController.at(i);
				//lonelyAtDv = k; // Value of dv
				lonelyAtV = find(dVels.begin(), dVels.end(), k) - dVels.begin() + 1; // Index value of the vel that dv relates to
			}
		}
		cout << endl;
	}
}

// Initialize values 
void Lrc::initValues()
{
	cout << "Received input: " << sInput << endl;
	cout << "isR: " << isRandom << endl;
	cout << "isC: " << isCheating << endl;

	// Convert string to char*
	//input = &*randomPlay.begin();

	stringstream s_stream(sInput);
	while(s_stream.good()) {
		string substr;
		getline(s_stream, substr, ','); // Get first string delimited by comma
		vels.push_back(stof(substr)); // Convert from str to double and add to vels
	}
	sort(vels.begin(), vels.end()); // Sort vector
	// Print all vels
	//for(int i = 0; i < vels.size(); i++) {
	//	cout << vels.at(i) << endl;
	//}
	if (isCheating)
		k = vels.size()-1;
	else
		k = vels.size();
	cout << "k: " << k << endl;
	neverLonelyFinal = true;
}

// Some notes:
// All runners "should" always be lonely at some time t.
// s = v*t

// Runner 0.1 and Runner 0.3
// distance: 0.5
// How long until catch-up? 0.5 / 0.2 = 2.5
// 2.5*0.3 = 0.75
// 2.5*0.1 = 0.25
// 0.25 + 0.5 = 0.75 ...

// Slowest dV = 1
// a runner can’t be lonely until the distance between the slowest ∆v is: kth / ∆v
