#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "BKTree.h"

void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}

//Makes all of the characters in a string lowercase
void makeStringAllLowercase(std::string &inputString)
{
	int inputStringLength = inputString.length();

	//Loop over every character except null character
	for (int i = 0; i < inputStringLength; i++)
	{
		inputString[i] = tolower(inputString[i]);
	}
}

//Custom string tokenizer for specific use case:
// 
//Takes (by reference) string to tokenize as first parameter
//Takes (by reference) string that constains delimiter(s)
//Takes (by reference) int as index for the starting character of the next token
// 
//Loop over string from starting index as nextTokenIndex comparing each character with each delimiter and if a delimiter is found,
//determine the length of the token to be returned, then find the index of the string where the character is not one of the delimiter(s),
//then set nextTokenIndex to that token and return the token that had its length determined
std::string stringTokenize(std::string& inputString, std::string& delimiters, int& nextTokenIndex)
{
	//Length of input string
	int inputStringLength = inputString.length();

	//Exception handling
	if (nextTokenIndex >= inputStringLength || nextTokenIndex < 0)
	{
		return std::string();
	}

	//Delimiter string may have multiple delimiters
	int delimitersLength = delimiters.length();

	//Shortcut if there are no delimiters
	if (delimitersLength == 0)
	{
		return inputString.substr(nextTokenIndex, inputStringLength);
	}

	//Stores the value of nextTokenIndex at the start of this function
	int startingIndex = nextTokenIndex;

	//This is the length of the token that will be returned, currently initialized to 0
	int lengthOfToken = 0;

	//After a delimiter is found, check for the next time a delimiter is not found is done
	//This counter tells how many times the current character was not one of the delimiters
	int delimiterNotFoundCount = 0;

	//Flag to see if length of tokens was set already
	bool lengthOfTokenSet = false;

	//Loop over all characters in inputString from starting index
	for (int i = startingIndex; i < inputStringLength; i++)
	{
		//Loop over every delimiter for current character
		for (int j = 0; j < delimitersLength; j++)
		{
			//If the current character is one of the delimiter(s)
			if (inputString[i] == delimiters[j])
			{
				//If first time delmiter is encountered, then set length of the token to be returned
				if (!lengthOfTokenSet)
				{
					lengthOfTokenSet = true;

					lengthOfToken = (i - startingIndex);

					//Skip inner for loop
					j = delimitersLength;
				}
				else
				{
					//Skip inner for loop
					j = delimitersLength;
				}
			}
			//If, at the last iteration of the delimiter loop, none of the delimiter(s) matched 
			else if (lengthOfTokenSet && (j == delimitersLength - 1) && (delimiterNotFoundCount == delimitersLength - 1))
			{
				//This is the index of the start of the next token
				nextTokenIndex = i;

				//Return the current token
				return inputString.substr(startingIndex, lengthOfToken);
			}
			//If the current character does not match one of the delimiters, then increment counter
			else if (lengthOfTokenSet && inputString[i] != delimiters[j])
			{
				delimiterNotFoundCount++;
			}
		}

		//If delimiter was not found on current character, then counter is set to 0 for next character
		delimiterNotFoundCount = 0;
	}

	//Set nextTokenIndex to index of null character in string
	nextTokenIndex = inputStringLength;

	//Returns last token
	return inputString.substr(startingIndex, (inputStringLength - startingIndex));
}

//Tokenizes each word in .txt file and inserts into tree
void readTextFileIntoTree(BKTree& inputTree, std::string pathToFile)
{
	//Check if file is .txt file
	if (pathToFile.find_last_of(".txt") == std::string::npos)
	{
		std::cerr << "Error: File is not .txt file\n";

		exit(1);
	}

	//Open .txt file
	std::ifstream inputFile(pathToFile);

	//Check if file exists
	if (!inputFile)
	{
		std::cerr << "Error: File not found\n";

		exit(1);
	}

	//Get whole file into stringstream, then put whole string stream into buffer
	std::stringstream stringStream;
	stringStream << inputFile.rdbuf();
	std::string buffer = stringStream.str();

	//Tokenize buffer and insert each word into tree using loop
	std::string delimiters = " ,.\n\t\r:;(){}[]?!-";
	int nextTokenIndex = 0;
	std::string currentWord = stringTokenize(buffer, delimiters, nextTokenIndex);

	while (!currentWord.empty())
	{
		makeStringAllLowercase(currentWord);

		inputTree.insertWord(currentWord);

		currentWord = stringTokenize(buffer, delimiters, nextTokenIndex);
	}

	//Close text file
	inputFile.close();
}