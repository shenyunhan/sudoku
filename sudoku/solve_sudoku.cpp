#include "stdafx.h"
#include "solve_sudoku.h"
#include "reader.h"
#include "writer.h"
#include "solver.h"

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

	Solver::init();
	Solver solver(&reader, &writer);
	solver.start();

	HANDLE hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
	LARGE_INTEGER due;
	due.QuadPart = 0LL;
	SetWaitableTimer(hTimer, &due, 1000, NULL, NULL, FALSE);

	HANDLE waitVector[2] = { hTimer, solver.get_synchronize_object() };
	for (;;)
	{
		int id = WaitForMultipleObjects(2, waitVector, FALSE, INFINITE);
		if (id == 0)
		{
			printf("%d sudoku puzzles solved.\r", solver.get_solved_cnt());
		}
		else break;
	}

	CloseHandle(hTimer);
	return solver.get_solved_cnt();
}