#ifndef NODE_HEAD_H
#define NODE_HEAD_H
#include <functional>

class Node
{
public:
	int row;
	int col;

	Node(int _row,int _col)
	{
		row = _row;
		col = _col;
	}
};

#endif