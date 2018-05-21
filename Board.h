#pragma once


enum BlockStatus {
	EMPTY,
	WHITE,
	BLACK,
	AVAILABLE
};

class Board {
public:

	BlockStatus(*getBoard())[8];
	bool isGameOver;	
	bool isFull;
	bool isnoAvailable;


	// You may need to modify constructor and destructor
	Board();
	~Board();

	// This function need to be implemented
	void mouseClick(int x,int y);
	void restart();
	void Reverse(int i, int j);
	void robot();
	void setAvailable();

private:
	BlockStatus m_board[8][8];

};
