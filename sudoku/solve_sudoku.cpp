#include "stdafx.h"
#include "dancing_links.h"
#include "solve_sudoku.h"
#include "reader.h"
#include "writer.h"

inline int encode(int r, int c, int v)
{
	return r * 81 + c * 9 + v + 1;
}

DLX<325, 730> solver;
int code[9][9][9];
int row[730][4];

int solve_sudoku(FILE* input_index)
{
	if (!input_index)
	{
		printf("ERROR: unable to open input file!\n");
		exit(1);
	}
	FILE* output_index = fopen("sudoku.txt", "w");
	if (!output_index)
	{
		fprintf(stderr, "ERROR: unable to open output file!\n");
		exit(1);
	}

	Reader reader(input_index);
	Writer writer(output_index);

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
				code[i][j][k] = encode(i, j, k);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
			{
				int r = code[i][j][k];
				row[r][0] = code[0][i][j];
				row[r][1] = code[1][i][k];
				row[r][2] = code[2][j][k];
				row[r][3] = code[3][i / 3 * 3 + j / 3][k];
			}
	int n, puzzle[9][9];
	for (n = 0; reader.fetch(puzzle); n++)
	{
		solver.clear();
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				for (int k = 0; k < 9; k++)
				{
					if (puzzle[i][j] != 0 && puzzle[i][j] != k + 1)
						continue;
					int r = code[i][j][k];
					solver.link(r, row[r], row[r] + 4);
				}
		std::vector<int> pos;
		solver.get_pos(pos);

		writer.pass_board(n, pos);
	}

	writer.set_kill();
	writer.join();

	return n;
}