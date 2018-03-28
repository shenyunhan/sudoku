#include "stdafx.h"
#include "solve_controller.h"

SolveController::SolveController(int n, Reader* reader, Writer* writer)
{
	for (int i = 0; i < n; i++)
		solver.emplace_back(reader, writer);
}

void SolveController::initialize()
{
	Solver::init();
}


void SolveController::start()
{
	for (auto& i : solver)
		i.start();
}

int SolveController::get_solved_cnt()
{
	int ret = 0;
	for (const auto& i : solver)
		ret += i.get_solved_cnt();
	return ret;
}

void SolveController::get_synchronize_objects(HANDLE* hObj) const
{
	for (int i = 0; i < (int)solver.size(); i++)
		hObj[i] = solver[i].get_synchronize_object();
}
