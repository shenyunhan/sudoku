#pragma once
#include "solver.h"
#include "reader.h"
#include "writer.h"

class SolveController
{
	Solver solver1, solver2;

public:
	SolveController(Reader*, Writer*);
	~SolveController();

	void start();
	int get_solved_cnt();

	void get_synchronize_objects(HANDLE&, HANDLE&);

	static void initialize();
};

