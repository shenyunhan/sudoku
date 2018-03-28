#pragma once
#include "stdafx.h"
#include "dancing_links.h"
#include "reader.h"
#include "writer.h"

class Solver
{
private:
	static int code[9][9][9];
	static int row[730][4];
	int solved_cnt;
	DLX<325, 730> dlx;
	Reader* pReader;
	Writer* pWriter;
	HANDLE hThread;

	void build(const_board_t&);

public:
	Solver(Reader*, Writer*);
	~Solver();
	static void init();
	void start();

	int get_solved_cnt() const
	{
		return solved_cnt;
	}
	HANDLE get_synchronize_object() const
	{
		return hThread;
	}

	friend unsigned WINAPI work_main(void*);
};
