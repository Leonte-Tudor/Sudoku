#pragma once
#include <iostream>

class Sudoku {
protected:
	int size;
	int sqs;
	int** grid;
public:
	Sudoku(int size);
	int** getgrid();
	int getsqs();
	void shuffle();
	void transpose();
	void swaplines();
	void swapcols();
	void disp();
	void delnums(int);
	int sumline(int);
	int sumcol(int);

};