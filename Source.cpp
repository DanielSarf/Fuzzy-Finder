#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <vector>

#include "Utilities.h"
#include "BKTree.h"

void printResults(std::vector<std::string>&, std::string, std::string);

int main()
{
	//Ask user to specify file
	std::string pathToFile;

	std::cout << "Please specify a txt file (you may specify file with directory): ";
	
	getline(std::cin, pathToFile);

	std::cout << "\nMaking tree...\n\n";

	//Set up BK Tree
	BKTree tree;

	//Insert each word from file into tree
	readTextFileIntoTree(tree, pathToFile);

	//If file is empty, exit the program
	if(tree.isEmpty())
	{
		std::cerr << "Tree is empty.";
		
		exit(1);
	}

	//Word to fuzzy search and maximum edit distance of word(s) returned
	std::string wordToSearch;
	int searchDistance;

	//Vector of strings that will be filled with search results
	//This is to avoid memory leaks
	std::vector<std::string> vectorOfWords;

	//Program loop
	char keepRunning = 'y';
	while (keepRunning == 'y')
	{
		//Clear console every loop
		clearScreen();

		//Ask the user for search query
		std::cout << "Enter word to search: ";
		std::cin >> wordToSearch;
		std::cout << std::endl;

		//Ask the user for maximum edit distance
		std::cout << "\nEnter search distance for search: ";
		std::cin >> searchDistance;

		std::cout << "\nSearching tree...\n";

		makeStringAllLowercase(wordToSearch);

		vectorOfWords = tree.fuzzySearch(wordToSearch, searchDistance);

		//Next screen on console
		clearScreen();

		//If there are no search results, ask user to increase search distance
		if (vectorOfWords.empty())
		{
			std::cout << "No word match found, please increase search distance\n";
		}
		//If there are search results, display them
		else
		{
			printResults(vectorOfWords, wordToSearch, pathToFile);
		}
		
		//Delete vector of strings for next search query

		//Loop to ensure the user enters the correct option
		do
		{
			std::cout << "\nWould you like to search again? (y/n): ";
			std::cin >> keepRunning;
			keepRunning = tolower(keepRunning);
		}
		while (keepRunning != 'y' && keepRunning != 'n');
	}
	
	std::cin.get();

	return 0;
}

void printResults(std::vector<std::string>& vectorOfWords, std::string wordToSearch, std::string nameOfFile)
{
	//Either wordToSearch or one of the search results from the fuzzy search is displayed
	std::string wordToDisplay;

	//Check if wordToSearch is present in search results, and set wordToDisplay to it if it is present
	for (int i = 0; i < vectorOfWords.size(); i++)
	{
		if (!wordToSearch.compare(vectorOfWords.at(i)))
		{
			wordToDisplay.assign(wordToSearch);

			break;
		}
	}

	//Check if wordToSearch is not present in search results
	if (wordToDisplay.compare(wordToSearch))
	{
		std::cout << "Did not find \"" << wordToSearch << "\" in " << nameOfFile;

		//Check if there were no search results
		if (vectorOfWords.size() == 0)
		{
			std::cout << "\n\nDid not find any similar words, please increase search distance";

			return;
		}

		//Ask user to choose a word that came from the fuzzy search since wordToSearch was not found in txt file
		std::cout << "\n\nDid you mean: \n";

		//Display search results in a numbered fashion
		for (int i = 0; i < vectorOfWords.size(); i++)
		{
			std::cout << std::endl << '\t' << (i + 1) << ": " << vectorOfWords.at(i);
		}

		int choice = 0;
		
		//Loop to ensure the user enters the correct option
		while (choice <= 0 || choice > vectorOfWords.size())
		{
			std::cout << "\n\nChoice: ";
			std::cin >> choice;

			if (choice <= 0 || choice > vectorOfWords.size())
			{
				std::cerr << "\n\nError: Choice is out of range";

			}
			else
			{
				//Set wordToDisplay to the chosen word
				wordToDisplay.assign(vectorOfWords.at(choice - 1));
			}
		}
	}

	clearScreen();

	//Open .txt file
	std::ifstream inputFile(nameOfFile);

	//Check if file exists
	if (!inputFile)
	{
		std::cerr << "Error: File not found\n";

		exit(1);
	}

	//Get whole file into stringstream
	std::stringstream stringStream;
	stringStream << inputFile.rdbuf();

	//Setting up string tokenizer that may be used later
	std::string delimiters = " ,.\t\n\r:;(){}[]?!-";
	std::string currentWord;
	int nextTokenIndex = 0;

	//Information that needs to be displayed on console
	int numberOfOccurances = 0;
	int numberOfLines = 0;
	int numberOfWordsOnLine = 0;

	//Loop over each line in stringStream
	std::string currentLine;
	while (getline(stringStream, currentLine))
	{
		//With every new line, increment numberOfLines
		numberOfLines++;

		//Tokenize currentLine
		currentWord = stringTokenize(currentLine, delimiters, nextTokenIndex);

		while (!currentWord.empty())
		{
			makeStringAllLowercase(currentWord);

			//Keeps tracks of what number the current word is on current line
			numberOfWordsOnLine++;

			//If the currentWord is the same as the wordToDisplay
			if (!wordToDisplay.compare(currentWord))
			{
				//With everytime the wordToDisplay is encountered in the current line, increment numberOfOccurances
				numberOfOccurances++;

				//Display information about location of currentWord
				std::cout << '"' << wordToDisplay << "\" is word " << numberOfWordsOnLine << " on line " << numberOfLines << std::endl;
			}

			//Keep tokenizing currentLine
			currentWord = stringTokenize(currentLine, delimiters, nextTokenIndex);
		}

		//numberOfWordsOnLine and nextTokenIndex are set to 0 for next line
		numberOfWordsOnLine = 0;

		nextTokenIndex = 0;
	}

	//Close text file
	inputFile.close();

	//Display how many times wordToDisplay was found in the document
	std::cout << "\nFound " << numberOfOccurances << " instance(s) of \"" << wordToDisplay << "\" in " << nameOfFile << std::endl;
}