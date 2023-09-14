#include "ui_system.h"
#include "Shape.h"

void UiSystem::initialize()
{
}

void UiSystem::render(Shape& shape)
{
	shape.draw();
}
