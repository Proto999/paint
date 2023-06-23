#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cairo/cairo.h>  // Включаем библиотеку Cairo

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
	EVENT_HELP
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

void drawLine(Context *cr, const Point &start, const Point &end)
{
	cr->MoveTo(start);
	cr->LineTo(end);
	cr->Stroke();
}

void drawRectangle(Context *cr, const Point &topLeft, const Point &bottomRight)
{
	double width = bottomRight.GetX() - topLeft.GetX();
	double height = bottomRight.GetY() - topLeft.GetY();
	cr->Rectangle(topLeft, width, height);
	cr->Stroke();
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
		else
		cr->Text(" Erasing ", "Cantarel", 17, Point(900, 625), 0x01);

	// Рисуем прямые
	if (g_mode == 0 || g_mode == 1) {
		for (size_t i = 0; i < g_vector.size() - 1; i += 2) {
			cr->SetLineWidth(1);
			cr->SetColor(RGB(g_color_v[i], g_color_v[i + 1], g_color_v[i + 2]));
			drawLine(cr, g_vector[i], g_vector[i + 1]);
		}
	}

	// Рисуем прямоугольники
	if (g_mode == 0 || g_mode == 2) {
		cr->SetLineWidth(1);
		cr->SetColor(RGB(0, 0, 0));
		for (size_t i = 0; i < g_vector.size() - 1; i += 2) {
			Point topLeft = g_vector[i];
			Point bottomRight = g_vector[i + 1];
			drawRectangle(cr, topLeft, bottomRight);
		}
	}
}

bool MainWindow::OnMouseMove(const Point &position)
{
	g_coords = position;
	NotifyChildren(EVENT_UP, g_coords);
	NotifyChildren(EVENT_DOWN, g_coords);
	NotifyChildren(EVENT_LEFT, g_coords);
	NotifyChildren(EVENT_RIGHT, g_coords);

	return true;
}

bool MainWindow::OnLeftMouseButtonClick(const Point &position)
{
	g_coords = position;
	NotifyChildren(EVENT_SET, g_coords);
	return true;
}

bool MainWindow::OnRightMouseButtonClick(const Point &position)
{
	g_coords = position;
	NotifyChildren(EVENT_UNSET, g_coords);
	return true;
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position)
{
	if (type == EVENT_CLOSE)
		Close();
	else if (type == EVENT_RED)
		m_color = RGB(1, 0, 0);
	else if (type == EVENT_GREEN)
		m_color = RGB(0, 1, 0);
	else if (type == EVENT_BLUE)
		m_color = RGB(0, 0, 1);
	else if (type == EVENT_BLACK)
		m_color = RGB(0, 0, 0);
	else if (type == EVENT_CHMODE) {
		g_mode++;
		if (g_mode > 2)
			g_mode = 0;
	}
	else if (type == EVENT_UP)
	{
		if (g_help == 1)
			ShowHelp(0);
		g_help = 0;
		g_psize.SetY(g_psize.GetY() + 1);
	}
	else if (type == EVENT_DOWN)
	{
		if (g_help == 1)
			ShowHelp(0);
		g_help = 0;
		g_psize.SetY(g_psize.GetY() - 1);
		if (g_psize.GetY() < 1)
			g_psize.SetY(1);
	}
	else if (type == EVENT_LEFT)
	{
		if (g_help == 1)
			ShowHelp(0);
		g_help = 0;
		g_psize.SetX(g_psize.GetX() - 1);
		if (g_psize.GetX() < 1)
			g_psize.SetX(1);
	}
	else if (type == EVENT_RIGHT)
	{
		if (g_help == 1)
			ShowHelp(0);
		g_help = 0;
		g_psize.SetX(g_psize.GetX() + 1);
	}
	else if (type == EVENT_SET)
	{
		if (g_help == 1)
			ShowHelp(0);
		g_help = 0;
		if (g_vector.size() % 2 == 0)
		{
			g_vector.push_back(position);
			g_color_v.push_back(m_color.GetR());
			g_color_v.push_back(m_color.GetG());
			g_color_v.push_back(m_color.GetB());
		}
		else
		{
			g_vector[g_vector.size() - 1] = position;
		}
	}
	else if (type == EVENT_UNSET)
	{
		if (g_help == 1)
			ShowHelp(0);
		g_help = 0;
		size_t closestIndex = FindClosestPointIndex(g_vector, position);
		if (closestIndex != -1)
		{
			g_vector.erase(g_vector.begin() + closestIndex, g_vector.begin() + closestIndex + 2);
			g_color_v.erase(g_color_v.begin() + closestIndex * 3, g_color_v.begin() + closestIndex * 3 + 3);
		}
	}
}

