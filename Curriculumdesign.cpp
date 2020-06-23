// Curriculumdesign.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Curriculumdesign.h"
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "msimg32.lib")
#define MAX_LOADSTRING 100

BOOL MyTransparentBlt2(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent)
{
	if (nWidthDest < 1)
		return false;
	if (nWidthSrc < 1)
		return false;
	if (nHeightDest < 1)
		return false;
	if (nHeightSrc < 1)
		return false;

	HDC dc = CreateCompatibleDC(NULL);
	HBITMAP bitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, GetDeviceCaps(dc, BITSPIXEL), NULL);
	if (bitmap == NULL)
	{
		DeleteDC(dc);
		return false;
	}
	HBITMAP oldBitmap = (HBITMAP)SelectObject(dc, bitmap);
	if (!BitBlt(dc, 0, 0, nWidthSrc, nHeightSrc, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY))
	{
		SelectObject(dc, oldBitmap);
		DeleteObject(bitmap);
		DeleteDC(dc);
		return false;
	}
	HDC maskDC = CreateCompatibleDC(NULL);
	HBITMAP maskBitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, 1, NULL);
	if (maskBitmap == NULL)
	{
		SelectObject(dc, oldBitmap);
		DeleteObject(bitmap);
		DeleteDC(dc);
		DeleteDC(maskDC);
		return false;
	}
	HBITMAP oldMask = (HBITMAP)SelectObject(maskDC, maskBitmap);
	SetBkColor(maskDC, RGB(0, 0, 0));
	SetTextColor(maskDC, RGB(255, 255, 255));
	if (!BitBlt(maskDC, 0, 0, nWidthSrc, nHeightSrc, NULL, 0, 0, BLACKNESS))
	{
		SelectObject(maskDC, oldMask);
		DeleteObject(maskBitmap);
		DeleteDC(maskDC);
		SelectObject(dc, oldBitmap);
		DeleteObject(bitmap);
		DeleteDC(dc);
		return false;
	}
	SetBkColor(dc, crTransparent);
	BitBlt(maskDC, 0, 0, nWidthSrc, nHeightSrc, dc, 0, 0, SRCINVERT);
	SetBkColor(dc, RGB(0, 0, 0));
	SetTextColor(dc, RGB(255, 255, 255));
	BitBlt(dc, 0, 0, nWidthSrc, nHeightSrc, maskDC, 0, 0, SRCAND);
	HDC newMaskDC = CreateCompatibleDC(NULL);
	HBITMAP newMask;
	newMask = CreateBitmap(nWidthDest, nHeightDest, 1, GetDeviceCaps(newMaskDC, BITSPIXEL), NULL);
	if (newMask == NULL)
	{
		SelectObject(dc, oldBitmap);
		DeleteDC(dc);
		SelectObject(maskDC, oldMask);
		DeleteDC(maskDC);
		DeleteDC(newMaskDC);
		DeleteObject(bitmap);
		DeleteObject(maskBitmap);
		return false;
	}
	SetStretchBltMode(newMaskDC, COLORONCOLOR);
	HBITMAP oldNewMask = (HBITMAP)SelectObject(newMaskDC, newMask);
	StretchBlt(newMaskDC, 0, 0, nWidthDest, nHeightDest, maskDC, 0, 0, nWidthSrc, nHeightSrc, SRCCOPY);
	SelectObject(maskDC, oldMask);
	DeleteDC(maskDC);
	DeleteObject(maskBitmap);
	HDC newImageDC = CreateCompatibleDC(NULL);
	HBITMAP newImage = CreateBitmap(nWidthDest, nHeightDest, 1, GetDeviceCaps(newMaskDC, BITSPIXEL), NULL);
	if (newImage == NULL)
	{
		SelectObject(dc, oldBitmap);
		DeleteDC(dc);
		DeleteDC(newMaskDC);
		DeleteObject(bitmap);
		return false;
	}
	HBITMAP oldNewImage = (HBITMAP)SelectObject(newImageDC, newImage);
	StretchBlt(newImageDC, 0, 0, nWidthDest, nHeightDest, dc, 0, 0, nWidthSrc, nHeightSrc, SRCCOPY);
	SelectObject(dc, oldBitmap); DeleteDC(dc); DeleteObject(bitmap);
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, newMaskDC, 0, 0, SRCAND);
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, newImageDC, 0, 0, SRCPAINT);
	SelectObject(newImageDC, oldNewImage);
	DeleteDC(newImageDC);
	SelectObject(newMaskDC, oldNewMask);
	DeleteDC(newMaskDC);
	DeleteObject(newImage);
	DeleteObject(newMask);
	return true;
}

// 全局变量:
double tPre, tNow, tStart, tSun, t1 = -1, t2 = -1, t3 = -1, t3pre = -1, t3now, tgamestart;
HBITMAP bm, hZombie[59], hbg, hSunF[36], hCh, hscpbox,
hscp, hpeb, hpehit, hshadow, hSnowPeashooterBullet,
hSun[29], hPeashooter[27], hRepeater[30], hSnowPea[30], hWallNut[32],
hWallnut_cracked1[22], hWallnut_cracked2[30],
hChomper[42], hJalapeno[16], hJalapenoAttack[25], hSquash[34],
hZombieAttack[43], hZombieLostHead[60], hZombieHead[9],
hZombieLostHeadAttack[22], hZombieDie[40], hBoomDie[50], hZombieblue[59],
hZombieAttackblue[43], hChomperAttack[19], hChomperDigest[19],
hSquashAttack[33], hCD, hStart, hSelector, hPrepareGrowPlants,
hPrepareGrowPlantsok, hPrepareGrowPlantsgo, hLargeWave, hFinalWave,
hFlagMeterEmpty, hFlagMeterFull, hFlagMeterParts2, hFlagMeterParts1,
hZombiesWon, hFlagMeterLevelProgress, hSquashR,
hplantchoosebox, hplantchoose, hplantnum[15], hfight1, hfight2,
hCar, hPotatomine[8], hPotatomineReady, hPotatominemashed, hSunshroom[21],
hSunshroomGrow[21], hFumeshroom[17], hExplosionSpudow, hScaredyshroom[18],
hScaredyshrooms[12], hSunsmall[29], hPuffshoot[6], hShroomBulletHit, hFumeShroombullet[10];
BITMAP zom, bg, start;
HDC mdc, hdc, bufdc;
bool STRAT = false, END = true, WIN = false;
int money = 50;
int DIE = 0;																	//判断游戏结束
int fume = 1;																	//大喷菇子弹的帧数判断
int planted = -1;																//没有选择要种的植物
int scooped = -1;																//-1没有选择铲子，1选择了铲子，2点击了植物
int hcposx, hcposy, scoopposx, scoopposy, lbuttondown = 0, lbuttonup = 0;		//左键按下放开
int markposx, markposy, releasex, releasey, movex, movey;						//鼠标移动位置，点击位置
int varvalue[10], CHOOSE;
int wwid = 820, whei = 630;
HRGN hrgn[29], hrgnsmall[29], hrgn_shadow, hrgn_plant[15], hrgn_cd;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
void paintStart(HDC hdc);

class Plant {
public:
	int num;
	int maxnum;
	double blood;//血量
	double tshoot;
	double recover;
	double grow;
	bool ali;//存活
	int vri;
}Pla[5][9];//最多5行9列

class Peb {
public:
	double x, y;			//位置
	double speed;			//速度
	int vri;				//冰豌豆、普通豌豆
	double j;
	int num;				//蘑菇孢子
	int maxnum;				//图片数量
};

class Head {
public:
	int num;
	int maxnum;
};

class Zomb {
public:
	int num;
	int maxnum;
	int condition;
	double recover;
	double disapper;
	double blood;
	double speed;
	double x, y;
	double i, j;
	Head head;
}Zob;

class Sun {
public:
	int num;
	int x, y;
	int spx, spy;
	int maxnum;
	double stime;
	int maxy;
	int size;
};

class CD {
public:
	int ali;
	double starttime;
	double lasting;
	double maxlasting;
}cd[9], moneyless[8];

class Plantvri {
public:
	int ex;
	int vri;
};

class Car {
public:
	bool alive;
}car[5];

vector<Zomb>Zombie;
vector<Zomb>Zombie_v[5];
vector<Zomb>ZomDie_v;
vector<Sun>Sun_v;
vector<Sun>Sun_coll;
vector<Peb>Peb_v;
vector<Plantvri>Plantvri_v;

bool cmp(Zomb z1, Zomb z2)
{
	return z1.x < z2.x;
}

void MyPaint(HDC hdc);
void paintPrepare(HDC hdc);
void paintLargewave(HDC hdc);
void ChooseP(HDC hdc);
void Drawplantready(int vri, HBITMAP hb, int x, int y);
void AlphaBmp(HDC hdc, HBITMAP hb, int x, int y, short li, HRGN hrgn, COLORREF col);
void AlphaBmp(HDC hdc, HBITMAP hBitmap, int x, int y, short limit, HRGN hRgn, COLORREF col);
HFONT myCreateFont(HDC hDC, int nCharHeight, BOOL bItalic);

HRGN BitmapToRgn(HBITMAP hBitmap, COLORREF col);

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CURRICULUMDESIGN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	//僵尸初始化

	Zomb nzom;
	nzom.num = 0;
	nzom.speed = 0.3;
	nzom.condition = 1;
	nzom.maxnum = 55;
	nzom.x = 750;
	nzom.blood = 50;
	nzom.head.num = 0;
	nzom.head.maxnum = 9;
	nzom.recover = -1;
	nzom.disapper = -1;
	int apper = false;
	int line;
	int groan;
	CHOOSE = -1;
	srand((unsigned int)time(NULL));
	tStart = GetTickCount();
	tSun = GetTickCount();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CURRICULUMDESIGN));

	MSG msg;

	char name[105];

	// 主消息循环:
	GetMessage(&msg, nullptr, 0, 0);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //通道1：消息循环。有消息时处理消息，没消息立即返回走函数循环
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else							//通道2：函数循环。不通过win消息机制运转，会更快
		{
			tNow = GetTickCount();      //取得系统开始运行到目前所经过的时间
			if (tNow - tPre >= 40)        //当此次循环运行与上次绘图时间相差0.05秒时再进行重绘操作
			{
				if (STRAT == false || (GetTickCount() - t1 <= 4000 && t1 != -1))
				{
					if (STRAT == false) {
						mciSendString("stop .\\image\\music\\bkmusic.wav", NULL, 0, NULL);
						mciSendString("play .\\image\\music\\ui_gkmusic.wav", NULL, 0, NULL);
					}
					paintStart(hdc);
				}
				else {
					if (CHOOSE == 0) {
						mciSendString("play .\\image\\music\\lookup.wav", NULL, 0, NULL);
						ChooseP(hdc);
					}
					else if (t2 == -1 || (t2 != -1 && GetTickCount() - t2 <= 1500)) {
						if (t2 == -1) {
							mciSendString("stop .\\image\\music\\readysetpla.wav", NULL, 0, NULL);
							mciSendString("play .\\image\\music\\readysetpla.wav", NULL, 0, NULL);
							t2 = GetTickCount();
						}
						paintPrepare(hdc);
					}
					else {
						t3now = GetTickCount();
						groan = rand() % 4;
						sprintf_s(name, "play .\\image\\music\\groan%d.wav\0", groan);
						mciSendString("play .\\image\\music\\bkmusic.wav", NULL, 0, NULL);
						if (t3 == -1) {
							t3 = GetTickCount();
							t3pre = t3;
							mciSendString("stop .\\image\\music\\losemusic.wav", NULL, 0, NULL);
							mciSendString("stop .\\image\\music\\evillaugh.wav", NULL, 0, NULL);
						}
						else if (GetTickCount() - t3 <= 100000) {//前100秒每20秒出一个僵尸
							if (t3now - t3pre >= 5000) {
								if (GetTickCount() <= 20050) {
									mciSendString("play .\\image\\music\\awooga.wav", NULL, 0, NULL);
								}
								mciSendString(name, NULL, 0, NULL);
								line = rand() % 5;
								Zombie_v[line].push_back(nzom);
								t3pre = t3now;
							}
						}
						else if (GetTickCount() - t3 <= 140000) {//100秒-140秒每12秒出一个僵尸
							if (t3now - t3pre >= 12000) {
								mciSendString(name, NULL, 0, NULL);
								line = rand() % 5;
								Zombie_v[line].push_back(nzom);
								t3pre = t3now;
							}

						}
						else if (GetTickCount() - t3 <= 170000) {//140-170秒第一波，持续30秒
							if (t3now - t3pre >= 1500) {
								if (GetTickCount() <= 140100) {
									mciSendString("play .\\image\\music\\hugewave.wav", NULL, 0, NULL);
								}
								mciSendString(name, NULL, 0, NULL);
								line = rand() % 5;
								Zombie_v[line].push_back(nzom);
								t3pre = t3now;
							}

						}
						else if (GetTickCount() - t3 <= 230000) {//170-230秒3秒一只
							if (t3now - t3pre >= 3000) {
								mciSendString(name, NULL, 0, NULL);
								line = rand() % 5;
								Zombie_v[line].push_back(nzom);
								t3pre = t3now;
							}

						}
						else if (GetTickCount() - t3 <= 250000) {//230秒最后一波
							if (t3now - t3pre >= 1000) {
								if (GetTickCount() <= 230100) {
									mciSendString("play .\\image\\music\\hugewave.wav", NULL, 0, NULL);
								}
								mciSendString(name, NULL, 0, NULL);
								line = rand() % 5;
								Zombie_v[line].push_back(nzom);
								t3pre = t3now;
							}
						}
						MyPaint(hdc);
					}
				}
			}
		}
	}

	return (int)msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CURRICULUMDESIGN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CURRICULUMDESIGN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, wwid, whei, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);//小dc，for大图
	bufdc = CreateCompatibleDC(hdc);//小dc,for小图
	bm = CreateCompatibleBitmap(hdc, wwid, whei);//大图 

	SelectObject(mdc, bm);
	int i;
	char name[60] = "";
	hbg = (HBITMAP)LoadImage(NULL, "image/bg.bmp", IMAGE_BITMAP, 1400, 600, LR_LOADFROMFILE);
	hCh = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP35));
	hscp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP25));
	hscpbox = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP26));
	hpeb = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP29));
	hpehit = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP33));
	hSnowPeashooterBullet = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP34));
	hStart = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP31));
	hSelector = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP27));
	hPrepareGrowPlants = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP22));
	hPrepareGrowPlantsok = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP24));
	hPrepareGrowPlantsgo = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP23));
	hLargeWave = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
	hFinalWave = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
	hFlagMeterEmpty = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
	hFlagMeterFull = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
	hFlagMeterParts2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
	hFlagMeterParts1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
	hZombiesWon = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP32));
	hFlagMeterLevelProgress = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
	hSquashR = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP30));
	hplantchoosebox = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP21));
	hplantchoose = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP20));
	hfight1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP36));
	hfight2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
	hshadow = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP28));
	hrgn_shadow = BitmapToRgn(hshadow, RGB(255, 255, 255));
	hCD = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP37));
	hrgn_cd = BitmapToRgn(hCD, RGB(255, 255, 255));
	hCar = (HBITMAP)LoadImage(NULL, "image/Car.bmp", IMAGE_BITMAP, 50, 40, LR_LOADFROMFILE);
	hPotatomineReady = (HBITMAP)LoadImage(NULL, "image/Potatomine/PotatoMineNotReady.bmp", IMAGE_BITMAP, 75, 55, LR_LOADFROMFILE);
	hPotatominemashed = (HBITMAP)LoadImage(NULL, "image/Potatomine/PotatoMinemashed.bmp", IMAGE_BITMAP, 132, 93, LR_LOADFROMFILE);
	hExplosionSpudow = (HBITMAP)LoadImage(NULL, "image/Potatomine/ExplosionSpudow.bmp", IMAGE_BITMAP, 186, 58, LR_LOADFROMFILE);
	hShroomBulletHit = (HBITMAP)LoadImage(NULL, "image/ShroomBulletHit.bmp", IMAGE_BITMAP, 52, 46, LR_LOADFROMFILE);

	for (i = 1; i <= 8; i++)//大喷菇孢子
	{
		sprintf_s(name, "image/Fumeshroom/FumeShroombullet (%d).bmp", i);
		hFumeShroombullet[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 343, 64, LR_LOADFROMFILE);
	}
	
	for (i = 0; i < 6; i++)//蘑菇孢子
	{
		sprintf_s(name, "image/Puff/puffshoot (%d).bmp\0", i);
		hPuffshoot[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 57, 28, LR_LOADFROMFILE);
	}

	for (i = 0; i < 29; i++)//阳光
	{
		sprintf_s(name, "image/Sun/Sun%d.bmp\0", i);
		hSun[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 68, 68, LR_LOADFROMFILE);
		hrgn[i] = BitmapToRgn(hSun[i], RGB(0, 0, 0));
	}
	for (i = 0; i < 29; i++)//阳光小
	{
		sprintf_s(name, "image/Sun/Sun%d.bmp\0", i);
		hSunsmall[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 34, 34, LR_LOADFROMFILE);
		hrgnsmall[i] = BitmapToRgn(hSunsmall[i], RGB(0, 0, 0));
	}
	for (i = 0; i < 26; i++)//豌豆射手
	{
		sprintf_s(name, "image/Peashooter/Peashooter%d.bmp\0", i);
		hPeashooter[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 71, 71, LR_LOADFROMFILE);
	}
	hrgn_plant[0] = BitmapToRgn(hPeashooter[0], RGB(0, 0, 0));
	for (i = 0; i < 36; i++)//太阳花
	{
		sprintf_s(name, "image/SunFlower/SunFlower%d.bmp\0", i);
		hSunF[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 73, 74, LR_LOADFROMFILE);
	}
	hrgn_plant[1] = BitmapToRgn(hSunF[0], RGB(0, 0, 0));
	for (i = 0; i < 32; i++)//土豆1
	{
		sprintf_s(name, "image/WallNut/WallNut%d.bmp\0", i);
		hWallNut[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 65, 71, LR_LOADFROMFILE);
	}

	hrgn_plant[2] = BitmapToRgn(hWallNut[0], RGB(0, 0, 0));
	for (i = 0; i < 22; i++)//土豆2
	{
		sprintf_s(name, "image/Wallnut_cracked1/Wallnut_cracked1%d.bmp\0", i);
		hWallnut_cracked1[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 65, 71, LR_LOADFROMFILE);
	}
	for (i = 0; i < 30; i++)//土豆3
	{
		sprintf_s(name, "image/Wallnut_cracked2/Wallnut_cracked2%d.bmp\0", i);
		hWallnut_cracked2[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 65, 71, LR_LOADFROMFILE);
	}

	for (i = 0; i < 30; i++)//寒冰射手
	{
		sprintf_s(name, "image/SnowPea/SnowPea%d.bmp\0", i);
		hSnowPea[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 71, 71, LR_LOADFROMFILE);
	}
	hrgn_plant[3] = BitmapToRgn(hSnowPea[0], RGB(0, 0, 0));

	for (i = 0; i < 30; i++)//豌豆射手2
	{
		sprintf_s(name, "image/Repeater/Repeater%d.bmp\0", i);
		hRepeater[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 73, 71, LR_LOADFROMFILE);
	}
	hrgn_plant[4] = BitmapToRgn(hRepeater[0], RGB(0, 0, 0));

	for (i = 0; i < 42; i++)//食人花
	{
		sprintf_s(name, "image/Chomper/Chomper%d.bmp\0", i);
		hChomper[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 130, 114, LR_LOADFROMFILE);
	}
	hrgn_plant[5] = BitmapToRgn(hChomper[0], RGB(0, 0, 0));
	for (i = 0; i < 16; i++)//辣椒
	{
		sprintf_s(name, "image/Jalapeno/Jalapeno%d.bmp\0", i);
		hJalapeno[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 68, 89, LR_LOADFROMFILE);
	}
	hrgn_plant[6] = BitmapToRgn(hJalapeno[0], RGB(0, 0, 0));
	for (i = 0; i < 25; i++)//辣椒攻击
	{
		sprintf_s(name, "image/JalapenoAttack/JalapenoAttack%d.bmp\0", i);
		hJalapenoAttack[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 755, 131, LR_LOADFROMFILE);
	}
	for (i = 0; i < 34; i++)//倭瓜
	{
		sprintf_s(name, "image/Squash/Squash%d.bmp\0", i);
		hSquash[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 100, 226, LR_LOADFROMFILE);
	}
	hrgn_plant[7] = BitmapToRgn(hSquash[0], RGB(0, 0, 0));

	for (i = 0; i < 8; i++) {//土豆雷
		sprintf_s(name, "image/Potatomine/potato%d.bmp\0", i);
		hPotatomine[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 74, 53, LR_LOADFROMFILE);
	}
	hrgn_plant[8] = BitmapToRgn(hPotatomine[0], RGB(255, 255, 255));

	for (i = 0; i < 21; i++) {//阳光菇,小
		sprintf_s(name, "image/Sunshroom/Sunshroom (%d).bmp\0", i);
		hSunshroom[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 45, 43, LR_LOADFROMFILE);
	}
	hrgn_plant[9] = BitmapToRgn(hSunshroom[0], RGB(255, 255, 255));

	for (i = 0; i < 21; i++) {//阳光菇,大
		sprintf_s(name, "image/Sunshroom/Sunshroom (%d).bmp\0", i);
		hSunshroomGrow[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 67, 64, LR_LOADFROMFILE);
	}

	for (i = 0; i < 17; i++) {//大喷菇
		sprintf_s(name, "image/Fumeshroom/Fumeshroom(%d).bmp\0", i);
		hFumeshroom[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 104, 94, LR_LOADFROMFILE);
	}
	hrgn_plant[10] = BitmapToRgn(hFumeshroom[0], RGB(255, 255, 255));

	for (i = 0; i < 18; i++) {//胆小菇
		sprintf_s(name, "image/Scaredyshroom/Scaredyshroom (%d).bmp\0", i);
		hScaredyshroom[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 63, 70, LR_LOADFROMFILE);
	}
	hrgn_plant[11] = BitmapToRgn(hScaredyshroom[0], RGB(255, 255, 255));
	for (i = 0; i < 12; i++) {//胆小菇害怕
		sprintf_s(name, "image/ScaredyshroomS/scaredyshrooms (%d).bmp\0", i);
		hScaredyshrooms[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 63, 46, LR_LOADFROMFILE);
	}

	for (i = 0; i < 43; i++) {//攻击僵尸
		sprintf_s(name, "image/ZombieAttack/ZombieAttack%d.bmp\0", i);
		hZombieAttack[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 60; i++) {//无头行走僵尸
		sprintf_s(name, "image/ZombieLostHead/ZombieLostHead%d.bmp\0", i);
		hZombieLostHead[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 9; i++) {//僵尸头
		sprintf_s(name, "image/ZombieHead/ZombieHead%d.bmp\0", i);
		hZombieHead[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 150, 186, LR_LOADFROMFILE);
	}
	for (i = 0; i < 22; i++) {//攻击僵尸失去头
		sprintf_s(name, "image/ZombieLostHeadAttack/ZombieLostHeadAttack%d.bmp\0", i);
		hZombieLostHeadAttack[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 40; i++) {//僵尸垂死
		sprintf_s(name, "image/ZombieDie/ZombieDie%d.bmp\0", i);
		hZombieDie[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 50; i++) {//boomdie
		sprintf_s(name, "image/BoomDie/BoomDie%d.bmp\0", i);
		hBoomDie[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 59; i++) {//行走僵尸
		sprintf_s(name, "image/Zombie/Zombie%d.bmp\0", i);
		hZombie[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 59; i++) {//行走僵尸（蓝
		sprintf_s(name, "image/Zombieblue/Zombieblue%d.bmp\0", i);
		hZombieblue[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 43; i++) {//攻击僵尸（蓝
		sprintf_s(name, "image/ZombieAttackblue/ZombieAttackblue%d.bmp\0", i);
		hZombieAttackblue[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 166, 144, LR_LOADFROMFILE);
	}
	for (i = 0; i < 19; i++) {//攻击食人花
		sprintf_s(name, "image/ChomperAttack/ChomperAttack%d.bmp\0", i);
		hChomperAttack[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 130, 114, LR_LOADFROMFILE);
	}
	for (i = 0; i < 19; i++) {//进食食人花
		sprintf_s(name, "image/ChomperDigest/ChomperDigest%d.bmp\0", i);
		hChomperDigest[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 130, 114, LR_LOADFROMFILE);
	}
	for (i = 0; i < 33; i++) {//攻击倭瓜
		sprintf_s(name, "image/SquashAttack/SquashAttack%d.bmp\0", i);
		hSquashAttack[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 100, 226, LR_LOADFROMFILE);
	}

	for (i = 1; i <= 12; i++) {//植物选项卡
		sprintf_s(name, "image/plant (%d).bmp\0", i);
		hplantnum[i] = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 50, 71, LR_LOADFROMFILE);
	}

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		if (planted != -1 || scooped == 1) {
			hcposx = (int)LOWORD(lParam);
			hcposy = (int)HIWORD(lParam);
		}
		movex = LOWORD(lParam);
		movey = HIWORD(lParam);
		return 0;
	case WM_LBUTTONDOWN:
		markposx = LOWORD(lParam);
		markposy = HIWORD(lParam);
		lbuttondown = 1;
		return 0;
	case WM_LBUTTONUP:
		releasex = LOWORD(lParam);
		releasey = HIWORD(lParam);
		lbuttonup = 1;
		return 0;
	case WM_DESTROY:
		DeleteObject(hbg);
		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteObject(hZombie);
		ReleaseDC(hWnd, hdc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void paintStart(HDC hdc) {//开始游戏菜单
	HCURSOR hcursor;
	static int i = 1, j;
	/*菜单背景*/
	SelectObject(bufdc, hStart);
	GetObject(hStart, sizeof(BITMAP), &bg);
	BitBlt(mdc, -75, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);

	if (movex > 450 && movex < 700 && movey >100 && movey < 200) {
		if (i != 0) {
			i = 0;
			mciSendString("play .\\image\\music\\ui_button.wav", NULL, 0, NULL);
		}
		hcursor = LoadCursor(NULL, IDC_HAND);//设置手型符号
		SetCursor(hcursor);
	}
	else {
		i = 1;
	}
	if (lbuttondown == 1 && markposx > 450 && markposx < 700 && markposy >100 && markposy < 200) {//范围内按下，判定修改，开始游戏
		i = 0;
		STRAT = true;
		END = false;
		CHOOSE = 0;//选择开始
		lbuttondown = -1;
		if (t1 == -1) {
			t1 = GetTickCount();
			mciSendString("stop .\\image\\music\\ui_gkmusic.wav", NULL, 0, NULL);
			mciSendString("play .\\image\\music\\losemusic.wav", NULL, 0, NULL);
		}
	}
	if (GetTickCount() - t1 >= 1000 && t1 != -1) {
		mciSendString("play .\\image\\music\\evillaugh.wav", NULL, 0, NULL);
	}
	SelectObject(bufdc, hSelector);
	GetObject(hSelector, sizeof(BITMAP), &start);
	MyTransparentBlt2(mdc, 400, 80, start.bmWidth, start.bmHeight / 2, bufdc, 0, (start.bmHeight / 2) * i, start.bmWidth, start.bmHeight / 2, RGB(255, 255, 255));

	BitBlt(hdc, 0, 0, wwid, whei, mdc, 0, 0, SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间

}

int	Num = 0,//已经选择植物的数量
kexuan[15] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };//卡片坐标
void ChooseP(HDC hdc) {//选择植物界面
	static int i, j, flag = 0;
	void setplant(int i);
	void cancelplant(int i);
	static int bkgroundx = 0,
		plantchooseboxx = 600,
		cardx[9] = { 0,35,90,145,200,255,310,365,420 }, cardy[9] = { 0,0,0,0,0,0,0,0,0 };

	if (CHOOSE == 0) {
		if (bkgroundx > -550) {
			bkgroundx = bkgroundx - 10;
		}
		else bkgroundx = -550;
	}


	SelectObject(bufdc, hbg);//移动的背景
	GetObject(hbg, sizeof(BITMAP), &bg);//最终x为-215
	BitBlt(mdc, bkgroundx, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);



	if (bkgroundx == -550 && flag == 0) {
		if (plantchooseboxx - 13 > 88) {
			plantchooseboxx = plantchooseboxx - 50;
		}
		else plantchooseboxx = 88;
		SelectObject(bufdc, hplantchoosebox);//植物备选框
		GetObject(hplantchoosebox, sizeof(BITMAP), &bg);
		MyTransparentBlt2(mdc, 20, plantchooseboxx, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, bg.bmWidth, bg.bmHeight, RGB(0, 0, 0));

		SelectObject(bufdc, hplantchoose);//植物已选框
		GetObject(hplantchoose, sizeof(BITMAP), &bg);
		BitBlt(mdc, 20, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
		if (plantchooseboxx == 88) {//备选框移动完成
			if (lbuttondown == 1) {//左键按下
				if (markposy > plantchooseboxx + 50 && markposy < plantchooseboxx + 120 && Num < 8) {
					for (i = 1; i < 9; i++) {//选植物，这里的i代表种类
						if (markposx > i * 55 - 20 && markposx < i * 55 + 30 && kexuan[i] == 1) {
							Num++;
							lbuttondown = -1;
							kexuan[i] = -1;
							setplant(i);//设置选择框的属性
							break;
						}

					}
				}
				else if (markposy > plantchooseboxx + 130 && markposy < plantchooseboxx + 200 && Num < 8) {
					for (i = 9; i <= 12; i++) {//第二行植物选择
						if (markposx > (i - 8) * 55 - 20 && markposx < (i - 8) * 55 + 30 && kexuan[i] == 1) {
							Num++;
							lbuttondown = -1;
							kexuan[i] = -1;
							setplant(i);//设置选择框的属性
							break;
						}
					}
				}
				else if (markposy > -130 && markposy < plantchooseboxx) {
					for (i = 0; i < Num; i++) {//取消植物，这里的i代表卡片向量中的植物顺序
						if (markposx > 45 + (i + 1) * 55 && markposx < 95 + (i + 1) * 55 && Plantvri_v[i].ex == 1) {
							Num--;
							lbuttondown = -1;
							kexuan[Plantvri_v[i].vri] = 1;
							cancelplant(i);
							break;
						}
					}
				}
				else if (markposy > plantchooseboxx + 400 && markposy < plantchooseboxx + 657) {//点击开始游戏
					if (Num == 8) {
						if (markposx > 170 && markposx < 327) {
							flag = 1;
							lbuttondown = -1;
							for (i = 0; i < 8; i++) {
								switch (Plantvri_v[i].vri) {//初始化植物信息
								case 1:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 100; break;
								case 2:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 50; break;
								case 3:cd[i].ali = -1; cd[i].maxlasting = 30000; varvalue[i] = 50; break;
								case 4:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 175; break;
								case 5:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 200; break;
								case 6:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 150; break;
								case 7:cd[i].ali = -1; cd[i].maxlasting = 50000; varvalue[i] = 125; break;
								case 8:cd[i].ali = -1; cd[i].maxlasting = 30000; varvalue[i] = 50; break;
								case 9:cd[i].ali = -1; cd[i].maxlasting = 20000; varvalue[i] = 25; break;
								case 10:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 25; break;
								case 11:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 75; break;
								case 12:cd[i].ali = -1; cd[i].maxlasting = 7500; varvalue[i] = 25; break;
								}
							}
						}
					}
				}
			}
		}
		for (i = 0; i < Num; i++) {
			SelectObject(bufdc, hplantnum[Plantvri_v[i].vri]);//画已选择的植物
			GetObject(hplantnum[Plantvri_v[i].vri], sizeof(BITMAP), &bg);
			BitBlt(mdc, 45 + (i + 1) * 55, plantchooseboxx - 80, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
		}
		for (i = 1; i <= 8; i++) {
			if (kexuan[i] == 1) {
				SelectObject(bufdc, hplantnum[i]);//未选择的备选植物
				GetObject(hplantnum[i], sizeof(BITMAP), &bg);
				BitBlt(mdc, cardx[i], plantchooseboxx + 50 + cardy[i], bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
			}
		}
		for (i = 9; i <= 12; i++) {
			if (kexuan[i] == 1) {
				SelectObject(bufdc, hplantnum[i]);//未选择的备选植物
				GetObject(hplantnum[i], sizeof(BITMAP), &bg);
				BitBlt(mdc, cardx[i - 8], plantchooseboxx + 60 + cardy[i - 8] + bg.bmHeight, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
			}
		}
		if (Num == 8) {//可开始游戏，开始游戏确认框
			SelectObject(bufdc, hfight2);
			GetObject(hfight2, sizeof(BITMAP), &bg);
			BitBlt(mdc, 170, plantchooseboxx + 400, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
		}
		else {
			SelectObject(bufdc, hfight1);
			GetObject(hfight1, sizeof(BITMAP), &bg);
			BitBlt(mdc, 170, plantchooseboxx + 400, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
		}
	}
	if (flag == 1) {
		SelectObject(bufdc, hplantchoosebox);//植物备选框
		GetObject(hplantchoosebox, sizeof(BITMAP), &bg);
		MyTransparentBlt2(mdc, 20, plantchooseboxx, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, bg.bmWidth, bg.bmHeight, RGB(0, 0, 0));

		SelectObject(bufdc, hplantchoose);//植物已选框
		GetObject(hplantchoose, sizeof(BITMAP), &bg);
		BitBlt(mdc, 20, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);

		for (i = 0; i < Num; i++) {
			SelectObject(bufdc, hplantnum[Plantvri_v[i].vri]);//画已选择的植物
			GetObject(hplantnum[Plantvri_v[i].vri], sizeof(BITMAP), &bg);
			BitBlt(mdc, 45 + (i + 1) * 55, 8, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
		}
	}

	if (flag == 1) {
		if (plantchooseboxx < 600) {
			plantchooseboxx = plantchooseboxx + 50;
		}
		else if (bkgroundx < -215) {
			bkgroundx = bkgroundx + 40;
		}
		else {
			CHOOSE = -1;
			mciSendString("stop .\\image\\music\\lookup.wav", NULL, 0, NULL);
			flag = 0;
		}
	}
	BitBlt(hdc, 0, 0, wwid, whei, mdc, 0, 0, SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间
}

void setplant(int i) {//i代表品种
	Plantvri newplant;
	newplant.ex = 1;
	newplant.vri = i;
	Plantvri_v.push_back(newplant);
}

void cancelplant(int i) {//i表示向量中植物的位置
	Plantvri_v.erase(Plantvri_v.begin() + i);
}

void paintPrepare(HDC hdc) {//准备安放植物
	int i;
	SelectObject(bufdc, hbg);
	GetObject(hbg, sizeof(BITMAP), &bg);
	BitBlt(mdc, -200, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);

	SelectObject(bufdc, hplantchoose);//植物已选框
	GetObject(hplantchoose, sizeof(BITMAP), &bg);
	BitBlt(mdc, 20, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
	for (i = 0; i < 8; i++) {
		SelectObject(bufdc, hplantnum[Plantvri_v[i].vri]);//画已选择的植物
		GetObject(hplantnum[Plantvri_v[i].vri], sizeof(BITMAP), &bg);
		BitBlt(mdc, 45 + (i + 1) * 55, 8, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
	}

	SelectObject(bufdc, hscpbox);
	GetObject(hscpbox, sizeof(BITMAP), &bg);
	BitBlt(mdc, 542, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
	SelectObject(bufdc, hscp);
	GetObject(hscp, sizeof(BITMAP), &start);
	MyTransparentBlt2(mdc, 550, 0, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	if (GetTickCount() - t2 < 500)
	{
		SelectObject(bufdc, hPrepareGrowPlants);
		GetObject(hPrepareGrowPlants, sizeof(BITMAP), &start);
		MyTransparentBlt2(mdc, 300, 200, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	}
	else if (GetTickCount() - t2 < 1000) {
		SelectObject(bufdc, hPrepareGrowPlantsok);
		GetObject(hPrepareGrowPlantsok, sizeof(BITMAP), &start);
		MyTransparentBlt2(mdc, 350, 200, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	}
	else if (GetTickCount() - t2 < 1500) {
		SelectObject(bufdc, hPrepareGrowPlantsgo);
		GetObject(hPrepareGrowPlantsgo, sizeof(BITMAP), &start);
		MyTransparentBlt2(mdc, 300, 180, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	}
	BitBlt(hdc, 0, 0, wwid, whei, mdc, 0, 0, SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间
}

void paintlargewave(HDC hdc) {
	SelectObject(bufdc, hLargeWave);
	GetObject(hLargeWave, sizeof(BITMAP), &start);
	MyTransparentBlt2(mdc, 300, 200, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(246, 255, 226));
}

int poscard;
void MyPaint(HDC hdc) {
	int i, j;
	/*处理设备消息*/
	if (lbuttondown == 1) {
		/*选择要种的植物*/

		if (planted == -1) {
			for (i = 1; i <= 8; i++) {
				if (markposx > 100 + (i - 1) * 50 + (i - 1) * 5 && markposx < 100 + i * 50 + (i - 1) * 5 && markposy > 12 && markposy < 72)
				{
					if (moneyless[i - 1].ali == -1 && cd[i - 1].ali == -1) {
						hcposx = markposx;
						hcposy = markposy;
						planted = Plantvri_v[i - 1].vri;//选择植物的种类
						poscard = i - 1;

						mciSendString("stop .\\image\\music\\seedlift.wav", NULL, 0, NULL);
						mciSendString("play .\\image\\music\\seedlift.wav", NULL, 0, NULL);
					}
				}
			}
		}
		else if (planted != -1) {/*种植物*/
			for (i = 0; i < 9; i++) {
				if (markposx > 45 + i * 80 && markposx < 45 + (i + 1) * 80) {
					for (j = 0; j < 5; j++) {
						if (markposy > 80 + j * 100 && markposy < 80 + (j + 1) * 100 && Pla[j][i].ali == false)
						{
							Pla[j][i].ali = true;
							Pla[j][i].num = 0;
							Pla[j][i].blood = 100;//初始血量100

							cd[poscard].starttime = GetTickCount();
							cd[poscard].ali = 1;

							mciSendString("stop .\\image\\music\\planted.wav", NULL, 0, NULL);
							mciSendString("play .\\image\\music\\planted.wav", NULL, 0, NULL);

							if (planted == 1) {
								Pla[j][i].vri = 1;//豌豆射手
								money -= 100;
								Pla[j][i].maxnum = 26;
								Pla[j][i].tshoot = GetTickCount();
							}
							if (planted == 2) {
								Pla[j][i].vri = 2;//太阳花
								money -= 50;
								Pla[j][i].maxnum = 36;
								Pla[j][i].tshoot = GetTickCount();
							}
							if (planted == 3) {
								Pla[j][i].vri = 3;//坚果
								money -= 50;
								Pla[j][i].blood = 2000;
								Pla[j][i].maxnum = 32;
							}
							if (planted == 4) {
								Pla[j][i].vri = 4;//寒冰射手
								money -= 175;
								Pla[j][i].maxnum = 30;
							}
							if (planted == 5) {
								Pla[j][i].vri = 5;//双发豌豆
								money -= 200;
								Pla[j][i].maxnum = 30;
								Pla[j][i].tshoot = GetTickCount();
							}
							if (planted == 6) {
								Pla[j][i].vri = 6;//食人花
								money -= 150;
								Pla[j][i].recover = -1;
								Pla[j][i].maxnum = 42;
							}
							if (planted == 7) {
								Pla[j][i].vri = 7;//辣椒
								money -= 125;
								Pla[j][i].maxnum = 16;
							}
							if (planted == 8) {
								Pla[j][i].vri = 8;//倭瓜
								money -= 50;
								Pla[j][i].maxnum = 34;
							}
							if (planted == 9) {
								Pla[j][i].vri = 9;//土豆雷
								money -= 25;
								Pla[j][i].maxnum = 8;
								Pla[j][i].grow = GetTickCount();
							}
							if (planted == 10) {
								Pla[j][i].vri = 10;//阳光菇
								money -= 25;
								Pla[j][i].maxnum = 21;
								Pla[j][i].grow = GetTickCount();
							}
							if (planted == 11) {
								Pla[j][i].vri = 11;//大喷菇
								money -= 25;
								Pla[j][i].maxnum = 17;
							}
							if (planted == 12) {
								Pla[j][i].vri = 12;//胆小菇
								money -= 25;
								Pla[j][i].maxnum = 18;
							}
							planted = -1;
							break;
						}
					}
				}
			}
			if (planted != -1) {
				planted = -1;
			}
		}

		if (scooped == -1) {/*选择铲子*/
			if (markposx > 550 && markposx < 600 && markposy > 0 && markposy < 72) {
				mciSendString("play .\\image\\music\\shovel.wav", NULL, 0, NULL);
				hcposx = markposx;
				hcposy = markposy;
				scooped = 1;

			}
		}
		else if (scooped == 1) {/*铲掉植物*/
			for (i = 0; i < 9; i++) {
				if (hcposx > 60 + i * 80 && hcposx < 60 + (i + 1) * 80) {
					for (j = 0; j < 5; j++) {
						if (hcposy > 80 + j * 100 && hcposy < 80 + (j + 1) * 100) {
							if (Pla[j][i].ali == true) {
								Pla[j][i].ali = false;
								scooped = -1;
								mciSendString("stop .\\image\\music\\planted2.wav", NULL, 0, NULL);
								mciSendString("play .\\image\\music\\planted2.wav", NULL, 0, NULL);
							}
							else {
								scooped = -1;
							}
						}
					}
				}
			}
		}
		//收太阳
		vector<Sun>::iterator Sun_iter;
		for (Sun_iter = Sun_v.begin(); Sun_iter != Sun_v.end(); ) {
			SelectObject(bufdc, hSun[0]);
			GetObject(hSun[0], sizeof(BITMAP), &zom);
			if (markposx > Sun_iter->x && markposx < Sun_iter->x + zom.bmWidth && markposy > Sun_iter->y && markposy < Sun_iter->y + zom.bmHeight) {//判断单击太阳
				Sun_iter->spx = (Sun_iter->x - 20) * 0.1;
				Sun_iter->spy = (0 + Sun_iter->y) * 0.1;
				Sun_coll.push_back(*Sun_iter);
				Sun_iter = Sun_v.erase(Sun_iter);
				mciSendString("stop .\\image\\music\\point.wav", NULL, 0, NULL);
				mciSendString("play .\\image\\music\\point.wav", NULL, 0, NULL);
			}
			else {
				Sun_iter++;
			}
		}
		lbuttondown = 0;
	}

	//先画背景到小dc上
	SelectObject(bufdc, hbg);
	GetObject(hbg, sizeof(BITMAP), &bg);
	BitBlt(mdc, -215, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);

	SelectObject(bufdc, hCar);
	GetObject(hCar, sizeof(BITMAP), &start);
	for (int i = 0; i < 5; i++) {
		MyTransparentBlt2(mdc, -15, 93 + i * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(24, 0, 24));
	}

	/*------------------------------植物选择框------------------------------------*/
	SelectObject(bufdc, hplantchoose);//植物已选框
	GetObject(hplantchoose, sizeof(BITMAP), &bg);
	BitBlt(mdc, 20, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
	for (i = 0; i < 8; i++) {
		SelectObject(bufdc, hplantnum[Plantvri_v[i].vri]);//画已选择的植物
		GetObject(hplantnum[Plantvri_v[i].vri], sizeof(BITMAP), &bg);
		BitBlt(mdc, 45 + (i + 1) * 55, 8, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
	}
	for (i = 0; i < 8; i++) {//没钱不能选
		if (money - varvalue[i] < 0.0) {
			moneyless[i].ali = 1;
			AlphaBmp(mdc, hCD, i * 55 + 100, 8, 100, hrgn_cd, RGB(0, 0, 0));
		}
		else {
			moneyless[i].ali = -1;
		}
	}
	//cd遮罩
	for (i = 0; i < 8; i++) {
		if (cd[i].ali == 1) {
			AlphaBmp(mdc, hCD, i * 55 + 100, 8 - ((GetTickCount() - cd[i].starttime) / cd[i].maxlasting) * 71.0, 100, hrgn_cd, RGB(0, 0, 0));
		}
		if (GetTickCount() - cd[i].starttime >= cd[i].maxlasting) {
			cd[i].ali = -1;
		}
	}
	/*------------------------------框------------------------------------*/
	SelectObject(bufdc, hscpbox);//框
	GetObject(hscpbox, sizeof(BITMAP), &bg);
	BitBlt(mdc, 542, 0, bg.bmWidth, bg.bmHeight, bufdc, 0, 0, SRCCOPY);
	/*------------------------------输出阳光数量------------------------------------*/
	TEXTMETRIC tm;			//定义包含字体信息的结构体变量.
	HFONT hf;
	static char hc_pos[20];
	sprintf_s(hc_pos, "%d\0", money);
	hf = myCreateFont(mdc, 20, 0);
	SelectObject(bufdc, hf);			//选入字体.
	GetTextMetrics(bufdc, &tm);
	SetTextColor(bufdc, RGB(255, 0, 0));	//设置字体的颜色
	TextOut(mdc, 50, 65, hc_pos, strlen(hc_pos));

	for (j = 0; j < 5; j++) {//对僵尸向量进行升序排序
		sort(Zombie_v[j].begin(), Zombie_v[j].end(), cmp);
	}
	/*-------------------------------------僵尸----------------------------------------*/
	vector<Zomb>::iterator iter, iter2;
	for (j = 0; j < 5; j++) {
		for (iter = Zombie_v[j].begin(); iter != Zombie_v[j].end();) {
			for (i = 0; i < 9; i++) {
				if (Pla[j][i].ali == true && (Pla[j][i].vri != 7 && Pla[j][i].vri != 8)) {//种了植物,辣椒和倭瓜不能吃
					if (iter->x < 60 + (i) * 80 && iter->x>60 + (i - 1) * 80) {
						if (iter->condition == 1) {//第一次遇到植物
							iter->speed = 0;
							iter->condition = 2;//攻击状态
							iter->maxnum = 43;
							iter->num = 0;
							iter->i = i;//记录下正在攻击的植物的行列数
							iter->j = j;
						}
						if (Pla[j][i].blood >= 0 && iter->condition == 2) {//植物还有血
							mciSendString("play .\\image\\music\\Chomp.wav", NULL, 0, NULL);
							Pla[j][i].blood -= 1;//植物扣血
						}
						else if (Pla[j][i].blood < 0) {//植物死了的下一帧所有攻击这个植物的僵尸都要恢复正常状态1
							Pla[j][i].ali = false;
							for (iter2 = Zombie_v[j].begin(); iter2 != Zombie_v[j].end(); iter2++) {
								if (iter2->i == i && iter2->j == j && iter2->condition == 2) {
									iter2->speed = 0.5;
									iter2->condition = 1;//僵尸恢复正常
									iter2->maxnum = 59;
									iter2->num = 0;
								}
							}
						}
					}
				}
			}


			if (iter->blood <= 0) {
				if (iter->condition == 1) {
					iter->condition = 3;//没有吃植物状态下失去头
					iter->maxnum = 60;
					iter->num = 0;
				}
				else if (iter->condition == 2) {
					iter->condition = 4;//吃植物状态下失去头
					iter->maxnum = 22;
					iter->num = 0;
				}
				else if ((iter->condition == 3 && iter->num == 60) || (iter->condition == 4 && iter->num == 22)) {
					iter->condition = 5;//boomdie
					iter->maxnum = 40;
					iter->num = 0;
				}
			}
			if (iter->num == iter->maxnum)
				iter->num = 0;

			if (!(iter->condition == 5)) {//倒下的僵尸不用画阴影
				AlphaBmp(mdc, hshadow, iter->x + 75, 130 + j * 100, 100, hrgn_shadow, RGB(255, 255, 255));
			}

			if (iter->condition == 1) {//行走
				if (GetTickCount() - iter->recover > 3000 || iter->recover == -1) {//超过3秒僵尸恢复正常
					iter->recover = -1;
					iter->speed = 0.5;
					SelectObject(bufdc, hZombie[iter->num]);
					GetObject(hZombie[iter->num], sizeof(BITMAP), &zom);
					MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(0, 0, 0));
				}
				else {//被冰冻了
					SelectObject(bufdc, hZombieblue[iter->num]);
					GetObject(hZombieblue[iter->num], sizeof(BITMAP), &zom);
					MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(33, 50, 122));
				}
				iter->x -= iter->speed;
			}
			else if (iter->condition == 2) {//攻击僵尸
				if (GetTickCount() - iter->recover > 1000 || iter->recover == -1) {//超过1秒僵尸恢复正常
					iter->recover = -1;
					iter->speed = 0.5;
					SelectObject(bufdc, hZombieAttack[iter->num]);
					GetObject(hZombieAttack[iter->num], sizeof(BITMAP), &zom);
					MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(0, 0, 0));
				}
				else {
					SelectObject(bufdc, hZombieAttackblue[iter->num]);
					GetObject(hZombieAttackblue[iter->num], sizeof(BITMAP), &zom);
					MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(33, 50, 122));
				}
			}
			else if (iter->condition == 3) {//没有吃植物情况下失去头
				SelectObject(bufdc, hZombieLostHead[iter->num]);
				GetObject(hZombieLostHead[iter->num], sizeof(BITMAP), &zom);
				MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(0, 0, 0));
				iter->x -= iter->speed;
			}
			else if (iter->condition == 4) {//攻击状态下失去头
				SelectObject(bufdc, hZombieLostHeadAttack[iter->num]);
				GetObject(hZombieLostHeadAttack[iter->num], sizeof(BITMAP), &zom);
				MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(0, 0, 0));
			}
			else if (iter->condition == 5) {//boomdie垂死
				SelectObject(bufdc, hZombieDie[iter->num]);
				GetObject(hZombieDie[iter->num], sizeof(BITMAP), &zom);
				MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(0, 0, 0));
			}
			else if (iter->condition == 6) {//炸成灰
				SelectObject(bufdc, hBoomDie[iter->num]);
				GetObject(hBoomDie[iter->num], sizeof(BITMAP), &zom);
				MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(0, 255, 0));
			}
			iter->num++;

			if (iter->condition == 3 || iter->condition == 4) {//画头
				if (iter->head.num < iter->head.maxnum) {
					SelectObject(bufdc, hZombieHead[iter->head.num]);
					GetObject(hZombieHead[iter->head.num], sizeof(BITMAP), &zom);
					MyTransparentBlt2(mdc, iter->x, 15 + j * 100, zom.bmWidth, zom.bmHeight, bufdc, 0, 0, zom.bmWidth, zom.bmHeight, RGB(0, 0, 0));
					iter->head.num++;
				}
			}

			if ((iter->condition == 5 || iter->condition == 6) && iter->num == iter->maxnum) {//全部状态画完出栈
				iter->y = 15 + j * 100;
				iter = Zombie_v[j].erase(iter);
			}
			else {
				iter++;
			}
		}
	}

	/*--------------------------------------植物------------------------------------------*/
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 5; j++) {
			if (Pla[j][i].ali == true) {//有植物
				//画阴影
				if (!(Pla[j][i].vri == 7 && Pla[j][i].maxnum == 25) && Pla[j][i].vri != 8) {//辣椒特判一下爆炸后不用画阴影
					AlphaBmp(mdc, hshadow, 37 + i * 80, 130 + j * 100, 100, hrgn_shadow, RGB(255, 255, 255));
				}
				if (Pla[j][i].num == Pla[j][i].maxnum) {
					Pla[j][i].num = 0;
				}
				if (Pla[j][i].vri == 1) {//豌豆射手
					SelectObject(bufdc, hPeashooter[Pla[j][i].num]);
					GetObject(hPeashooter[Pla[j][i].num], sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, 47 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					if (!Zombie_v[j].empty()) {//当前行有僵尸
						if (GetTickCount() - Pla[j][i].tshoot >= 2500) {//2.5秒发射一个豌豆
							mciSendString("play .\\image\\music\\throw.wav", NULL, 0, NULL);
							Peb temppball;
							temppball.x = 80 + i * 80;
							temppball.y = 80 + j * 100;
							temppball.speed = 10;
							temppball.j = j;
							temppball.vri = 1;
							Peb_v.push_back(temppball);
							Pla[j][i].tshoot = GetTickCount();
						}
					}
				}
				else if (Pla[j][i].vri == 2) {//太阳花
					SelectObject(bufdc, hSunF[Pla[j][i].num]);
					GetObject(hSunF[Pla[j][i].num], sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, 45 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					if (GetTickCount() - Pla[j][i].tshoot > 24000) {//24秒产出一个阳光
						Sun tempsun;
						tempsun.num = 0;
						tempsun.stime = -1;//没有落地
						tempsun.x = rand() % 60 + 30 + i * 80;
						tempsun.y = 60 + j * 100;
						tempsun.spy = 4;
						tempsun.maxnum = 29;
						tempsun.size = 1;
						switch (j) {
						case 0:
							tempsun.maxy = 110;
							break;
						case 1:
							tempsun.maxy = 210;
							break;
						case 2:
							tempsun.maxy = 310;
							break;
						case 3:
							tempsun.maxy = 410;
							break;
						case 4:
							tempsun.maxy = 510;
							break;
						}
						Sun_v.push_back(tempsun);
						Pla[j][i].tshoot = GetTickCount();
					}
				}
				else if (Pla[j][i].vri == 3) {//坚果墙
					if (Pla[j][i].blood > 1500) {
						SelectObject(bufdc, hWallNut[Pla[j][i].num]);
						GetObject(hWallNut[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 45 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					}
					else if (Pla[j][i].blood > 750) {
						if (Pla[j][i].maxnum == 32) {
							Pla[j][i].maxnum = 22;
							Pla[j][i].num = 0;
						}
						SelectObject(bufdc, hWallnut_cracked1[Pla[j][i].num]);
						GetObject(hWallnut_cracked1[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 45 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					}
					else {
						if (Pla[j][i].maxnum == 22) {
							Pla[j][i].maxnum = 30;
							Pla[j][i].num = 0;
						}
						SelectObject(bufdc, hWallnut_cracked2[Pla[j][i].num]);
						GetObject(hWallnut_cracked2[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 45 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					}
				}
				else if (Pla[j][i].vri == 4) {//寒冰射手
					SelectObject(bufdc, hSnowPea[Pla[j][i].num]);
					GetObject(hSnowPea[Pla[j][i].num], sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, 45 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					if (!Zombie_v[j].empty()) {//当前行有僵尸
						if (GetTickCount() - Pla[j][i].tshoot >= 2500) {//2.5秒发射一个冰冻豌豆
							mciSendString("play .\\image\\music\\throw.wav", NULL, 0, NULL);
							Peb temppball;
							temppball.x = 80 + i * 80;
							temppball.y = 80 + j * 100;
							temppball.speed = 10;
							temppball.j = j;
							temppball.vri = 2;
							Peb_v.push_back(temppball);
							Pla[j][i].tshoot = GetTickCount();
						}
					}
				}
				else if (Pla[j][i].vri == 5) {//双发射手
					SelectObject(bufdc, hRepeater[Pla[j][i].num]);
					GetObject(hRepeater[Pla[j][i].num], sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, 45 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));

					if (!Zombie_v[j].empty()) {//当前行有僵尸
						if (GetTickCount() - Pla[j][i].tshoot >= 1250) {//1.25秒发射一个豌豆
							mciSendString("play .\\image\\music\\throw.wav", NULL, 0, NULL);
							Peb temppball;
							temppball.x = 80 + i * 80;
							temppball.y = 80 + j * 100;
							temppball.speed = 10;
							temppball.j = j;
							temppball.vri = 1;
							Peb_v.push_back(temppball);
							Pla[j][i].tshoot = GetTickCount();
						}
					}
				}
				else if (Pla[j][i].vri == 6) {//食人花
					if (!Zombie_v[j].empty() && Pla[j][i].recover == -1) {
						vector<Zomb>::iterator Zombie_iter4;
						for (Zombie_iter4 = Zombie_v[j].begin(); Zombie_iter4 != Zombie_v[j].end(); ) {
							if (Zombie_iter4->x >= i * 80 && Zombie_iter4->x <= 30 + (i + 1) * 80) {
								Pla[j][i].maxnum = 20;
								Pla[j][i].num = 0;
								Pla[j][i].recover = GetTickCount();
								Zombie_iter4->disapper = 1;
								break;
							}
							else {
								Zombie_iter4++;
							}
						}
					}
					else if (Pla[j][i].recover != -1 && Pla[j][i].maxnum == 20 && Pla[j][i].num == 19) {
						Pla[j][i].maxnum = 19;
						Pla[j][i].num = 0;
					}
					else if (Pla[j][i].recover != -1 && Pla[j][i].maxnum == 19 && GetTickCount() - Pla[j][i].recover > 15000) {
						Pla[j][i].recover = -1;
						Pla[j][i].maxnum = 42;
						Pla[j][i].num = 0;
					}

					if (Pla[j][i].maxnum == 42) {
						SelectObject(bufdc, hChomper[Pla[j][i].num]);
						GetObject(hChomper[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 45 + i * 80, 40 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					}
					else if (Pla[j][i].maxnum == 20) {
						if (Pla[j][i].num == 13) {
							vector<Zomb>::iterator Zombie_iter4;
							for (Zombie_iter4 = Zombie_v[j].begin(); Zombie_iter4 != Zombie_v[j].end(); ) {
								if (Zombie_iter4->disapper == 1) {
									Zombie_iter4 = Zombie_v[j].erase(Zombie_iter4);
									break;
								}
								else {
									Zombie_iter4++;
								}
							}
						}
						mciSendString("play .\\image\\music\\bigchomp.wav", NULL, 0, NULL);
						SelectObject(bufdc, hChomperAttack[Pla[j][i].num]);
						GetObject(hChomperAttack[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 45 + i * 80, 40 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					}
					else if (Pla[j][i].maxnum == 19) {
						SelectObject(bufdc, hChomperDigest[Pla[j][i].num]);
						GetObject(hChomperDigest[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 45 + i * 80, 40 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					}
				}
				else if (Pla[j][i].vri == 7) {//辣椒
					if (Pla[j][i].maxnum == 16 && Pla[j][i].num <= 15) {
						SelectObject(bufdc, hJalapeno[Pla[j][i].num]);
						GetObject(hJalapeno[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 45 + i * 80, 75 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
						if (Pla[j][i].num == 15) {
							mciSendString("play .\\image\\music\\boom.wav", NULL, 0, NULL);
							Pla[j][i].maxnum = 25;
							Pla[j][i].num = 0;
						}
					}
					else if (Pla[j][i].maxnum == 25 && Pla[j][i].num <= 24) {
						SelectObject(bufdc, hJalapenoAttack[Pla[j][i].num]);
						GetObject(hJalapenoAttack[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 37, 30 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
						vector<Zomb>::iterator Zombie_iter3;
						for (Zombie_iter3 = Zombie_v[j].begin(); Zombie_iter3 != Zombie_v[j].end(); Zombie_iter3++) {
							Zombie_iter3->blood = -1;
							Zombie_iter3->condition = 6;
							Zombie_iter3->maxnum = 50;
							Zombie_iter3->num = 0;
						}
						if (Pla[j][i].num == 24) {
							Pla[j][i].ali = false;
						}
					}
				}
				else if (Pla[j][i].vri == 8) {//倭瓜
					if (Pla[j][i].maxnum == 34) {//正常状态
						SelectObject(bufdc, hSquash[Pla[j][i].num]);
						GetObject(hSquash[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 30 + i * 80, -70 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
						if (!Zombie_v[j].empty()) {
							vector<Zomb>::iterator Zombie_iter4;
							for (Zombie_iter4 = Zombie_v[j].begin(); Zombie_iter4 != Zombie_v[j].end(); ) {
								if (Zombie_iter4->x >= 45 + (i + 1) * 80 - 80 && Zombie_iter4->x <= 45 + (i + 2) * 80 - 80) {
									Pla[j][i].maxnum = 33;
									Pla[j][i].num = 0;
									Pla[j][i].tshoot = 1;
									break;
								}
								else {
									Zombie_iter4++;
								}
							}
						}
					}
					else if (Pla[j][i].maxnum == 33) {//攻击状态
						if (Pla[j][i].tshoot > 0 && Pla[j][i].tshoot <= 6) {//斜视
							mciSendString("play .\\image\\music\\squash_hmm.mp3", NULL, 0, NULL);
							Pla[j][i].num = 0;
							SelectObject(bufdc, hSquashR);
							GetObject(hSquashR, sizeof(BITMAP), &start);
							MyTransparentBlt2(mdc, 30 + i * 80, -70 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 0, 0));
							Pla[j][i].tshoot++;
						}
						else if (Pla[j][i].tshoot > 6) {//上天
							Pla[j][i].num = 0;
							SelectObject(bufdc, hSquash[0]);
							GetObject(hSquash[0], sizeof(BITMAP), &start);
							MyTransparentBlt2(mdc, 30 + i * 80 + (Pla[j][i].tshoot - 6) * 8, -70 + j * 100 - (Pla[j][i].tshoot - 6) * 8, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
							Pla[j][i].tshoot++;
							if (Pla[j][i].tshoot == 16) {
								Pla[j][i].tshoot = -1;
							}
						}
						else {
							if (Pla[j][i].num == 10) {
								mciSendString("play .\\image\\music\\squash.wav", NULL, 0, NULL);
								if (!Zombie_v[j].empty()) {
									vector<Zomb>::iterator Zombie_iter4;
									for (Zombie_iter4 = Zombie_v[j].begin(); Zombie_iter4 != Zombie_v[j].end(); ) {
										if (Zombie_iter4->x >= 45 + (i + 1) * 80 - 80 && Zombie_iter4->x <= 45 + (i + 2) * 80 - 80) {
											Zombie_iter4 = Zombie_v[j].erase(Zombie_iter4);
										}
										else {
											Zombie_iter4++;
										}
									}
								}
							}
							SelectObject(bufdc, hSquashAttack[Pla[j][i].num]);
							GetObject(hSquashAttack[Pla[j][i].num], sizeof(BITMAP), &start);
							MyTransparentBlt2(mdc, 30 + (i + 1) * 80, -70 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
						}
						if (Pla[j][i].num == 32) {
							Pla[j][i].ali = false;
						}
					}
				}
				else if (Pla[j][i].vri == 9) {//土豆雷
					
					if (GetTickCount() - Pla[j][i].grow <= 10000) {
						SelectObject(bufdc, hPotatomineReady);
						GetObject(hPotatomineReady, sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 43 + i * 80, 103 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
					}
					if (GetTickCount() - Pla[j][i].grow >= 10000) {
						if (GetTickCount() - Pla[j][i].grow <= 10100)
						{
							mciSendString("play .\\image\\music\\dirt_rise.wav", NULL, 0, NULL);
						}
						SelectObject(bufdc, hPotatomine[Pla[j][i].num]);
						GetObject(hPotatomine[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 48 + i * 80, 108 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
						vector<Zomb>::iterator Zombie_iter5;
						for (Zombie_iter5 = Zombie_v[j].begin(); Zombie_iter5 != Zombie_v[j].end(); ) {
							if (Zombie_iter5->x >= 80 + i * 80 - 80 && Zombie_iter5->x <= 80 + (i + 1) * 80 - 80) {
								Zombie_iter5 = Zombie_v[j].erase(Zombie_iter5);
								Pla[j][i].maxnum = 0;
								Pla[j][i].grow = GetTickCount();
								break;
							}
							else {
								Zombie_iter5++;
							}
						}
					}
					if (Pla[j][i].maxnum == 0)
					{
						if (GetTickCount() - Pla[j][i].grow <= 10)
							mciSendString("play .\\image\\music\\potatomine.wav", NULL, 0, NULL);
						if (GetTickCount() - Pla[j][i].grow <= 2000) {
							SelectObject(bufdc, hPotatominemashed);
							GetObject(hPotatominemashed, sizeof(BITMAP), &start);
							MyTransparentBlt2(mdc, 38 + i * 80, 68 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
							SelectObject(bufdc, hExplosionSpudow);
							GetObject(hExplosionSpudow, sizeof(BITMAP), &start);
							MyTransparentBlt2(mdc, 48 + i * 80, 98 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
						}
						else {
							Pla[j][i].ali = false;
						}
						
					}
				}
				else if (Pla[j][i].vri == 10) {//阳光菇
					if (GetTickCount() - Pla[j][i].grow <= 20000) {//产出小阳光阶段
						SelectObject(bufdc, hSunshroom[Pla[j][i].num]);
						GetObject(hSunshroom[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 55 + i * 80, 108 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
						if (GetTickCount() - Pla[j][i].tshoot > 2000) {//24秒产出一个阳光
							Sun tempsun;
							tempsun.num = 0;
							tempsun.stime = -1;//没有落地
							tempsun.x = rand() % 60 + 30 + i * 80;
							tempsun.y = 90 + j * 100;
							tempsun.spy = 4;
							tempsun.maxnum = 29;
							tempsun.size = 2;
							switch (j) {
							case 0:
								tempsun.maxy = 120;
								break;
							case 1:
								tempsun.maxy = 220;
								break;
							case 2:
								tempsun.maxy = 320;
								break;
							case 3:
								tempsun.maxy = 420;
								break;
							case 4:
								tempsun.maxy = 520;
								break;
							}
							Sun_v.push_back(tempsun);
							Pla[j][i].tshoot = GetTickCount();
						}
					}
					else {
						if (GetTickCount() - Pla[j][i].grow <= 30100) {
							mciSendString("play .\\image\\music\\plantgrow.wav", NULL, 0, NULL);
						}

						SelectObject(bufdc, hSunshroomGrow[Pla[j][i].num]);
						GetObject(hSunshroomGrow[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 48 + i * 80, 93 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
						if (GetTickCount() - Pla[j][i].tshoot > 2000) {//24秒产出一个阳光
							Sun tempsun;
							tempsun.num = 0;
							tempsun.stime = -1;//没有落地
							tempsun.x = rand() % 60 + 30 + i * 80;
							tempsun.y = 60 + j * 100;
							tempsun.spy = 4;
							tempsun.maxnum = 29;
							tempsun.size = 1;
							switch (j) {
							case 0:
								tempsun.maxy = 110;
								break;
							case 1:
								tempsun.maxy = 210;
								break;
							case 2:
								tempsun.maxy = 310;
								break;
							case 3:
								tempsun.maxy = 410;
								break;
							case 4:
								tempsun.maxy = 510;
								break;
							}
							Sun_v.push_back(tempsun);
							Pla[j][i].tshoot = GetTickCount();
						}
					}
				}
				else if (Pla[j][i].vri == 11) {//大喷菇
					SelectObject(bufdc, hFumeshroom[Pla[j][i].num]);
					GetObject(hFumeshroom[Pla[j][i].num], sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, 35 + i * 80, 65 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
					if (!Zombie_v[j].empty()) {//当前行有僵尸
						if (GetTickCount() - Pla[j][i].tshoot >= 2000) {//2秒攻击一次
							mciSendString("play .\\image\\music\\fume.wav", NULL, 0, NULL);
							SelectObject(bufdc, hFumeShroombullet[(fume+1)/2]);
							GetObject(hFumeShroombullet[1], sizeof(BITMAP), &start);
							MyTransparentBlt2(mdc, 125 + i * 80, 70 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
							fume ++;
							if (fume == 16)
							{
								vector<Zomb>::iterator Zombie_iter5;
								for (Zombie_iter5 = Zombie_v[j].begin(); Zombie_iter5 != Zombie_v[j].end(); Zombie_iter5++) {
									if (Zombie_iter5->x >= 45 + i * 80 - 80 && Zombie_iter5->x <= 45 + (i + 5) * 80 - 80)
										Zombie_iter5->blood -= 5;
								}
								Pla[j][i].tshoot = GetTickCount();
								fume = 1;
							}
						}
					}
				}
				else if (Pla[j][i].vri == 12) {//胆小菇
				if (Pla[j][i].maxnum == 18) {
					SelectObject(bufdc, hScaredyshroom[Pla[j][i].num]);
					GetObject(hScaredyshroom[Pla[j][i].num], sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, 45 + i * 80, 83 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
					if (!Zombie_v[j].empty()) {
						vector<Zomb>::iterator Zombie_iter6;
						for (Zombie_iter6 = Zombie_v[j].begin(); Zombie_iter6 != Zombie_v[j].end(); ) {
							if (Zombie_iter6->x >= 45 + (i + 1) * 80 - 80 && Zombie_iter6->x <= 45 + (i + 2) * 80 - 80) {
								Pla[j][i].maxnum = 12;
								Pla[j][i].num = 0;
								break;
							}
							else {
								Zombie_iter6++;
							}
						}
					}
					if (!Zombie_v[j].empty()) {//当前行有僵尸
						if (GetTickCount() - Pla[j][i].tshoot >= 2500) {//2.5秒发射一个孢子
							mciSendString("play .\\image\\music\\puff.mp3", NULL, 0, NULL);
							Peb temppball;
							temppball.x = 80 + i * 80;
							temppball.y = 93 + j * 100;
							temppball.speed = 10;
							temppball.j = j;
							temppball.vri = 3;
							temppball.num = 0;
							temppball.maxnum = 6;
							Peb_v.push_back(temppball);
							Pla[j][i].tshoot = GetTickCount();
						}
					}
				}
					
					if (Pla[j][i].maxnum == 12) {
						SelectObject(bufdc, hScaredyshrooms[Pla[j][i].num]);
						GetObject(hScaredyshrooms[Pla[j][i].num], sizeof(BITMAP), &start);
						MyTransparentBlt2(mdc, 48 + i * 80, 113 + j * 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
					}
				}
				Pla[j][i].num++;
			}
		}
	}
	/*格子中的铲子*/
	if (scooped == -1) {
		SelectObject(bufdc, hscp);
		GetObject(hscp, sizeof(BITMAP), &start);
		MyTransparentBlt2(mdc, 550, 0, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	}

	/*---------------------------------------攻击--------------------------------------------*/
	vector<Peb>::iterator pball_iter;
	vector<Zomb>::iterator Zombie_iter2;
	for (pball_iter = Peb_v.begin(); pball_iter != Peb_v.end();) {
		j = pball_iter->j;
		int pball_shooted = 0;
		for (Zombie_iter2 = Zombie_v[j].begin(); Zombie_iter2 != Zombie_v[j].end(); Zombie_iter2++) {
			if (pball_iter->x >= Zombie_iter2->x + 55 && pball_iter->x <= Zombie_iter2->x + 80 && (Zombie_iter2->condition != 5 && Zombie_iter2->condition != 6)) {//射中僵尸
				if (pball_iter->vri == 1) {//普通豌豆击中
					mciSendString("play .\\image\\music\\plastichit.wav", NULL, 0, NULL);
				}
				if (pball_iter->vri == 2) {//蓝豆子减速
					mciSendString("play .\\image\\music\\frozen.wav", NULL, 0, NULL);
					Zombie_iter2->speed = 0.1;
					Zombie_iter2->recover = GetTickCount();
				}
				if (pball_iter->vri == 3) {//孢子击中
					mciSendString("play .\\image\\music\\puffhit.wav", NULL, 0, NULL);
				}
				Zombie_iter2->blood -= 5;//僵尸扣血
				if (pball_iter->vri != 3) {
					SelectObject(bufdc, hpehit);
					GetObject(hpehit, sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, pball_iter->x, pball_iter->y, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
				}
				else {
					SelectObject(bufdc, hShroomBulletHit);
					GetObject(hShroomBulletHit, sizeof(BITMAP), &start);
					MyTransparentBlt2(mdc, pball_iter->x, pball_iter->y, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
				}
				
				pball_iter = Peb_v.erase(pball_iter);
				pball_shooted = 1;

				break;
			}
			else {
				continue;
			}
		}
		if (pball_shooted == 0) {//飞行过程
			if (pball_iter->vri == 1) {//豌豆
				SelectObject(bufdc, hpeb);
				GetObject(hpeb, sizeof(BITMAP), &start);
				MyTransparentBlt2(mdc, pball_iter->x, pball_iter->y, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
			}
			else if (pball_iter->vri == 2) {//冰豌豆
				SelectObject(bufdc, hSnowPeashooterBullet);
				GetObject(hSnowPeashooterBullet, sizeof(BITMAP), &start);
				MyTransparentBlt2(mdc, pball_iter->x, pball_iter->y, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 255, 0));
			}
			else if (pball_iter->vri == 3) {//孢子
				SelectObject(bufdc, hPuffshoot[pball_iter->num]);
				GetObject(hPuffshoot[0], sizeof(BITMAP), &start);
				MyTransparentBlt2(mdc, pball_iter->x, pball_iter->y, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
				pball_iter->num++;
				if (pball_iter->num == pball_iter->maxnum)
					pball_iter->num = 0;
			}

			pball_iter->x += pball_iter->speed;
			pball_iter++;
		}
	}
	/*-------------------------------------正在掉落的太阳-------------------------------------*/
	vector<Sun>::iterator Sun_iter;
	for (Sun_iter = Sun_v.begin(); Sun_iter != Sun_v.end();) {
		if (Sun_iter->num == Sun_iter->maxnum)
			Sun_iter->num = 0;
		if (Sun_iter->size == 1)
			AlphaBmp(mdc, hSun[Sun_iter->num], Sun_iter->x, Sun_iter->y, 230, hrgn[Sun_iter->num], RGB(0, 0, 0));
		else
			AlphaBmp(mdc, hSunsmall[Sun_iter->num], Sun_iter->x, Sun_iter->y, 230, hrgnsmall[Sun_iter->num], RGB(0, 0, 0));

		//落地超过6秒阳光消失消失
		if (Sun_iter->y >= Sun_iter->maxy) {//落地
			Sun_iter->y = Sun_iter->maxy;
			if (Sun_iter->stime == -1) {//第一次落地
				Sun_iter->stime = GetTickCount();//太阳落地
			}
		}
		else {
			Sun_iter->y += Sun_iter->spy;
		}
		Sun_iter->num++;
		if (GetTickCount() - Sun_iter->stime >= 6000 && Sun_iter->stime != -1) {//落地超过6秒&&落地
			Sun_iter = Sun_v.erase(Sun_iter);
		}
		else {
			Sun_iter++;
		}
	}
	/*-------------------------------------------正在收集的太阳----------------------------------------------*/
	for (Sun_iter = Sun_coll.begin(); Sun_iter != Sun_coll.end();) {
		if (Sun_iter->x <= 20 || Sun_iter->y <= 0) {
			Sun_iter->x = 20;
			Sun_iter->y = 0;
		}
		if (Sun_iter->num == Sun_iter->maxnum)
			Sun_iter->num = 0;
		if (Sun_iter->size == 1)
			AlphaBmp(mdc, hSun[Sun_iter->num], Sun_iter->x, Sun_iter->y, 230, hrgn[Sun_iter->num], RGB(0, 0, 0));
		else
			AlphaBmp(mdc, hSunsmall[Sun_iter->num], Sun_iter->x, Sun_iter->y, 230, hrgnsmall[Sun_iter->num], RGB(0, 0, 0));
		if ((Sun_iter->x <= 20 || Sun_iter->y <= 0) && Sun_iter->size == 1) {
			money += 25;
			Sun_iter = Sun_coll.erase(Sun_iter);
		}
		else if ((Sun_iter->x <= 20 || Sun_iter->y <= 0) && Sun_iter->size == 2) {
			money += 15;
			Sun_iter = Sun_coll.erase(Sun_iter);
		}
		else {
			Sun_iter->x = Sun_iter->x - Sun_iter->spx;
			Sun_iter->y = Sun_iter->y - Sun_iter->spy;
			Sun_iter->num++;
			Sun_iter++;
		}
	}
	/*---------------------------------------跟随鼠标动的植物--------------------------------------------*/
	if (planted != -1) {
		if (planted == 1) {//豌豆射手
			Drawplantready(1, hPeashooter[0], 62, 83);
			SelectObject(bufdc, hPeashooter[0]);
			GetObject(hPeashooter[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 2) {//向日葵
			Drawplantready(2, hSunF[0], 60, 80);
			SelectObject(bufdc, hSunF[0]);
			GetObject(hSunF[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 3) {//坚果墙
			Drawplantready(3, hWallNut[0], 60, 83);
			SelectObject(bufdc, hWallNut[0]);
			GetObject(hWallNut[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 4) {//寒冰射手
			Drawplantready(4, hSnowPea[0], 60, 83);
			SelectObject(bufdc, hSnowPea[0]);
			GetObject(hSnowPea[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 5) {//双发射手
			Drawplantready(5, hRepeater[0], 60, 83);
			SelectObject(bufdc, hRepeater[0]);
			GetObject(hRepeater[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 6) {//食人花
			Drawplantready(6, hChomper[0], 60, 40);
			SelectObject(bufdc, hChomper[0]);
			GetObject(hChomper[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 90, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 7) {//辣椒
			Drawplantready(7, hJalapeno[0], 60, 75);
			SelectObject(bufdc, hJalapeno[0]);
			GetObject(hJalapeno[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 8) {//倭瓜
			Drawplantready(8, hSquash[0], 45, -70);
			SelectObject(bufdc, hSquash[0]);
			GetObject(hSquash[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 40, hcposy - 200, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (planted == 9) {//土豆雷
			Drawplantready(9, hPotatomine[0], 60, 108);
			SelectObject(bufdc, hPotatomine[0]);
			GetObject(hPotatomine[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
		}
		else if (planted == 10) {//阳光菇
			Drawplantready(10, hSunshroom[0], 70, 108);
			SelectObject(bufdc, hSunshroom[0]);
			GetObject(hSunshroom[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 25, hcposy - 40, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
		}
		else if (planted == 11) {//大喷菇
			Drawplantready(11, hFumeshroom[0], 45, 73);
			SelectObject(bufdc, hFumeshroom[0]);
			GetObject(hFumeshroom[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 40, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
		}
		else if (planted == 12) {//胆小菇
			Drawplantready(12, hScaredyshroom[0], 63, 83);
			SelectObject(bufdc, hScaredyshroom[0]);
			GetObject(hScaredyshroom[0], sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
		}
	}

	/*---------------------------------------跟着鼠标移动的铲子-------------------------------------------------*/
	if (scooped == 1) {
		SelectObject(bufdc, hscp);
		GetObject(hscp, sizeof(BITMAP), &start);
		MyTransparentBlt2(mdc, hcposx - 30, hcposy - 60, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	}

	/*--------------------------------------------进度条----------------------------------------------------*/
	if (GetTickCount() - t3 <= 142000 && GetTickCount() - t3 >= 140000) {
		paintlargewave(hdc);
	}
	else if (GetTickCount() - t3 <= 232000 && GetTickCount() - t3 >= 230000) {
		SelectObject(bufdc, hFinalWave);
		GetObject(hFinalWave, sizeof(BITMAP), &start);
		MyTransparentBlt2(mdc, 300, 200, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	}
	else if (GetTickCount() - t3 >= 250000) {
		for (j = 0; j < 5; j++) {
			if (!Zombie_v[j].empty()) {
				break;
			}
		}
		if (j == 5) {
			Sun_v.clear();
			Sun_coll.clear();
			mciSendString("stop .\\image\\music\\bkmusic.wav", NULL, 0, NULL);
			mciSendString("play .\\image\\music\\winmusic.wav", NULL, 0, NULL);
			t3 = GetTickCount();
			WIN = true;
			END = true;
		}
	}

	if (END == false) {
		for (j = 0; j < 5; j++) {
			if (!Zombie_v[j].empty() && Zombie_v[j].begin()->x <= -50) {
				Sun_v.clear();
				Sun_coll.clear();
				if (!DIE) {
					DIE = GetTickCount();
				}
				mciSendString("stop .\\image\\music\\bkmusic.wav", NULL, 0, NULL);
				mciSendString("play .\\image\\music\\losemusic.wav", NULL, 0, NULL);
				if (GetTickCount() - DIE >= 5000)
				{
					mciSendString("play .\\image\\music\\chomp.wav", NULL, 0, NULL);
				}
				if (GetTickCount() - DIE >= 5500) {
					mciSendString("play .\\image\\music\\chomp2.wav", NULL, 0, NULL);
				}
				if (GetTickCount() - DIE >= 6000) {
					mciSendString("play .\\image\\music\\scream.wav", NULL, 0, NULL);
				}
				t3 = GetTickCount();
				END = true;
				WIN = false;
			}
		}
	}
	else {
		if (WIN == false) {
			SelectObject(bufdc, hZombiesWon);
			GetObject(hZombiesWon, sizeof(BITMAP), &start);
			MyTransparentBlt2(mdc, 100, 100, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(0, 0, 0));
		}
		else if (WIN == true) {

		}

		if (GetTickCount() - t3 >= 6000) {//还原
			STRAT = false;
			t1 = t2 = t3 = -1;
			Peb_v.clear();
			Plantvri_v.clear();
			Num = 0;
			for (i = 0; i < 9; i++) {
				kexuan[i] = 1;
			}
			markposx = markposy = releasex = releasey = movex = movey = -1;
			for (i = 0; i < 5; i++) {
				for (j = 0; j < 9; j++) {
					Pla[i][j].ali = false;
				}
			}
		}
		for (i = 0; i < 5; i++)
			Zombie_v[i].clear();
		Sun_v.clear();
		money = 50;
	}


	/*-----------------------------------------进度条-------------------------------------------------*/
	SelectObject(bufdc, hFlagMeterFull);//满
	GetObject(hFlagMeterFull, sizeof(BITMAP), &start);
	MyTransparentBlt2(mdc, 620, 570, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	SelectObject(bufdc, hFlagMeterEmpty);//空
	GetObject(hFlagMeterEmpty, sizeof(BITMAP), &start);
	MyTransparentBlt2(mdc, 620, 570, start.bmWidth * (1 - ((GetTickCount() - t3) / 270000.0)), start.bmHeight, bufdc, 0, 0, start.bmWidth * (1 - ((GetTickCount() - t3) / 270000.0)), start.bmHeight, RGB(255, 255, 255));
	SelectObject(bufdc, hFlagMeterParts2);//旗子
	GetObject(hFlagMeterParts2, sizeof(BITMAP), &start);
	MyTransparentBlt2(mdc, 626, 560, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	MyTransparentBlt2(mdc, 690, 560, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	SelectObject(bufdc, hFlagMeterLevelProgress);//“关卡进程”
	GetObject(hFlagMeterLevelProgress, sizeof(BITMAP), &start);
	MyTransparentBlt2(mdc, 655, 582, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));
	SelectObject(bufdc, hFlagMeterParts1);//僵尸头
	GetObject(hFlagMeterParts1, sizeof(BITMAP), &start);
	double x1 = 626 + 135.0 * (1 - ((GetTickCount() - t3) / 250000.0));
	if ((1 - ((GetTickCount() - t3) / 250000.0)) < 0) {
		x1 = 626;
	}
	MyTransparentBlt2(mdc, x1, 560, start.bmWidth, start.bmHeight, bufdc, 0, 0, start.bmWidth, start.bmHeight, RGB(255, 255, 255));

	//画到屏幕上
	BitBlt(hdc, 0, 0, wwid, whei, mdc, 0, 0, SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间，判断刷新
}

HFONT myCreateFont(HDC hDC, int nCharHeight, BOOL bItalic)
{
	HFONT hFont;
	hFont = CreateFont(				//定义字体句柄.
		nCharHeight,				//字体高度.
		0,							//由系统根据高宽比选取字体最佳宽度值.
		0,							//文本倾斜度为0,表示水平.
		0,							//字体倾斜度为0.
		400,						//字体粗度.400为正常.
		bItalic,					//是斜体字?,
		0,							//无下划线.
		0,							//无删除线.
		ANSI_CHARSET,				//表示所用的字符集为ANSI_CHARSET.
		OUT_DEFAULT_PRECIS,			//删除精度为缺省值.
		CLIP_DEFAULT_PRECIS,		//裁剪精度为缺省值.
		DEFAULT_QUALITY,			//输出质量为缺省值.
		DEFAULT_PITCH | FF_DONTCARE,	//字间距和字体系列使用缺省值.
		"黑体");					//字体名称.
	if (hFont == NULL) return NULL;
	else return hFont;
}		//消息处理函数声明.

void Drawplantready(int var, HBITMAP hx, int x, int y) {
	int i, j;
	for (i = 0; i < 10; i++) {
		if (hcposx > 60 + i * 80 && hcposx < 60 + (i + 1) * 80) {
			for (j = 0; j < 5; j++) {
				if (hcposy > 80 + j * 100 && hcposy < 80 + (j + 1) * 100)
				{
					if (Pla[j][i].ali == false) {
						AlphaBmp(mdc, hx, x + i * 80 - 15, y + j * 100, 100, hrgn_plant[var - 1], RGB(0, 0, 0));
					}
				}
			}
		}
	}
}

void AlphaBmp(HDC hdc, HBITMAP hBitmap, int x, int y, short limit, HRGN hRgn, COLORREF col)
{
	BLENDFUNCTION blendfunc;
	HDC hMaskDc;
	HBITMAP hOld = NULL;
	BITMAP Bmp;
	blendfunc.BlendFlags = 0;
	blendfunc.AlphaFormat = 0;
	blendfunc.SourceConstantAlpha = (unsigned char)limit;
	blendfunc.BlendOp = AC_SRC_OVER;
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bmp);

	hMaskDc = CreateCompatibleDC(hdc);
	hOld = (HBITMAP)SelectObject(hMaskDc, hBitmap);


	HDC hTempDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap2 = CreateCompatibleBitmap(hdc, Bmp.bmWidth, Bmp.bmHeight);
	hOld = (HBITMAP)SelectObject(hTempDC, hBitmap2);
	BitBlt(hTempDC, 0, 0, Bmp.bmWidth, Bmp.bmHeight, hdc, x, y, SRCCOPY);
	AlphaBlend(hTempDC, 0, 0, Bmp.bmWidth, Bmp.bmHeight, hMaskDc, 0, 0, Bmp.bmWidth, Bmp.bmHeight, blendfunc);
	HRGN hR = CreateRectRgn(0, 0, Bmp.bmWidth, Bmp.bmHeight);
	HBRUSH hbr = CreateSolidBrush(col);
	CombineRgn(hR, hR, hRgn, RGN_DIFF);
	FillRgn(hTempDC, hR, hbr);

	MyTransparentBlt2(hdc, x, y, Bmp.bmWidth, Bmp.bmHeight, hTempDC, 0, 0, Bmp.bmWidth, Bmp.bmHeight, col);

	SelectObject(hTempDC, hOld);
	DeleteObject(hBitmap2);
	DeleteObject(hR);
	DeleteObject(hbr);
	DeleteDC(hTempDC);

	SelectObject(hMaskDc, hOld);
	DeleteDC(hMaskDc);
}

HRGN BitmapToRgn(HBITMAP hBitmap, COLORREF col)
{
	HDC hMemDc;
	HBITMAP hOld;
	COLORREF CPixel;
	HRGN rTemp, hGoal;
	BITMAP Bmp;
	GetObject(hBitmap, sizeof(BITMAP), &Bmp);
	hMemDc = CreateCompatibleDC(NULL);
	hOld = (HBITMAP)SelectObject(hMemDc, hBitmap);
	hGoal = CreateRectRgn(0, 0, Bmp.bmWidth, Bmp.bmHeight);
	for (int x = 0; x <= Bmp.bmWidth; x++)
		for (int y = 0; y <= Bmp.bmHeight; y++)
		{
			CPixel = GetPixel(hMemDc, x, y);
			if (CPixel == col)
			{
				rTemp = CreateRectRgn(x, y, x + 1, y + 1);
				CombineRgn(hGoal, hGoal, rTemp, RGN_XOR);
				DeleteObject(rTemp);
			}
		}
	SelectObject(hMemDc, hOld);
	DeleteDC(hMemDc);
	return hGoal;
}
