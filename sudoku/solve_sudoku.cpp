#include "stdafx.h"
#include "dancing_links.h"
#include "solve_sudoku.h"
#include "output_daemon.h"

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

DLX<325, 730> solver;
int solve_sudoku(FILE* input_index)
{
	HANDLE hSemaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);

	// Start output daemon thread.
	init_output_daemon(hSemaphore);
	_beginthread(output_main, 0, NULL);

	int n, puzzle[9][9];
	for (n = 0; ~fgetint(puzzle[0][0], input_index); n++)
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (i || j) fgetint(puzzle[i][j], input_index);
		solver.clear();
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				for (int k = 0; k < 9; k++)
				{
					if (puzzle[i][j] != 0 && puzzle[i][j] != k + 1)
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
		assert(pos.size() == 81);

		pass_board(n, std::move(pos));
		ReleaseSemaphore(hSemaphore, 1, NULL);
	}

	mark_output_kill();
	join_output_daemon();
	CloseHandle(hSemaphore);

	return n;
}