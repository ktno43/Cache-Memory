/*****************************************
Kyle Nguyen
COMP 222
Spring 2018
Prof. Robert McIlhenny
T/R 3:30 P.M - 4:45 P.M

Assignment #2: Cache Memory
main.c

Version 3.0

The program works as expected and follows
program specifications.  It creates a
cache and main memory given input and
displays the proper hit/miss displays
when trying to access (read/write)
to/from memory.
*****************************************/

#include <stdio.h>
#include <stdlib.h>

// Function Declarations
void readOrWrite(void);
void accessCache(char cInput, int nMainMemAddr, int nData);
void displayMenu(void);
void setParamaters(void);
void freeStuff(void);

// Global Variables
int * g_pMainMem = NULL; // Dynamic array for main memory
int g_nMainMemSize = 0; // Main memory size
int g_nCacheSize = 0; // Cache size
int g_nBlockSize = 0; // Block size
int g_nNumLines = 0; // Number of lines in cache

					 // Structure of dynamic cache
struct cacheNode
{ // A cache node consists of a tag and a block
	int m_nTag; // Variable for tag
	int * m_pBlock; // Variable for block which is a dynamic array

} *cache = NULL;

typedef struct cacheNode line; // Define variable line for cache nodes

// Main Function
int main()
{
	displayMenu(); // Function call to Display Menu of Options

	printf("\n\nProgram Ending . . .\n");  //Program Ending
	printf("Press ENTER to close --->  ");
	getchar();
	getchar();

	return EXIT_SUCCESS;
}

// Display menu of options
void displayMenu()
{
	char cInput = '\0'; // Variable for input data

	while (cInput != 'c')
	{
		printf("Cache memory allocation and mapping:\n");
		printf("------------------------------------\n");
		printf("a) Enter parameters\n");
		printf("b) Access cache for reading/writing and transfer data\n");
		printf("c) Quit\n\n");

		printf("Enter selection:  ");
		scanf(" %c", &cInput); // Get the character pressed from user

		switch (cInput)
		{
			case 'a':
				setParamaters(); // Function call to assign values to parameters
				break;

			case 'b':
				readOrWrite(); // Function call to read/write
				break;

			case 'c':
				freeStuff(); // Function call to free dynamically allocated memory
				return;

			default:
				printf("Invalid input. \n"); // Invalid character read
		}
		printf("\n\n");
		getchar();
	}
	return;
}

// Function to set parameters
void setParamaters()
{
	// Input value for size of main memory
	printf("\nEnter main memory size (words): ");
	scanf("%d", &g_nMainMemSize);

	// Input value for size of cache
	printf("Enter cache size (words):  ");
	scanf("%d", &g_nCacheSize);

	// Input value for size of block
	printf("Enter block size (words/block):  ");
	scanf("%d", &g_nBlockSize);

	// Is main memory free?
	if (g_pMainMem != NULL)
		freeStuff();

	// Build Main memory
	g_pMainMem = (int *)malloc(g_nMainMemSize * sizeof(int));

	// Fill Main memory
	int i = 0;
	for (i = 0; i < g_nMainMemSize; i++)
	{
		g_pMainMem[i] = g_nMainMemSize - i; // Because specifications said so
	}

	// Is cache free?
	if (cache != NULL)
		freeStuff();

	// Build Cache
	g_nNumLines = g_nCacheSize / g_nBlockSize; // Set number of lines in cache given paramaters entered
	cache = (line *)malloc(g_nNumLines * sizeof(line)); // Allocate the cache according to the number of lines

	// Fill Cache
	int j = 0;
	for (j = 0; j < g_nNumLines; j++) { // For ever line in the cache, each tag = -1, and block = null
		cache[j].m_nTag = -1; // Assign to -1 because -1 will never be a tag
		cache[j].m_pBlock = NULL; // Assign to null because there's nothing in it
	}
	return;
}

void readOrWrite()
{
	int nInputFlag = 1; // Flag to stop loop
	char cInput = '\0'; // Variable for input data
	int nMainMemAddr = 0; // Memory address user wants to read/write from
	int nData = 0; // Variable for the value the user was looking for

	while (nInputFlag == 1)
	{
		printf("\nSelect read (r) or write (w):  ");
		scanf(" %c", &cInput); // Get the character pressed from user

		switch (cInput)
		{
			case 'r':
				printf("\nEnter main memory address to read from: ");
				scanf("%d", &nMainMemAddr);  // Input main memory address to read from
				break;

			case 'w':
				printf("\nEnter main memory address to write to: ");
				scanf("%d", &nMainMemAddr);  // Input main memory address to read from

				printf("Enter value to write: ");
				scanf("%d", &nData);  // Input main memory address to read from
				break;

			default:
				printf("Invalid input. \n"); // Invalid character read
		}

		if (cInput == 'r' || cInput == 'w')
		{ // Did they enter a valid character?
			nInputFlag = 0; // Yes so stop the loop
		}
	}
	accessCache(cInput, nMainMemAddr, nData); // Function call to access the cache
}

// Function to access the cache
void accessCache(char cInput, int nMainMemAddr, int nData)
{
	int nBase = (nMainMemAddr / g_nBlockSize) * g_nBlockSize; // Formula to calculate base (given)
	int nTag = nMainMemAddr / g_nCacheSize; // Formula to calculate tag (Ch4 slides)
	int nBlock = (nMainMemAddr % g_nCacheSize) / g_nBlockSize; // Formula to calculate block/set (Ch4 slides)
	int nWord = nMainMemAddr % g_nBlockSize; // Formula to calculate words (Ch4 slides)

	if (cache[nBlock].m_nTag == -1) // Don't want to compare with NULL, because dangerous, use the value given to default tag
	{
		cache[nBlock].m_pBlock = (int *)malloc(g_nBlockSize * sizeof(int)); // Create a new block
	}

	if (cache[nBlock].m_nTag != nTag)
	{ // Display read/write miss
		if (cInput == 'r')
			printf("\nRead miss!\n");

		else
			printf("\nWrite miss!\n");

		cache[nBlock].m_nTag = nTag; // Set the tag to the newly created block in cache

		int i = 0; // because c
		for (i = 0; i < g_nBlockSize; i++) // Transfer the equivalent block from main memory to newly created block in cache, one word at a time
		{
			cache[nBlock].m_pBlock[i] = g_pMainMem[nBase + i];
		}
	}

	else
	{ // Display read/write hit
		if (cInput == 'r')
			printf("\nRead hit!\n");

		else
			printf("\nWrite hit!\n");

	}

	// Transfer data to/from cache
	if (cInput == 'r')
		nData = cache[nBlock].m_pBlock[nWord];

	else
	{
		cache[nBlock].m_pBlock[nWord] = nData;
		g_pMainMem[nMainMemAddr] = nData;
	}

	// Display not so beautiful ASCII art
	printf("-------------------------------------------------\n");
	printf("|  Tag: %d  ", nTag);
	printf("|  Block: %d  ", nBlock);
	printf("|  Word: %d (%d)\t|\n", nWord, nData);
	printf("-------------------------------------------------\n");

	return;
}

void freeStuff() // Deallocate memory
{
	if (cache != NULL) // Is the cache null?
	{
		int i = 0;
		for (i = 0; i < g_nNumLines; i++) // It's not null so are all the lines in the cache null?
		{
			if (cache[i].m_nTag != -1) // If the tag isn't -1 it was dynamically allocated
			{
				free(cache[i].m_pBlock); // Free!!!!
				cache[i].m_pBlock = NULL;
			}
		}
		free(cache); // There's no more lines that were dynamically allocated, so free the cache now
		cache = NULL;
	}

	if (g_pMainMem != NULL) // Is the main memory null?
	{
		free(g_pMainMem); // Free!!!!!!
		g_pMainMem = NULL;
	}
	return;
}
