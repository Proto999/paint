#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "window.h"
#include "button.h"
#include "GUI.h"

enum UserEventType
{
	EVENT_CLOSE = 1,
	EVENT_UP,
	EVENT_DOWN,
	EVENT_LEFT,
	EVENT_RIGHT,
	EVENT_SET,
	EVENT_UNSET,
	EVENT_CLEAR,
	EVENT_CHMODE,
	EVENT_RED,
	EVENT_GREEN,
	EVENT_BLUE,
	EVENT_BLACK,
	EVENT_HELP,
	
};

std::vector<Point>	g_vector;
std::vector<int>    g_color_v;
int                 g_color_f = 0;
int					g_mode = 0;
char				g_text[11];
Point				g_coords(0,0);
Point				g_psize(15,15);
int					g_help = 0;

// родительское окно
class MainWindow : public Window
{
public:
	MainWindow() {}
	~MainWindow() {}

	void OnCreate();
	void OnDraw(Context *cr);
	bool OnMouseMove(const Point &position);
	bool OnLeftMouseButtonClick(const Point &position);
	bool OnRightMouseButtonClick(const Point &position);
	void OnNotify(Window *child, uint32_t type, const Point &position);
	bool OnKeyPress(uint64_t value);

private:
	RGB         m_color;
};

	void DrawLine(const Point& p1, const Point& p2, Context *cr)
	{
	cr->SetLineWidth(15);
	cr->Line(p1, p2);
	}

	void DrawRectangle(const Point& p1, const Point& p2, Context *cr)
	{
	cr->SetLineWidth(15);
    cr->Rectangle(p1, p2);
	}
	void DrawFillRectangle(const Point& p1, const Point& p2, Context *cr)
	{
	cr->SetLineWidth(15);
    cr->FillRectangle(p1, p2);
	}

void MainWindow::OnDraw(Context *cr)
{
	std::cout << "MainWindow::OnDraw()" << std::endl;
	Point size = GetSize();
	Point center(size.GetX()/2,size.GetY()/2);

	// зальем прямоугольник окна серым цветом
	cr->SetColor(RGB(0.8,0.8,0.8));
	cr->FillRectangle(Point(0,0), size);

	cr->SetColor(RGB(1,1,1));
	cr->FillRectangle(Point(0,0), Point(1000, 600));
	
	cr->SetColor(RGB(0,0,0));
	cr->SetLineWidth(3);
	cr->Line(Point(0,600), Point(1000, 600));
	if (g_mode == 0)
		cr->Text(" Moving ", "Cantarel", 17, Point(900, 625), 0x01);
	else if (g_mode == 1)
		cr->Text(" Printing ", "Cantarel", 17, Point(900, 625), 0x01);
	else if (g_mode == 2)
		cr->Text(" Erasing ", "Cantarel", 17, Point(900, 625), 0x01);
	else if (g_mode == 3)
		cr->Text(" Line ", "Cantarel", 17, Point(900, 625), 0x01);
	else if (g_mode == 4)
		cr->Text(" Rectangle ", "Cantarel", 17, Point(900, 625), 0x01);
	else if (g_mode == 5)
		cr->Text(" Fill Rectangle ", "Cantarel", 17, Point(900, 625), 0x01);
	sprintf(g_text, "%ld", g_vector.size());
	cr->Text(g_text, "Cantarel", 17, Point(900, 650), 0x01);

	for (int i = 0; i < g_vector.size(); i++)
	{		cr->SetColor(RGB(0 ,0 ,0));
		if (g_color_v[i] == 1)
			cr->SetColor(RGB(1, 0, 0));
		if (g_color_v[i] == 2)
			cr->SetColor(RGB(0, 1, 0));
		if (g_color_v[i] == 3)
			cr->SetColor(RGB(0, 0, 1));
		cr->FillRectangle(g_vector[i], g_psize);
	}

	if (g_help == 1)
	{
		cr->SetColor(RGB(0.8,0.8,0.8));
		cr->FillRectangle(Point(0,0), Point(1000, 600));
		cr->SetColor(RGB(0,0,0));
		cr->Text("УПРАВЛЕНИЕ:", "Cantarel", 20, Point(500, 50), 0x01);
		cr->Text("W-A-S-D / СТРЕЛКИ / МЫШЬ - ПЕРЕМЕЩЕНИЕ КУРСОРА", "Cantarel", 20, Point(500, 100), 0x01);
		cr->Text("E - СМЕНА РЕЖИМА УПРАВЛЕНИЯ (РИСОВАНИЕ, СТИРАНИЕ, ПЕРЕМЕЩЕНИЕ)", "Cantarel", 20, Point(500, 150), 0x01);
		cr->Text("ЛКМ - ПОСТАВИТЬ ТОЧКУ", "Cantarel", 20, Point(500, 200), 0x01);
		cr->Text("ПКМ - УБРАТЬ ТОЧКУ", "Cantarel", 20, Point(500, 250), 0x01);
		cr->Text("R - ВЫБРАТЬ КРАСНЫЙ ЦВЕТ", "Cantarel", 20, Point(500, 300), 0x01);
		cr->Text("G - ВЫБРАТЬ ЗЕЛЕНЫЙ ЦВЕТ", "Cantarel", 20, Point(500, 350), 0x01);
		cr->Text("B - ВЫБРАТЬ СИНИЙ ЦВЕТ", "Cantarel", 20, Point(500, 400), 0x01);
		cr->Text("N - ВЫБРАТЬ ЧЕРНЫЙ ЦВЕТ", "Cantarel", 20, Point(500, 450), 0x01);
		cr->Text("С - ОЧИСТИТЬ ХОЛСТ", "Cantarel", 20, Point(500, 500), 0x01);
		cr->Text("Q - ВЫХОД", "Cantarel", 20, Point(500, 550), 0x01);
	}

	cr->SetColor(RGB(0.5,0.1,0.9));
	cr->FillRectangle(g_coords, g_psize);

	if (g_mode == 3) //LINE
			{
				{	
			Point p1 = g_vector[g_vector.size() - 2];  // Последняя добавленная точка
            Point p2 = g_vector[g_vector.size() - 1];  // Предпоследняя добавленная точка
		for (int i = 0; i < g_vector.size(); i++)
	{		cr->SetColor(RGB(0 ,0 ,0));
		if (g_color_v[i] == 1)
			cr->SetColor(RGB(1, 0, 0));
		if (g_color_v[i] == 2)
			cr->SetColor(RGB(0, 1, 0));
		if (g_color_v[i] == 3)
			cr->SetColor(RGB(0, 0, 1));
	}
			DrawLine(p1, p2, cr);
				}
			ReDraw();
			}
	if (g_mode == 4) //Rectangle
			{
				{
			Point p1 = g_vector[g_vector.size() - 2];  // Последняя добавленная точка
            Point p2 = g_vector[g_vector.size() - 1];  // Предпоследняя добавленная точка
		for (int i = 0; i < g_vector.size(); i++)
	{		cr->SetColor(RGB(0 ,0 ,0));
		if (g_color_v[i] == 1)
			cr->SetColor(RGB(1, 0, 0));
		if (g_color_v[i] == 2)
			cr->SetColor(RGB(0, 1, 0));
		if (g_color_v[i] == 3)
			cr->SetColor(RGB(0, 0, 1));
	}
			DrawRectangle(p1, p2, cr);
				}
			ReDraw();
			}
	if (g_mode == 5) //Fill Rectangle
			{
				{
			Point p1 = g_vector[g_vector.size() - 2];  // Последняя добавленная точка
            Point p2 = g_vector[g_vector.size() - 1];  // Предпоследняя добавленная точка
		for (int i = 0; i < g_vector.size(); i++)
	{		cr->SetColor(RGB(0 ,0 ,0));
		if (g_color_v[i] == 1)
			cr->SetColor(RGB(1, 0, 0));
		if (g_color_v[i] == 2)
			cr->SetColor(RGB(0, 1, 0));
		if (g_color_v[i] == 3)
			cr->SetColor(RGB(0, 0, 1));
	}
			DrawFillRectangle(p1, p2, cr);
				}
			ReDraw();
			}		
}

void MainWindow::OnCreate()
{
	std::cout << "MainWindow::OnCreate()" << std::endl;

	AddChild(new Button(" UP ",EVENT_UP), Point(450,600), Point(100,50));
	AddChild(new Button(" DOWN ",EVENT_DOWN), Point(450,650), Point(100,50));
	AddChild(new Button(" <- ",EVENT_LEFT), Point(350,650), Point(100,50));
	AddChild(new Button(" -> ",EVENT_RIGHT), Point(550,650), Point(100,50));
	AddChild(new Button(" SET ",EVENT_SET), Point(150,650), Point(50,50));
	AddChild(new Button("UNSET",EVENT_UNSET), Point(200,650), Point(50,50));
	AddChild(new Button(" CLEAR ",EVENT_CLEAR), Point(150,600), Point(100,50));
	AddChild(new Button(" MODE ",EVENT_CHMODE), Point(750,600), Point(100,50));
	AddChild(new Button(" R ",EVENT_RED), Point(750,650), Point(25,50));
	AddChild(new Button(" G ",EVENT_GREEN), Point(775,650), Point(25,50));
	AddChild(new Button(" B ",EVENT_BLUE), Point(800,650), Point(25,50));
	AddChild(new Button(" BL ",EVENT_BLACK), Point(825,650), Point(25,50));
	AddChild(new Button(" HELP ",EVENT_HELP), Point(50,600), Point(100,50));
}

void	SetPoint(void)
{
	g_color_v.push_back(g_color_f);
	g_vector.push_back(g_coords);
}


void	UnsetPoint(void)
{
	for (int i = 0; i < g_vector.size(); i++)
	{
		if (abs(g_vector[i].GetX() - g_coords.GetX()) < 10 && abs(g_vector[i].GetY() - g_coords.GetY()) < 10)
		{
			g_color_v.erase(g_color_v.cbegin() + i);
			g_vector.erase(g_vector.cbegin() + i);
		}	 
	}
}


void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position)
{

	std::cout << "MainWindow::OnNotify()" << std::endl;
	if(type == EVENT_CLOSE)
	{
		DeleteMe();
	}
	else if(type == EVENT_UP && g_coords.GetY() - g_psize.GetY() >= 0)
	{
		if (g_mode == 1)
			SetPoint();
		if (g_mode == 2)
			UnsetPoint();
		g_coords.SetY(g_coords.GetY() - g_psize.GetY());
		ReDraw();
	}
	else if(type == EVENT_DOWN && g_coords.GetY() + g_psize.GetY() <= (600 - g_psize.GetY()))
	{
		if (g_mode == 1)
			SetPoint();
		if (g_mode == 2)
			UnsetPoint();
		g_coords.SetY(g_coords.GetY() + g_psize.GetY());
		ReDraw();
	}
	else if(type == EVENT_LEFT && g_coords.GetX() - g_psize.GetX() >= 0)
	{
		if (g_mode == 1)
			SetPoint();
		if (g_mode == 2)
			UnsetPoint();
		g_coords.SetX(g_coords.GetX() - g_psize.GetX());
		ReDraw();
	}
	else if(type == EVENT_RIGHT && g_coords.GetX() + g_psize.GetX() <= (1000 - g_psize.GetX()))
	{
		if (g_mode == 1)
			SetPoint();
		if (g_mode == 2)
			UnsetPoint();
		g_coords.SetX(g_coords.GetX() + g_psize.GetX());
		ReDraw();
	}
	else if(type == EVENT_SET)
	{
		SetPoint();
		ReDraw();
	}
	else if(type == EVENT_UNSET)
	{
		UnsetPoint();
		ReDraw();
	}
	else if(type == EVENT_CLEAR)
	{
		g_vector.clear();
		ReDraw();
	}
	else if(type == EVENT_CHMODE)
	{
		g_mode++;
		if (g_mode == 6)
			g_mode = 0;
		ReDraw();
	}
	else if(type == EVENT_RED)
	{
		g_color_f = 1;
		ReDraw();
	}
	else if(type == EVENT_GREEN)
	{
		g_color_f = 2;
		ReDraw();
	}
	else if(type == EVENT_BLUE)
	{
		g_color_f = 3;
		ReDraw();
	}
	else if(type == EVENT_BLACK)
	{
		g_color_f = 0;
		ReDraw();
	}
	else if(type == EVENT_HELP)
	{
		g_help = !g_help;
		ReDraw();
	}	
	
}	

bool MainWindow::OnMouseMove(const Point &position)
{
	// std::cout << "MainWindow::OnMouseMove(" << position.GetX() << "," << position.GetY() << ")" << std::endl;
	if (g_mode == 1)
		SetPoint();
	if (g_mode == 2)
		UnsetPoint();
	if (position.GetX() <= (1000 - g_psize.GetX()) && (position.GetY() <= (600 - g_psize.GetY())))
	{
		g_coords.SetX(position.GetX());
		g_coords.SetY(position.GetY());
	}
	ReDraw();
	return true;
}

bool MainWindow::OnLeftMouseButtonClick(const Point &position)
{
	SetPoint();
	ReDraw();
	return true;
}

bool MainWindow::OnRightMouseButtonClick(const Point &position)
{
    UnsetPoint();
	ReDraw();
	return true;
}

bool MainWindow::OnKeyPress(uint64_t keyval)
{
	std::cout << "MainWindow::OnKeyPress(" << keyval << ")" << std::endl;

	if (keyval == 'w' || keyval == 65362)
	{
		if(g_coords.GetY() - g_psize.GetY() >= 0)
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetY(g_coords.GetY() - g_psize.GetY());
		}
	}
	if (keyval == 'a' || keyval == 65361)
	{
		if(g_coords.GetX() - g_psize.GetX() >= 0)
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetX(g_coords.GetX() - g_psize.GetX());
		}
	}
	if (keyval == 's' || keyval == 65364)
	{
		if(g_coords.GetY() + g_psize.GetY() <= (600 - g_psize.GetY()))
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetY(g_coords.GetY() + g_psize.GetY());
		}
	}
	if (keyval == 'd' || keyval == 65363)
	{
		if(g_coords.GetX() + g_psize.GetX() <= (1000 - g_psize.GetX()))
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetX(g_coords.GetX() + g_psize.GetX());
		}
	}
	if (keyval == 'c')
	{
		g_vector.clear();
	}
	if (keyval == 'q')
	{
		DeleteMe();
		return true;
	}
	if (keyval == 'e')
	{
		g_mode++;
		if (g_mode == 6)
			g_mode = 0;
	}
	if (keyval == 'n')
	{
		g_color_f = 0;
	}
	if (keyval == 'r')
	{
		g_color_f = 1;
	}
	if (keyval == 'g')
	{
		g_color_f = 2;
	}
	if (keyval == 'b')
	{
		g_color_f = 3;
	}
	ReDraw();
	return true;
}


// функция main
int main(int argc, char **argv)
{
	MainWindow *pWindow = new MainWindow;

	int res = Run(argc, argv, pWindow, 1000, 700);

	delete pWindow;

	return res;
}
