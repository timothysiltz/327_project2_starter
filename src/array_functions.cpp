/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 21, 2019
 *      Author: Tim Siltz
 */

//============================================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "constants.h"
#include "utilities.h"
#include "array_functions.h"
//============================================================================
using namespace std;
using namespace constants;

/* structure to entry words and number of times they occur */
struct entry{
	string word;
	int number_occurrences;
};

/* global array of entry structs (global to this file) */
entry global_array[MAX_WORDS];

/* variable to keep entry of next available slot in array */
int next_slot = 0;

/* clears array */
void clearArray(){
	next_slot = 0;
}

/* gets size of array */
int getArraySize(){
	return next_slot;
}

/* gets word at index i */
std::string getArrayWordAt(int i){
	return global_array[i].word;
}

/* gets number of occurrences of word at index i */
int getArrayWord_NumbOccur_At(int i){
	return global_array[i].number_occurrences;
}

/* processes input file */
bool processFile(std::fstream &myfstream){
	if (!myfstream.is_open()){
		return false;
	}

	string line;

	while(!myfstream.eof()){
		getline(myfstream, line);
		processLine(line);
	}

	return true;
}

/* processes line into tokens */
void processLine(std::string &myString){
	stringstream ss(myString);
	string token;

	while(getline(ss, token, constants::CHAR_TO_SEARCH_FOR)){
		processToken(token);
	}
}

/* processes tokens into array */
void processToken(std::string &token){
	strip_unwanted_chars(token);

	if (!strip_unwanted_chars(token)){
		return;
	}

	else if (token != ""){
		string temp_token_1 = token;
		toUpper(temp_token_1);

		for(int i = 0; i < next_slot; i++){
			string temp_token_2 = global_array[i].word;
			toUpper(temp_token_2);

			if(temp_token_1 == temp_token_2){
				global_array[i].number_occurrences += 1;
				return;
			}
		}

		if (token.length() != 0){
			global_array[next_slot] = entry();
			global_array[next_slot].number_occurrences = 1;
			global_array[next_slot].word = token;
			next_slot += 1;
		}
	}
}

/* opens file */
bool openFile(std::fstream& myfile, const std::string& myFileName,
		std::ios_base::openmode mode){
	myfile.open(myFileName.c_str(), mode);
	processFile(myfile);
	return myfile.is_open();
}

/* closes open file */
void closeFile(std::fstream& myfile){
	if(myfile.is_open()){
		myfile.close();
	}
}

/* outputs array into file */
int writeArraytoFile(const std::string &outputfilename){
	ofstream output;
	output.open(outputfilename.c_str());
	if (!output.is_open()){
		return FAIL_FILE_DID_NOT_OPEN;
	}

	else if (next_slot == 0){
		output.close();
		return FAIL_NO_ARRAY_DATA;
	}

	else {
		for(int i = 0; i < next_slot; i++){
			output << global_array[i].word << " " << global_array[i].number_occurrences << endl;
		}

		output.close();
		return SUCCESS;
	}
}

/* sorts array (ascending) */
void sortArray(constants::sortOrder so){
	entry temp;

	switch (so) {
	case NONE:
		break;

	case ASCENDING:
		for(int i = 0; i < next_slot; i++){
			for(int j = i; j < next_slot; j++){
				string temp_word_1 = global_array[i].word;
				string temp_word_2 = global_array[j].word;
				toUpper(temp_word_1);
				toUpper(temp_word_2);

				if (temp_word_1 > temp_word_2) {
					temp = global_array[i];
					global_array[i] = global_array[j];
					global_array[j] = temp;
				}
			}
		}
		break;
	}
}
