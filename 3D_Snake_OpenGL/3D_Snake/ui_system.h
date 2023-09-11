#pragma once
class Shape;

class UiSystem
{
public:
	static void initialize();
	static void render(Shape&);
};