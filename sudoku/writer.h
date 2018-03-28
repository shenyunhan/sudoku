#pragma once
#include "stdafx.h"

class Writer
{
private:
	FILE* index;
	bool kill;
	volatile unsigned int next_id;
	std::queue<int> idx;
	std::queue<std::vector<int>> boards;
	HANDLE hThread, semaphore;
	CRITICAL_SECTION lock;

public:
	Writer(FILE*);
	~Writer();

	Writer(const Writer&) = delete;
	Writer(Writer&&) = delete;

	void pass(uint32_t, std::vector<int>&);
	void join();
	void set_kill();
	friend void WINAPI output_main(void*);
};
