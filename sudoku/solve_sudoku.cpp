#include "stdafx.h"
#include "solve_sudoku.h"
#include "reader.h"
#include "writer.h"
#include "solver.h"
#include "solve_controller.h"

void alert_thread_main(void* args)
{
	HANDLE* hWaitHandles = (HANDLE*)args;
	WaitForMultipleObjects(2, hWaitHandles, TRUE, INFINITE);
}

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

	SolveController::initialize();
	SolveController controller = SolveController(&reader, &writer);
	controller.start();

	// Solver::init();
	// Solver solver(&reader, &writer);
	// solver.start();

	HANDLE hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
	LARGE_INTEGER due;
	due.QuadPart = 0LL;
	SetWaitableTimer(hTimer, &due, 1000, NULL, NULL, FALSE);

	HANDLE* hControllerWaitHandles = new HANDLE[2];
	controller.get_synchronize_objects(hControllerWaitHandles[0], hControllerWaitHandles[1]);
	HANDLE hAlert = (HANDLE)_beginthread(alert_thread_main, 0, hControllerWaitHandles);

	HANDLE waitVector[2] = { hTimer, hAlert };
	for (;;)
	{
		int id = WaitForMultipleObjects(2, waitVector, FALSE, INFINITE);
		if (id == 0)
		{
			printf("%d sudoku puzzles solved.\r", controller.get_solved_cnt());
		}
		else break;
	}

	CloseHandle(hTimer);
	CloseHandle(hAlert);
	return controller.get_solved_cnt();
}