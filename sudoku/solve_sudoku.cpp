#include "stdafx.h"
#include "dancing_links.h"
#include "solve_sudoku.h"

inline char fgetch(FILE* index)
{
	static const int siz = 1 << 15;
	static char buffer[siz], *s = buffer, *t = buffer;
	if (s == t) s = buffer, t = s + fread(buffer, 1, siz, index);
	return s == t ? 0 : *(s++);
}

inline int fgetint(int& res, FILE* index)
{
	res = 0;
	char ch;
	do
	{
		ch = fgetch(index);
		if (ch == 0) return -1;
	} while (!isdigit(ch) && ch != '-');
	do
	{
		res = res * 10 + ch - '0';
		ch = fgetch(index);
	} while (isdigit(ch));
	return 1;
}

inline int encode(int r, int c, int v)
{
	return r * 81 + c * 9 + v + 1;
}

inline void decode(int x, int& r, int& c, int& v)
{
	x--;
	v = x % 9, x /= 9;
	c = x % 9, x /= 9;
	r = x;
}

DLX<325, 730> solver;
int solve_sudoku(FILE* input_index)
{
	FILE* output_index = fopen("sudoku.txt", "w");
	if (!output_index)
	{
		fprintf(stderr, "ERROR: unable to open output file!\n");
		exit(1);
	}
	int n, board[9][9];
	for (n = 0; ~fgetint(board[0][0], input_index); n++)
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (i || j) fgetint(board[i][j], input_index);
		solver.clear();
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				for (int k = 0; k < 9; k++)
				{
					if (board[i][j] != 0 && board[i][j] != k + 1)
						continue;
					std::vector<int> c;
					c.push_back(encode(0, i, j));
					c.push_back(encode(1, i, k));
					c.push_back(encode(2, j, k));
					c.push_back(encode(3, i / 3 * 3 + j / 3, k));
					solver.add_row(encode(i, j, k), c.begin(), c.end());
				}
		std::vector<int> pos;
		solver.get_pos(pos);
		for (auto i : pos)
		{
			int r, c, v;
			decode(i, r, c, v);
			board[r][c] = v + 1;
		}
		if (n) fputc('\n', output_index);
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (j) fputc(' ', output_index);
				fprintf(output_index, "%d", board[i][j]);
			}
			fputc('\n', output_index);
		}
	}
	return n;
}