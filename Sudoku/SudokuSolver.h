#pragma once
#include <iostream>
#include <list>
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"

class SudokuSolver
{
public:
	SudokuSolver(int** grid, bool dispsteps, int sqs);

	// Display functions 
	void DisplayGrid();
	void DisplayGridNice(int ai, int aj, int offset);
	void DisplayGridPossibilities();
	
	// Basic update functions
	void UpdateKnown();
	void UpdatePossibilities();
	void UpdateGrid();
	// Functions necessary for exploring solutions
	bool DetectError(int offset);
	void UpdateOccurrences();
	int MinOccurrences();
	std::list<int*> GetIndex(int m);

	// Calculating the solution(s)
	bool CalculateSolution(int ai, int aj, int rec);
	void CalculateEverySolutions(std::list<int**>* listposs, int ai, int aj, int rec);

public:
	int sqs;                       // Square-root of the size (= 3 for "classic" sudokus) 
	int size;

private:
	int** grid;
	bool** known;                  // true iff the corresponding cell of the grid is already known
	bool*** possibilities;         // All the possibilities left for each cell of the grid
	int*** occurnumber;            // Number of times each number appears in "possibilites" in each line, column and box and also how many possibilities each cell has left

	bool dispsteps;                // Do we plot the calculation steps?
	bool** justknown;              // true if we just found out with grid[i][j] is, false otherwise. Useful for the DisplayGridNice function.
};
