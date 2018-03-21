#pragma once
#include <vector>
#include <windows.h>

void init_output_daemon(HANDLE);
void pass_board(int, const std::vector<int>&);
void WINAPI output_main(void*);
void mark_output_kill();
void join_output_daemon();
