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

unsigned WINAPI input_main(void* args) 
{
	Reader* pReader = (Reader*)args;

	for (;;)
	{
		if (fgetint(pReader->puzzle[0][0], pReader->index) == -1)
		{
			SetEvent(pReader->hEofEvent);
			break;
		}

		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (i || j) fgetint(pReader->puzzle[i][j], pReader->index);

		++pReader->id;
		SignalObjectAndWait(pReader->hFetchEvent, pReader->hReadEvent, INFINITE, FALSE);
	}

	return 0;
}

Reader::Reader(FILE* index) : index(index), id(0)
{
	hEofEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	hFetchEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	hReadEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	hThread = (HANDLE)_beginthreadex(NULL, 0, input_main, this, 0, NULL);

	hFetchWaitChain[0] = hFetchEvent;
	hFetchWaitChain[1] = hEofEvent;
}

Reader::~Reader()
{
	fclose(index);
	CloseHandle(hEofEvent);
	CloseHandle(hFetchEvent);
	CloseHandle(hReadEvent);
	CloseHandle(hThread);
}

bool Reader::fetch(int& id, board_t& res)
{
	int ret = WaitForMultipleObjects(2, hFetchWaitChain, FALSE, INFINITE);
	if (ret != WAIT_OBJECT_0) return false;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			res[i][j] = puzzle[i][j];
	id = this->id;

	SetEvent(hReadEvent);
	return true;
}