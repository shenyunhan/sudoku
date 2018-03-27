#include "stdafx.h"
#include "create_sudoku.h"

void create_sudoku(int n)
{
	FILE* output_index = fopen("sudoku.txt", "w");
	if (!output_index)
	{
		fprintf(stderr, "ERROR: unable to open output file!\n");
		exit(1);
	}
	int shift[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
	for (int i = 0; i < 6 && n; i++)
	{
		if (i)
		{
			std::next_permutation(shift + 3, shift + 6);
			shift[6] = 2, shift[7] = 5, shift[8] = 8;
		}
		for (int j = 0; j < 6 && n; j++)
		{
			if (j) std::next_permutation(shift + 6, shift + 9);
			char row[10] = "512346789";
			for (int k = 0; k < 40320 && n; k++)
			{
				if (k) std::next_permutation(row + 1, row + 9);
				for (int r = 0; r < 9; r++)
				{
					for (int c = 0; c < 9; c++)
					{
						if (c) fputc(' ', output_index);
						fputc(row[(c + shift[r]) % 9], output_index);
					}
					fputc('\n', output_index);
				}
				if (--n) fputc('\n', output_index);
				fflush(output_index);
			}
		}
	}
	fclose(output_index);
}