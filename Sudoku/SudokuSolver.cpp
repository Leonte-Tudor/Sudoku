#include "SudokuSolver.h"

SudokuSolver::SudokuSolver(int** grid, bool dispsteps, int sqs)
{
	this->sqs = sqs;
	size = sqs * sqs;
	this->grid = grid;

	this->known = new bool* [size];
	for (int i = 0; i < size; ++i)
	{
		known[i] = new bool[size];
		for (int j = 0; j < size; ++j)
		{
			known[i][j] = grid[i][j] > 0;
		}
	}

	this->possibilities = new bool** [size];
	for (int i = 0; i < size; ++i)
	{
		possibilities[i] = new bool* [size];
		for (int j = 0; j < size; ++j)
		{
			possibilities[i][j] = new bool[size];
			for (int k = 0; k < size; ++k)
			{
				if (known[i][j])
				{
					possibilities[i][j][k] = (k == grid[i][j] - 1);
				}
				else
				{
					possibilities[i][j][k] = true;
				}
			}
		}
	}

	this->occurnumber = new int** [4];
	for (int t = 0; t < 4; ++t)
	{
		occurnumber[t] = new int* [size];
		for (int i = 0; i < size; ++i)
		{
			occurnumber[t][i] = new int[size];
			for (int j = 0; j < size; ++j)
			{
				occurnumber[t][i][j] = size;
			}
		}
	}

	this->dispsteps = dispsteps;
	this->justknown = new bool* [size];
	for (int i = 0; i < size; ++i)
	{
		justknown[i] = new bool[size];
		for (int j = 0; j < size; ++j)
		{
			justknown[i][j] = false;
		}
	}
}




// Display functions

void SudokuSolver::DisplayGrid()
{
	for (int i = 0; i < size; ++i)
	{
		if (i > 0 && i % sqs == 0)
		{
			std::cout << std::string(2 * sqs, '-');
			std::cout << "|";
			std::cout << std::string(2 * sqs + 1, '-');
			std::cout << "|";
			std::cout << std::string(2 * sqs, '-') << std::endl;
		}
		for (int j = 0; j < size; ++j)
		{
			if (j > 0 && j % sqs == 0)
			{
				std::cout << "| ";
			}
			std::cout << grid[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

void SudokuSolver::DisplayGridNice(int ai, int aj, int offset)
// ai and aj correspond to a position where we try a new possibility that we will display in red, cf. the resolution functions.
// offset increases as the program goes deeper in the possibility tree, which permitts to see clearly how the computer solves the sudoku
{
	for (int i = 0; i < size; ++i)
	{
		std::cout << std::string(offset, ' ');
		if (i > 0 && i % sqs == 0)
		{
			std::cout << std::string(2 * sqs, '-');
			std::cout << "|";
			std::cout << std::string(2 * sqs + 1, '-');
			std::cout << "|";
			std::cout << std::string(2 * sqs, '-') << std::endl;
			std::cout << std::string(offset, ' ');
		}
		for (int j = 0; j < size; ++j)
		{
			if (j > 0 && j % sqs == 0)
			{
				std::cout << "| ";
			}
			if (i == ai && j == aj)
			{
				std::cout << RED << grid[i][j] << RESET << ' ';       // Red means that we try a new branch of the possibility tree
			}
			else {
				if (justknown[i][j])
				{
					std::cout << BLUE << grid[i][j] << RESET << ' ';      // Blue means that it's a number we just found
				}
				else
					std::cout << grid[i][j] << ' ';
			}
		}
		std::cout << std::endl;
	}
}

void DisplayPossibilites(bool* posij, int kmin, int kmax)
{
	for (int k = kmin; k <= kmax; ++k)
	{
		if (posij[k])
		{
			std::cout << k + 1 << ' ';
		}
		else
		{
			std::cout << "  ";
		}
	}
}

void SudokuSolver::DisplayGridPossibilities()
// Displays all the possibilities left in a nice and visual way. Not used anywhere in the program, but feel free to play with it.
{
	int indstyle = 177;

	std::string betweenlines = std::string(18 * sqs + 15, '-');
	betweenlines[6 * sqs + 4] = char(indstyle);
	betweenlines[12 * sqs + 10] = char(indstyle);

	for (int i = 0; i < size; ++i)
	{
		// First line

		for (int j = 0; j < size; ++j)
		{
			if (known[i][j])
			{
				std::cout << "* * * ";
			}
			else
			{
				DisplayPossibilites(possibilities[i][j], 0, 2);
			}
			if (j < size - 1 && (j + 1) % sqs == 0)
			{
				std::cout << char(indstyle) << ' ';
			}
			else if (j < size - 1)
			{
				std::cout << "| ";
			}
		}

		std::cout << std::endl;



		// Second line

		for (int j = 0; j < size; ++j)
		{
			if (known[i][j])
			{
				std::cout << "* " << grid[i][j] << " * ";
			}
			else
			{
				DisplayPossibilites(possibilities[i][j], 3, 5);
			}
			if (j < size - 1 && (j + 1) % sqs == 0)
			{
				std::cout << char(indstyle) << ' ';
			}
			else if (j < size - 1)
			{
				std::cout << "| ";
			}
		}

		std::cout << std::endl;



		// Third line

		for (int j = 0; j < size; ++j)
		{
			if (known[i][j])
			{
				std::cout << "* * * ";
			}
			else
			{
				DisplayPossibilites(possibilities[i][j], 6, 8);
			}
			if (j < size - 1 && (j + 1) % sqs == 0)
			{
				std::cout << char(indstyle) << ' ';
			}
			else if (j < size - 1)
			{
				std::cout << "| ";
			}
		}

		std::cout << std::endl;




		// For beauty

		if (i < size - 1 && (i + 1) % sqs == 0)
		{
			std::cout << std::string(18 * sqs + 15, char(indstyle)) << std::endl;
		}
		else if (i < size - 1)
		{
			std::cout << betweenlines << std::endl;
		}
	}
}




// Basic update functions

void SudokuSolver::UpdateKnown()
// Updates the "known" array
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			known[i][j] = grid[i][j] > 0;
		}
	}
}

void SudokuSolver::UpdatePossibilities()
// Updates the "possibilities" array
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			for (int k = 0; k < size; ++k)
				possibilities[i][j][k] = true;
		}
	}

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (known[i][j])      // If we already know grid[i][j], every other number than grid[i][j] are set to false
			{
				for (int k = 0; k < size; ++k)
				{
					if (k != grid[i][j] - 1)
						possibilities[i][j][k] = false;
				}
			}
			else                 // If we don't know grid[i][j] already:
			{
				// By lines

				for (int j1 = 0; j1 < size; ++j1)
				{
					if (known[i][j1])
					{
						int k = grid[i][j1] - 1;
						possibilities[i][j][k] = false;
					}
				}

				// By column

				for (int i1 = 0; i1 < size; ++i1)
				{
					if (known[i1][j])
					{
						int k = grid[i1][j] - 1;
						possibilities[i][j][k] = false;
					}
				}

				// By box

				int imin = i / sqs;
				imin = imin * sqs;
				int jmin = j / sqs;
				jmin = jmin * sqs;

				for (int i1 = imin; i1 < imin + sqs; ++i1)
				{
					for (int j1 = jmin; j1 < jmin + sqs; ++j1)
					{
						if (known[i1][j1])
						{
							int k = grid[i1][j1] - 1;
							possibilities[i][j][k] = false;
						}
					}
				}
			}
		}
	}
}

void SudokuSolver::UpdateGrid()
// Updates the grid solution
{
	// Reset justknown

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
			justknown[i][j] = false;
	}

	// In this function, we update the grid's values whenever we get only one possibility 

	// Case 1: There is only one possibility in a cell

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (!known[i][j])
			{
				int nbposs = 0;
				int indk = -1;
				for (int k = 0; k < size; ++k)
				{
					if (possibilities[i][j][k])
					{
						nbposs += 1;
						indk = k;
					}
				}
				if (nbposs == 1)
				{
					grid[i][j] = indk + 1;
					justknown[i][j] = true;
				}
			}
		}
	}



	// Case 2: One number can be in only one place in its whole line

	for (int i = 0; i < size; ++i)
	{
		for (int k = 0; k < size; ++k)
		{
			int indj = -1;
			int nbposs = 0;
			for (int j = 0; j < size; ++j)
			{
				if (!known[i][j] && possibilities[i][j][k])
				{
					nbposs += 1;
					indj = j;
				}
			}
			if (nbposs == 1)
			{
				grid[i][indj] = k + 1;
				justknown[i][indj] = true;
			}
		}
	}

	// Case 3: One number can be in only one place in its whole column

	for (int j = 0; j < size; ++j)
	{
		for (int k = 0; k < size; ++k)
		{
			int indi = -1;
			int nbposs = 0;
			for (int i = 0; i < size; ++i)
			{
				if (!known[i][j] && possibilities[i][j][k])
				{
					nbposs += 1;
					indi = i;
				}
			}
			if (nbposs == 1)
			{
				grid[indi][j] = k + 1;
				justknown[indi][j] = true;
			}
		}
	}

	// Case 4: One number can be in only one place in its whole box

	for (int ci = 0; ci < sqs; ++ci)
	{
		for (int cj = 0; cj < sqs; ++cj)
		{
			int imin = ci * sqs;
			int jmin = cj * sqs;
			for (int k = 0; k < size; ++k)
			{
				int indi = -1;
				int indj = -1;
				int nbposs = 0;
				for (int i = imin; i < imin + sqs; ++i)
				{
					for (int j = jmin; j < jmin + sqs; ++j)
					{
						if (!known[i][j] && possibilities[i][j][k])
						{
							nbposs += 1;
							indi = i;
							indj = j;
						}
					}
				}
				if (nbposs == 1)
				{
					grid[indi][indj] = k + 1;
					justknown[indi][indj] = true;
				}
			}
		}
	}
}




// Functions necessary for exploring solutions

bool IsInjective(int* array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (array[i] > 0)
		{
			for (int j = i + 1; j < size; ++j)
			{
				if (array[j] == array[i])
				{
					return false;
				}
			}
		}
	}

	return true;
}

int Sum(int* array, int size)
{
	int rep = 0;
	for (int i = 0; i < size; ++i)
	{
		rep = rep + array[i];
	}

	return rep;
}

bool SudokuSolver::DetectError(int offset)
// Detects if there is an error in the current solution, so we know the branch we are currently exploring is a dead end
{
	// First, we detect whether there is an error in the grid itself

	// By line

	for (int i = 0; i < size; ++i)
	{
		if (!IsInjective(grid[i], size))
		{
			if (dispsteps)
			{
				std::cout << std::string(offset, ' ');
				std::cout << "There is a number that appears twice in line number " << i + 1 << " ";
			}
			return true;
		}
	}


	// By column

	for (int j = 0; j < size; ++j)
	{
		int* tab = new int[size];   // tab = grille[:][j]
		for (int i = 0; i < size; ++i)
		{
			tab[i] = grid[i][j];
		}

		if (!IsInjective(tab, size))
		{
			if (dispsteps)
			{
				std::cout << std::string(offset, ' ');
				std::cout << "There is a number that appears twice in column number " << j + 1 << " ";
			}
			return true;
		}
		delete[]tab;
	}


	// By box

	for (int ci = 0; ci < sqs; ++ci)
	{
		for (int cj = 0; cj < sqs; ++cj)
		{
			int* tab = new int[size];   // tab = numbers in box (ci,cj)
			int imin = sqs * ci;
			int imax = sqs * (ci + 1) - 1;
			int jmin = sqs * cj;
			int jmax = sqs * (cj + 1) - 1;
			for (int i = imin; i <= imax; ++i)
			{
				for (int j = jmin; j <= jmax; ++j)
				{
					tab[(i - imin) * sqs + j - jmin] = grid[i][j];
				}
			}

			if (!IsInjective(tab, size))
			{
				if (dispsteps)
				{
					std::cout << std::string(offset, ' ');
					std::cout << "There is a number that appears twice in box (" << ci + 1 << ", " << cj + 1 << ") ";
				}
				return true;
			}
			delete[]tab;
		}
	}





	// Secondly, we detect whether there is an error in the "possibilities" array

	// Case 1: There is a cell left without possibilities

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			int nbposs = 0;
			for (int k = 0; k < size; ++k)
			{
				if (possibilities[i][j][k])
					nbposs += 1;
			}

			if (nbposs == 0)
			{
				if (dispsteps)
				{
					std::cout << std::string(offset, ' ');
					std::cout << "The box (" << i + 1 << ", " << j + 1 << ") has no possibilities ";
				}
				return true;
			}
		}
	}


	// Case 2: There a line, a column or a box where a number has no more possibilities to appear

	for (int k = 0; k < size; ++k)
	{
		// By line

		for (int i = 0; i < size; ++i)
		{
			bool appears = false;

			for (int j = 0; j < size; ++j)
			{
				if (grid[i][j] == k + 1 || possibilities[i][j][k])
					appears = true;
			}
			if (!appears)
			{
				if (dispsteps) {
					std::cout << std::string(offset, ' ');
					std::cout << "There is no " << k + 1 << " in line " << i + 1 << " ";
				}
				return true;
			}
		}


		// By column

		for (int j = 0; j < size; ++j)
		{
			bool appears = false;

			for (int i = 0; i < size; ++i)
			{
				if (grid[i][j] == k + 1 || possibilities[i][j][k])
					appears = true;
			}
			if (!appears)
			{
				if (dispsteps)
				{
					std::cout << std::string(offset, ' ');
					std::cout << "There is no " << k + 1 << " in column " << j + 1 << " ";
				}
				return true;
			}
		}


		// By box

		for (int ci = 0; ci < sqs; ++ci)
		{
			for (int cj = 0; cj < sqs; ++cj)
			{
				bool appears = false;
				int imin = ci * sqs;
				int jmin = cj * sqs;

				for (int i = imin; i < imin + sqs; ++i)
				{
					for (int j = jmin; j < jmin + sqs; ++j)
					{
						if (grid[i][j] == k + 1 || possibilities[i][j][k])
							appears = true;
					}
				}
				if (!appears)
				{
					if (dispsteps)
					{
						std::cout << std::string(offset, ' ');
						std::cout << "There is no " << k + 1 << " in box (" << ci + 1 << ", " << cj + 1 << ") ";
					}
					return true;
				}
			}
		}
	}

	return false;
}

int CountTrue(bool* boolarray, int size)
// Counts the number of "true" entries
{
	int rep = 0;

	for (int i = 0; i < size; ++i)
	{
		if (boolarray[i])
			rep += 1;
	}

	return rep;
}
int CountTrue(bool** boolarray, int size)
// Counts the number of "true" entries
{
	int rep = 0;

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (boolarray[i][j])
				rep += 1;
		}
	}

	return rep;
}

void SudokuSolver::UpdateOccurrences()
// Updates the "occurnumber" array
{
	// By line

	for (int i = 0; i < size; ++i)
	{
		for (int k = 0; k < size; ++k)
		{
			int occurrences = 0;
			for (int j = 0; j < size; ++j)
			{
				if (grid[i][j] == k + 1)
				{
					occurrences = -1;
					break;
				}
				else
				{
					if (possibilities[i][j][k])
					{
						occurrences += 1;
					}
				}
			}
			occurnumber[0][i][k] = occurrences;
		}
	}


	// By column

	for (int j = 0; j < size; ++j)
	{
		for (int k = 0; k < size; ++k)
		{
			int occurrences = 0;
			for (int i = 0; i < size; ++i)
			{
				if (grid[i][j] == k + 1)
				{
					occurrences = -1;
					break;
				}
				else
				{
					if (possibilities[i][j][k])
					{
						occurrences += 1;
					}
				}
			}
			occurnumber[1][j][k] = occurrences;
		}
	}


	// By box

	for (int ci = 0; ci < sqs; ++ci)
	{
		for (int cj = 0; cj < sqs; ++cj)
		{
			for (int k = 0; k < size; ++k)
			{
				int occurrences = 0;
				int imin = ci * sqs;
				int jmin = cj * sqs;
				for (int i = imin; i < imin + sqs; ++i)
				{
					for (int j = jmin; j < jmin + sqs; ++j)
					{
						if (grid[i][j] == k + 1)
						{
							occurrences = -1;
							break;
						}
						else
						{
							if (possibilities[i][j][k])
							{
								occurrences += 1;
							}
						}
					}
				}
				occurnumber[2][ci * sqs + cj][k] = occurrences;
			}
		}
	}


	// By possibilites number per cell

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (known[i][j])
				occurnumber[3][i][j] = -1;
			else
			{
				occurnumber[3][i][j] = CountTrue(possibilities[i][j], size);
			}
		}
	}
}

int SudokuSolver::MinOccurrences()
// Calculates the minimum of the value returned by the above function. Will be taken in argument by the next function
{
	int rep = size;

	for (int t = 0; t < 4; ++t)
	{
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				if (occurnumber[t][i][j] > 0 && occurnumber[t][i][j] < rep)
					rep = occurnumber[t][i][j];
			}
		}
	}

	return rep;
}

std::list<int*> SudokuSolver::GetIndex(int m)
// returns the list of branches we have to explore in the possibility tree: for cand element of the list returned, we will try to set cand[2] at position (cand[0],cand[1]) and see if we have a solution.
// m is the minimum number of branches we will have to explore. The list returned will be of size m, so we will explore a minimum number of branches
{
	std::list<int*> rep = {};

	// By line

	for (int i = 0; i < size; ++i)
	{
		for (int k = 0; k < size; ++k)
		{
			if (occurnumber[0][i][k] == m)
			{
				int ind = 0;
				for (int j = 0; j < size; ++j)
				{
					if (possibilities[i][j][k])
					{
						int* repi = new int[3];
						repi[0] = i;
						repi[1] = j;
						repi[2] = k + 1;
						rep.push_back(repi);
						ind += 1;
					}
				}
				return rep;
			}
		}
	}


	// By column

	for (int j = 0; j < size; ++j)
	{
		for (int k = 0; k < size; ++k)
		{
			if (occurnumber[1][j][k] == m)
			{
				int ind = 0;
				for (int i = 0; i < size; ++i)
				{
					if (possibilities[i][j][k])
					{
						int* repi = new int[3];
						repi[0] = i;
						repi[1] = j;
						repi[2] = k + 1;
						rep.push_back(repi);
						ind += 1;
					}
				}
				return rep;
			}
		}
	}




	// By box

	for (int ci = 0; ci < sqs; ++ci)
	{
		for (int cj = 0; cj < sqs; ++cj)
		{
			for (int k = 0; k < size; ++k)
			{
				if (occurnumber[2][ci * sqs + cj][k] == m)
				{
					int ind = 0;
					int imin = sqs * ci;
					int jmin = sqs * cj;
					for (int i = imin; i < imin + sqs; ++i)
					{
						for (int j = jmin; j < jmin + sqs; ++j)
						{
							if (possibilities[i][j][k])
							{
								int* repi = new int[3];
								repi[0] = i;
								repi[1] = j;
								repi[2] = k + 1;
								rep.push_back(repi);
								ind += 1;
							}
						}
					}
					return rep;
				}
			}
		}
	}



	// By possibilities number

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (occurnumber[3][i][j] == m)
			{
				int ind = 0;
				for (int k = 0; k < size; ++k)
				{
					if (possibilities[i][j][k])
					{
						int* repi = new int[3];
						repi[0] = i;
						repi[1] = j;
						repi[2] = k + 1;
						rep.push_back(repi);
						ind += 1;
					}
				}
				return rep;
			}
		}
	}

	return rep;
}




// Resolution functions

bool SudokuSolver::CalculateSolution(int ai, int aj, int rec)
// Calculates only one solution
{
	UpdateKnown();
	UpdatePossibilities();
	UpdateOccurrences();

	bool stop = false;
	int nbrec = 0;
	int aicopy = ai;
	int ajcopy = aj;

	do
	{
		UpdateGrid();
		if (CountTrue(justknown, size) == 0)       // if we haven't found any new cell, we stop the loop and start exploring several hypothesis, using the three functions defined above
		{
			stop = true;
		}

		UpdateKnown();
		UpdatePossibilities();
		UpdateOccurrences();

		if (dispsteps && (!stop || nbrec == 0))
		{
			DisplayGridNice(aicopy, ajcopy, 10 * rec);
			std::cout << std::endl << std::endl << std::endl;
		}

		nbrec = nbrec + 1;
		aicopy = -1;
		ajcopy = -1;

		if (DetectError(10 * rec))         // If we detect an error, we are in a dead end so it's useless going any further
		{
			if (ai >= 0 && aj >= 0 && dispsteps)
			{
				std::cout << "so we don't have a " << grid[ai][aj] << " at the index (" << ai + 1 << ", " << aj + 1 << ")" << std::endl << std::endl;
			}
			return false;
		}

	} while (!stop);

	if (CountTrue(known, size) == size * size)        // If we know all the cells, then it means we solved the sudoku
	{
		return true;
	}

	else {                     // Else, we explore solutions

		// Copy of grid to remember its value at the beginning of the loop

		int** gridcopy = new int* [size];
		for (int i = 0; i < size; ++i)
		{
			gridcopy[i] = new int[size];
			for (int j = 0; j < size; ++j)
			{
				gridcopy[i][j] = grid[i][j];
			}
		}

		int m = MinOccurrences();                             // We calculate the solutions to be tested in a smart way: we find the minimum number of hypothesis
		std::list<int*> index = GetIndex(m);                  // That covers every possibility, and then we test them all 
		for (int* cand : index)
		{
			int i = cand[0];
			int j = cand[1];
			int val = cand[2];
			grid[i][j] = val;

			if (dispsteps)
			{
				std::cout << std::string(10 * rec, ' ');
				std::cout << "We replace index (" << i + 1 << ", " << j + 1 << ") by " << val << std::endl << std::endl;
			}

			// Recursion with one of the hypothesis
			if (CalculateSolution(i, j, rec + 1))                       // If this hypothesis is true, then we have our solution             
			{
				return true;
			}
			else                                                        // If this hypothesis leads to a dead end, then we replace grid by gridcopy so we can try the next hyptohesis
			{
				for (int i = 0; i < size; ++i)
				{
					for (int j = 0; j < size; ++j)
						grid[i][j] = gridcopy[i][j];
				}
			}
		}

		if (dispsteps && ai >= 0 && aj >= 0)
		{
			std::cout << std::string(10 * (rec - 1), ' ');
			std::cout << "No possibility works, so we don't have a " << grid[ai][aj] << " ";
			std::cout << " at the index (" << ai + 1 << ", " << aj + 1 << ")" << std::endl << std::endl;
		}

		return false;
	}
}

void SudokuSolver::CalculateEverySolutions(std::list<int**>* listposs, int ai, int aj, int rec)
// Calculates every solution, and adds them to "listposs"
{
	UpdateKnown();
	UpdatePossibilities();
	UpdateOccurrences();

	bool stop = false;
	int nbrec = 0;
	int aicopy = ai;
	int ajcopy = aj;

	do
	{
		UpdateGrid();
		if (CountTrue(justknown, size) == 0)                      // if we haven't found any new cell, we stop the loop and start exploring several hypothesis, using the three functions defined above
		{
			stop = true;
		}

		UpdateKnown();
		UpdatePossibilities();
		UpdateOccurrences();

		if (dispsteps && (!stop || nbrec == 0))
		{
			DisplayGridNice(aicopy, ajcopy, 10 * rec);
			std::cout << std::endl << std::endl << std::endl;
		}

		nbrec = nbrec + 1;
		aicopy = -1;
		ajcopy = -1;

		if (DetectError(10 * rec))                     // If we detect an error, we are in a dead end so it's useless going any further
		{
			if (ai >= 0 && aj >= 0 && dispsteps)
			{
				std::cout << "so we don't have a " << grid[ai][aj] << " at the index (" << ai + 1 << ", " << aj + 1 << ")" << std::endl << std::endl;
			}
			return;
		}

	} while (!stop);

	if (CountTrue(known, size) == size * size)                           // If we know all the cells, then it means we solved the sudoku
	{
		{
			// We have to copy the grid every time otherwise every element of the list will have the same pointer

			int** gridcopy = new int* [size];
			for (int i = 0; i < size; ++i)
			{
				gridcopy[i] = new int[size];
				for (int j = 0; j < size; ++j)
				{
					gridcopy[i][j] = grid[i][j];
				}
			}
			(*listposs).push_back(gridcopy);               // If we have a solution, then we simply add it to "listposs"
		}
		if (dispsteps)
			std::cout << "A solution has been found" << std::endl << std::endl << std::endl;
	}

	else {                           // Else, we explore solutions

		// Copy of grid to remember its value at the beginning of the loop

		int** gridcopy = new int* [size];
		for (int i = 0; i < size; ++i)
		{
			gridcopy[i] = new int[size];
			for (int j = 0; j < size; ++j)
			{
				gridcopy[i][j] = grid[i][j];
			}
		}

		int m = MinOccurrences();                                    // We calculate the solutions to be tested in a smart way: we find the minimum number of hypothesis
		std::list<int*> index = GetIndex(m);                         // That covers every possibility, and then we test them all 

		for (int* cand : index)
		{
			int i = cand[0];
			int j = cand[1];
			int val = cand[2];
			grid[i][j] = val;

			if (dispsteps)
			{
				std::cout << std::string(10 * rec, ' ');
				std::cout << "We replace index (" << i + 1 << ", " << j + 1 << ") by " << val << std::endl << std::endl;
			}

			// Recursion with one of the hypothesis 
			CalculateEverySolutions(listposs, i, j, rec + 1);

			for (int i = 0; i < size; ++i)
			{
				for (int j = 0; j < size; ++j)
					grid[i][j] = gridcopy[i][j];
			}
		}

		for (int i = 0; i < size; ++i)                    // We delete gridcopy not to fill the memory
			delete[]gridcopy[i];
		delete[]gridcopy;

		if (dispsteps && ai >= 0 && aj >= 0)
		{
			std::cout << std::string(10 * (rec - 1), ' ');
			std::cout << "We finished exploring the solution with a " << grid[ai][aj];
			std::cout << " at the index (" << ai + 1 << ", " << aj + 1 << ")" << std::endl << std::endl;
		}
	}
}