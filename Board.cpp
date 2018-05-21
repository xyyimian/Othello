#include"Board.h"
#include <vector>
//#include <fstream>  
using namespace std;


BlockStatus (*Board::getBoard())[8] {
	return m_board;
}



Board::Board() : isGameOver(false), isFull(false), isnoAvailable(false) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			m_board[i][j] = BlockStatus::EMPTY;
		}
	}
	m_board[4][4] = m_board[3][3] = BlockStatus::BLACK;
	m_board[3][4] = m_board[4][3] = BlockStatus::WHITE;
	m_board[2][4] = m_board[3][5] = m_board[4][2] = m_board[5][3] = BlockStatus::AVAILABLE;
}

Board::~Board() {}

/*
/*
* You should complete the functions below.
* The things you could consider:
* 1. Reverse the pieces to Black
* 1. Compute all available position for AI.
* 2. Design an algorithm to make the best choice. And then set corresponding position to "WHITE". Reverse the pieces afterwards.
* 3. Recompute all available position for player and set correspoding position to "AVAILABLE"
*/






void Board::mouseClick(int i, int j) {
	m_board[i][j] = BlockStatus::BLACK;
}


void Board::restart() {

}


void Board::Reverse(int i, int j) {

}
void Board::robot() {	

}

void Board::setAvailable() {

}


