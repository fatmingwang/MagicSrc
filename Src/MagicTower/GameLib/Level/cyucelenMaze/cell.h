#ifndef CELL_H
#define CELL_H


class Cell {
 private:
  int row, column;
  bool walls[4];
  bool visited;
  //put some item here?
  bool m_bIsDeadEnd;
  enum direction { TOP, RIGHT, BOTTOM, LEFT ,MAX };

 public:
  Cell(int i, int j);

  void removeWalls(Cell &);

  void setWalls();
  void setVisited(bool);
  bool isVisited();

  int getRow();
  int getColumn();
  void	Render(){}
};

#endif