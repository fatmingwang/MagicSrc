#ifndef GRID_H
#define GRID_H

#include <stack>
#include <vector>
#include "cell.h"
//#include "ncurses.h"

class Grid
{
	//let random be more stright.
	struct sRandomData
	{
		eDirection Direction;
		int iKeepDirectionCountMax;
		int iKeepDirectionCountSmall;
		int iRestTimeToKeepDirectionC;
	};
private:
  int width;
  int height;

  std::vector<Cell> cells;
  std::stack<Cell *> backtrace;
  Cell *m_pCurrent;

  void createCells();
  int CalculateIndex(int i, int j);
  std::vector<Cell *> getAvailableNeighbors();
  Cell *findNextCell();
  void render();

 public:
  Grid(int width, int height);
  //-1 for generator all at once
  void generateMaze(int e_iStep = -1);
};

#endif