#include "gol.hpp"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120
#define CELL 2500

using namespace blit;

short world[2][SCREEN_WIDTH][SCREEN_HEIGHT];

Timer dt;

void search_life()
{
	for (int y=0; y<SCREEN_HEIGHT; y++)			//copy world
		for (int x=0; x<SCREEN_WIDTH; x++)
			world[1][x][y] = world[0][x][y];

	int dir[9]{-1,0,1,1,1,0,-1,-1,-1};				//test for life around cell
	
	for (int y=0; y<SCREEN_HEIGHT; y++)
	{
		for (int x=0; x<SCREEN_WIDTH; x++)
		{
			short count = 0;
		
			for (int i=0; i<8; i++)
			{
				int xx = x + dir[i + 1];
				int yy = y + dir[7 - i];
				
				if (xx < 0)							//out of screen
					xx = SCREEN_WIDTH - 1;
				else if (xx == SCREEN_WIDTH)
					xx = 0;
				
				if (yy < 0)
					yy = SCREEN_HEIGHT - 1;
				else if (yy == SCREEN_HEIGHT)
					yy = 0;
				
				if (world[1][xx][yy] > 0)
					count++;
			}
				
			if (world[1][x][y] == 0)
			{
				if (count == 3)
					world[0][x][y] = 1;				//new life
			}
//			else if (count != 1 && count != 3)		//13-3
			else if (count < 2 || count > 3)		//23-3 Conway standard
//			else if (count != 3)					//3-3
//			else if (count < 3 || count > 4)		//34-3
//			else if (count != 3 && count != 5)		//35-3
			{
				world[0][x][y] = 0;
			}
		}
	}
}

void render_timer(Timer &t)
{
    search_life();
}

void reder_menu()
{
	
}

// init()

void init() 
{
    SCREEN_WIDTH == 160? set_screen_mode(ScreenMode::lores):set_screen_mode(ScreenMode::hires);
	
//	for (int y=10; y<SCREEN_HEIGHT - 20; y++)
//		for (int x=((SCREEN_WIDTH-SCREEN_HEIGHT) / 2) + 10; x<((SCREEN_WIDTH-SCREEN_HEIGHT) / 2) + (SCREEN_HEIGHT - 10); x++)
//			if (rand() %5 == 0)
//				world[0][x][y] = 1;

	for (int i=0; i<CELL; i++)
		world[0][((SCREEN_WIDTH-SCREEN_HEIGHT) / 2) + 10 + rand () %(SCREEN_HEIGHT - 20)][10 + rand() %(SCREEN_HEIGHT - 20)] = 1;

//	for (int i=0; i<CELL; i++)
//		world[0][70 + rand () %20][50 + rand() %20] = 1;

/*
	short unit[6][5][7] = {
		{{1,1,1,0,0,0,0},
		{1,0,0,0,0,0,0},
		{0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0}},
		{{0,1,1,1,1,0,0},
		{1,0,0,0,1,0,0},
		{0,0,0,0,1,0,0},
		{1,0,0,1,0,0,0},
		{0,0,0,0,0,0,0}},
		{{0,1,1,1,1,1,0},
		{1,0,0,0,0,1,0},
		{0,0,0,0,0,1,0},
		{1,0,0,0,1,0,0},
		{0,0,1,0,0,0,0}},
		{{0,0,0,0,0,0,0},
		{1,1,1,0,1,1,1},
		{1,0,0,0,0,0,1},
		{1,1,1,0,1,1,1},
		{0,0,0,0,0,0,0}},
		{{1,1,0,0,0,0,0},
		{0,1,1,0,0,0,0},
		{0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0}},
		{{0,1,1,1,1,1,1},
		{1,0,0,0,0,0,1},
		{0,0,0,0,0,0,1},
		{1,0,0,0,0,1,0},
		{0,0,1,1,0,0,0}}};	

	for (int y=0; y<5; y++)
		for (int x=0; x<7; x++)
			if (unit[3][y][x] > 0)
				world[0][x + (SCREEN_WIDTH / 2)][y + (SCREEN_HEIGHT / 2)] = 1;
*/


/*	
	for (int y=0; y<5; y++)
		for (int x=0; x<7; x++)
		{
			if (unit[3][y][x] > 0)
				world[0][12 + y][x] = 1;
			if (unit[2][y][x] > 0)
				world[0][x][y + 60] = 1;
			if (unit[1][x][y] > 0)
				world[0][SCREEN_WIDTH - y][x + 10] = 1;
			if (unit[0][x][y] > 0)
				world[0][x + 80][y] = 1;
		}
*/

	dt.init(render_timer, 75, -1);
	dt.start();
}


// render(time)

void render(uint32_t time) 
{

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    screen.alpha = 255;
    screen.mask = nullptr;

    screen.pen = Pen(0, 255, 0);
	for (int y=0; y<SCREEN_HEIGHT; y++)
		for (int x=0; x<SCREEN_WIDTH; x++)
			if (world[0][x][y] > 0)
                screen.pixel(Point(x, y));
			
    // screen.text("Conway - Game of life", minimal_font, Point(1, 1));
}


// update(time)

void update(uint32_t time) 
{
//	if (buttons.released & Button::A)
//		search_life();
}
