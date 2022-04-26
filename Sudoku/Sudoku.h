#pragma once
#include <iostream>

class Sudoku {
protected:
	int size;
	int sqs;
	int** grid;
	bool typing;
	clock_t begin_time;
public:
	float timetypind;
	int currenti, currentj;
	int* typingtext;
	int typingnum;
	int typingsize;
	int typingind;
public:
	Sudoku(int size);
	//Sudoku(const Sudoku&);
	//~Sudoku();
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

	void UpdateTextbox(int i, int j);
	void IntKBInput(int k);
	void Update();
	friend std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku)
	{
		out << sudoku.currenti << ' ' << sudoku.currentj;
		return out;
	}
};