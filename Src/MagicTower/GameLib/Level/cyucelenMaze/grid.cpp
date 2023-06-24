#include "grid.h"
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "cell.h"

Grid::Grid(int width, int height)
{
  this->width = width;
  this->height = height;
  createCells();
  m_pCurrent = &cells[0];
}

void Grid::createCells()
{
  for (int row = 0; row < height; row++)
  {
    for (int column = 0; column < width; column++)
    {
      cells.push_back(Cell(row, column));
    }
  }
}

void Grid::generateMaze(int e_iStep)
{
    int l_iNumStep = e_iStep;
    while (e_iStep == -1 || l_iNumStep)
    {
        --l_iNumStep;
        m_pCurrent->setVisited(true);
        Cell *next = findNextCell();

        if (next != nullptr)
        {
            next->setVisited(true);
            backtrace.push(m_pCurrent);
            m_pCurrent->removeWalls(*next);
            m_pCurrent = next;
        }
        else
        if (backtrace.size() > 0)
        {
            m_pCurrent = backtrace.top();
            backtrace.pop();
        }
        else 
        if (backtrace.size() == 0)
        {//finish
            break;
        }
    }
}

void Grid::render()
{
  for (Cell c : cells)
  {
    c.Render();
  }
}

Cell *Grid::findNextCell()
{
  std::vector<Cell *> availableNeighbors = getAvailableNeighbors();
  if (availableNeighbors.size() > 0)
  {
    return availableNeighbors.at(rand() % availableNeighbors.size());
  }

  return nullptr;
}

std::vector<Cell *> Grid::getAvailableNeighbors() {
  std::vector<Cell *> neighbors;

  int currentRow = m_pCurrent->getRow();
  int currentColumn = m_pCurrent->getColumn();

  int neighborIndexes[4] =
  {
      CalculateIndex(currentRow - 1, currentColumn),
      CalculateIndex(currentRow, currentColumn + 1),
      CalculateIndex(currentRow + 1, currentColumn),
      CalculateIndex(currentRow, currentColumn - 1),
  };

  for (int i : neighborIndexes)
  {
    if (i != -1 && !cells[i].isVisited())
    {
        neighbors.push_back(&cells[i]);
    }
  }

  return neighbors;
}

int Grid::CalculateIndex(int row, int column)
{
    if (row < 0 || column < 0 || column > width - 1 || row > height - 1)
    {
        return -1;
    }
    return column + row * width;
}
