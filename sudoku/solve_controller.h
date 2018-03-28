#pragma once
#include "solver.h"
#include "reader.h"
#include "writer.h"

class SolveController
{
	int thread_cnt;
	Solver** solver;

public:
	SolveController(int, Reader*, Writer*);
	~SolveController();

	SolveController(const SolveController&) = delete;
	SolveController(SolveController&&) = delete;

	void start();
	int get_solved_cnt();

	void get_synchronize_objects(HANDLE*);

	static void initialize();
};

