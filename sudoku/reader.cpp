#include "stdafx.h"
#include "reader.h"

inline char fgetch(FILE* index)
{
	static const int siz = 1 << 15;
	static char buffer[siz], *s = buffer, *t = buffer;
	if (s == t) s = buffer, t = s + fread(buffer, 1, siz, index);
	return s == t ? 0 : *(s++);
}

inline int fgetint(int& res, FILE* index)
{
	res = 0;
	char ch;
	do
	{
		ch = fgetch(index);
		if (ch == 0) return -1;
	} while (!isdigit(ch) && ch != '-');
	do
	{
		res = res * 10 + ch - '0';
		ch = fgetch(index);
	} while (isdigit(ch));
	return 1;
}

void WINAPI input_main(void* args) 
{
	Reader* pReader = (Reader*)args;

	for (;;)
	{
		if (fgetint(pReader->puzzle[0][0], pReader->index) == -1)
		{
			pReader->eof = true;
			SetEvent(pReader->hFetchEvent);
			CloseHandle(pReader->hFetchEvent);
			break;
		}

		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (i || j) fgetint(pReader->puzzle[i][j], pReader->index);

		++pReader->idx;
		SignalObjectAndWait(pReader->hFetchEvent, pReader->hReadEvent, INFINITE, FALSE);
	}
}

Reader::Reader(FILE* index) : index(index), eof(false), idx(0)
{
	hFetchEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	hReadEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	hThread = (HANDLE)_beginthread(input_main, 0, this);
}

Reader::~Reader()
{
	fclose(index);
	CloseHandle(hFetchEvent);
	CloseHandle(hReadEvent);
	CloseHandle(hThread);
}

bool Reader::fetch(int& idx, board_t& res)
{
	int ret = WaitForSingleObject(hFetchEvent, INFINITE);
	if (ret != WAIT_OBJECT_0) return false;

	if (eof) return false;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			res[i][j] = puzzle[i][j];
	idx = this->idx;

	SetEvent(hReadEvent);
	return true;
}