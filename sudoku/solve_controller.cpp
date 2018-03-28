#include "stdafx.h"
#include "solve_controller.h"


SolveController::SolveController(Reader* reader, Writer* writer)
	: solver1(reader, writer), solver2(reader, writer)
{
}


SolveController::~SolveController()
{
}


void SolveController::initialize()
{
	Solver::init();
}


void SolveController::start()
{
	solver1.start();
	solver2.start();
}

int SolveController::get_solved_cnt()
{
	return solver1.get_solved_cnt() + solver2.get_solved_cnt();
}

void SolveController::get_synchronize_objects(HANDLE& hObj1, HANDLE& hObj2)
{
	hObj1 = solver1.get_synchronize_object();
	hObj2 = solver2.get_synchronize_object();
}
