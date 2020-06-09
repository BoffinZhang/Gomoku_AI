#include "GameControl.h"
#include<cstdio>
int main() {
	//freopen("²âÊÔÊı¾İ.txt", "w", stdout);
	srand(0);
	AIPlayer ai;
	ai.TEST();
	GameControl gc = GameControl();
	gc.mainloop();
	return 0;
}
