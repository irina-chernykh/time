#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

const int WINDOW_W = 400;
const int WINDOW_H = 400;
const int CENTRE_WINDOW_X = WINDOW_W / 2;
const int CENTRE_WINDOW_Y = WINDOW_H / 2;
const int AMOUNT_POINTS = 60;
const int SCALE = 180;

Vector2f HOUR_ARROW = { 90, 6 };
Vector2f MINUTE_ARROW = { 115, 4 };
Vector2f SECOND_ARROW = { 160, 2 };

struct Shapes {
	RectangleShape secArrow;
	RectangleShape minArrow;
	RectangleShape hourArrow;
	CircleShape centre;
	CircleShape point;
}watch;

void DrawPoints(RenderWindow & window, Shapes & watch, Vector2f(&coordinatePoints)[AMOUNT_POINTS])
{
	Vector2f coordinatePoint;
	for (int i = 0; i < AMOUNT_POINTS; i++) {
		coordinatePoint.x = CENTRE_WINDOW_X + SCALE * cos(i * 6 * float(M_PI) / 180);
		coordinatePoint.y = CENTRE_WINDOW_Y + SCALE * sin(i * 6 * float(M_PI) / 180);
		coordinatePoints[i] = { coordinatePoint.x, coordinatePoint.y };
	}

	for (int i = 0; i < AMOUNT_POINTS; i++) {
		if (i % 15 == 0) {
			watch.point.setRadius(6);
			watch.point.setOrigin(6 / 2, 6 / 2);
			watch.point.setFillColor(Color::White);
		}
		else {
			watch.point.setRadius(1);
			watch.point.setOrigin(1 / 2, 1 / 2);
			watch.point.setFillColor(Color::White);
		}
		watch.point.setPosition(coordinatePoints[i].x, coordinatePoints[i].y);
		window.draw(watch.point);
	}
}


void PositionOneArrow(RectangleShape &arrow, float height, float width) {
	arrow.setSize(Vector2f(height, width));
	arrow.setOrigin(Vector2f(0, 2));
	arrow.setPosition(float(CENTRE_WINDOW_X), float(CENTRE_WINDOW_Y));
}

void PositionArrows() {
	PositionOneArrow(watch.hourArrow, HOUR_ARROW.y, HOUR_ARROW.x);
	watch.hourArrow.setFillColor(Color::White);
	PositionOneArrow(watch.minArrow, MINUTE_ARROW.y, MINUTE_ARROW.x);
	watch.minArrow.setFillColor(Color::Black);
	PositionOneArrow(watch.secArrow, SECOND_ARROW.y, SECOND_ARROW.x);
	watch.secArrow.setFillColor(Color::Red);

	watch.centre.setRadius(9);
	watch.centre.setFillColor(Color::White);
	watch.centre.setPosition((CENTRE_WINDOW_X)-watch.centre.getRadius(), (CENTRE_WINDOW_Y)-watch.centre.getRadius());
}

void TimeIsOn(RenderWindow &window)
{
	PositionArrows();
	Vector2f coordinatePoints[AMOUNT_POINTS];
	SYSTEMTIME sysTime;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		GetSystemTime(&sysTime);
		watch.secArrow.setRotation(float(sysTime.wSecond * 360 / 60 - 180));
		watch.minArrow.setRotation(float(sysTime.wMinute * 360 / 60 + sysTime.wSecond * 6 / 60 - 180));
		watch.hourArrow.setRotation(float((sysTime.wHour + 3) * 30 + (sysTime.wMinute * 30 / 60) - 180));
		std::cout << sysTime.wHour + 3 << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "\n";

		window.clear(Color(100, 100, 100));
		window.draw(watch.hourArrow);
		window.draw(watch.minArrow);
		window.draw(watch.secArrow);
		window.draw(watch.centre);
		DrawPoints(window, watch, coordinatePoints);

		window.display();
	}
}

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 100;
	RenderWindow window(VideoMode(WINDOW_W, WINDOW_H), "Time", sf::Style::Default, settings);
	TimeIsOn(window);
	return 0;
}