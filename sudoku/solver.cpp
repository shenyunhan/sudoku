#include "stdafx.h"
#include "solver.h"

int Solver::code[9][9][9];
int Solver::row[730][4];

inline int encode(int r, int c, int v)
{
	return r * 81 + c * 9 + v + 1;
}

unsigned WINAPI work_main(void* args)
{
	Solver* pSolver = reinterpret_cast<Solver*>(args);
	board_t puzzle;
	int idx;
	while (pSolver->pReader->fetch(idx, puzzle))
	{
		pSolver->build(puzzle);
		std::vector<int> pos;
		pSolver->dlx.get_pos(pos);

		pSolver->pWriter->pass(idx, pos);
		++pSolver->solved_cnt;
	}

	return 0;
}

Solver::Solver(Reader* pReader, Writer* pWriter) :
	solved_cnt(0), dlx(), pReader(pReader), pWriter(pWriter)
{
	hThread = (HANDLE)_beginthreadex(NULL, 0, work_main, this, CREATE_SUSPENDED, NULL);
}

Solver::~Solver()
{
	CloseHandle(hThread);
}

void Solver::init()
{
	static volatile uint32_t initialized = FALSE;
	if (InterlockedCompareExchange(&initialized, TRUE, FALSE) == TRUE)
		return;

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
}

void Solver::build(const_board_t& puzzle)
{
	dlx.clear();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
			{
				if (puzzle[i][j] != 0 && puzzle[i][j] != k + 1)
					continue;
				int r = code[i][j][k];
				dlx.link(r, row[r], row[r] + 4);
			}
}

void Solver::start()
{
	ResumeThread(hThread);
}