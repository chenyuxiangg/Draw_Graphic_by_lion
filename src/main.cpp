#include <iostream>
#include "CYufa.h"
#include "Cyuyi.h"
#include "UI.h" 
using namespace std; 

extern Cyuyi* g_yuyi = new Cyuyi(); 
extern CYufa* g_yufa = new CYufa();
extern double* g_parameter = &g_yufa->GetParameter();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	UI::Getinstance()->myInit(hInstance);
	HWND hwnd = UI::Getinstance()->Gethwnd();
	ShowWindow(hwnd,nCmdShow);
	
	MSG msg={};
	while(msg.message != WM_QUIT){
		if(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if(g_yuyi != nullptr){
		delete g_yuyi;
		g_yuyi = nullptr;
	}
	if(g_yufa != nullptr){
		delete g_yufa;
		g_yufa = nullptr;
	}
	return 0;
}


