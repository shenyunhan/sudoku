#pragma once
class Reader
{
private:
	FILE* index;
	bool eof;
	int idx;
	board_t puzzle;
	HANDLE hThread, hFetchEvent, hReadEvent;

public:
	Reader(FILE*);
	~Reader();
	bool fetch(int&, board_t&);
	friend void WINAPI input_main(void*);
};

