/**
 * test.cpp
 *
 * @class test
 * @brief Small program testing DLM API.
 * @date 21-05-2012
 */

#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "../api/dlm.h"

using namespace std;

int main(int argc, char **argv)
{
	DLM_init(argc, argv);
	char lock_type = 'l';
	int rid = 0;
	int type = 0;
	long timeout = 0;
	int result = 0;
	while(cin)
	{
		cout << "PID: " << getpid() - 3 << endl;
		cout << "Podaj rodzaj operacji (l, t, u): " << endl;
		cin >> lock_type;
		if(!cin)
			break;
		switch(lock_type)
		{
			case 'l':
				cout << "ZAKLADANIE BLOKADY" << endl;
				cout << "Podaj rid: " << endl;
				cin >> rid;
				cout << "Podaj typ lock'a: " << endl;
				cin >> type;
				cout << "Podaj timeout: " << endl;
				cin >> timeout;
				result = DLM_lock(rid, (LockType)type, timeout);
				cout << "Wynik operacji: " << result << endl;
				break;

			case 't':
				cout << "PROBA ZALOZENIA BLOKADY" << endl;
				cout << "Podaj rid: " << endl;
				cin >> rid;
				cout << "Podaj typ lock'a: " << endl;
				cin >> type;
				result = DLM_trylock(rid, (LockType)type);
				cout << "Wynik operacji: " << result << endl;
				break;
			case 'u':
				cout << "ZDEJMOWANIE BLOKADY" << endl;
				cout << "Podaj rid: " << endl;
				cin >> rid;
				result = DLM_unlock(rid);
				cout << "Wynik operacji: " << result << endl;
				break;
		}
	}
	return 0;
}
