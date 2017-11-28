#include <windows.h>
#include <queue>
#include <vector>
#include <time.h>
#include "resource.h"
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "msimg32.lib")

using namespace std;

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM ) ;        //声明用来处理消息的函数
void AlphaBmp(HDC hdc,HBITMAP hBitmap,int x,int y,short limit,HRGN hRgn,COLORREF col);
HRGN BitmapToRgn(HBITMAP hBitmap,COLORREF col);
void MyPaint(HDC hdc);//游戏进程
void paintSurface(HDC hdc);//
//void PaintSun(HDC hdc);//
void paintready(HDC hdc);//准备好开始
void paintlargewave(HDC hdc);//最后一波
void checkPlant(HDC hdc);//选择植物界面
void drawplantready(int var,HBITMAP hx,int x,int y);
HBITMAP hZombie[59],hbackground,hSunFlawor[36],hChoose,hscoopbox,hscoop,hpball,hPeaBulletHit,hshadow,hSnowPeashooterBullet,
hSun[29],hPeashooter[27],hRepeater[30],hSnowPea[30],hWallNut[32],hWallnut_cracked1[22],hWallnut_cracked2[30],
hChomper[42],hJalapeno[16],hJalapenoAttack[25],hSquash[34],hZombieAttack[43],hZombieLostHead[60],hZombieHead[9],
hZombieLostHeadAttack[22],hZombieDie[40],hBoomDie[50],hZombieblue[59],hZombieAttackblue[43],hChomperAttack[19],hChomperDigest[19],
hSquashAttack[33],hCD,hSurface,hSelector,hPrepareGrowPlants,hPrepareGrowPlantsok,hPrepareGrowPlantsgo,hLargeWave,hFinalWave,
hFlagMeterEmpty,hFlagMeterFull,hFlagMeterParts2,hFlagMeterParts1,hZombiesWon,hFlagMeterLevelProgress,hSquashR,
hplantchoosebox,hplantchoose,hplantnum[9],hfight1,hfight2;
BITMAP bmzo,bmbg,bmsf;
HBITMAP bm;
HDC mdc,hdc,bufdc;
HWND hwnd ;
HRGN hrgn[29],hrgn_shadow,hrgn_plant[10],hrgn_cd;
double tPre,tNow,tStart,tSun,t1 = -1 ,t2 = -1,t3 = -1,t3pre = -1,t3now,tgamestart,tSurface;
int winwidth = 840,winheight = 630;
int money = 50;
int planted = -1;//没有选择要种的植物
int scooped = -1;//-1没有选择铲子，1选择了铲子，2点击了植物
int hcposx,hcposy,scoopposx,scoopposy,lbuttondown = 0,lbuttonup = 0;
int markposx,markposy,releasex,releasey,movex,movey;
int varvalue[8],CHOOSE;
bool STRAT = false,END = true,WIN = false;
struct Plant{
	int num;
	int maxnum;
	double blood;
	bool ex;//是否存在
	int var;//品种
	double tshoot;
	double recover;
}pp[5][9];//种了的花的属性

struct Pball{
	double x,y;
	double speed;
	int var;//1为正常豌豆，2为寒冰豌豆
	double j;//所在行
}pball;

struct Head{
	int num;
	int maxnum;
}head;

struct Zombie{
	int num;
	int maxnum;
	int condition;
	double recover;
	double disapper;
	double blood;
	double speed;
	double x,y;//当前坐标
	double i,j;//正在攻击的植物的坐标
	Head head;
}Zombie1;


struct Sun{
	int num;
	int x,y;
	int speadx;
	int speady;
	int maxnum;
	double starttime;//落地时间
	int maxy;//落地的y坐标
}natrueSun;

struct CD{
	int ex;
	double starttime;
	double lasting;
	double maxlasting;
}cd[8],moneyless[8];

struct Plantvar{
	int ex;
	int var;
};

vector<Zombie>Zombie_1;
vector<Zombie>Zombie_vct[5];
vector<Zombie>ZombieDying_vct;
vector<Sun>Sun_vct;
vector<Sun>Sun_collect;
vector<Pball>pball_vct;
vector<Plantvar>plantvar_vct;

bool cmp(Zombie &z1,Zombie &z2){
	return z1.x<z2.x;//升序排序，每列僵尸
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow )
{
    static TCHAR szAppName[] = TEXT("MyWindow") ;
    
    MSG msg ;
    WNDCLASS wndclass ;        //声明一个窗口类对象

    //以下为窗口类对象wndclass的属性
    wndclass.style = CS_HREDRAW | CS_VREDRAW ;                         //窗口样式
    wndclass.lpszClassName = szAppName ;                               //窗口类名
    wndclass.lpszMenuName = NULL ;                                     //窗口菜单:无
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH) ;    //窗口背景颜色
    wndclass.lpfnWndProc = WndProc ;                                   //窗口处理函数
    wndclass.cbWndExtra = 0 ;                                          //窗口实例扩展:无
    wndclass.cbClsExtra = 0 ;                                          //窗口类扩展:无
    wndclass.hInstance = hInstance ;                                   //窗口实例句柄
    wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION ) ;               //窗口最小化图标:使用缺省图标
    wndclass.hCursor = LoadCursor( NULL, IDC_ARROW ) ;                 //窗口采用箭头光标

    if( !RegisterClass( &wndclass ) )
    {    //注册窗口类, 如果注册失败弹出错误提示
        MessageBox( NULL, TEXT("窗口注册失败!"), TEXT("错误"), MB_OK | MB_ICONERROR ) ;
        return 0 ;
    }

    hwnd = CreateWindow(                   //创建窗口
                szAppName,                 //窗口类名
                TEXT("我的窗口"),           //窗口标题
                WS_OVERLAPPEDWINDOW,       //窗口的风格
                CW_USEDEFAULT,             //窗口初始显示位置x:使用缺省值
                CW_USEDEFAULT,             //窗口初始显示位置y:使用缺省值
                winwidth,             //窗口的宽度:使用缺省值
                winheight,             //窗口的高度:使用缺省值
                NULL,                      //父窗口:无
                NULL,                      //子菜单:无
                hInstance,                 //该窗口应用程序的实例句柄 
                NULL                       //
            ) ;

    ShowWindow( hwnd, iCmdShow ) ;        //显示窗口
    UpdateWindow( hwnd ) ;                //更新窗口

	/*----------------------初始化----------------------------------------*/
	hdc = GetDC(hwnd);
	mdc = CreateCompatibleDC(hdc);//小dc，for大图
	bufdc = CreateCompatibleDC(hdc);//小dc,for小图
	bm = CreateCompatibleBitmap(hdc,winwidth,winheight);//大图 

	SelectObject(mdc,bm);
	int i;
	char name[50]="";
	hbackground = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_MAP1));
	hChoose = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_CHOOSE));
	hscoopbox = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_SCOOPBOX));
	hscoop = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_SCOOP));
	hpball = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_PBALL));
	hPeaBulletHit = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_PeaBulletHit));
	hSnowPeashooterBullet = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_SnowPeashooterBullet));
	hSurface =LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_Surface));
	hSelector = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_Selector));
	hPrepareGrowPlants = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_PrepareGrowPlants));
	hPrepareGrowPlantsok = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_PrepareGrowPlantsok));
	hPrepareGrowPlantsgo = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_PrepareGrowPlantsgo));
	hLargeWave = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_LargeWave));
	hFinalWave = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_FinalWave));
	hFlagMeterEmpty = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_FlagMeterEmpty));
	hFlagMeterFull = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_FlagMeterFull));
	hFlagMeterParts2 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_FlagMeterParts2));
	hFlagMeterParts1 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_FlagMeterParts1));
	hZombiesWon = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ZombiesWon));
	hFlagMeterLevelProgress = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_FlagMeterLevelProgress));
	hSquashR = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_SquashR));
	hplantchoosebox = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_plantchoosebox));
	hplantchoose = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_plantchoose1));
	hfight1 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_fight1));
	hfight2 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_fight2));

	hshadow = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_SHADOW));
	hrgn_shadow = BitmapToRgn(hshadow,RGB(255,255,255));

	hCD =  LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_CD));
	hrgn_cd = BitmapToRgn(hCD,RGB(255,255,255));

	for(i=0;i<29;i++)//阳光
	{
		sprintf(name,"re/Sun/Sun%d.bmp\0",i);
		hSun[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,78,78,LR_LOADFROMFILE);
		hrgn[i]=BitmapToRgn(hSun[i],RGB(0,0,0));
	}

	for(i=0;i<36;i++)//太阳花
	{
		sprintf(name,"re/SunFlower/SunFlower%d.bmp\0",i);
		hSunFlawor[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,73,74,LR_LOADFROMFILE);
	}
	hrgn_plant[0] = BitmapToRgn(hSunFlawor[0],RGB(0,0,0));

	for(i=0;i<26;i++)//豌豆射手
	{
		sprintf(name,"re/Peashooter/Peashooter%d.bmp\0",i);
		hPeashooter[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,71,71,LR_LOADFROMFILE);
	}
	hrgn_plant[1] = BitmapToRgn(hPeashooter[0],RGB(0,0,0));
	
	
	for(i=0;i<30;i++)//豌豆射手2
	{
		sprintf(name,"re/Repeater/Repeater%d.bmp\0",i);
		hRepeater[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,73,71,LR_LOADFROMFILE);
	}
	hrgn_plant[2] = BitmapToRgn(hRepeater[0],RGB(0,0,0));
	
	for(i=0;i<30;i++)//寒冰射手
	{
		sprintf(name,"re/SnowPea/SnowPea%d.bmp\0",i);
		hSnowPea[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,71,71,LR_LOADFROMFILE);
	}
	hrgn_plant[3] = BitmapToRgn(hSnowPea[0],RGB(0,0,0));
	
	for(i=0;i<32;i++)//土豆1
	{
		sprintf(name,"re/WallNut/WallNut%d.bmp\0",i);
		hWallNut[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,65,71,LR_LOADFROMFILE);
	}
	hrgn_plant[4] = BitmapToRgn(hWallNut[0],RGB(0,0,0));
	for(i=0;i<22;i++)//土豆2
	{
		sprintf(name,"re/Wallnut_cracked1/Wallnut_cracked1%d.bmp\0",i);
		hWallnut_cracked1[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,65,71,LR_LOADFROMFILE);
	}
	for(i=0;i<30;i++)//土豆3
	{
		sprintf(name,"re/Wallnut_cracked2/Wallnut_cracked2%d.bmp\0",i);
		hWallnut_cracked2[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,65,71,LR_LOADFROMFILE);
	}
	
	for(i=0;i<42;i++)//食人花
	{
		sprintf(name,"re/Chomper/Chomper%d.bmp\0",i);
		hChomper[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,130,114,LR_LOADFROMFILE);
	}
	hrgn_plant[5] = BitmapToRgn(hChomper[0],RGB(0,0,0));
	for(i=0;i<16;i++)//辣椒
	{
		sprintf(name,"re/Jalapeno/Jalapeno%d.bmp\0",i);
		hJalapeno[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,68,89,LR_LOADFROMFILE);
	}
	hrgn_plant[6] = BitmapToRgn(hJalapeno[0],RGB(0,0,0));
	for(i=0;i<25;i++)//辣椒攻击
	{
		sprintf(name,"re/JalapenoAttack/JalapenoAttack%d.bmp\0",i);
		hJalapenoAttack[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,755,131,LR_LOADFROMFILE);
	}
	for(i=0;i<34;i++)//倭瓜
	{
		sprintf(name,"re/Squash/Squash%d.bmp\0",i);
		hSquash[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,100,226,LR_LOADFROMFILE);
	}
	hrgn_plant[7] = BitmapToRgn(hSquash[0],RGB(0,0,0));
	for( i = 0 ; i < 43 ; i++ ){//攻击僵尸
		sprintf(name,"re/ZombieAttack/ZombieAttack%d.bmp\0",i);
		hZombieAttack[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 60 ; i++ ){//无头行走僵尸
		sprintf(name,"re/ZombieLostHead/ZombieLostHead%d.bmp\0",i);
		hZombieLostHead[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 9 ; i++ ){//僵尸头
		sprintf(name,"re/ZombieHead/ZombieHead%d.bmp\0",i);
		hZombieHead[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,150,186,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 22 ; i++ ){//攻击僵尸失去头
		sprintf(name,"re/ZombieLostHeadAttack/ZombieLostHeadAttack%d.bmp\0",i);
		hZombieLostHeadAttack[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 40 ; i++ ){//僵尸垂死
		sprintf(name,"re/ZombieDie/ZombieDie%d.bmp\0",i);
		hZombieDie[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 50 ; i++ ){//boomdie
		sprintf(name,"re/BoomDie/BoomDie%d.bmp\0",i);
		hBoomDie[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 59 ; i++ ){//行走僵尸
		sprintf(name,"re/Zombie/Zombie%d.bmp\0",i);
		hZombie[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 59 ; i++ ){//行走僵尸（蓝
		sprintf(name,"re/Zombieblue/Zombieblue%d.bmp\0",i);
		hZombieblue[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 43 ; i++ ){//攻击僵尸（蓝
		sprintf(name,"re/ZombieAttackblue/ZombieAttackblue%d.bmp\0",i);
		hZombieAttackblue[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,166,144,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 19 ; i++ ){//攻击食人花
		sprintf(name,"re/ChomperAttack/ChomperAttack%d.bmp\0",i);
		hChomperAttack[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,130,114,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 19 ; i++ ){//进食食人花
		sprintf(name,"re/ChomperDigest/ChomperDigest%d.bmp\0",i);
		hChomperDigest[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,130,114,LR_LOADFROMFILE);
	}
	for( i = 0 ; i < 33 ; i++ ){//攻击倭瓜
		sprintf(name,"re/SquashAttack/SquashAttack%d.bmp\0",i);
		hSquashAttack[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,100,226,LR_LOADFROMFILE);
	}
	
	for( i=1 ; i <= 8 ; i++ ){//植物选项卡
		sprintf(name,"re/plant (%d).bmp\0",i);
		hplantnum[i] = (HBITMAP)LoadImage(NULL,name,IMAGE_BITMAP,50,71,LR_LOADFROMFILE);
	}

	/*---------------------初始化-------------------------*/
	
	Zombie newzom;
	newzom.num=0;
	newzom.speed=0.5;
	newzom.condition=1;
	newzom.maxnum=59;
	newzom.x = 750;
	newzom.blood = 100;
	newzom.head.num = 0;
	newzom.head.maxnum = 9;
	newzom.recover = -1; 
	newzom.disapper = -1;
	int apper = false;
	int line;
	int groan;
	CHOOSE = -1;//选择未开始
	srand(time(NULL));
	tStart = GetTickCount();//获取游戏开始时间
	tSun = GetTickCount();
	
	/*----------------------游戏循环----------------------------------------*/
	while(msg.message!=WM_QUIT){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			TranslateMessage( &msg ) ;                 //将虚拟键消息转换为字符消息
			DispatchMessage( &msg ) ; 
		}
		else
		{
			tNow = GetTickCount();//记录现在的时间
			if( tNow - tPre >= 50){
				if(STRAT==false || (GetTickCount() - t1 <= 3000 && t1 != -1)){//都不满足才下一步
					if(STRAT==false){
						mciSendString("stop .\\re\\music\\bkmusic.wav",NULL,0,NULL);
						mciSendString("play .\\re\\music\\ui_gkmusic.wav",NULL,0,NULL);
					}		
					paintSurface(hdc);//开始菜单
				}
				else{
					if(CHOOSE == 0){
						mciSendString("play .\\re\\music\\lookup.wav",NULL,0,NULL);
						checkPlant(hdc);//选择植物界面
					}
					else if( t2 == -1 || (t2 != -1 && GetTickCount() - t2 <= 1500)){//准备。好。开始
						if(t2 == -1) {
							mciSendString("stop .\\re\\music\\readysetpla.wav", NULL, 0, NULL);
							mciSendString("play .\\re\\music\\readysetpla.wav",NULL,0,NULL);
							t2 = GetTickCount();//设置t2
						}
						paintready(hdc);
					}
					else{
						t3now = GetTickCount();
						groan = rand()%4;
						sprintf(name,"play .\\re\\music\\groan%d.wav\0",groan);
						mciSendString("play .\\re\\music\\bkmusic.wav",NULL,0,NULL);
						if(t3 == -1){//记录游戏开始时间
							t3 = GetTickCount();
							t3pre = t3;
							mciSendString("stop .\\re\\music\\losemusic.wav",NULL,0,NULL);
							mciSendString("stop .\\re\\music\\evillaugh.wav",NULL,0,NULL);
						}
						else if(GetTickCount() - t3 <= 60000){//60秒内每15秒增加一只僵尸
							if(t3now - t3pre >=15000){
								if(GetTickCount() - t3 <= 15100){
									mciSendString("play .\\re\\music\\awooga.wav",NULL,0,NULL);
								}
								mciSendString(name,NULL,0,NULL);
								line = rand()%5;
								Zombie_vct[line].push_back(newzom);
								t3pre = t3now;
							}
						}
						else if(GetTickCount() - t3 <= 100000){//60-100秒内9秒增加一只僵尸
							if(t3now - t3pre >=9000){
								mciSendString(name,NULL,0,NULL);
								line = rand()%5;
								Zombie_vct[line].push_back(newzom);
								t3pre = t3now;
							}
						}
						else if(GetTickCount() - t3 <= 140000){//100-140秒内5秒增加一只僵尸
							if(t3now - t3pre >=5000){
								mciSendString(name,NULL,0,NULL);
								line = rand()%5;
								Zombie_vct[line].push_back(newzom);
								t3pre = t3now;
							}
						}
						else if(GetTickCount() - t3 <=170000  && GetTickCount() - t3 >= 162000){//一大波正在靠近
							if(t3now - t3pre >=1500){
								line = rand()%5;
								mciSendString(name,NULL,0,NULL);
								Zombie_vct[line].push_back(newzom);
								t3pre = t3now;
							}
						}
						else if(GetTickCount() - t3 <=190000  && GetTickCount() - t3 >= 180000){
							if(t3now - t3pre >=1500){
								line = rand()%5;
								mciSendString(name,NULL,0,NULL);
								Zombie_vct[line].push_back(newzom);
								t3pre = t3now;
							}
						}
						else if(GetTickCount() - t3 <=210000  && GetTickCount() - t3 >= 200000){
							if(t3now - t3pre >=1500){
								line = rand()%5;
								mciSendString(name,NULL,0,NULL);
								Zombie_vct[line].push_back(newzom);
								t3pre = t3now;
							}
						}
						else if(GetTickCount() - t3 <=240000  && GetTickCount() - t3 >= 232000){//最后一波
							if(GetTickCount() - t3 <=232100){
								mciSendString("play .\\re\\music\\hugewave.wav",NULL,0,NULL);
							}
							if(t3now - t3pre >=1000){
								line = rand()%5;
								mciSendString(name,NULL,0,NULL);
								Zombie_vct[line].push_back(newzom);
								t3pre = t3now;
							}
						}
						MyPaint(hdc);
					}
				}
			}
		}
	}
    return msg.wParam ;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int i,j;
    switch( message )        //处理得到的消息
    {
    case WM_CREATE:          //窗口创建完成时发来的消息
        return 0;
    case WM_LBUTTONDOWN:     //处理鼠标左键被按下的消息
		markposx = LOWORD(lParam);
		markposy = HIWORD(lParam);
		lbuttondown = 1;

        return 0;
	case WM_LBUTTONUP:     //处理鼠标左键被释放的消息
		releasex = LOWORD(lParam);
		releasey = HIWORD(lParam);
		lbuttonup = 1;

        return 0;
	case WM_MOUSEMOVE://鼠标移动时获取鼠标位置
		if(planted != -1||scooped==1){
			hcposx=(int)LOWORD(lParam);
			hcposy=(int)HIWORD(lParam);
		}
		movex = LOWORD(lParam);
		movey = HIWORD(lParam);
		return 0;
    case WM_DESTROY:         //处理窗口关闭时的消息
		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteObject(hbackground);
		DeleteObject(hZombie);
		ReleaseDC(hwnd,hdc);
        PostQuitMessage( 0 ) ;
        return 0;
    }
    return DefWindowProc( hwnd, message, wParam, lParam ) ;        //DefWindowProc处理我们自定义的消息处理函数没有处理到的消息
}

HFONT myCreateFont(HDC hDC,int nCharHeight,BOOL bItalic);
void paintSurface(HDC hdc){//开始游戏菜单
	HCURSOR hcursor;
	static int i = 1,j;
	/*菜单背景*/
	SelectObject(bufdc,hSurface);
	GetObject(hSurface,sizeof(BITMAP),&bmbg);
	BitBlt(mdc,-75,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	
	if(movex > 450 && movex <700 && movey >100 &&movey <200){
		if(i!=0){
			i = 0;
			mciSendString("play .\\re\\music\\ui_button.wav",NULL,0,NULL);
		}
		//hcursor = LoadCursor(NULL,IDC_HAND);//设置手型符号
		//SetCursor(hcursor);
	}
	else{
		i=1;
	}
	if(lbuttondown ==1 &&markposx > 450 && markposx <700 && markposy >100 &&markposy <200){//按下，改为到植物选择区域
		i = 0;
		STRAT=true;
		END = false;
		CHOOSE = 0;//选择开始
		lbuttondown = -1;
		if(t1 == -1){
			t1 = GetTickCount();
			mciSendString("stop .\\re\\music\\ui_gkmusic.wav",NULL,0,NULL);
			mciSendString("play .\\re\\music\\losemusic.wav",NULL,0,NULL);
			mciSendString("play .\\re\\music\\evillaugh.wav",NULL,0,NULL);
		}
	}
	SelectObject(bufdc,hSelector);
	GetObject(hSelector,sizeof(BITMAP),&bmsf);
	TransparentBlt(mdc,400,80,bmsf.bmWidth,bmsf.bmHeight/2,bufdc,0,(bmsf.bmHeight/2)*i,bmsf.bmWidth,bmsf.bmHeight/2,RGB(255,255,255));

	BitBlt(hdc,0,0,winwidth,winheight,mdc,0,0,SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间

}
int	Num = 0,//已经选择的个数
kexuan[9]={1,1,1,1,1,1,1,1,1};//卡片坐标
void checkPlant(HDC hdc){//选择植物界面
	static int i,j,flag=0;
	void setplant(int i);
	void cancelplant(int i);
	static int bkgroundx = 0,//背景坐标
		plantchooseboxx = 600,//植物备选框坐标
		cardx[9] = {0,35,90,145,200,255,310,365,420},cardy[9] = {0,0,0,0,0,0,0,0,0};

	if(CHOOSE==0){
		if(bkgroundx>-500){
			bkgroundx = bkgroundx-10;
		}
		else bkgroundx=-500;
	}
	
	
	SelectObject(bufdc,hbackground);//移动的背景
	GetObject(hbackground,sizeof(BITMAP),&bmbg);//最终x为-200
	BitBlt(mdc,bkgroundx,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);

	

	if(bkgroundx==-500 && flag==0){
		if(plantchooseboxx-13 > 88){
			plantchooseboxx = plantchooseboxx - 50;
		}
		else plantchooseboxx = 88;
		SelectObject(bufdc,hplantchoosebox);//植物备选框
		GetObject(hplantchoosebox,sizeof(BITMAP),&bmbg);
		TransparentBlt(mdc,20,plantchooseboxx,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,bmbg.bmWidth,bmbg.bmHeight,RGB(0,0,0));

		SelectObject(bufdc,hplantchoose);//植物已选框
		GetObject(hplantchoose,sizeof(BITMAP),&bmbg);
		BitBlt(mdc,20,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
		if(plantchooseboxx==88){//备选框移动完成
			if(lbuttondown==1){//左键按下
				if(markposy>plantchooseboxx+50 && markposy<plantchooseboxx+120){
					for(i=1;i<=8;i++){//选植物，这里的i代表种类
						if(markposx>i*55-20 && markposx<i*55+30 && kexuan[i]==1){
							Num++;
							lbuttondown = -1;
							kexuan[i] = -1;
							setplant(i);//设置选择框的属性
							break;
						}
					}
				}
				else if(markposy>-130 && markposy<200){
					for(i=0;i<Num;i++){//取消植物，这里的i代表卡片向量中的植物顺序
						if(markposx>45+(i+1)*55 && markposx<95+(i+1)*55 && plantvar_vct[i].ex==1){
							Num--;
							lbuttondown = -1;
							kexuan[plantvar_vct[i].var] = 1;
							cancelplant(i);//注意
							break;
						}
					}
				}
				else if(markposy>plantchooseboxx+400&&markposy<plantchooseboxx+657){//点击开始游戏
					if(Num==8){
						if(markposx>170&&markposx<327){
							//CHOOSE=-1;
							flag=1;
							lbuttondown = -1;
							//mciSendString("stop .\\re\\music\\lookup.wav",NULL,0,NULL);
							for(i=0;i<8;i++){
								switch (plantvar_vct[i].var){//初始化植物信息
								case 1:cd[i].ex = -1;cd[i].maxlasting = 3000;varvalue[i] = 50;break;
								case 2:cd[i].ex = -1;cd[i].maxlasting = 5000;varvalue[i] = 100;break;
								case 3:cd[i].ex = -1;cd[i].maxlasting = 8000;varvalue[i] = 200;break;
								case 4:cd[i].ex = -1;cd[i].maxlasting = 6500;varvalue[i] = 175;break;
								case 5:cd[i].ex = -1;cd[i].maxlasting = 15000;varvalue[i] = 50;break;
								case 6:cd[i].ex = -1;cd[i].maxlasting = 15000;varvalue[i] = 175;break;
								case 7:cd[i].ex = -1;cd[i].maxlasting = 25000;varvalue[i] = 175;break;
								case 8:cd[i].ex = -1;cd[i].maxlasting = 20000;varvalue[i] = 50;break;
								}
							}
						}
					}
				}
			}
		}
		for(i=0;i<Num;i++){
			SelectObject(bufdc,hplantnum[plantvar_vct[i].var]);//画已选择的植物
			GetObject(hplantnum[plantvar_vct[i].var],sizeof(BITMAP),&bmbg);
			BitBlt(mdc,45+(i+1)*55,plantchooseboxx - 80,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
		}
		for(i=1;i<=8;i++){
			if(kexuan[i]==1){
				SelectObject(bufdc,hplantnum[i]);//未选择的备选植物
				GetObject(hplantnum[i],sizeof(BITMAP),&bmbg);
				BitBlt(mdc,cardx[i],plantchooseboxx + 50 + cardy[i],bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
			}
		}
		if(Num == 8){//可开始游戏，开始游戏确认框
			SelectObject(bufdc,hfight2);
			GetObject(hfight2,sizeof(BITMAP),&bmbg);
			BitBlt(mdc,170,plantchooseboxx+400,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
		}
		else{
			SelectObject(bufdc,hfight1);
			GetObject(hfight1,sizeof(BITMAP),&bmbg);
			BitBlt(mdc,170,plantchooseboxx+400,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
		}
	}
	if(flag==1){
		SelectObject(bufdc,hplantchoosebox);//植物备选框
		GetObject(hplantchoosebox,sizeof(BITMAP),&bmbg);
		TransparentBlt(mdc,20,plantchooseboxx,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,bmbg.bmWidth,bmbg.bmHeight,RGB(0,0,0));

		SelectObject(bufdc,hplantchoose);//植物已选框
		GetObject(hplantchoose,sizeof(BITMAP),&bmbg);
		BitBlt(mdc,20,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
		
		for(i=0;i<Num;i++){
			SelectObject(bufdc,hplantnum[plantvar_vct[i].var]);//画已选择的植物
			GetObject(hplantnum[plantvar_vct[i].var],sizeof(BITMAP),&bmbg);
			BitBlt(mdc,45+(i+1)*55,8,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
		}
		for(i=1;i<=8;i++){
			if(kexuan[i]==1){
				SelectObject(bufdc,hplantnum[i]);//未选择的备选植物
				GetObject(hplantnum[i],sizeof(BITMAP),&bmbg);
				BitBlt(mdc,cardx[i],plantchooseboxx + 50 + cardy[i],bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
			}
		}
	}

	if(flag==1){
		if(plantchooseboxx < 600){
			plantchooseboxx = plantchooseboxx + 50;
		}
		else if(bkgroundx < -200){
			bkgroundx = bkgroundx + 40;
		}
		else {
			CHOOSE =-1;
			mciSendString("stop .\\re\\music\\lookup.wav",NULL,0,NULL);
			flag=0;
		}
	}
	BitBlt(hdc,0,0,winwidth,winheight,mdc,0,0,SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间
}

void setplant(int i){//i代表品种
	Plantvar newplant;
	newplant.ex = 1;
	newplant.var = i;
	plantvar_vct.push_back(newplant);
}
void cancelplant(int i){//这里的i代表顺序
	vector<Plantvar>::iterator it=&plantvar_vct[i];
	plantvar_vct.erase(it);
}
void paintready(HDC hdc){//准备好开始
	int i,j;
	SelectObject(bufdc,hbackground);
	GetObject(hbackground,sizeof(BITMAP),&bmbg);
	BitBlt(mdc,-200,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	
	SelectObject(bufdc,hplantchoose);//植物已选框
	GetObject(hplantchoose,sizeof(BITMAP),&bmbg);
	BitBlt(mdc,20,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	for(i=0;i<8;i++){
		SelectObject(bufdc,hplantnum[plantvar_vct[i].var]);//画已选择的植物
		GetObject(hplantnum[plantvar_vct[i].var],sizeof(BITMAP),&bmbg);
		BitBlt(mdc,45+(i+1)*55,8,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	}

	SelectObject(bufdc,hscoopbox);
	GetObject(hscoopbox,sizeof(BITMAP),&bmbg);
	BitBlt(mdc,542,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	SelectObject(bufdc,hscoop);
	GetObject(hscoop,sizeof(BITMAP),&bmsf);
	TransparentBlt(mdc,550,0,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	if(GetTickCount() - t2 < 500){
		
		SelectObject(bufdc,hPrepareGrowPlants);
		GetObject(hPrepareGrowPlants,sizeof(BITMAP),&bmsf);
		TransparentBlt(mdc,300,200,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	}
	else if(GetTickCount() - t2 <1000){
		SelectObject(bufdc,hPrepareGrowPlantsok);
		GetObject(hPrepareGrowPlantsok,sizeof(BITMAP),&bmsf);
		TransparentBlt(mdc,350,200,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	}
	else if(GetTickCount() - t2 < 1500){
		SelectObject(bufdc,hPrepareGrowPlantsgo);
		GetObject(hPrepareGrowPlantsgo,sizeof(BITMAP),&bmsf);
		TransparentBlt(mdc,300,180,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	}




	BitBlt(hdc,0,0,winwidth,winheight,mdc,0,0,SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间
}
void paintlargewave(HDC hdc){
	SelectObject(bufdc,hLargeWave);
	GetObject(hLargeWave,sizeof(BITMAP),&bmsf);
	TransparentBlt(mdc,300,200,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(246,255,226));
}
int poscard;
void MyPaint(HDC hdc){

	int i,j;
	/*处理设备消息*/
	if(lbuttondown==1){
		//收太阳
		vector<Sun>::iterator Sun_iter;
		for (Sun_iter = Sun_vct.begin() ; Sun_iter != Sun_vct.end() ; ){
			SelectObject(bufdc,hSun[0]);
			GetObject(hSun[0],sizeof(BITMAP),&bmzo);
			if(markposx > Sun_iter->x && markposx < Sun_iter->x+bmzo.bmWidth && markposy > Sun_iter->y && markposy < Sun_iter->y+bmzo.bmHeight){//判断单击太阳
				Sun_iter->speadx = (Sun_iter->x-20)*0.1;
				Sun_iter->speady = (0 + Sun_iter->y)*0.1;
				Sun_collect.push_back(*Sun_iter);
				Sun_iter=Sun_vct.erase(Sun_iter);
				mciSendString("stop .\\re\\music\\point.wav", NULL, 0, NULL);
				mciSendString("play .\\re\\music\\point.wav",NULL,0,NULL);
			}
			else{
				Sun_iter++;
			}
		}
		
		/*选择要种的植物*/
		
		if(planted == -1){
			for(i = 1 ; i <= 8 ; i++){
				if(markposx > 100+(i-1)*50+(i-1)*5 && markposx < 100+i*50+(i-1)*5 && markposy > 12 && markposy < 72)
				{
					if(moneyless[i-1].ex==-1 && cd[i-1].ex == -1){
						hcposx=markposx;
						hcposy=markposy;
						planted = plantvar_vct[i-1].var;//选择植物的种类
						poscard=i-1;

						mciSendString("stop .\\re\\music\\planted2.wav", NULL, 0, NULL);
						mciSendString("play .\\re\\music\\planted2.wav",NULL,0,NULL);
					}
				}
			}
		}
		else if( planted != -1 ){/*种植物*/
			for(i = 0 ; i < 9 ; i++){
				if(markposx>60 + i*80&&markposx<60 + (i+1)*80){
					for(j = 0 ; j < 5 ; j++){
						if(markposy>80 + j*100 && markposy<80 + (j+1)*100&&pp[j][i].ex==false)
						{
							pp[j][i].ex = true;
							pp[j][i].num = 0;
							pp[j][i].blood = 150;//初始血量100

							cd[poscard].starttime = GetTickCount();
							cd[poscard].ex =1;

							mciSendString("stop .\\re\\music\\planted.wav", NULL, 0, NULL);
							mciSendString("play .\\re\\music\\planted.wav",NULL,0,NULL);
							
							if(planted == 1){
								pp[j][i].var = 1;//种太阳花
								money -= 50;
								pp[j][i].maxnum = 36;
								pp[j][i].tshoot=GetTickCount();
							}
							if(planted == 2){
								pp[j][i].var = 2;//豌豆射手
								money -= 100;
								pp[j][i].maxnum = 26;
								pp[j][i].tshoot=GetTickCount();
							}
							if(planted == 3){
								pp[j][i].var = 3;//豌豆射手2
								money -= 200;
								pp[j][i].maxnum = 30;
								pp[j][i].tshoot=GetTickCount();
							}
							if(planted == 4){
								pp[j][i].var = 4;//寒冰射手
								money -= 175;
								pp[j][i].maxnum = 30;
							}
							if(planted == 5){
								pp[j][i].var = 5;//土豆
								money -= 50;
								pp[j][i].blood = 250;
								pp[j][i].maxnum = 32;
							}
							if(planted == 6){
								pp[j][i].var = 6;//食人花
								money -= 150;
								pp[j][i].recover = -1;
								pp[j][i].maxnum = 42;
							}
							if(planted == 7){
								pp[j][i].var = 7;//辣椒
								money -= 125;
								pp[j][i].maxnum = 16;
							}
							if(planted == 8){
								pp[j][i].var = 8;//倭瓜
								money -= 50;
								pp[j][i].maxnum = 34;
							}
							planted =-1;
							break;
						}
					}
				}
			}
			if(planted != -1){
				planted =-1;
			}
		}

		if(scooped == -1){/*选择铲子*/
			if(markposx > 550 && markposx < 600 && markposy > 0 && markposy <72){
				hcposx=markposx;
				hcposy=markposy;
				scooped = 1;
				
			}
		}
		else if(scooped == 1){/*铲掉植物*/
			for(i = 0 ; i < 9 ; i++){
				if(hcposx>60 + i*80&&hcposx<60 + (i+1)*80){
					for(j = 0 ; j < 5 ; j++){
						if(hcposy>80 + j*100&&hcposy<80 + (j+1)*100){
							if(pp[j][i].ex == true){
								pp[j][i].ex = false;
								scooped = -1;
								//PlaySound(TEXT(".\\re\\music\\planted2.wav"),NULL,SND_ASYNC || SND_NOSTOP);
								mciSendString("stop .\\re\\music\\planted2.wav", NULL, 0, NULL);
								mciSendString("play .\\re\\music\\planted2.wav",NULL,0,NULL);
							}
							else{
								scooped = -1;
							}
						}
					}
				}
			}
		}
		lbuttondown = 0;
	}

	//先画背景到小dc上
	SelectObject(bufdc,hbackground);
	GetObject(hbackground,sizeof(BITMAP),&bmbg);
	BitBlt(mdc,-200,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	
	/*------------------------------植物选择框------------------------------------*/
	SelectObject(bufdc,hplantchoose);//植物已选框
	GetObject(hplantchoose,sizeof(BITMAP),&bmbg);
	BitBlt(mdc,20,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	for(i=0;i<8;i++){
		SelectObject(bufdc,hplantnum[plantvar_vct[i].var]);//画已选择的植物
		GetObject(hplantnum[plantvar_vct[i].var],sizeof(BITMAP),&bmbg);
		BitBlt(mdc,45+(i+1)*55,8,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	}
	for(i=0;i<8;i++){//没钱不能选
		if(money - varvalue[i] < 0.0){
			moneyless[i].ex = 1;
			AlphaBmp(mdc,hCD,i*55+100,8,100,hrgn_cd,RGB(0,0,0));
		}
		else{
			moneyless[i].ex = -1;
		}
	}
	//cd遮罩
	for(i=0;i<8;i++){
		if(cd[i].ex ==1){
			AlphaBmp(mdc,hCD,i*55+100,8 - ((GetTickCount() - cd[i].starttime)/cd[i].maxlasting)*71.0,100,hrgn_cd,RGB(0,0,0));
		}
		if(GetTickCount() - cd[i].starttime >= cd[i].maxlasting){
			cd[i].ex = -1;
		}
	}
	/*------------------------------框------------------------------------*/
	SelectObject(bufdc,hscoopbox);//框
	GetObject(hscoopbox,sizeof(BITMAP),&bmbg);
	BitBlt(mdc,542,0,bmbg.bmWidth,bmbg.bmHeight,bufdc,0,0,SRCCOPY);
	/*------------------------------输出阳光数量------------------------------------*/
	
	PAINTSTRUCT ps;			//定义包含绘图信息的结构体变量
	TEXTMETRIC tm;			//定义包含字体信息的结构体变量.
	HFONT hf;
	static char hc_pos[20];
	sprintf(hc_pos,"%d\0",money);
	hf = myCreateFont(mdc,20,0);		
	SelectObject(bufdc,hf);			//选入字体.
	GetTextMetrics(bufdc,&tm);
	SetTextColor(bufdc,RGB(255,0,0));	//设置字体的颜色
	TextOut(mdc,45,65,hc_pos,strlen(hc_pos));

	for(j=0;j<5;j++){//对僵尸向量进行升序排序
		sort(Zombie_vct[j].begin(),Zombie_vct[j].end(),cmp);
	}
	
	/*--------------------------------------植物------------------------------------------*/
	for(i = 0 ; i < 9 ; i++){
		for(j = 0 ; j < 5 ; j++){
			if(pp[j][i].ex == true){//有植物
				//画阴影
				if(!( pp[j][i].var==7 && pp[j][i].maxnum==25)&&pp[j][i].var!=8){//辣椒特判一下爆炸后不用画阴影
					AlphaBmp(mdc,hshadow,52 + i*80,130 + j*100,100,hrgn_shadow,RGB(255,255,255));
				}
				if(pp[j][i].num == pp[j][i].maxnum){
					pp[j][i].num = 0;
				}
				if(pp[j][i].var == 1){//有太阳花
					SelectObject(bufdc,hSunFlawor[pp[j][i].num]);
					GetObject(hSunFlawor[pp[j][i].num],sizeof(BITMAP),&bmsf);
					TransparentBlt(mdc,60 + i*80,80 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					if(GetTickCount()-pp[j][i].tshoot > 8000){//25秒产出一个阳光
						Sun tempsun;
						tempsun.num = 0;
						tempsun.starttime = -1;//没有落地
						tempsun.x = rand()%60 + 30 + i*80;
						tempsun.y = 60 + j*100;
						tempsun.speady = 4;
						tempsun.maxnum = 29;
						switch(j){
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
						Sun_vct.push_back(tempsun);
						pp[j][i].tshoot = GetTickCount();
					}
				}
				else if(pp[j][i].var == 2){//豌豆射手
					SelectObject(bufdc,hPeashooter[pp[j][i].num]);
					GetObject(hPeashooter[pp[j][i].num],sizeof(BITMAP),&bmsf);
					TransparentBlt(mdc,62 + i*80,83 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					if(!Zombie_vct[j].empty()){//当前行有僵尸
						if(GetTickCount()-pp[j][i].tshoot > 2000){//2秒发射一个豌豆
							//PlaySound(TEXT(".\\re\\music\\throw.wav"),NULL,SND_ASYNC || SND_NOSTOP);
							//mciSendString("stop .\\re\\music\\throw.wav", NULL, 0, NULL);
							mciSendString("play .\\re\\music\\throw.wav",NULL,0,NULL);
							Pball temppball;
							temppball.x = 80 + i*80;
							temppball.y = 80 + j*100;
							temppball.speed = 10;
							temppball.j = j;
							temppball.var = 1;
							pball_vct.push_back(temppball);
							pp[j][i].tshoot = GetTickCount();
						}
					}
				}
				else if(pp[j][i].var == 3){//豌豆射手2
					SelectObject(bufdc,hRepeater[pp[j][i].num]);
					GetObject(hRepeater[pp[j][i].num],sizeof(BITMAP),&bmsf);
					TransparentBlt(mdc,60 + i*80,83 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					if(!Zombie_vct[j].empty()){//当前行有僵尸
						if(GetTickCount()-pp[j][i].tshoot > 1000){//1秒发射一个豌豆
							//PlaySound(TEXT(".\\re\\music\\throw.wav"),NULL,SND_ASYNC || SND_NOSTOP);
							//mciSendString("stop .\\re\\music\\throw.wav", NULL, 0, NULL);
							mciSendString("play .\\re\\music\\throw.wav",NULL,0,NULL);
							Pball temppball;
							temppball.x = 80 + i*80;
							temppball.y = 80 + j*100;
							temppball.speed = 10;
							temppball.j = j;
							temppball.var = 1;
							pball_vct.push_back(temppball);
							pp[j][i].tshoot = GetTickCount();
						}
					}
				}
				else if(pp[j][i].var == 4){//寒冰射手
					SelectObject(bufdc,hSnowPea[pp[j][i].num]);
					GetObject(hSnowPea[pp[j][i].num],sizeof(BITMAP),&bmsf);
					TransparentBlt(mdc,60 + i*80,83 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					if(!Zombie_vct[j].empty()){//当前行有僵尸
						if(GetTickCount()-pp[j][i].tshoot > 2000){//2秒发射一个冰冻豌豆
							//PlaySound(TEXT(".\\re\\music\\throw.wav"),NULL,SND_ASYNC || SND_NOSTOP);
							//mciSendString("stop .\\re\\music\\throw.wav", NULL, 0, NULL);
							mciSendString("play .\\re\\music\\throw.wav",NULL,0,NULL);
							Pball temppball;
							temppball.x = 80 + i*80;
							temppball.y = 80 + j*100;
							temppball.speed = 10;
							temppball.j = j;
							temppball.var = 2;
							pball_vct.push_back(temppball);
							pp[j][i].tshoot = GetTickCount();
						}
					}
				}
				else if(pp[j][i].var == 5){//土豆
					if(pp[j][i].blood > 150){
					SelectObject(bufdc,hWallNut[pp[j][i].num]);
					GetObject(hWallNut[pp[j][i].num],sizeof(BITMAP),&bmsf);
					TransparentBlt(mdc,60 + i*80,83 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					}
					else if(pp[j][i].blood >75){
						if(pp[j][i].maxnum==32){
							pp[j][i].maxnum = 22;
							pp[j][i].num=0;
						}
						SelectObject(bufdc,hWallnut_cracked1[pp[j][i].num]);
						GetObject(hWallnut_cracked1[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,60 + i*80,83 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					}
					else{
						if(pp[j][i].maxnum==22){
							pp[j][i].maxnum = 30;
							pp[j][i].num=0;
						}
						SelectObject(bufdc,hWallnut_cracked2[pp[j][i].num]);
						GetObject(hWallnut_cracked2[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,60 + i*80,83 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					}
				}
				else if(pp[j][i].var == 6){//食人花
					if(!Zombie_vct[j].empty() && pp[j][i].recover==-1){
						vector<Zombie>::iterator Zombie_iter4;
						for(Zombie_iter4 = Zombie_vct[j].begin();Zombie_iter4 != Zombie_vct[j].end() ;  ){
							if(Zombie_iter4->x >= i*80 && Zombie_iter4->x <= 30+(i+1)*80){
								pp[j][i].maxnum = 20;//特殊原因，写大1，实际是19
								pp[j][i].num = 0;
								pp[j][i].recover = GetTickCount();
								Zombie_iter4->disapper = 1;
								//Zombie_iter4 = Zombie_vct[j].erase(Zombie_iter4);
								break;
							}
							else {
								Zombie_iter4++;
							}
						}
					}
					else if(pp[j][i].recover != -1 && pp[j][i].maxnum == 20 && pp[j][i].num == 19){
						pp[j][i].maxnum = 19;
						pp[j][i].num = 0;
					}
					else if(pp[j][i].recover != -1 && pp[j][i].maxnum == 19 && GetTickCount() - pp[j][i].recover > 15000){
						pp[j][i].recover = -1;
						pp[j][i].maxnum = 42;
						pp[j][i].num = 0;
					}

					if(pp[j][i].maxnum == 42){
						SelectObject(bufdc,hChomper[pp[j][i].num]);
						GetObject(hChomper[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,60 + i*80,40 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					}
					else if(pp[j][i].maxnum == 20){
						if(pp[j][i].num==13){
							vector<Zombie>::iterator Zombie_iter4;//缓一缓消失的僵尸
							for(Zombie_iter4 = Zombie_vct[j].begin();Zombie_iter4 != Zombie_vct[j].end() ;  ){
								if(Zombie_iter4->disapper == 1){
									Zombie_iter4 = Zombie_vct[j].erase(Zombie_iter4);
									break;
								}
								else {
									Zombie_iter4++;
								}
							}
						}
						//mciSendString("stop .\\re\\music\\Chomp.wav",NULL,0,NULL);
						mciSendString("play .\\re\\music\\gope.wav",NULL,0,NULL);
						SelectObject(bufdc,hChomperAttack[pp[j][i].num]);
						GetObject(hChomperAttack[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,60 + i*80,40 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					}
					else if(pp[j][i].maxnum==19){
						//mciSendString("play .\\re\\music\\Chomp2.wav",NULL,0,NULL);
						SelectObject(bufdc,hChomperDigest[pp[j][i].num]);
						GetObject(hChomperDigest[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,60 + i*80,40 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					}
				}
				else if(pp[j][i].var == 7){//辣椒
					if(pp[j][i].maxnum ==16&&pp[j][i].num<=15){
						SelectObject(bufdc,hJalapeno[pp[j][i].num]);
						GetObject(hJalapeno[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,60 + i*80,75 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
						if(pp[j][i].num==15){
							mciSendString("play .\\re\\music\\boom.wav",NULL,0,NULL);
							pp[j][i].maxnum = 25;
							pp[j][i].num = 0;
						}
					}
					else if(pp[j][i].maxnum ==25 && pp[j][i].num <= 24){
						SelectObject(bufdc,hJalapenoAttack[pp[j][i].num]);
						GetObject(hJalapenoAttack[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,52,30 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
					//	if(pp[j][i].num%2==0 && pp[j][i].num <20){//僵尸从左到右被炸到
							vector<Zombie>::iterator Zombie_iter3;
							for(Zombie_iter3=Zombie_vct[j].begin();Zombie_iter3!=Zombie_vct[j].end();Zombie_iter3++){
							//	if(Zombie_iter3->x <= 52 + (pp[j][i].num/2+1)*100){
									Zombie_iter3->blood = -1;
									Zombie_iter3->condition = 6;//boomdie
									Zombie_iter3->maxnum = 50;
									Zombie_iter3->num = 0;
							//	}
							}
					//	}
					//	else 
							if(pp[j][i].num==24){
							pp[j][i].ex = false;
						}
					}
				}
				else if(pp[j][i].var == 8){//倭瓜
					if(pp[j][i].maxnum == 34){//正常状态
						SelectObject(bufdc,hSquash[pp[j][i].num]);
						GetObject(hSquash[pp[j][i].num],sizeof(BITMAP),&bmsf);
						TransparentBlt(mdc,45 + i*80,-70 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
						if(!Zombie_vct[j].empty()){
							vector<Zombie>::iterator Zombie_iter4;
							for(Zombie_iter4 = Zombie_vct[j].begin();Zombie_iter4 != Zombie_vct[j].end() ;  ){
								if(Zombie_iter4->x >= 45+(i+1)*80-80 && Zombie_iter4->x <= 45+(i+2)*80-80){
									pp[j][i].maxnum = 33;
									pp[j][i].num = 0;
									pp[j][i].tshoot = 1;
									break;
									//Zombie_iter4 = Zombie_vct[j].erase(Zombie_iter4);
								}
								else {
									Zombie_iter4++;
								}
							}
						}
					}
					else if(pp[j][i].maxnum==33){//攻击状态
						if(pp[j][i].tshoot>0&&pp[j][i].tshoot<=6){//斜视
							pp[j][i].num=0;
							SelectObject(bufdc,hSquashR);
							GetObject(hSquashR,sizeof(BITMAP),&bmsf);
							TransparentBlt(mdc,45 + i*80,-70 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,0,0));
							pp[j][i].tshoot++;
						}
						else if(pp[j][i].tshoot>6){//上天
							pp[j][i].num=0;
							SelectObject(bufdc,hSquash[0]);
							GetObject(hSquash[0],sizeof(BITMAP),&bmsf);
							TransparentBlt(mdc,45 + i*80+(pp[j][i].tshoot-6)*8,-70 + j*100-(pp[j][i].tshoot-6)*8,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
							pp[j][i].tshoot++;
							if(pp[j][i].tshoot==16){
								pp[j][i].tshoot=-1;
							}
						}
						else{
							if(pp[j][i].num==10){
								if(!Zombie_vct[j].empty()){
									vector<Zombie>::iterator Zombie_iter4;
									for(Zombie_iter4 = Zombie_vct[j].begin();Zombie_iter4 != Zombie_vct[j].end() ;  ){
										if(Zombie_iter4->x >= 45+(i+1)*80-80 && Zombie_iter4->x <= 45+(i+2)*80-80){
											Zombie_iter4 = Zombie_vct[j].erase(Zombie_iter4);
										}
										else {
											Zombie_iter4++;
										}
									}
								}
							}
							SelectObject(bufdc,hSquashAttack[pp[j][i].num]);
							GetObject(hSquashAttack[pp[j][i].num],sizeof(BITMAP),&bmsf);
							TransparentBlt(mdc,45 + (i+1)*80,-70 + j*100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
						}
						if(pp[j][i].num==32){
							pp[j][i].ex=false;
						}
					}
				}
				pp[j][i].num++;
			}
		}
	}
	/*格子中的铲子*/
	if(scooped == -1){
		SelectObject(bufdc,hscoop);
		GetObject(hscoop,sizeof(BITMAP),&bmsf);
		TransparentBlt(mdc,550,0,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	}
	/*-------------------------------------僵尸----------------------------------------*/
	vector<Zombie>::iterator iter,iter2;
	for(j=0;j<5;j++){
		for (iter=Zombie_vct[j].begin();iter!=Zombie_vct[j].end();) {
			
			for(i = 0 ; i < 9 ; i++){
				if(pp[j][i].ex == true&&(pp[j][i].var!=7&&pp[j][i].var!=8)){//种了植物,辣椒和倭瓜不能吃
					if(iter->x < 60+(i)*80&&iter->x>60+(i-1)*80){//僵尸到达了花???这个判断怎么有问题
						if(iter->condition ==1){//第一次遇到植物
							iter->speed = 0;
							iter->condition = 2;//攻击状态
							iter->maxnum = 43;
							iter->num = 0;
							iter->i=i;//记录下正在攻击的植物的行列数
							iter->j=j;
						}
						if(pp[j][i].blood >= 0 && iter->condition==2){//植物还有血
							mciSendString("play .\\re\\music\\Chomp.wav",NULL,0,NULL);
							pp[j][i].blood -=1;//植物扣血
						}
						else if(pp[j][i].blood < 0){//植物死了的下一帧所有攻击这个植物的僵尸都要恢复正常状态1
							pp[j][i].ex = false;
							for (iter2=Zombie_vct[j].begin();iter2!=Zombie_vct[j].end();iter2++) {
								if(iter2->i == i && iter2->j == j && iter2->condition==2){
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


			if(iter->blood<=0){
				if(iter->condition==1){
					iter->condition = 3;//没有吃植物状态下失去头
					iter->maxnum = 60;
					iter->num = 0;
				}
				else if(iter->condition==2){
					iter->condition = 4;//吃植物状态下失去头
					iter->maxnum = 22;
					iter->num = 0;
				}
				else if((iter->condition==3 && iter->num == 60)||(iter->condition == 4 && iter->num == 22)){
					iter->condition = 5;//biedie
					iter->maxnum = 40;
					iter->num = 0;
				}
			}
			if(iter->num == iter->maxnum)
				iter->num =0;

			if(!( iter->condition == 5)){//倒下的僵尸不用画阴影
					AlphaBmp(mdc,hshadow,iter->x+75,130+j*100,100,hrgn_shadow,RGB(255,255,255));
			}
			
			if(iter->condition == 1){//行走
				if(GetTickCount() - iter->recover >1000 || iter->recover== -1){//超过1秒僵尸恢复正常
					iter->recover = -1;
					iter->speed = 0.5;
					SelectObject(bufdc,hZombie[ iter->num ]);
					GetObject(hZombie[ iter->num ],sizeof(BITMAP),&bmzo);
					TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(0,0,0));
				}
				else{//被冰冻了
					SelectObject(bufdc,hZombieblue[ iter->num ]);
					GetObject(hZombieblue[ iter->num ],sizeof(BITMAP),&bmzo);
					TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(33,50,122));
				}
				iter->x-=iter->speed;
			}
			else if( iter->condition == 2 ){//攻击僵尸
				if(GetTickCount() - iter->recover >1000 || iter->recover== -1){//超过1秒僵尸恢复正常
					iter->recover = -1;
					iter->speed = 0.5;
					SelectObject( bufdc , hZombieAttack[ iter->num ] );
					GetObject( hZombieAttack[ iter->num ] , sizeof( BITMAP ), &bmzo );
					TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(0,0,0));
				}
				else{
					SelectObject(bufdc,hZombieAttackblue[ iter->num ]);
					GetObject(hZombieAttackblue[ iter->num ],sizeof(BITMAP),&bmzo);
					TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(33,50,122));
				}
			}
			else if( iter->condition == 3 ){//没有吃植物情况下失去头
				SelectObject(bufdc,hZombieLostHead[ iter->num ]);
				GetObject(hZombieLostHead[ iter->num ],sizeof(BITMAP),&bmzo);
				TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(0,0,0));
				iter->x-=iter->speed;
			}
			else if( iter->condition == 4 ){//攻击状态下失去头
				SelectObject(bufdc,hZombieLostHeadAttack[ iter->num ]);
				GetObject(hZombieLostHeadAttack[ iter->num ],sizeof(BITMAP),&bmzo);
				TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(0,0,0));
			}
			else if( iter->condition ==5 ){//bedie垂死
				SelectObject(bufdc,hZombieDie[ iter->num ]);
				GetObject(hZombieDie[ iter->num ],sizeof(BITMAP),&bmzo);
				TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(0,0,0));
			}
			else if(iter->condition == 6){//boomdie
				SelectObject(bufdc,hBoomDie[ iter->num ]);
				GetObject(hBoomDie[ iter->num ],sizeof(BITMAP),&bmzo);
				TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(0,255,0));
			}
			iter->num++;
			
			if(iter->condition==3 || iter->condition==4){//画头
				if(iter->head.num < iter->head.maxnum ){
					SelectObject(bufdc,hZombieHead[ iter->head.num ]);
					GetObject(hZombieHead[ iter->head.num ],sizeof(BITMAP),&bmzo);
					TransparentBlt(mdc,iter->x,15+j*100,bmzo.bmWidth,bmzo.bmHeight,bufdc,0,0,bmzo.bmWidth,bmzo.bmHeight,RGB(0,0,0));
					iter->head.num++;
				}
			}
			
			if( (iter->condition == 5||iter->condition == 6) && iter->num == iter->maxnum){//全部状态画完出栈
				iter->y = 15+j*100; 
				iter = Zombie_vct[j].erase(iter);
			}
			else{
				iter++;
			}
		}
	}
	
	/*---------------------------------------发射的豌豆--------------------------------------------*/
	vector<Pball>::iterator pball_iter;
	vector<Zombie>::iterator Zombie_iter2;
	for (pball_iter=pball_vct.begin();pball_iter!=pball_vct.end();){
		j=pball_iter->j;
		int pball_shooted = 0;
		for (Zombie_iter2=Zombie_vct[j].begin();Zombie_iter2!=Zombie_vct[j].end();Zombie_iter2++){
			 if(pball_iter->x >= Zombie_iter2->x + 55 && pball_iter->x <= Zombie_iter2->x + 80 && (Zombie_iter2->condition != 5&& Zombie_iter2->condition != 6)){//射中僵尸
				Zombie_iter2->blood -= 5;//僵尸扣血
				SelectObject(bufdc,hPeaBulletHit);
				GetObject(hPeaBulletHit,sizeof(BITMAP),&bmsf);
				TransparentBlt(mdc,pball_iter->x,pball_iter->y,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
				if(pball_iter->var == 2){//蓝豆子减速
					Zombie_iter2->speed=0.1;
					Zombie_iter2->recover = GetTickCount();
				}
				pball_iter = pball_vct.erase(pball_iter);
				pball_shooted=1;
				//mciSendString("stop .\\re\\music\\plastichit.wav", NULL, 0, NULL);
				mciSendString("play .\\re\\music\\plastichit.wav",NULL,0,NULL);

				break;//射中之后没这列其他僵尸什么事情了
			}
			else{//没有射中这只僵尸
			}
		}
		if(pball_shooted==0){//没有射中这行僵尸
			if(pball_iter->var ==1){//绿豆子
				SelectObject(bufdc,hpball);
				GetObject(hpball,sizeof(BITMAP),&bmsf);
				TransparentBlt(mdc,pball_iter->x,pball_iter->y,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(246,255,226));
			}
			else if(pball_iter->var ==2){//蓝豆子
				SelectObject(bufdc,hSnowPeashooterBullet);
				GetObject(hSnowPeashooterBullet,sizeof(BITMAP),&bmsf);
				TransparentBlt(mdc,pball_iter->x,pball_iter->y,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,255,0));
			}
			pball_iter->x+=pball_iter->speed;
			pball_iter++;
		}
	}

	/*---------------------------------------自然产生阳光--------------------------------------------*/
	Sun tempsun;
	if(GetTickCount() - tSun > 10000){//每隔10秒加入一颗太阳
		tempsun.num = 0;
		tempsun.starttime = -1;//没有落地
		tempsun.x = rand()%700+30;
		tempsun.y = 0;
		tempsun.speady = 4;
		tempsun.maxnum = 29;
		i = rand()%5;
		switch(i){
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
		Sun_vct.push_back(tempsun);
		tSun = GetTickCount();
	}
	/*-------------------------------------画掉落的太阳-------------------------------------*/
	vector<Sun>::iterator Sun_iter;
	for (Sun_iter=Sun_vct.begin();Sun_iter!=Sun_vct.end();){
		if(Sun_iter->num == Sun_iter->maxnum)
				Sun_iter->num = 0;
			AlphaBmp(mdc,hSun[Sun_iter->num],Sun_iter->x,Sun_iter->y,230,hrgn[Sun_iter->num],RGB(0,0,0));
			
			//落地超过5秒阳光消失消失
			if(Sun_iter->y >= Sun_iter->maxy ){//落地
				Sun_iter->y = Sun_iter->maxy;
				if(Sun_iter->starttime == -1){//第一次落地
					Sun_iter->starttime = GetTickCount();//太阳落地
				}
			}
			else{
				Sun_iter->y+=Sun_iter->speady;
			}
			Sun_iter->num++;
			if(GetTickCount() - Sun_iter->starttime >= 5000 && Sun_iter->starttime !=- 1){//落地超过5秒&&落地
				Sun_iter = Sun_vct.erase(Sun_iter);
			}
			else{
				Sun_iter++;
			}
	}
	/*-------------------------------------------画正在被收集的太阳----------------------------------------------*/
	for(Sun_iter=Sun_collect.begin();Sun_iter!=Sun_collect.end();){
		if(Sun_iter->x <= 20 || Sun_iter->y <= 0){
			Sun_iter->x = 20;
			Sun_iter->y = 0;
		}
		if(Sun_iter->num == Sun_iter->maxnum)
			Sun_iter->num = 0;
		AlphaBmp(mdc,hSun[Sun_iter->num],Sun_iter->x,Sun_iter->y,230,hrgn[Sun_iter->num],RGB(0,0,0));
		if(Sun_iter->x <= 20 || Sun_iter->y <= 0 ){
			money+=25;
			Sun_iter = Sun_collect.erase(Sun_iter);
		}
		else{
			Sun_iter->x = Sun_iter->x - Sun_iter->speadx;
			Sun_iter->y = Sun_iter->y - Sun_iter->speady;
			Sun_iter->num++;
			Sun_iter++;
		}
	}
	/*---------------------------------------跟随鼠标动的植物--------------------------------------------*/
	if(planted != -1){
		if(planted == 1){//1是向日葵
			drawplantready(1,hSunFlawor[0],60,80);
			SelectObject(bufdc,hSunFlawor[0]);
			GetObject(hSunFlawor[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 30,hcposy - 60,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(planted ==2){//2豌豆射手
			drawplantready(2,hPeashooter[0],62,83);
			SelectObject(bufdc,hPeashooter[0]);
			GetObject(hPeashooter[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 30,hcposy - 60,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(planted ==3){//2豌豆射手
			drawplantready(3,hRepeater[0],60,83);
			SelectObject(bufdc,hRepeater[0]);
			GetObject(hRepeater[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 30,hcposy - 60,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(planted ==4){//寒冰射手
			drawplantready(4,hSnowPea[0],60,83);
			SelectObject(bufdc,hSnowPea[0]);
			GetObject(hSnowPea[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 30,hcposy - 60,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(planted == 5){//土豆
			drawplantready(5,hWallNut[0],60,83);
			SelectObject(bufdc,hWallNut[0]);
			GetObject(hWallNut[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 30,hcposy - 60,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(planted == 6){//食人花
			drawplantready(6,hChomper[0],60,40);
			SelectObject(bufdc,hChomper[0]);
			GetObject(hChomper[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 30,hcposy - 90,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(planted == 7){//辣椒
			drawplantready(7,hJalapeno[0],60,75);
			SelectObject(bufdc,hJalapeno[0]);
			GetObject(hJalapeno[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 30,hcposy - 60,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(planted == 8){//倭瓜
			drawplantready(8,hSquash[0],45,-70);
			SelectObject(bufdc,hSquash[0]);
			GetObject(hSquash[0],sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,hcposx - 40,hcposy - 200,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
	}

	/*---------------------------------------移动的铲子-------------------------------------------------*/
	if(scooped == 1){
		SelectObject(bufdc,hscoop);
		GetObject(hscoop,sizeof(BITMAP),&bmsf);
		TransparentBlt(mdc,hcposx - 30,hcposy - 60,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	}

	/*--------------------------------------------进度提示----------------------------------------------------*/
	if(GetTickCount() - t3 <= 162000 && GetTickCount() - t3 >= 160000){
		paintlargewave(hdc);
	}
	else if(GetTickCount() - t3 <= 232000 && GetTickCount() - t3 >= 230000){
		SelectObject(bufdc,hFinalWave);
		GetObject(hFinalWave,sizeof(BITMAP),&bmsf);
		TransparentBlt(mdc,300,200,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	}
	else if(GetTickCount() - t3 >= 250000){
		for(j=0;j<5;j++){
			if(!Zombie_vct[j].empty()){
				break;
			}
		}
		if(j==5){
			Sun_vct.clear();
			Sun_collect.clear();
			mciSendString("stop .\\re\\music\\bkmusic.wav",NULL,0,NULL);
			mciSendString("play .\\re\\music\\winmusic.wav",NULL,0,NULL);
			t3 = GetTickCount();
			WIN=true;
			END=true;
		}
	}

	if(END == false){
		for(j=0;j<5;j++){
			if(!Zombie_vct[j].empty() && Zombie_vct[j].begin()->x <= -10){
				Sun_vct.clear();
				Sun_collect.clear();
				mciSendString("stop .\\re\\music\\bkmusic.wav",NULL,0,NULL);
				mciSendString("play .\\re\\music\\scream.wav",NULL,0,NULL);
				t3 = GetTickCount();
				END = true;
				WIN = false;
			}
		}
	}
	else{//end==true
		if(WIN==false){
			SelectObject(bufdc,hZombiesWon);
			GetObject(hZombiesWon,sizeof(BITMAP),&bmsf);
			TransparentBlt(mdc,100,100,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(0,0,0));
		}
		else if(WIN==true){
			
		}

		if(GetTickCount() - t3 >= 6000){//还原
			STRAT = false;
			t1=t2=t3=-1;
			pball_vct.clear();
			plantvar_vct.clear();
			Num=0;
			for(i=0;i<9;i++){
				kexuan[i]=1;
			}
			markposx = markposy=releasex=releasey=movex=movey=-1;
			for(i=0;i<5;i++){
				for(j=0;j<9;j++){
					pp[i][j].ex = false;
				}
			}
		}
		for(i=0;i<5;i++)
			Zombie_vct[i].clear();
		Sun_vct.clear();
		money = 50;
	}
	

	/*-----------------------------------------进度条-------------------------------------------------*/
	SelectObject(bufdc,hFlagMeterFull);//满
	GetObject(hFlagMeterFull,sizeof(BITMAP),&bmsf);
	TransparentBlt(mdc,620,570,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	SelectObject(bufdc,hFlagMeterEmpty);//空
	GetObject(hFlagMeterEmpty,sizeof(BITMAP),&bmsf);
	TransparentBlt(mdc,620,570,bmsf.bmWidth*(1-((GetTickCount() - t3)/270000.0)),bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth*(1-((GetTickCount() - t3)/270000.0)),bmsf.bmHeight,RGB(255,255,255));
	SelectObject(bufdc,hFlagMeterParts2);//旗子
	GetObject(hFlagMeterParts2,sizeof(BITMAP),&bmsf);
	TransparentBlt(mdc,626,560,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	TransparentBlt(mdc,670,560,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	SelectObject(bufdc,hFlagMeterLevelProgress );//标志“关卡进程”
	GetObject(hFlagMeterLevelProgress,sizeof(BITMAP),&bmsf);
	TransparentBlt(mdc,655,582,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));
	SelectObject(bufdc,hFlagMeterParts1);//僵尸头
	GetObject(hFlagMeterParts1,sizeof(BITMAP),&bmsf);
	double x1=626 + 135.0*(1-((GetTickCount() - t3)/250000.0));
	if((1-((GetTickCount() - t3)/250000.0))<0){
		x1=626;
	}
	TransparentBlt(mdc,x1,560,bmsf.bmWidth,bmsf.bmHeight,bufdc,0,0,bmsf.bmWidth,bmsf.bmHeight,RGB(255,255,255));

	
	
	//画到屏幕上
	BitBlt(hdc,0,0,winwidth,winheight,mdc,0,0,SRCCOPY);
	tPre = GetTickCount();//记录此次绘图时间
}

void drawplantready(int var,HBITMAP hx,int x,int y){
	int i,j;
	for(i = 0 ; i < 9 ; i++){
		if(hcposx>60 + i*80&&hcposx<60 + (i+1)*80){
			for(j = 0 ; j < 5 ; j++){
				if(hcposy>80 + j*100 && hcposy<80 + (j+1)*100)
				{
					if(pp[j][i].ex==false){
						AlphaBmp(mdc,hx,x+i*80,y+j*100,100,hrgn_plant[var-1],RGB(0,0,0));
					}
				}
			}
		}
	}
}

HFONT myCreateFont(HDC hDC,int nCharHeight,BOOL bItalic)
{
	HFONT hFont;
	hFont=CreateFont(				//定义字体句柄.
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
		DEFAULT_PITCH|FF_DONTCARE,	//字间距和字体系列使用缺省值.
		"黑体");					//字体名称.
	if(hFont == NULL) return NULL;
	else return hFont;
}		//消息处理函数声明.

void AlphaBmp(HDC hdc,HBITMAP hBitmap,int x,int y,short limit,HRGN hRgn,COLORREF col)
{
	BLENDFUNCTION blendfunc;
	HDC hMaskDc;
	HBITMAP hOld=NULL;
	BITMAP Bmp;
	blendfunc.BlendFlags=0;
	blendfunc.AlphaFormat=0;
	blendfunc.SourceConstantAlpha=(unsigned char)limit; 
	blendfunc.BlendOp=AC_SRC_OVER;
	GetObject(hBitmap,sizeof(BITMAP),(LPSTR)&Bmp);
	
	hMaskDc=CreateCompatibleDC(hdc);
	hOld=(HBITMAP)SelectObject(hMaskDc,hBitmap);
	
	
	HDC hTempDC=CreateCompatibleDC(hdc);
	HBITMAP hBitmap2=CreateCompatibleBitmap(hdc,Bmp.bmWidth,Bmp.bmHeight);
	hOld=(HBITMAP)SelectObject(hTempDC,hBitmap2);
	BitBlt(hTempDC,0,0,Bmp.bmWidth,Bmp.bmHeight,hdc,x,y,SRCCOPY);
	AlphaBlend          (hTempDC,0,0,Bmp.bmWidth,Bmp.bmHeight,hMaskDc,0,0,Bmp.bmWidth,Bmp.bmHeight,blendfunc);
	HRGN hR=CreateRectRgn(0,0,Bmp.bmWidth,Bmp.bmHeight);
	HBRUSH hbr=CreateSolidBrush(col);
	CombineRgn(hR,hR,hRgn,RGN_DIFF);
	FillRgn(hTempDC,hR,hbr);
	
	TransparentBlt(hdc,x,y,Bmp.bmWidth,Bmp.bmHeight,hTempDC,0,0,Bmp.bmWidth,Bmp.bmHeight,col);
	
	SelectObject(hTempDC,hOld);
	DeleteObject(hBitmap2);
	DeleteObject(hR);
	DeleteObject(hbr);
	DeleteDC(hTempDC);
	
	SelectObject(hMaskDc,hOld);
	DeleteDC(hMaskDc);
}
HRGN BitmapToRgn(HBITMAP hBitmap,COLORREF col)
{
	HDC hMemDc;
	HBITMAP hOld;
	COLORREF CPixel;
	HRGN rTemp,hGoal;
	BITMAP Bmp;
    GetObject(hBitmap,sizeof(BITMAP),&Bmp);
	hMemDc=CreateCompatibleDC(NULL);
	hOld=(HBITMAP)SelectObject(hMemDc,hBitmap);
	hGoal=CreateRectRgn(0,0,Bmp.bmWidth,Bmp.bmHeight);
	for(int x=0;x<=Bmp.bmWidth;x++)
		for(int y=0;y<=Bmp.bmHeight;y++)
		{
			CPixel=GetPixel(hMemDc,x,y);
			if(CPixel==col)
			{
                rTemp=CreateRectRgn(x,y,x+1,y+1);
				CombineRgn(hGoal,hGoal,rTemp,RGN_XOR);
				DeleteObject(rTemp);
			}
		}
		SelectObject(hMemDc,hOld);
		DeleteDC(hMemDc);
		return hGoal;
}