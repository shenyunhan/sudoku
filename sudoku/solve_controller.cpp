#include "stdafx.h"
#include "solve_controller.h"


SolveController::SolveController(int thread_cnt, Reader* reader, Writer* writer) :
	thread_cnt(thread_cnt)
{
	solver = new Solver*[thread_cnt];
	for (int i = 0; i < thread_cnt; i++)
		solver[i] = new Solver(reader, writer);
}


SolveController::~SolveController()
{
	for (int i = 0; i < thread_cnt; i++)
		delete solver[i];
	delete[] solver;
}


void SolveController::initialize()
{
	Solver::init();
}


void SolveController::start()
{
	for (int i = 0; i < thread_cnt; i++)
		solver[i]->start();
}

int SolveController::get_solved_cnt()
{
	int ret = 0;
	for (int i = 0; i < thread_cnt; i++)
		ret += solver[i]->get_solved_cnt();
	return ret;
}

void SolveController::get_synchronize_objects(HANDLE* hObj)
{
	for (int i = 0; i < thread_cnt; i++)
		hObj[i] = solver[i]->get_synchronize_object();
}
