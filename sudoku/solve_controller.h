#pragma once
#include "solver.h"
#include "reader.h"
#include "writer.h"

class SolveController
{
	std::vector<Solver> solver;

public:
	SolveController(int, Reader*, Writer*);
	~SolveController() = default;

	void start();
	int get_solved_cnt();

	void get_synchronize_objects(HANDLE*) const;

	static void initialize();
};

