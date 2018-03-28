#include "stdafx.h"
#include "solve_sudoku.h"
#include "reader.h"
#include "writer.h"
#include "solver.h"
#include "solve_controller.h"

#define SOLVER_THREAD_CNT (logical_processor_count / 2 + 1)

void alert_thread_main(void* args)
{
	HANDLE* hWaitHandles = (HANDLE*)args;
	WaitForMultipleObjects(SOLVER_THREAD_CNT, hWaitHandles, TRUE, INFINITE);
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
	SolveController controller(SOLVER_THREAD_CNT, &reader, &writer);
	controller.start();
	
	HANDLE hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
	LARGE_INTEGER due;
	due.QuadPart = 0LL;
	SetWaitableTimer(hTimer, &due, 1000, NULL, NULL, FALSE);

	HANDLE* hControllerWaitHandles = new HANDLE[SOLVER_THREAD_CNT];
	controller.get_synchronize_objects(hControllerWaitHandles);
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

	writer.set_kill();
	writer.join();

	delete[] hControllerWaitHandles;
	CloseHandle(hTimer);
	CloseHandle(hAlert);
	return controller.get_solved_cnt();
}

#undef SOLVER_THREAD_CNT