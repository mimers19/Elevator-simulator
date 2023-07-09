#include "stdafx.h"
#include "draw2.h"

HWND hwndButton, massfield;
LPCWSTR pClassName = L"WinM";
HDC hdc;
int wmId, wmEvent;
PAINTSTRUCT ps;

bool dir = true; // true - up || false - down

vector <person> prs;
vector <person> prs_elev;
elevator_c elevator;
const level lvl[5] = { {0,500}, {0, 385}, {0, 305}, {0, 185}, {0, 105} };
const RECT elevator_tunnel = {605, 55, 1195, 675};//to refresh drawings on only a part of the screen
const RECT floor_q = {100, 680, 2000, 100};
string temp;
wstring stemp;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK windowDrawing(HWND, HINSTANCE hInstance);
VOID OnPaint();
VOID NewPaint();
VOID Refresh(HWND hWnd, bool all);
VOID Const(HWND hWnd);
VOID go_to(HWND hWnd, int to);
VOID floor(HWND hWnd, int to, int from);
int where_to();
VOID mass_refresh(HWND hWnd);
VOID draw_man();
VOID draw_elev();
int floor_queue(int floor);

// main function (exe hInstance)
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	//Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	//creating window(expended)
	HWND hWnd = CreateWindowEx(0, pClassName, L"Vizualization in automatics", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, 1800, 900, nullptr, nullptr, hInstance, nullptr);

	windowDrawing(hWnd, hInstance);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//messeges
	MSG msg;
	BOOL gResult;
	do {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0);

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
	GdiplusShutdown(gdiplusToken);
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_E:
			go_to(hWnd, where_to());
			Refresh(hWnd, false);

			if (dir == true)
				elevator.poz.y--; //up
			else
				elevator.poz.y++; //down
			return 0L;
			break;
		case TIMER_D:
			KillTimer(hWnd, TIMER_S);
			go_to(hWnd, lvl[0].poz.y);
			Refresh(hWnd, false);

			if (dir == true)
				elevator.poz.y--; //up
			else
				elevator.poz.y++; //down
			if (lvl[0].poz.y == elevator.poz.y)
			{
				KillTimer(hWnd, TIMER_D);
			}
			return 0L;
				break;
		case TIMER_S:
			SetTimer(hWnd, TIMER_D, 10, (TIMERPROC)NULL);
			return 0L;
			break;
		default:
			break;
		}

	case WM_PAINT:

		Const(hWnd);
		mass_refresh(hWnd);
		
		return 0;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		////////////////////////////////////////////////////////////////////////////////////
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case F4T3:
			Const(hWnd);
			floor(hWnd, lvl[3].poz.y, lvl[4].poz.y);
			break;
		case F4T2:
			Const(hWnd);
			floor(hWnd, lvl[2].poz.y, lvl[4].poz.y);
			break;
		case F4T1:
			Const(hWnd);
			floor(hWnd, lvl[1].poz.y, lvl[4].poz.y);
			break;
		case F4T0:
			Const(hWnd);
			floor(hWnd, lvl[0].poz.y, lvl[4].poz.y);
			break;
		case F3T4:
			Const(hWnd);
			floor(hWnd, lvl[4].poz.y, lvl[3].poz.y);
			break;
		case F3T2:
			Const(hWnd);
			floor(hWnd, lvl[2].poz.y, lvl[3].poz.y);
			break;
		case F3T1:
			Const(hWnd);
			floor(hWnd, lvl[1].poz.y, lvl[3].poz.y);
			break;
		case F3T0:
			Const(hWnd);
			floor(hWnd, lvl[0].poz.y, lvl[3].poz.y);
			break;
		case F2T4:
			Const(hWnd);
			floor(hWnd, lvl[4].poz.y, lvl[2].poz.y);
			break;
		case F2T3:
			Const(hWnd);
			floor(hWnd, lvl[3].poz.y, lvl[2].poz.y);
			break;
		case F2T1:
			Const(hWnd);
			floor(hWnd, lvl[1].poz.y, lvl[2].poz.y);
			break;
		case F2T0:
			Const(hWnd);
			floor(hWnd, lvl[0].poz.y, lvl[2].poz.y);
			break;
		case F1T4:
			Const(hWnd);
			floor(hWnd, lvl[4].poz.y, lvl[1].poz.y);
			break;
		case F1T3:
			Const(hWnd);
			floor(hWnd, lvl[3].poz.y, lvl[1].poz.y);
			break;
		case F1T2:
			Const(hWnd);
			floor(hWnd, lvl[2].poz.y, lvl[1].poz.y);
			break;
		case F1T0:
			Const(hWnd);
			floor(hWnd, lvl[0].poz.y, lvl[1].poz.y);
			break;
		case F0T4:
			Const(hWnd);
			floor(hWnd, lvl[4].poz.y, lvl[0].poz.y);
			break;
		case F0T3:
			Const(hWnd);
			floor(hWnd, lvl[3].poz.y, lvl[0].poz.y);
			break;
		case F0T2:
			Const(hWnd);
			floor(hWnd, lvl[2].poz.y, lvl[0].poz.y);
			break;
		case F0T1:
			Const(hWnd);
			floor(hWnd, lvl[1].poz.y, lvl[0].poz.y);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		////////////////////////////////////////////////////////////////////////////////////
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, TIMER_D);
		KillTimer(hWnd, TIMER_S);
		KillTimer(hWnd, TIMER_E);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK windowDrawing(HWND hWnd, HINSTANCE hInstance)
{


	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 100,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F4T3,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 140,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F4T2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 180,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F4T1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 220,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F4T0,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need






	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 300,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F2T4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 340,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F2T3,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 380,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F2T1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 420,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F2T0,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need






	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 500,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F0T4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 540,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F0T3,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 580,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F0T2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		50, 620,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F0T1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need





	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 180,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F3T4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 220,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F3T2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 260,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F3T1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 300,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F3T0,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need




	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 380,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F1T4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 420,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F1T3,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 460,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F1T2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1400, 500,                                  // the left and top co-ordinates
		40, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)F1T0,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	return 1;
}

VOID OnPaint()
{
	Graphics graphics(hdc);
	Pen w_pen(Color(255, 255, 255, 255), 5);
	Pen b_pen(Color(255, 0, 0, 255), 5);
	Pen b_pen2(Color(255, 0, 0, 255), 2);
	Pen help(Color(255, 255, 0, 0), 6);

	graphics.DrawLine(&b_pen2, 600, 680, 600, 50);
	graphics.DrawLine(&b_pen, 600, 480, 600, 500);
	graphics.DrawLine(&b_pen, 600, 280, 600, 300);
	graphics.DrawLine(&b_pen, 600, 100, 600, 50); //left wall
	graphics.DrawLine(&b_pen2, 1200, 680, 1200, 50);
	graphics.DrawLine(&b_pen, 1200, 360, 1200, 380);
	graphics.DrawLine(&b_pen, 1200, 560, 1200, 680);
	graphics.DrawLine(&b_pen, 1200, 180, 1200, 50); //right wall
	graphics.DrawLine(&b_pen, 600, 50, 1200, 50); //top
	graphics.DrawLine(&b_pen, 0, 280, 600, 280); //4 floor
	graphics.DrawLine(&b_pen, 0, 480, 600, 480); //2 floor
	graphics.DrawLine(&b_pen, 1200, 360, 2000, 360); //3 floor
	graphics.DrawLine(&b_pen, 1200, 560, 2000, 560); //1 floor
	graphics.DrawLine(&b_pen, 0, 680, 1200, 680); //0 floor
	graphics.DrawRectangle(&w_pen, elevator.poz.x, elevator.poz.y, 580, 170);
}

VOID Const(HWND hWnd)
{
	InvalidateRect(hWnd, &elevator_tunnel, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	OnPaint();
	EndPaint(hWnd, &ps);
	
}

VOID Refresh(HWND hWnd, bool all)
{
	if (all == false)
	{
		InvalidateRect(hWnd, &elevator_tunnel, TRUE);
	}
	else
	{
		InvalidateRect(hWnd, &floor_q, TRUE);
	}
	hdc = BeginPaint(hWnd, &ps);
	NewPaint();
	OnPaint();
	draw_man();
	draw_elev();
	EndPaint(hWnd, &ps);
	
}

VOID NewPaint()
{
	Graphics graphics(hdc);
	Pen w_pen(Color(255, 255, 255, 255), 5);
	graphics.DrawRectangle(&w_pen, elevator.poz.x, elevator.poz.y, 580, 170);
	
}

VOID go_to(HWND hWnd, int to)
{
	if (to > elevator.poz.y)
	{
		dir = false;
	}
	else if (to < elevator.poz.y)
	{
		dir = true;
	}
	if (elevator.poz.y == lvl[0].poz.y || elevator.poz.y == lvl[1].poz.y || elevator.poz.y == lvl[2].poz.y || elevator.poz.y == lvl[3].poz.y || elevator.poz.y == lvl[4].poz.y)
	{
		for (int i = 0; i < prs_elev.size(); i++)
		{
			if (prs_elev[i].to == elevator.poz.y)
			{
				KillTimer(hWnd, TIMER_E);
				elevator.current_weight -= prs_elev[i].weight;
				prs_elev.erase(prs_elev.begin() + i);
				mass_refresh(hWnd);
				i--;
			}
		}
		for (int i = 0; i < prs.size(); i++)
		{
			if ((prs[i].from == elevator.poz.y) && (((prs[i].to < elevator.poz.y) && (dir == true)) || ((prs[i].to > elevator.poz.y) && (dir == false))))
			{
				if (elevator.current_weight < 560)
				{
					KillTimer(hWnd, TIMER_E);
					prs_elev.push_back(prs[i]);
					elevator.current_weight += prs[i].weight;
					prs.erase(prs.begin() + i);
					mass_refresh(hWnd);
					Refresh(hWnd, true);
					i--;
				}
			}

		}
		SetTimer(hWnd, TIMER_E, elevator.velocity, (TIMERPROC)NULL);
	}

	if (prs.size()==0 && prs_elev.size()==0)
	{
		KillTimer(hWnd, TIMER_E);
		SetTimer(hWnd, TIMER_S, 5000, (TIMERPROC)NULL);
	}
}

VOID floor(HWND hWnd, int to, int from)
{
	KillTimer(hWnd, TIMER_S);
	SetTimer(hWnd, TIMER_E, elevator.velocity, (TIMERPROC)NULL);
	person prs_buff;
	prs_buff.from = from;
	prs_buff.to = to;
	prs.push_back(prs_buff);
	Refresh(hWnd, true);
}

int where_to()
{
	int buff_min = -1;
	int buff_max = 9999999;
	int buff_min1 = -1;
	int buff_max1 = 9999999;
	if (prs_elev.size() != 0)
	{
		for (int i = 0; i < prs_elev.size(); i++)
		{
			if (prs_elev[i].to > buff_min)
			{
				buff_min = prs_elev[i].to;
			}
			if (prs_elev[i].to < buff_max)
			{
				buff_max = prs_elev[i].to;
			}
		}
	}
	else
	{
		for (int i = 0; i < prs.size(); i++)
		{
			if (prs[i].from > buff_min)
			{
				buff_min1 = prs[i].from;
			}
			if (prs[i].from < buff_max)
			{
				buff_max1 = prs[i].from;
			}
		}
	}
	if (dir == true)
	{
		if (buff_max < buff_max1)
		{
			return buff_max;
		}
		else
		{
			return buff_max1;
		}
	}
	else
	{
		if (buff_min > buff_min1)
		{
			return buff_min;
		}
		else
		{
			return buff_min1;
		}
	}
}

VOID mass_refresh(HWND hWnd)
	{
		temp = "mass (in kilograms): " + std::to_string(elevator.current_weight);
		stemp = std::wstring(temp.begin(), temp.end());
		massfield = CreateWindow(TEXT("STATIC"),                      // The class name required is button
			stemp.c_str(),                  // the caption of the button
			WS_CHILD | WS_VISIBLE | WS_VISIBLE,  // the styles
			0, 0,                                  // the left and top co-ordinates
			200, 30,                              // width and height
			hWnd,                                 // parent window handle
			(HMENU)MASSFIELD,                   // the ID of your button
			NULL,                            // the instance of your application
			NULL);
	}

VOID draw_man()
{

	for (int j = 0; j < 5; j++)
	{
		int offset = lvl[j].poz.y;
		int q = floor_queue(lvl[j].poz.y);
		Graphics graphics(hdc);
		Pen purp(Color(255, 255, 0, 255), 4);

		for (int i = 0; i < q * 59; i = i + 60)
		{
			int offset2;
			int var;
			if (j == 1 || j == 3)
			{
				var = -i - 40;
				offset2 = 1220;
			}
			else
			{
				var = i;
				offset2 = 560;
			}
			POINT circle_start = { offset2 - (var + 20), offset + 20 };
			RectF circle = { static_cast<REAL>(circle_start.x), static_cast<REAL>(circle_start.y), 40, 40 };

			graphics.DrawEllipse(&purp, circle);
			graphics.DrawLine(&purp, offset2 - var, offset + 60, offset2 - var, offset + 130);
			graphics.DrawLine(&purp, offset2 - var, offset + 130, offset2 + 20 - var, offset + 178);
			graphics.DrawLine(&purp, offset2 - var, offset + 130, offset2 - 20 - var, offset + 178);
			graphics.DrawLine(&purp, offset2 - var, offset + 70, offset2 + 20 - var, offset + 100);
			graphics.DrawLine(&purp, offset2 - var, offset + 70, offset2 - 20 - var, offset + 100);
		}

	}

}

int floor_queue(int floor)
{
	int counter = 0;
	for (int i = 0; i < prs.size(); i++)
	{
		if (prs[i].from == floor)
		{
			counter++;
		}

	}
	return counter;

}

void draw_elev()
{
	Graphics graphics(hdc);
	Pen purp(Color(255, 255, 0, 255), 4);
	int offset = elevator.poz.y-10;
	int offset2 = 636;
	int var;


	for (int i = 0; i < prs_elev.size() * 54; i = i + 55)
	{
		var = i;

		POINT circle_start = { offset2 + (var - 20), offset + 20 };
		RectF circle = { static_cast<REAL>(circle_start.x), static_cast<REAL>(circle_start.y), 40, 40 };

		graphics.DrawEllipse(&purp, circle);
		graphics.DrawLine(&purp, offset2 + var, offset + 60, offset2 + var, offset + 130);
		graphics.DrawLine(&purp, offset2 + var, offset + 130, offset2 + 20 + var, offset + 178);
		graphics.DrawLine(&purp, offset2 + var, offset + 130, offset2 - 20 + var, offset + 178);
		graphics.DrawLine(&purp, offset2 + var, offset + 70, offset2 + 20 + var, offset + 100);
		graphics.DrawLine(&purp, offset2 + var, offset + 70, offset2 - 20 + var, offset + 100);
	}
}