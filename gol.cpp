// Game of life
// 2024 M. Gerloff


#include "gol.hpp"
#include "assets.hpp"

using namespace blit;

short world[2][320][240];

int screen_width = 160;
int screen_height = 120;

int cells = 2340;

int menu_setting[5]{0,0,0,0,0};
int menu_item = 4;

int state = 0;

Timer dt;

void search_life()
{
	for (int y=0; y<screen_height; y++)			//copy world
		for (int x=0; x<screen_width; x++)
			world[1][x][y] = world[0][x][y];

	int dir[9]{-1,0,1,1,1,0,-1,-1,-1};				//test for life around cell
	
	for (int y=0; y<screen_height; y++)
	{
		for (int x=0; x<screen_width; x++)
		{
			short count = 0;
		
			for (int i=0; i<8; i++)
			{
				int xx = x + dir[i + 1];
				int yy = y + dir[7 - i];
				
				if (xx < 0)							//out of screen
					xx = screen_width - 1;
				else if (xx == screen_width)
					xx = 0;
				
				if (yy < 0)
					yy = screen_height - 1;
				else if (yy == screen_height)
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

void render_sim() 
{
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    screen.alpha = 255;
    screen.mask = nullptr;

    screen.pen = Pen(0, 255, 0);
	for (int y=0; y<screen_height; y++)
		for (int x=0; x<screen_width; x++)
			if (world[0][x][y] > 0)
                screen.pixel(Point(x, y));
}

void render_timer(Timer &t)
{
    search_life();
}


void start()
{
	if (menu_setting[0] == 0)
	{
		set_screen_mode(ScreenMode::lores);
		screen_width = 160;
		screen_height =	120;
	}
	else
	{
		set_screen_mode(ScreenMode::hires);
		screen_width = 320;
		screen_height = 240;
	}

	if (menu_setting[1] > 0)
	{
//		int sim_time[3]{50,75,100};
		dt.init(render_timer, (250 - (menu_setting[1] * 75)), -1);
		dt.start();
	}
	
	if (menu_setting[2] == 0)
	{
		for (int y=0; y<screen_height; y++)			//copy world
			for (int x=0; x<screen_width; x++)
				world[0][x][y] = 0;
		for (int i=0; i<cells; i++)
			world[0][((screen_width-screen_height) / 2) + 10 + rand () %(screen_height - 20)][10 + rand() %(screen_height - 20)] = 1;
	}
}

void update_menu()
{
    int menu_max[5]{1,3,1,0,0};

    if (buttons.released & Button::DPAD_UP && menu_item>0)
    {
        menu_item--;
        channels[5].trigger_attack();
    }
    else if (buttons.released & Button::DPAD_DOWN && menu_item<4)
    {
        menu_item++;
        channels[5].trigger_attack();
    }

	if (menu_item == 3 && menu_setting[2] == 0)
	{
		if (cells > 0 && buttons & Button::DPAD_LEFT)
			cells-=10;
		else if (cells <10000 && buttons & Button::DPAD_RIGHT)
			cells+=10;
	}
    else if (buttons.released & Button::DPAD_LEFT)
    {
        menu_setting[menu_item]--;
        if (menu_setting[menu_item]<0)
            menu_setting[menu_item]=menu_max[menu_item];
        if (menu_item<3)
            channels[6].trigger_attack();
    }
    else if (buttons.released & Button::DPAD_RIGHT)
    {
        menu_setting[menu_item]++;
        if (menu_setting[menu_item]>menu_max[menu_item])
            menu_setting[menu_item]=0;
        if (menu_item<3)
            channels[6].trigger_attack();
    }
    else if (buttons.released & Button::A && menu_item==4)
    {
        channels[6].trigger_attack();
        state = 1;
        start();
    }
}

void render_menu()
{
    std::string txt={"conway's game of life"};
    std::string menu[4]={"resolution","timer","setup","cells"};
    std::string select[4][4]={{"low","high"},{"manuel","slow","standard","fast"},{"random","draw"},{""}};

	select[3][0] = std::to_string(cells);

    screen.pen = Pen(0,0,0);
    screen.clear();
	
	screen.pen = Pen(0,255,0);
	screen.alpha = 100;
	screen.text(txt,minimal_font, Point(29,25), true, TextAlign::top_left);
	screen.alpha = 255;
	screen.text(txt,minimal_font, Point(28,25), true, TextAlign::top_left);
	
    for (int i=0;i<5;i++)
    {
		menu_item == i? screen.alpha = 255: screen.alpha = 100;
		if (i == 4)
		{
			screen.pen = Pen(255,255,255);
        	screen.text("start simulation", minimal_font, Point(37,46+(i*11)), true, TextAlign::top_left);
		}
		else if (i == 3 && menu_setting[2] == 1)
		{
			screen.pen = Pen(255,255,255);	
        	screen.text("start editor", minimal_font, Point(47,42+(i*11)), true, TextAlign::top_left);
		}
		else
		{
			screen.pen = Pen(0,255,0);
		    screen.text(menu[i], minimal_font, Point(37,42+(i*11)), true, TextAlign::top_left);  
			screen.pen = Pen(255,255,255);
		    screen.text(select[i][menu_setting[i]], minimal_font, Point(123,42+(i*11)), true, TextAlign::top_right); 
		}
    }
}

//
// init()
//

void init() 
{
    set_screen_mode(ScreenMode::lores);

    channels[5].waveforms = Waveform::SINE; //menu tick
    channels[5].volume = 7500;
    channels[5].frequency = 600;
    channels[5].attack_ms = 5;
    channels[5].decay_ms = 80;
    channels[5].sustain = 0;
    channels[5].release_ms = 5;

    channels[6].waveforms = Waveform::SAW; //menu switch
    channels[6].volume = 6000;
    channels[6].frequency = 100;
    channels[6].attack_ms = 5;
    channels[6].decay_ms = 200;
    channels[6].sustain = 0;
    channels[6].release_ms = 5;
}

//
// render(time)
//

void render(uint32_t time) 
{
    screen.pen = Pen(255,255,255);
    screen.alpha = 255;
    screen.mask = nullptr;

    if (state == 0) 
        render_menu();
	else if (state == 1)
		render_sim();	
}

//
// update(time)
//

void update(uint32_t time) 
{
    if (state==0)
		update_menu();
    else
		if (menu_setting[1] == 0 && buttons.pressed & blit::Button::A)
			search_life();
        if (buttons & blit::Button::B)
		{
			set_screen_mode(ScreenMode::lores);
            state=0;
		}
}

