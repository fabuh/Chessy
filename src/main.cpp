#include "pch.h"

//Console App
//int main(int argc, char* argv[])
//{
//    chessy::StartGame();
//	return 0;
//}

//Windows App
#include <windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	chessy::StartGame();
	return 0;
}