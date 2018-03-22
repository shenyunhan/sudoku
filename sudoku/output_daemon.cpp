#include "stdafx.h"

std::queue<int> num;
std::queue<std::vector<int>> works;
CRITICAL_SECTION lock;
HANDLE semaphore;
HANDLE join;
bool kill;

void pass_board(int n, const std::vector<int>& pos)
{
	EnterCriticalSection(&lock);
	num.push(n);
	works.push(pos);
	LeaveCriticalSection(&lock);
}

inline void decode(int x, int& r, int& c, int& v)
{
	x--;
	v = x % 9, x /= 9;
	c = x % 9, x /= 9;
	r = x;
}

char board[10][10];

inline void get_board(const std::vector<int>& pos)
{
	for (auto i : pos)
	{
		int r, c, v;
		decode(i, r, c, v);
		board[r][c] = '1' + v;
	}
}

void init_output_daemon(HANDLE hEvent)
{
	semaphore = hEvent;
	InitializeCriticalSection(&lock);
	join = CreateMutexEx(NULL, NULL, 0, MUTEX_ALL_ACCESS);
}

void mark_output_kill()
{
	EnterCriticalSection(&lock);
	kill = true;
	LeaveCriticalSection(&lock);
}

void WINAPI output_main(void* args)
{
	FILE* output_index = fopen("sudoku.txt", "w");
	if (!output_index)
	{
		fprintf(stderr, "ERROR: unable to open output file!\n");
		exit(1);
	}
	for (;;)
	{
		WaitForSingleObject(semaphore, INFINITE);
		EnterCriticalSection(&lock);

		if (num.size() == 0 && kill)
		{
			LeaveCriticalSection(&lock);
			break;
		}

		int n = num.front();
		num.pop();
		get_board(works.front());
		works.pop();

		LeaveCriticalSection(&lock);

		if (n) fputc('\n', output_index);
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (j) fputc(' ', output_index);
				fputc(board[i][j], output_index);
			}
			fputc('\n', output_index);
		}
		fflush(output_index);
	}
	ReleaseMutex(join);
}

void join_output_daemon()
{
	WaitForSingleObject(join, INFINITE);
}
