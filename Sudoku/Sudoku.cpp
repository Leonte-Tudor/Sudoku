#include "Sudoku.h"
#include "SudokuSolver.h"
#include <iostream>

Sudoku::Sudoku(int sqs) {
    int i, j, k = 1;
    this->sqs = sqs;
    this->size = sqs * sqs;
    this->grid = new int* [size];
    for (i = 0; i < size; i++)
        this->grid[i] = new int[size];

    for (i = 0; i < sqs; i++)
        for (j = 0; j < sqs; j++)
            this->grid[i][j] = k++;

    bool onesolution = true;
    bool dispsteps = false;
    bool dispallsol = false;

    SudokuSolver sudokusolver(this->grid, dispsteps, sqs);
    const clock_t begin_time = clock();
    sudokusolver.CalculateSolution(-1, -1, 0);
    std::cout << "The initial Sudoku:" << std::endl << std::endl;
    std::cout << "Time : " << float(clock() - begin_time) / CLOCKS_PER_SEC << " s";
    std::cout << std::endl << std::endl;
    sudokusolver.DisplayGrid();
    std::cout << std::endl << std::endl << std::endl;

    /*{
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {4, 5, 6, 7, 8, 9, 1, 2, 3},
            {7, 8, 9, 1, 2, 3, 4, 5, 6},
            {2, 3, 4, 5, 6, 7, 8, 9, 1},
            {5, 6, 7, 8, 9, 1, 2, 3, 4},
            {8, 9, 1, 2, 3, 4, 5, 6, 7},
            {3, 4, 5, 6, 7, 8, 9, 1, 2},
            {6, 7, 8, 9, 1, 2, 3, 4, 5},
            {9, 1, 2, 3, 4, 5, 6, 7, 8}
        }
        */
}

//Sudoku::~Sudoku()
//{
//    for (int i = 0; i < size; ++i)
//        delete[]grid[i];
//    delete[]grid;
//}
//Sudoku SudokuGen(int size) {
int** Sudoku::getgrid() {
    return grid;
}
int Sudoku::getsqs() {
    return sqs;
}
void Sudoku::disp() {
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
    std::cout << std::endl;
}

void Sudoku::transpose() {
    int** aux = new int* [size];
    int i, j;
    for (int i = 0; i < size; i++)
    {
        aux[i] = new int[size];
        for (int j = 0; j < size; j++)
            aux[i][j] = grid[i][j];
    }

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
            grid[j][i] = aux[i][j];
    }
    for (i = 0; i < size; i++)
        delete[] aux[i];
    delete[] aux;
};

void Sudoku::swaplines() {
    int l, lx;
    do
    {
        l = rand() % size;
        lx = rand() % sqs + l / sqs * sqs;
    } while (lx == l);
    int* aux = new int[size];
    int j;

    for (j = 0; j < size; j++)
    {
        aux[j] = grid[l][j];
        grid[l][j] = grid[lx][j];
        grid[lx][j] = aux[j];

    }
}

void Sudoku::swapcols() {
    int c, cx;
    do
    {
        c = rand() % size;
        cx = rand() % sqs + c / sqs * sqs;
    } while (cx == c);
    int* aux = new int[size];
    int i;

    for (i = 0; i < size; i++)
    {
        aux[i] = grid[i][c];
        grid[i][c] = grid[i][cx];
        grid[i][cx] = aux[i];

    }
}

void Sudoku::shuffle() {

    srand(time(NULL));

    transpose();

    int i, r = rand() % 25 + 10;
    for (i = 0; i <= r; i++)
    {
        swaplines();
        swapcols();
    }

    transpose();

}
int Sudoku::sumline(int r1) {
    int s = 0;
    for (int j = 0; j < size; j++)
        s = s + grid[r1][j];
    return s;
}
int Sudoku::sumcol(int r2) {
    int s = 0;
    for (int i = 0; i < size; i++)
        s = s + grid[i][r2];
    return s;
}
void Sudoku::delnums(int dif) {
    int i = 0, r1, r2, f;
    switch (dif) {
    case 1:
        f = size * size - (size * size / sqs + 2 * sqs);
        break;
    case 2:
        f = size * size - size * sqs;
        break;
    case 3:
        f = size * size - (size * sqs - sqs * (sqs - 1));
        break;
    }
    r1 = rand() % size;
    r2 = rand() % size;
    while (i < f)
    {
        while (grid[r1][r2] == 0 || sumline(r1) == grid[r1][r2] || sumcol(r2) == grid[r1][r2])
        {
            r1 = rand() % size;
            r2 = rand() % size;
        }
        grid[r1][r2] = 0;
        i++;
    }
    std::cout << "Numbers deleted: " << f << std::endl;
    std::cout << "Non-zero numbers remaining: " << size * size - f << std::endl;
};