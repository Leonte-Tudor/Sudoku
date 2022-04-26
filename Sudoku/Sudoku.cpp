#include "Sudoku.h"
#include "SudokuSolver.h"
#include <iostream>

int count_digit(int number) {
    int count = 0;
    while (number != 0) {
        number = number / 10;
        count++;
    }
    return count;
}

void DisplayArray(int* array, int size)
{
    for (int i = 0; i < size; ++i)
    {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
}

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
    sudokusolver.CalculateSolution(-1, -1, 0);

    typing = false;
    currenti = -1;
    currentj = -1;
    typingsize = count_digit(size);
    typingtext = new int[typingsize];
    for (int i = 0; i < typingsize; ++i)
        typingtext[i] = -1;
    typingnum = 0;
    typingind = 0;
}

//Sudoku::Sudoku(const Sudoku& S)
//{
//    grid = S.grid;
//    size = S.size;
//    sqs = S.sqs;
//}
//
//Sudoku::~Sudoku()
//{
//    for (int i = 0; i < size; ++i)
//        delete[]grid[i];
//    delete[]grid;
//}

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


void Sudoku::UpdateTextbox(int i, int j)
{
    currenti = i;
    currentj = j;
    if (i < 0 || i >= size || j < 0 || j >= size)
        typing = false;
    else
    {
        typing = true;
        begin_time = clock();
    }
}


void Sudoku::IntKBInput(int k)
{
    if (k >= 0 && k <= 9)
    {
        if (typingind <= typingsize-1)
        {
            typingtext[typingind] = k;
            typingind += 1;
            typingnum *= 10;
            typingnum += k;
        }
    }
    else if (k == -1)
    {
        if (typingind >= 0)
        {
            typingind -= 1;
            typingtext[typingind] = -1;
            typingnum = typingnum / 10;
        }
    }
    else if (k == 10)
    {
        if (typingnum >= 1 && typingnum <= size)
        {
            grid[currenti][currentj] = typingnum;
            currenti = -1;
            currentj = -1;
        }
        for (int i = 0; i < typingsize; ++i)
            typingtext[i] = -1;
        typingnum = 0;
        typingind = 0;
    }
    DisplayArray(typingtext, typingsize);
}


void Sudoku::Update()
{
    timetypind = float(clock() - begin_time) / CLOCKS_PER_SEC;
}



