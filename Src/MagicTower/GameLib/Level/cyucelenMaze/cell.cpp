#include "cell.h"
#include <stdlib.h>
#include <iostream>

Cell::Cell(int row, int column) {
  this->row = row;
  this->column = column;
  visited = false;
  m_bIsDeadEnd = false;
  setWalls();
}

void Cell::setWalls() {
  for (int i = 0; i < 4; i++) walls[i] = true;
}

void Cell::setVisited(bool b) { visited = b; }
bool Cell::isVisited() { return visited; }

int Cell::getRow() { return row; }
int Cell::getColumn() { return column; }

void Cell::removeWalls(Cell &next) {
  int x = column - next.column;
  int y = row - next.row;

  if (x == 1) {
    walls[LEFT] = false;
    next.walls[RIGHT] = false;
  } else if (x == -1) {
    walls[RIGHT] = false;
    next.walls[LEFT] = false;
  } else if (y == 1) {
    walls[TOP] = false;
    next.walls[BOTTOM] = false;
  } else if (y == -1) {
    walls[BOTTOM] = false;
    next.walls[TOP] = false;
  }
  int l_iNumDeadEnd = 0;
  for (int e_Direction = direction::TOP; e_Direction != direction::MAX; ++e_Direction)
  {
        if(!walls[e_Direction])
        {
            ++l_iNumDeadEnd;
        }
  }
  if (l_iNumDeadEnd >= 3)
  {
      m_bIsDeadEnd = true;
  }
  
}