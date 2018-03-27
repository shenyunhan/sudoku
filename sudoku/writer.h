#pragma once
#include "stdafx.h"

class Writer
{
private:
	FILE* index;
	bool kill;
	std::queue<int> idx;
	std::queue<std::vector<int>> boards;
	HANDLE hThread, semaphore;
	CRITICAL_SECTION lock;

public:
	Writer(FILE*);
	~Writer();
	void pass_board(int n, std::vector<int>&);
	void join();
	void set_kill();
	friend void WINAPI output_main(void*);
};

