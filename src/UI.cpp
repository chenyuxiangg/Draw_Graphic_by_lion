#include "UI.h"
#include "Cyuyi.h"
#include "CYufa.h"
#include <fstream>
#include <memory.h>
#include <cmath>

extern CYufa* g_yufa;

UI* UI::m_instance = nullptr;
size_t UI::m_line = 0;

UI* UI::Getinstance(){
	if(m_instance == nullptr){
		m_instance = new UI();
	}
	return m_instance;
}

void UI::myInit(HINSTANCE hInstance){
	m_hwnd = nullptr;
	m_hdc = nullptr;
	
	WNDCLASSEX myClass;
	
	memset(&myClass,0,sizeof(myClass));
	myClass.cbSize		 = sizeof(WNDCLASSEX);
	myClass.lpfnWndProc	 = WindowProc;
	myClass.hInstance	 = hInstance;
	
	myClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	myClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	myClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	myClass.lpszClassName = TEXT("myclass");
	
	if(!RegisterClassEx(&myClass)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"),TEXT("Error!"),MB_ICONEXCLAMATION|MB_OK);
		return;
	}
	
	m_hwnd = CreateWindowEx(NULL,myClass.lpszClassName,TEXT("翔哥绘图"),WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
	NULL,NULL,hInstance,NULL);
	
	if(m_hwnd == nullptr){
		MessageBox(NULL, TEXT("Window Creation Failed!"),TEXT("Error!"),MB_ICONEXCLAMATION|MB_OK);
		return;
	}
	m_hdc = GetWindowDC(m_hwnd);
}



void UI::DrawPixel(HDC hdc,unsigned long x,unsigned long y){
	SetPixel(hdc,x,y,m_color);
}


void UI::DrawLoop(double start,double end,double step,ExprNode* x_nptr,ExprNode* y_nptr){
	extern double* g_parameter;
	extern Cyuyi* g_yuyi;
	for(*g_parameter = start;*g_parameter <= end;*g_parameter+=step){
		g_yuyi->CalcCoord(x_nptr,y_nptr);
		Point point = g_yuyi->GetCurrentPoint();
		DrawPixel(m_hdc,(unsigned long)point.x,(unsigned long)point.y);
	}
}

void UI::SetColor(COLORREF col){
	m_color = col;
}

HWND UI::Gethwnd(){
	return m_hwnd;
}

LRESULT CALLBACK UI::WindowProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam){
	switch(umsg){
		case WM_DESTROY:
			extern Cyuyi* g_yuyi;
			extern CYufa* g_yufa;
			if(g_yuyi != nullptr){
				delete g_yuyi;
				g_yuyi = nullptr;
			}
			if(g_yufa != nullptr){
				delete g_yufa;
				g_yufa = nullptr;
			}
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN:
			{
				switch(wParam){
					case VK_RETURN:
						myStart();
						break;
					default:
						break;
				}
			}
			return 0;
		default:
			return DefWindowProc(hwnd,umsg,wParam,lParam);
	}
}

void UI::myStart(){

	//先刷新屏幕
	Getinstance()->Flush();

	std::ifstream infile;
	infile.open("test.txt");
	if(!infile.is_open()){
		MessageBox(NULL,TEXT("文件打开失败"),TEXT("error"),MB_OK);
		return;
	}
	char str[100];
	while(!infile.eof()){
		m_line++;
		memset(str,0,sizeof(str));
		infile.getline(str,100);
		g_yufa->Parser(str);
	}
	infile.close();
	m_line = 0;
}

size_t UI::GetLine() {
	return m_line;
}

void UI::Flush() {
	RECT rc;
	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	SelectObject(m_hdc, hBrush);
	//选取整个窗口
	GetClientRect(m_hwnd, &rc);
	//填充窗口
	FillRect(m_hdc, &rc, hBrush);
	//删除画刷节约资源
	DeleteObject(hBrush);
}
