#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include "Utilities.h"
#include "SLList.h"
#include "BKTree.h"

void printResults(std::shared_ptr<SLList<std::string>>, std::string, std::string, bool);

int main()
{
	//Ask user to specify file
	std::string nameOfFile;

	std::cout << "Please specify a txt file (you may specify file with directory): ";
	
	getline(std::cin, nameOfFile);

	//Set up case sensitive BK Tree
	BKTree caseSensitiveTree;

	std::cout << "\nMaking tree 1...\n\n";

	//Since there are two BK Trees, multithreading is used
	//Insert each word from file into tree
	std::thread thread1(readTextFileIntoTree, std::ref(caseSensitiveTree), nameOfFile, true);
	
	//Set up case insensitive BK Tree
	BKTree caseInsensitiveTree;

	std::cout << "\nMaking tree 2...\n\n";

	//Insert each word from file into tree
	std::thread thread2(readTextFileIntoTree, std::ref(caseInsensitiveTree), nameOfFile, false);

	//Wait for both threads
	thread1.join();
	thread2.join();

	//If file is empty, exit the program
	if(caseSensitiveTree.isEmpty())
	{
		std::cerr << "Trees are empty.";
		
		exit(1);
	}

	//Word to fuzzy search, if the search is case sensitive and maximum edit distance of word(s) returned
	std::string wordToSearch;
	bool isCaseSensitive;
	int searchDistance;

	//Shared pointer to list of strings that will be filled with search results
	//This is to avoid memory leaks
	std::shared_ptr<SLList<std::string>> listOfWords;

	//Program loop
	char keepRunning = 'y';
	char isCaseSensitiveChoice = 'n';
	while (keepRunning == 'y' || keepRunning == 'Y')
	{
		//Clear console every loop
		clearScreen();

		//Ask the user for search query
		std::cout << "Enter word to search: ";
		std::cin >> wordToSearch;
		std::cout << std::endl;

		//Loop to ensure the user enters the correct option
		do
		{
			//Ask the user if search should be case sensitive
			std::cout << "Should the search be case sensitive? (y/n): ";
			std::cin >> isCaseSensitiveChoice;
		}
		while (isCaseSensitiveChoice != 'y' && isCaseSensitiveChoice != 'n' && isCaseSensitiveChoice != 'Y' && isCaseSensitiveChoice != 'N');

		isCaseSensitive = (isCaseSensitiveChoice == 'y' || isCaseSensitiveChoice == 'Y') ? true : false;

		//Ask the user for maximum edit distance
		std::cout << "\nEnter search distance for search: ";
		std::cin >> searchDistance;

		std::cout << "\nSearching tree...\n";

		//Get potential word matches either caseSensitiveTree or caseInsensitiveTree. Search distance is set to searchDistance
		if (isCaseSensitive)
		{
			listOfWords = caseSensitiveTree.fuzzySearch(wordToSearch, searchDistance);
		}
		else
		{
			makeStringAllLowercase(wordToSearch);

			listOfWords = caseInsensitiveTree.fuzzySearch(wordToSearch, searchDistance);
		}

		//Next screen on console
		clearScreen();

		//If there are no search results, ask user to increase search distance
		if (listOfWords->isEmpty())
		{
			std::cout << "No word match found, please increase search distance\n";
		}
		//If there are search results, display them
		else
		{
			printResults(listOfWords, wordToSearch, nameOfFile, isCaseSensitive);
		}
		
		//Delete list of strings for next search query

		//Loop to ensure the user enters the correct option
		do
		{
			std::cout << "\nWould you like to search again? (y/n): ";
			std::cin >> keepRunning;
		}
		while (keepRunning != 'y' && keepRunning != 'n' && keepRunning != 'Y' && keepRunning != 'N');
	}
	
	system("pause>0");

	return 0;
}

void printResults(std::shared_ptr<SLList<std::string>> listOfWords, std::string wordToSearch, std::string nameOfFile, bool isCaseSensitive = true)
{
	//Either wordToSearch or one of the search results from the fuzzy search is displayed
	std::string wordToDisplay;

	//Check if wordToSearch is present in search results, and set wordToDisplay to it if it is present
	for (int i = 0; i < listOfWords->getLength(); i++)
	{
		if (!wordToSearch.compare(listOfWords->getAtPos(i)))
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
		if (listOfWords->getLength() == 0)
		{
			std::cout << "\n\nDid not find any similar words, please increase search distance";

			return;
		}

		//Ask user to choose a word that came from the fuzzy search since wordToSearch was not found in txt file
		std::cout << "\n\nDid you mean: \n";

		//Display search results in a numbered fashion
		for (int i = 0; i < listOfWords->getLength(); i++)
		{
			std::cout << std::endl << '\t' << (i + 1) << ": " << listOfWords->getAtPos(i);
		}

		int choice = 0;
		
		//Loop to ensure the user enters the correct option
		while (choice <= 0 || choice > listOfWords->getLength())
		{
			std::cout << "\n\nChoice: ";
			std::cin >> choice;

			if (choice <= 0 || choice > listOfWords->getLength())
			{
				std::cerr << "\n\nError: Choice is out of range";

			}
			else
			{
				//Set wordToDisplay to the chosen word
				wordToDisplay.assign(listOfWords->getAtPos(choice - 1));
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
	std::string delimiters = " ,.\t:;(){}[]?!-";
	std::string currentWord;
	size_t nextTokenIndex = 0;

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
			if (!isCaseSensitive)
			{
				makeStringAllLowercase(currentWord);
			}

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