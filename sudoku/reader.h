#pragma once
class Reader
{
private:
	FILE* index;
	bool eof;
	board_t puzzle;
	HANDLE hThread, hFetchEvent, hReadEvent;

public:
	Reader(FILE*);
	~Reader();
	bool fetch(board_t&);
	friend void WINAPI input_main(void*);
};

