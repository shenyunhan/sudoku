#pragma once
class Reader
{
private:
	FILE* index;
	int id;
	board_t puzzle;
	HANDLE hThread, hEofEvent, hFetchEvent, hReadEvent;
	HANDLE hFetchWaitChain[2];

public:
	Reader(FILE*);
	~Reader();

	Reader(const Reader&) = delete;
	Reader(Reader&&) = delete;

	bool fetch(int&, board_t&);
	friend unsigned WINAPI input_main(void*);
};

