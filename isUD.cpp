//============================================================================
// Name        : isUD.cpp
// Author      : Denis Ievlev & Lior Shmuel
// Copyright   :
// Description : This program is checking the input code if it is uniquely decodable
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>

std::vector<std::pair<int, int> > vectorOfSets; //vector of input alphabet as a set
std::vector<std::pair<int, int> > C1; //first set
std::vector<std::pair<int, int> > C2; //second set

bool contains(std::vector<std::pair<int, int>> &vec, std::pair<int, int> elem) {
	for (std::vector<std::pair<int, int> >::iterator iter = vec.begin();
			iter != vec.end(); ++iter) {
		if (*iter == elem)
			return true;
	}
	return false;
}

bool isPrefix(std::pair<int, int> p1, std::pair<int, int> p2,
		std::vector<std::pair<int, int>> &vec) {
	int v1 = 1, v2 = 1;
	int minLength, maxLength;
	if (p1.first < p2.first) {
		minLength = p1.first;
		maxLength = p2.first;
	} else {
		minLength = p2.first;
		maxLength = p1.first;
	}
	for (int i = 0; i < p1.first - 1; i++) {
		v1 <<= 1;
	}

	for (int i = 0; i < p2.first - 1; ++i) {
		v2 <<= 1;
	}
	/*
	 * checking if the short number is prefix of the long number
	 * returns false when it is not a prefix.
	 */
	for (int i = minLength; i > 0; i--) {
		if (((((v1 & p1.second) == 0) && ((v2 & p2.second) == 0)))
				|| ((((v1 & p1.second) != 0)) && ((v2 & p2.second) != 0))) {
			v1 >>= 1;
			v2 >>= 1;
		} else
			return false;
	}
	/*
	 * If the short number is prefix of the long number,  add the
	 * suffix of the long one to the set of suffixes
	 * returns true, cause the short number is prefix of the long.
	 */
	v1 = 1;
	for (int i = 0; i < maxLength - minLength - 1; ++i) {
		v1 = (v1 << 1) + 1;
	}
	int elem = v1 & ((p1.first > p2.first) ? p1.second : p2.second);
	if (!contains(vec, std::make_pair(maxLength - minLength, elem))) {
		vec.push_back(std::make_pair(maxLength - minLength, elem));
	}
	return true;
}
/*
 * Prints one pair of the set
 * The first number represents the length of the word
 * The second number represents the word in decimal
 */
void printPair(std::pair<int, int> a) {
	std::cout << "(" << a.first << "," << std::bitset<5>(a.second) << ")";
}
/*
 * Parsing the input string.
 */
std::pair<int, int> parse(std::string &str) {
	std::istringstream st(str);
	std::string first;
	std::string second = "";
	std::getline(st, first, ',');
	first = first.substr(1);
	std::getline(st, second);
	second = second.substr(1, 2);
	int num1 = std::stoi(first);
	int num2 = std::stoi(second);
	return std::make_pair(num1, num2);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr
				<< "Not valid number of parameters. Please insert only to the path to the input file"
				<< std::endl;
		exit(0);
	}
	std::ifstream file;
	file.open(argv[1]);
	std::string myVec; //input lines
	if (!file.is_open())
		std::cerr << "Can't open the file. Please check the path to the file"
				<< std::endl;
	else
		while (getline(file, myVec)) {
			if (myVec != "") {
				vectorOfSets.push_back(parse(myVec));
			}
		}
	file.close();

	for (std::vector<std::pair<int, int> >::iterator it = vectorOfSets.begin();
			it != vectorOfSets.end(); ++it) {
		for (std::vector<std::pair<int, int> >::iterator iter = it;
				iter != vectorOfSets.end(); ++iter) {
			if ((iter + 1) != vectorOfSets.end()) {
				isPrefix(*it, *(iter + 1), C1);
			}
		}
	}
	//print C1
	std::cout << "C1 vector contains" << std::endl;

	for (std::vector<std::pair<int, int> >::iterator iter = C1.begin();
			iter != C1.end(); ++iter) {
		printPair(*iter);
	}
	std::cout << std::endl;
	for (std::vector<std::pair<int, int> >::iterator it = vectorOfSets.begin();
			it != vectorOfSets.end(); ++it) {
		for (std::vector<std::pair<int, int> >::iterator iter = C1.begin();
				iter != C1.end(); ++iter) {
			isPrefix(*it, *iter, C2);
		}
	}

	//print C2
	std::cout << "C2 vector contains" << std::endl;
	for (std::vector<std::pair<int, int> >::iterator iter = C2.begin();
			iter != C2.end(); ++iter) {
		printPair(*iter);
	}
	std::cout << "\n**************************************" << std::endl;
	if (C2.size() > 0)
		std::cout << "* The code is not uniquely decodable *" << std::endl;
	else
		std::cout << "* The code is uniquely decodable     *" << std::endl;
	std::cout << "**************************************" << std::endl;
	return 0;
}

