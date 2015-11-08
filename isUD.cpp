//============================================================================
// Name        : isUD.cpp
// Author      :
// Version     :
// Copyright   :
// Description : This program is checking the input code if it uniquely decodable
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
std::vector<std::pair<int, int> > vectorOfSets; //vector of input alphabet as a set
std::vector<std::pair<int, int> > C1; //first set
std::vector<std::pair<int, int> > C2; //second set

bool isPrefix(pair<int, int> p1, pair<int, int> p2,
		vector<pair<int, int>> &vec) {
	int v1 = 1;
	int v2 = 1;
	int minLength;
	if (p1.first < p2.first)
		minLength = p1.first;
	else
		minLength = p2.first;
	for (int i = 0; i < p1.first - 1; i++) {
		v1 = v1 << 1;
	}

	for (int i = 0; i < p2.first - 1; ++i) {
		v2 = v2 << 1;
	}

	for (int i = minLength; i > 0; i--) {
		if (!(v1 & p1.second) == (v2 & p2.second))
			return false;
	}
	v1 = minLength | 1;
	vec.push_back(
			make_pair(minLength,
					v1 & ((p1.first > p2.first) ? p1.second : p2.second)));
	return true;
}
void printPair(pair<int,int>a){
	cout<<"("<<a.first<<","<<a.second<<")";
}
std::pair<int, int> parse(std::string &str) {
	std::istringstream st(str);
	string first;
	string second = "";
	std::getline(st, first, ',');
	first = first.substr(1);
	std::getline(st, second);
	second = second.substr(1, 2);
	int num1 = std::stoi(first);
	int num2 = std::stoi(second);
	return make_pair(num1, num2);
}

int main(int argc, char *argv[]) {
	ifstream file;
	file.open(argv[1]);
	string myVec;
	int i = 0;
	if (!file.is_open())
		cout << "can't open the file" << endl;
	else
		while (getline(file, myVec)) {
			vectorOfSets.push_back(parse(myVec));
			//cout << vectorOfSets[i].first << " " << vectorOfSets[i].second
			//		<< endl;
			i++;
		}
	file.close();

	for (std::vector<pair<int, int> >::iterator it = vectorOfSets.begin();
			it != vectorOfSets.end(); ++it) {
		for (std::vector<pair<int, int> >::iterator iter = it;
				iter != vectorOfSets.end(); ++iter) {

			if ((iter + 1) != vectorOfSets.end()) {
				isPrefix(*it, *(iter + 1), C1);
			}
		}
	}

	for (std::vector<pair<int, int> >::iterator it = vectorOfSets.begin();
			it != vectorOfSets.end(); ++it) {
		for (std::vector<pair<int, int> >::iterator iter = C1.begin();
				iter != C1.end(); ++iter) {
			isPrefix(*it, *iter, C2);
		}
	}

	for (std::vector<pair<int, int> >::iterator iter = C2.begin();
			iter != C2.end(); ++iter) {
		printPair(*iter);
	}

	return 0;
}

