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
		cr->Text(" Erasing ", "Cant.
