//====================================================================================================================
// Name        : isUD.cpp
// Author      : Denis Ievlev & Lior Shmuel
// Copyright   :
// Description : This program checks if the input code (format: (length,decimal representation)) is uniquely decodable
//====================================================================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>

std::vector<std::pair<int, int> > vectorOfSets; //vector of input alphabet as a set
std::vector<std::pair<int, int> > C1; //first set
std::vector<std::pair<int, int> > C2; //second set
std::vector<std::pair<int, int> > resOfInters; //result of intersection of two sets
bool contains(std::vector<std::pair<int, int>> &vec, std::pair<int, int> elem) {
	for (std::vector<std::pair<int, int> >::iterator iter = vec.begin();
			iter != vec.end(); ++iter) {
		if (*iter == elem)
			return true;
	}
	return false;
}
bool intersection(std::vector<std::pair<int, int>> &v1,
		std::vector<std::pair<int, int>> &v2,
		std::vector<std::pair<int, int>> &res) {
	for (std::vector<std::pair<int, int> >::iterator iter = v1.begin();
			iter != v1.end(); ++iter) {
		if (contains(v2, (std::pair<int, int>) *iter)) {
			res.push_back(*iter);
		}
	}
	if (res.size() > 0)
		return true;
	else
		return false;
}
/*
 * Prints one pair of the set
 * The first number represents the length of the word
 * The second number represents the word in decimal
 */
void printPair(std::pair<int, int> a) {
	std::cout << "(" << a.first << "," << std::bitset<5>(a.second) << ")";
}

void printVector(std::vector<std::pair<int, int>> &vec) {
	for (std::vector<std::pair<int, int> >::iterator iter = vec.begin();
			iter != vec.end(); ++iter)
		printPair(*iter);
	std::cout<<std::endl;

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
	int sufix = v1 & ((p1.first > p2.first) ? p1.second : p2.second);
	if (!contains(vec, std::make_pair(maxLength - minLength, sufix))) {
		vec.push_back(std::make_pair(maxLength - minLength, sufix));
	}
	return true;
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
	if (!file.is_open()) {
		std::cerr << "Can't open the file. Please check the path to the file"
				<< std::endl;
		exit(1);
	} else
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
	//print vector vectorOfSets
	std::cout
			<< "-----------------------------------------------------------------------------"
			<< std::endl;
	std::cout << "The given set of codewords is:" << std::endl;
	printVector(vectorOfSets);

	//print vector C1
	std::cout
			<< "-----------------------------------------------------------------------------"
			<< std::endl;
	std::cout
			<< "C1 set (all sufixes of codewords where the prefix of \nthe codeword is another codeword) contains:"
			<< std::endl;

	printVector(C1);

	for (std::vector<std::pair<int, int> >::iterator it = vectorOfSets.begin();
			it != vectorOfSets.end(); ++it) {
		for (std::vector<std::pair<int, int> >::iterator iter = C1.begin();
				iter != C1.end(); ++iter) {
			isPrefix(*it, *iter, C2);
		}
	}
	std::cout
			<< "-----------------------------------------------------------------------------"
			<< std::endl;

	//print vector C2
	std::cout
			<< "C2 set (the unioun of codewords in C1 vector where the words are prefix of \nanother codeword in vector C and contrariwise)  contains:"
			<< std::endl;
	printVector(C2);
	std::cout
			<< "-----------------------------------------------------------------------------"
			<< std::endl;

	std::cout << "\n**************************************" << std::endl;
	if (intersection(vectorOfSets, C2, resOfInters))
		std::cout << "* The code is not uniquely decodable *" << std::endl;
	else
		std::cout << "* The code is uniquely decodable     *" << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << "\nIntersection of input vector of codewords and C2 vector is: "
			<< std::endl;
	printVector(resOfInters);

	return 0;
}

