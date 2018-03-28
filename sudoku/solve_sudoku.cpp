#include "stdafx.h"
#include "solve_sudoku.h"
#include "reader.h"
#include "writer.h"
#include "solver.h"
#include "solve_controller.h"

unsigned alert_thread_main(void* args)
{
	HANDLE* hWaitHandles = (HANDLE*)args;
	WaitForMultipleObjects(SOLVER_THREAD_CNT, hWaitHandles, TRUE, INFINITE);
	return 0;
}

int solve_sudoku(FILE* input_index)
{
	FILE* output_index = fopen("sudoku.txt", "w");
	if (!output_index)
	{
		fprintf(stderr, "ERROR: unable to open output file!\n");
		exit(1);
	}

	Reader reader(input_index);
	Writer writer(output_index);

	SolveController::initialize();
	SolveController controller = SolveController(SOLVER_THREAD_CNT, &reader, &writer);
	controller.start();

	HANDLE hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
	LARGE_INTEGER due;
	due.QuadPart = 0LL;
	SetWaitableTimer(hTimer, &due, 1000, NULL, NULL, FALSE);

	HANDLE* hControllerWaitHandles = new HANDLE[SOLVER_THREAD_CNT];
	controller.get_synchronize_objects(hControllerWaitHandles);
	HANDLE hAlert = (HANDLE)_beginthreadex(NULL, 0, alert_thread_main, hControllerWaitHandles, 0, NULL);

	HANDLE waitVector[2] = { hTimer, hAlert };
	for (;;)
	{
		int id = WaitForMultipleObjects(2, waitVector, FALSE, INFINITE);
		if (id == 0)
		{
			printf("%d sudoku puzzles solved.\n", controller.get_solved_cnt());
		}
		else break;
	}

	delete[] hControllerWaitHandles;
	CloseHandle(hTimer);
	CloseHandle(hAlert);
	return controller.get_solved_cnt();
}