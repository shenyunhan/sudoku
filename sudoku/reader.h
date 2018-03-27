#pragma once
class Reader
{
private:
	FILE* index;
	HANDLE hThread;
	bool eof;
	int puzzle[9][9];
	HANDLE hFetchEvent, hReadEvent;

public:
	Reader(FILE*);
	~Reader();
	bool fetch(int (&)[9][9]);
	friend void WINAPI input_main(void*);
};

