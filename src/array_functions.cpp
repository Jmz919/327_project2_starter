/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: keith
 */

//============================================================================
//	TODO add necessary includes here
//============================================================================
#include <fstream>
#include "array_functions.h"


//============================================================================
//	stuff you will need
//============================================================================

//TODO add a global array of entry structs (global to this file)
struct word {
	std::string wrd;
	int occur;
};

//TODO define a structure to track words and number of times they occur
struct word words[];

//TODO add variable to keep track of next available slot in array
int index = 0;

//TODO define all functions in header file

//zero out array that tracks words and their occurrences
void clearArray() {
	struct word temp[];
	words = temp;
}

//how many unique words are in array
int getArraySize() {
	return sizeof(words);
}

//get data at a particular location
std::string getArrayWordAt(int i) {
	return words[i].wrd;
}

int getArrayWord_NumbOccur_At(int i) {
	return words[i].occur;
}

/*loop through whole file, one line at a time
 * call processLine on each line
 * returns false: myfstream is not open
 *         true: otherwise*/
bool processFile(std::fstream &myfstream) {
	if (myfstream.is_open()) {
		std::string line;

		while (!myfstream.eof()) {
			getline(myfstream, line);
			processLine(line);
		}

		return true;
	}
	return false;
}

/*take 1 line and extract all the tokens from it
feed each token to processToken for recording*/
void processLine(std::string &myString) {
	std::string wrd = "";
	std::string line[100];
	int slot = 0;

	for (int i = 0; i < myString.length(); i++) {
		if (myString.substr(i, i+1) == " ") {
			line[slot] = wrd;
			slot++;
			if (slot == sizeof(line)) {
				std::string temp[slot];
				for (int i = 0; i < sizeof(line); i++) {
					temp[i] = line[i];
				}
				line = temp;
			}
			wrd = "";
		}
		else {
			wrd += myString.substr(i, i+1);
		}
	}

	for (int i = 0; i < sizeof(line); i++) {
		processToken(line[i]);
	}
}

/*Keep track of how many times each token seen*/
void processToken(std::string &token) {
	bool first = true;

	for (int i = 0; i < sizeof(words); i++) {
		if (token == words[i].wrd) {
			words[i].occur += 1;
			first = false;
			break;
		}
	}

	if (first) {
		struct word first_occur;
		first_occur.wrd = token;
		first_occur.occur = 1;
		words[index] = first_occur;
		index++;

		if (index == sizeof(words)) {
			struct word temp[index];
			for (int i = 0; i < sizeof(words); i++) {
				temp[i] = words[i];
			}
			words = temp;
		}
	}
}

/*if you are debugging the file must be in the project parent directory
  in this case Project2 with the .project and .cProject files*/
bool openFile(std::fstream& myfile, const std::string& myFileName,
		std::ios_base::openmode mode = std::ios_base::in) {
	myfile.open(myFileName.c_str(), mode);
	return myfile.is_open();
}

/*iff myfile is open then close it*/
void closeFile(std::fstream& myfile) {
	if (myfile.is_open()) {
		myfile.close();
	}
}

/* serializes all content in myEntryArray to file outputfilename
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * 			FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * 			SUCCESS if all data is written and outputfilename closes OK
 * */
int writeArraytoFile(const std::string &outputfilename) {
	using namespace std;
	using namespace constants;

	if (sizeof(words) == 0) {
		return FAIL_NO_ARRAY_DATA;
	}

	ofstream myEntryArray;
	myEntryArray.open(outputfilename.c_str());

	if (myEntryArray.is_open()) {
		myEntryArray << words;
		myEntryArray.close();
		return SUCCESS;
	}

	return FAIL_FILE_DID_NOT_OPEN;

}

/*
 * Sort myEntryArray based on so enum value.
 * You must provide a solution that handles alphabetic sorting (A-Z)
 * The presence of the enum implies a switch statement based on its value
 */
void sortArray(constants::sortOrder so) {
	using namespace constants;
	using namespace std;
	switch (so) {
	case (ASCENDING):
		for (int i = 0; i < sizeof(words) - 1; i++) {
			for (int j = 0; j < sizeof(words) - i - 1; j++) {
				if (words[j].wrd.compare(words[j+1].wrd) > 0) {
					struct word temp;
					temp.wrd = words[j].wrd;
					temp.occur = words[j].occur;

					words[j] = words[j+1];
					words[j+1] = temp;
				}
			}
		}
		break;
	case (DESCENDING):
		for (int i = sizeof(words) - 1; i < 0; i--) {
			for (int j = sizeof(words) - i - 1; j < 0; j--) {
				if (words[j].wrd.compare(words[j-1].wrd) > 0) {
					struct word temp;
					temp.wrd = words[j].wrd;
					temp.occur = words[j].occur;

					words[j] = words[j-1];
					words[j-1] = temp;
				}
			}
		}
		break;
	case (NUMBER_OCCURRENCES):
		for (int i = 0; i < sizeof(words) - 1; i++) {
			for (int j = 0; j < sizeof(words) - i - 1; j++) {
				if (words[j].occur < words[j+1].occur) {
					struct word temp;
					temp.wrd = words[j].wrd;
					temp.occur = words[j].occur;

					words[j] = words[j+1];
					words[j+1] = temp;
				}
			}
		}
		break;
	}
}
