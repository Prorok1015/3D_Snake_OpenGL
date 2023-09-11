#include "ui_system.h"
#include "Window.h"
#include "Mesh.h"
#include "enums.h"
#include "Shape.h"

void UiSystem::initialize()
{
}

void UiSystem::render(Shape& shape)
{
	shape.draw();
}
