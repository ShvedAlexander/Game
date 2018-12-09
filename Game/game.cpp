// отключить некоторые предупреждения
#define UNICODE
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)


#include <Windows.h>
#include <commctrl.h>
#include <ctime>
#include <cstdio>
#include <fstream>

// размеры главного окна, отступы и т.п. 
#define MAIN_WIDTH     640
#define MAIN_HEIGHT    480
#define PADDING        5
#define TEXT_WIDTH     50
#define TEXT_HEIGHT    25
#define BUT_START_WIDTH     110
#define BUT_START_HEIGHT    55
#define MAX_ANSWERS    5

#define TIMEOUT			10 // количество секунд на вопрос

// идентификаторы элементов управления
#define BUT_CMD_1      1001 // кнопка первой команды
#define BUT_CMD_2      1002 // кнопка второй команды
#define EDIT_R         1003 // текстовое поле количество раундов
#define EDIT_Q         1004 // текстовое поле количество вопросов
#define ANSWER_1       1005 // флажок первого ответа
#define ANSWER_2       1006 // флажок второго ответа
#define ANSWER_3       1007 // флажок третьего ответа
#define ANSWER_4       1008 // флажок четвертого ответа
#define ANSWER_5       1009 // флажок пятого ответа
#define EDIT_TEXT      1010 // поле вопроса
#define BUT_START      1011 // кнопка начала игры

// переменные
LPCWSTR szClassName	= L"Game";
LPCWSTR szWndName = L"Игра";
LPCWSTR szMainIco = L"main.ico";
HWND    hwnd;
HINSTANCE hInst;

HWND hBut1;
HWND hBut2;
HWND hButStart;
HWND hEditR;
HWND hEditQ;
HWND hCheckBox1;
HWND hCheckBox2;
HWND hCheckBox3;
HWND hCheckBox4;
HWND hCheckBox5;
HWND hQuestionText;
HWND hQuestionNumber;
HWND hRoundNumber;
HWND hTimer;
LPCWSTR sCmd1 = L"Первая команда";
LPCWSTR sCmd2 = L"Вторая команда";
LPTSTR sCmd1Ico; // иконка первой команды
LPTSTR sCmd2Ico; // иконка второй команды
LPCWSTR sStart = L"Начать игру";

char answer[1024];

int count1;
int count2;

bool break1;
bool break2;

int iRound;
int iQuestion;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM,  LPARAM);

HWND CreateButIcon(HWND, INT, INT, INT, INT, 
                    LPCWSTR, INT, LPTSTR, HWND*);
HWND CreateBut(HWND, INT, INT, INT, INT, LPCWSTR, INT);
HWND CreateLabel(HWND, INT, INT, INT, INT, LPCWSTR);
HWND CreateText(HWND, INT, INT, INT, INT, LPCWSTR, INT);
HWND CreateMultiRowText(HWND, INT, INT, INT, INT, LPCWSTR, INT);
HWND CreateCheckBox(HWND, INT, INT, INT, INT, LPCWSTR, INT);

int WINAPI WinMain (HINSTANCE hInstance,
				HINSTANCE hPrevInstance,
				char * szCmdLine, int iCmdShow)
{
    MSG         msg;
    WNDCLASS    wc;
	// заполняется стуктура главного окна
    hInst = hInstance;
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImage(0, szMainIco, IMAGE_ICON,0,0,LR_LOADFROMFILE);	 
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR)szClassName;

	RegisterClass(&wc);
	DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
	// получить размер экрана
	// что бы в центре расположить окно
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

	// создается главное окно программы
	// при этом происходит событие WM_CREATE
	// в обработчике OnCreate(...) создаются элементы управления
	hwnd = CreateWindow ((LPCWSTR)szClassName, (LPCWSTR)szWndName, 
                        WS_EX_TOOLWINDOW | WS_CAPTION | 
						WS_SYSMENU | WS_MINIMIZEBOX,
                        (dm.dmPelsWidth - MAIN_WIDTH)/2, 
						(dm.dmPelsHeight - MAIN_HEIGHT)/2, 
                        MAIN_WIDTH, MAIN_HEIGHT, NULL, NULL, hInstance, NULL);  
	// показывается на экране
	ShowWindow (hwnd, SW_SHOW);
    UpdateWindow (hwnd);
	// главное окно получает сообщения от системы	
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg) ;
        DispatchMessage (&msg) ;
    }
    return msg.wParam ;
}

// определяет установлен флажок или нет
bool IsChecked(HWND hwnd)
{
	int state = SendMessage( hwnd, BM_GETCHECK, 0, (LPARAM)0);
	SendMessage( hwnd, BM_SETCHECK, BST_UNCHECKED, (LPARAM)0);
	return state == BST_CHECKED;
}

// читает ответ команды и определяет правильный он или нет
// если правильный возвращает 1, если неправильный, то 0
int GetAnswer()
{
	int res = 0;
	char tmp[32];
	ZeroMemory(tmp, 32);
	if(IsChecked(hCheckBox1))
	{
		strcat(tmp, "1");
	}
	if(IsChecked(hCheckBox2))
	{
		if(strlen(tmp) > 0)
		{
			strcat(tmp, ",");
		}
		strcat(tmp, "2");
	}
	if(IsChecked(hCheckBox3))
	{
		if(strlen(tmp) > 0)
		{
			strcat(tmp, ",");
		}
		strcat(tmp, "3");
	}
	if(IsChecked(hCheckBox4))
	{
		if(strlen(tmp) > 0)
		{
			strcat(tmp, ",");
		}
		strcat(tmp, "4");
	}
	if(IsChecked(hCheckBox5))
	{
		if(strlen(tmp) > 0)
		{
			strcat(tmp, ",");
		}
		strcat(tmp, "5");
	}

	if(0 == strcmp(tmp, answer))
	{
		res = 1;
	}
	return res;
}

// записать текст в элемент управления 
void SetElementText(HWND hwnd, char *string, int number){
	wchar_t tmp[512];
	WCHAR *buf;
	int nChars;

	if(number > 0){
		swprintf_s(tmp, L"%s%d", string, number);
	}
	else{
		nChars = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)string, -1, NULL, 0);
		buf = new WCHAR[nChars];
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)string, -1, buf, nChars);
		swprintf_s(tmp, L"%s", buf);
		delete [] buf;
	}

	SendMessage( hwnd, WM_SETTEXT, 0, (LPARAM)tmp);
}

// прочитать текст из элемента управления
void GetElementText(HWND hwnd, WCHAR buf [], int len)
{
	SendMessage( hwnd, WM_GETTEXT, len, (LPARAM)buf);
}

int GetQuestionNumber()
{
	srand(time(NULL));
	int n = std::rand();
	return n;
}

bool DisplayQuestion(int pos)
{
	std::streamsize n;
	char buf[1024];
	int i;
	bool b = false;
    std::ifstream in;
	in.open("questions.txt");

	if(! in.is_open())
	{
		return b;
	}

	i = 0;
	while(!in.eof())
	{
		in.getline(buf, 512);
		if(i%(MAX_ANSWERS + 2) == 0)// вопрос
		{
			if(i / (MAX_ANSWERS + 2) == pos)// тот который нужен
			{
				// записать вопрос
				SetElementText(hQuestionText, buf, 0);
				// первый ответ
				in.getline(buf, 512);
				n = in.gcount();
				ShowWindow(hCheckBox1, n > 1);
				SetElementText(hCheckBox1, buf, 0);
				// второй ответ
				in.getline(buf, 512);
				n = in.gcount();
				ShowWindow(hCheckBox2, n > 1);
				SetElementText(hCheckBox2, buf, 0);
				// третий ответ
				in.getline(buf, 512);
				n = in.gcount();
				ShowWindow(hCheckBox3, n > 1);
				SetElementText(hCheckBox3, buf, 0);
				// четвертый ответ
				in.getline(buf, 512);
				n = in.gcount();
				ShowWindow(hCheckBox4, n > 1);
				SetElementText(hCheckBox4, buf, 0);
				// пятый ответ
				in.getline(buf, 512);
				n = in.gcount();
				ShowWindow(hCheckBox5, n > 1);
				SetElementText(hCheckBox5, buf, 0);

				// правильный ответ
				in.getline(answer, 1024);
				break;
			}
		}
		i ++;
	}

	in.close();

	return b;
}
/**********************************************************
*/
void StartGame(void){
	WCHAR buf[16];
	int i,j,k,q;
	int iRounds = 0;
	int iQuestions = 0;
	WCHAR title[128];

	ShowWindow(hCheckBox1, true);
	ShowWindow(hCheckBox2, true);
	ShowWindow(hCheckBox3, true);
	ShowWindow(hCheckBox4, true);
	ShowWindow(hCheckBox5, true);

	count1 = 0;
	count2 = 0;
	break1 = false;
	break2 = false;

	GetElementText(hEditR, buf, 16);
	iRounds = _wtoi(buf);
	GetElementText(hEditQ, buf, 16);
	iQuestions = _wtoi(buf);

	iQuestion = 1;
	iRound = 1;

	EnableWindow(hBut1, true);
	EnableWindow(hBut2, true);

	q = 0;// первый вопрос
	// цикл по раундам
	for(i = 0; i < iRounds; i ++)
	{
		SetElementText(hRoundNumber, (char *)"", i+1);
		// цикл по вопросам
		for(j = 0; j < iQuestions; j ++)
	    {
			//n = GetQuestionNumber();
			DisplayQuestion(q);
			q ++;
			SetElementText(hQuestionNumber, (char *)"", j+1);
			EnableWindow(hCheckBox1, false);
			EnableWindow(hCheckBox2, false);
			EnableWindow(hCheckBox3, false);
			EnableWindow(hCheckBox4, false);
			EnableWindow(hCheckBox5, false);
			for(k = 1; k < TIMEOUT + 1; k ++)
			{
				Sleep(1000);
				SetElementText(hTimer, (char *)"", k);
				// нажали кнопку для ответа
				if(break1 || break2)
				{
					while(break1 || break2)
					{
						Sleep(500);
					}
					// переход к следующему вопросу
					break;
				}
			}
		}
	}

	// кнопку начала игры сделать активной
	// что бы можно было новую игру начать
    EnableWindow(hButStart, true);
	const wchar_t* str = L"";
	if(count1 > count2) str = L"Победила команда 1";
	if(count1 < count2) str = L"Победила команда 2";
	if(count1 == count2) str = L"Ничья";
	swprintf_s(title, L"команда 1:%d | команда 2:%d. %s", count1, count2, str);
	SetWindowText(hwnd, title);

	EnableWindow(hBut1, false);
	EnableWindow(hBut2, false);

	ShowWindow(hCheckBox1, false);
	ShowWindow(hCheckBox2, false);
	ShowWindow(hCheckBox3, false);
	ShowWindow(hCheckBox4, false);
	ShowWindow(hCheckBox5, false);

	SetWindowText(hQuestionText, L"");

	ExitThread( IDYES );
}
/**********************************************************
        Функции обработчики событий
*/
// создание элементов управления главного окна 
void OnCreate(HWND hwnd)
{
	INT w = MAIN_WIDTH / 2 - PADDING * 2;
	INT h = w / 5 * 2;
	
	// ввод данных для начала игры
	CreateLabel(hwnd, PADDING, PADDING, MAIN_WIDTH / 3, 
		        TEXT_HEIGHT, L"Количество раундов");
	hEditR = CreateText(hwnd, PADDING*2 + MAIN_WIDTH / 3, PADDING, 
                        TEXT_WIDTH, TEXT_HEIGHT, 
						(LPCWSTR)L"2", EDIT_R);// 1 раунд

	CreateLabel(hwnd, PADDING, PADDING*2+TEXT_HEIGHT, 
		        MAIN_WIDTH / 3, TEXT_HEIGHT, L"Вопросов в раунде");
	hEditQ = CreateText(hwnd, PADDING*2 + MAIN_WIDTH / 3, 
						PADDING*2+TEXT_HEIGHT, 
                        TEXT_WIDTH, TEXT_HEIGHT, 
						(LPCWSTR)L"3", EDIT_R); // 2 вопроса
    // кнопка "начало игры"
	hBut1 = CreateBut(hwnd, PADDING*3 + MAIN_WIDTH / 3 + TEXT_WIDTH, 
		                PADDING, BUT_START_WIDTH, BUT_START_HEIGHT, 
                        (LPCWSTR)sStart, BUT_START);

	CreateLabel(hwnd, PADDING*4 + MAIN_WIDTH / 3 + TEXT_WIDTH + BUT_START_WIDTH, 
		        PADDING, TEXT_WIDTH*3/2, TEXT_HEIGHT, L"Раунд ");
	hRoundNumber = CreateLabel(hwnd, PADDING*4 + MAIN_WIDTH / 3 + TEXT_WIDTH*5/2 + BUT_START_WIDTH, 
		        PADDING, TEXT_WIDTH/3*2, TEXT_HEIGHT, L"0");
	CreateLabel(hwnd, PADDING*4 + MAIN_WIDTH / 3 + TEXT_WIDTH + BUT_START_WIDTH, 
		        PADDING + TEXT_HEIGHT, TEXT_WIDTH*3/2, TEXT_HEIGHT, L"Вопрос ");
	hQuestionNumber = CreateLabel(hwnd, PADDING*4 + MAIN_WIDTH / 3 + TEXT_WIDTH*5/2 + BUT_START_WIDTH, 
		        PADDING + TEXT_HEIGHT, TEXT_WIDTH/3*2, TEXT_HEIGHT, L"0");
	hTimer = CreateLabel(hwnd, PADDING*4 + MAIN_WIDTH / 3 + TEXT_WIDTH*4 + BUT_START_WIDTH, 
		        PADDING, TEXT_WIDTH, TEXT_HEIGHT, L"0");

	hQuestionText = CreateMultiRowText(hwnd, PADDING, 
						TEXT_HEIGHT*2+PADDING*3, 
                        MAIN_WIDTH - PADDING*3, TEXT_HEIGHT*5, 
						(LPCWSTR)L"", EDIT_TEXT);
	hCheckBox1 = CreateCheckBox(hwnd, 
                    PADDING, 
					TEXT_HEIGHT*7+PADDING*3, 
					MAIN_WIDTH - PADDING*10, TEXT_HEIGHT, 
					(LPCWSTR)L"", ANSWER_1);
	hCheckBox2 = CreateCheckBox(hwnd, 
                    PADDING, 
					TEXT_HEIGHT*8+PADDING*3, 
					MAIN_WIDTH - PADDING*10, TEXT_HEIGHT, 
					(LPCWSTR)L"", ANSWER_1);
	hCheckBox3 = CreateCheckBox(hwnd, 
                    PADDING, 
					TEXT_HEIGHT*9+PADDING*3, 
					MAIN_WIDTH - PADDING*10, TEXT_HEIGHT, 
					(LPCWSTR)L"", ANSWER_1);
	hCheckBox4 = CreateCheckBox(hwnd, 
                    PADDING, 
					TEXT_HEIGHT*10+PADDING*3, 
					MAIN_WIDTH - PADDING*10, TEXT_HEIGHT, 
					(LPCWSTR)L"", ANSWER_1);
	hCheckBox5 = CreateCheckBox(hwnd, 
                    PADDING, 
					TEXT_HEIGHT*11+PADDING*3, 
					MAIN_WIDTH - PADDING*10, TEXT_HEIGHT, 
					(LPCWSTR)L"", ANSWER_1);
	ShowWindow(hCheckBox1, false);
	ShowWindow(hCheckBox2, false);
	ShowWindow(hCheckBox3, false);
	ShowWindow(hCheckBox4, false);
	ShowWindow(hCheckBox5, false);
	// кнопки для команд
	//hBut1 = CreateButIcon(hwnd, PADDING, MAIN_HEIGHT - PADDING*6 - h, w, h, 
    //                (LPCWSTR)sCmd1, BUT_CMD_1, sCmd1Ico, NULL);
	hBut1 = CreateBut(hwnd, PADDING, MAIN_HEIGHT - PADDING*6 - h, w, h, 
                    (LPCWSTR)sCmd1, BUT_CMD_1);
	//hBut2 = CreateButIcon(hwnd, w+PADDING*2, MAIN_HEIGHT - PADDING*6 - h, w, h, 
    //                (LPCWSTR)sCmd2, BUT_CMD_2, sCmd2Ico, NULL);
	hBut2 = CreateBut(hwnd, w+PADDING*2, MAIN_HEIGHT - PADDING*6 - h, w, h, 
                    (LPCWSTR)sCmd2, BUT_CMD_2);
	EnableWindow(hBut1, false);
	EnableWindow(hBut2, false);
} 
LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
        case WM_CREATE :
			OnCreate(hwnd);
		    break;
		case WM_DESTROY :
            PostQuitMessage(0);
            break;
		case WM_COMMAND :
            switch( LOWORD(wParam))
            {
				case BUT_START:
					CreateThread (NULL, 0, 
						(LPTHREAD_START_ROUTINE)StartGame, 
						(LPVOID)NULL, 0, (LPDWORD)NULL);

					break;
                case BUT_CMD_1:
					if(!break1) // остановили для ответа
					{
						break1 = true;
						// кнопку второй команды заблокировать
						EnableWindow(hBut2, false);
						// разблокировать флажки для ввода ответа
						EnableWindow(hCheckBox1, true);
						EnableWindow(hCheckBox2, true);
						EnableWindow(hCheckBox3, true);
						EnableWindow(hCheckBox4, true);
						EnableWindow(hCheckBox5, true);
					}
					else // нажата второй раз для продолжения игры
					{
						break1 = false;
						// кнопку второй команды разблокировать
						EnableWindow(hBut2, true);
						count1 += GetAnswer();
					}
				    break;
				case BUT_CMD_2:
					if(!break2) // остановили для ответа
					{
						break2 = true;
						// кнопку первой команды заблокировать
						EnableWindow(hBut1, false);
						// разблокировать флажки для ввода ответа
						EnableWindow(hCheckBox1, true);
						EnableWindow(hCheckBox2, true);
						EnableWindow(hCheckBox3, true);
						EnableWindow(hCheckBox4, true);
						EnableWindow(hCheckBox5, true);
					}
					else // нажата второй раз для продолжения игры
					{
						break2 = false;
						// кнопку первой команды разблокировать
						EnableWindow(hBut1, true);
						count2 += GetAnswer();
					}
				    break;
			    default :
				    break;
			}
		    break;
		default :
		    break;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam);
}

/********************************************************** 
        Функции создания элементов управления
*/
HWND CreateButIcon(HWND hwnd2, INT x, INT y, INT w, INT h, 
                    LPCWSTR name, INT cmd, LPTSTR file, HWND *ptth = NULL)
{
    HWND ww;
	HANDLE himg;
	TOOLINFO ti;
	HINSTANCE hinst = (HINSTANCE) GetWindowLong(hwnd2, GWL_HINSTANCE);


    ww = CreateWindowEx(
						WS_EX_CONTROLPARENT, (LPCWSTR)L"BUTTON", name,
						WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_ICON, 
						x, y, w, h, hwnd2, (HMENU) cmd,
						hinst,
						NULL);

	SendMessage(ww, WM_SETFONT, 
		        (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 
		        MAKELPARAM(TRUE, 0));

	himg = LoadImage(0, file, IMAGE_ICON,0,0,LR_LOADFROMFILE);
	if(himg != NULL){
		SendMessage( ww, BM_SETIMAGE, IMAGE_ICON, (long)himg );
	}

	HWND hTooltip = CreateWindow (
									TOOLTIPS_CLASS, 
									(LPCWSTR)NULL, 
									WS_POPUP|TTS_NOPREFIX|TTS_ALWAYSTIP, 
									CW_USEDEFAULT, CW_USEDEFAULT, 
									CW_USEDEFAULT, CW_USEDEFAULT, 
									NULL, (HMENU)NULL, hinst, NULL
								);
 
    memset (&ti, 0, sizeof TOOLINFO);
    ti.cbSize = sizeof TOOLINFO;
    ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    ti.uId = (UINT)ww;
    ti.lpszText = (LPWSTR)name;
    ti.hinst = hinst;
    SendMessage (hTooltip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);  

	if( ptth != NULL ){
		*ptth = hTooltip;
	}

    return ww;
}

HWND CreateLabel(HWND hwnd2, INT x, INT y, INT w, INT h, LPCWSTR name)
{
    HWND ww;
    ww = CreateWindow( (LPCWSTR)L"STATIC", (LPCWSTR)name, 
                        WS_VISIBLE | WS_CHILD | SS_RIGHT,
                        x, y, w,  h, hwnd2, (HMENU) 99,
                        (HINSTANCE) GetWindowLong(hwnd2, GWL_HINSTANCE),
                        NULL);
	if (ww != NULL) 
	{
		HDC hdc = GetDC(ww);
		HFONT fontTitle = CreateFont( 
                                (GetDeviceCaps(hdc, LOGPIXELSY) * 16) / 72,
								0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, 
								(LPCWSTR)L"MS Sans Serif");
		SendMessage( ww, WM_SETFONT, (WPARAM)fontTitle, TRUE );
		ReleaseDC(ww, hdc );		
		return ww; 
	}
	return NULL;
}

HWND CreateCheckBox(HWND hwnd, 
                    INT x, INT y, INT w, INT h, 
					LPCWSTR name, INT cmd)
{
    HWND ww;
    ww = CreateWindowEx(WS_EX_CONTROLPARENT, (LPCWSTR)L"BUTTON", name,
                        WS_VISIBLE | WS_CHILD | BS_RIGHTBUTTON | 
					    BS_AUTOCHECKBOX | WS_TABSTOP | SS_LEFT,
					    x, y, w, h, hwnd, (HMENU) cmd,
					    (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                        NULL);
    if (ww != NULL) 
	{
		HDC hdc = GetDC(ww);
		HFONT fontTitle = CreateFont( 
                                (GetDeviceCaps(hdc, LOGPIXELSY) * 16) / 72,
                                0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, 
                                (LPCWSTR)L"MS Sans Serif");
		SendMessage( ww, WM_SETFONT, (WPARAM)fontTitle, TRUE );
		ReleaseDC(ww, hdc );		
		return ww; 
	}
	return NULL;
}

HWND CreateText(HWND hwnd2, INT x, INT y, INT w, INT h, LPCWSTR name, INT cmd)
{
    HWND ww;
    ww = CreateWindow( (LPCWSTR)L"EDIT", name, 
                    WS_VISIBLE | WS_CHILD | SS_SUNKEN | SS_LEFT | WS_BORDER,
                    x, y, w,  h, hwnd2, (HMENU) cmd,
                    (HINSTANCE) GetWindowLong(hwnd2, GWL_HINSTANCE), NULL);
	if (ww != NULL) 
	{
		HDC hdc = GetDC(ww);
		HFONT fontTitle = CreateFont(
                                (GetDeviceCaps(hdc, LOGPIXELSY) * 16) / 72,
                                0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, 
                                (LPCWSTR)L"MS Sans Serif");
		SendMessage( ww, WM_SETFONT, (WPARAM)fontTitle, TRUE );
		ReleaseDC(ww, hdc );		
		return ww; 
	}

	return NULL;
}

HWND CreateMultiRowText(HWND hwnd2, INT x, INT y, INT w, INT h, 
                        LPCWSTR name, INT cmd)
{
    HWND ww;
    ww = CreateWindow( (LPCWSTR)L"EDIT", (LPCWSTR)name, 
		        WS_VISIBLE | WS_CHILD | SS_SUNKEN | SS_LEFT | 
		        WS_BORDER | ES_MULTILINE,
                x, y, w,  h, hwnd2, (HMENU) cmd,
                (HINSTANCE) GetWindowLong(hwnd2, GWL_HINSTANCE), NULL);
	if (ww != NULL) 
	{
		HDC hdc = GetDC(ww);
		HFONT fontTitle = CreateFont( (GetDeviceCaps(hdc, LOGPIXELSY) * 16) / 72,
								0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, 
								(LPCWSTR)L"MS Sans Serif");
		SendMessage( ww, WM_SETFONT, (WPARAM)fontTitle, TRUE );
		ReleaseDC(ww, hdc );		
		return ww; 
	}
	return NULL;
}

HWND CreateBut(HWND hwnd2, INT x, INT y, INT w, INT h, LPCWSTR name, INT cmd)
{
    HWND ww;

    ww = CreateWindowEx(WS_EX_CONTROLPARENT, (LPCWSTR)L"BUTTON", (LPCWSTR)name,
    WS_VISIBLE | WS_CHILD | WS_TABSTOP, x, y, w, h, hwnd2, (HMENU) cmd,
    (HINSTANCE) GetWindowLong(hwnd2, GWL_HINSTANCE), NULL);
	if(ww != NULL)
	{
	    HDC hdc = GetDC(ww);
		HFONT fontTitle = CreateFont( (GetDeviceCaps(hdc, LOGPIXELSY) * 16) / 72,
								0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, 
								(LPCWSTR)L"MS Sans Serif");
		SendMessage( ww, WM_SETFONT, (WPARAM)fontTitle, TRUE );
		ReleaseDC(ww, hdc );		
		return ww; 
	}
    return ww;
}