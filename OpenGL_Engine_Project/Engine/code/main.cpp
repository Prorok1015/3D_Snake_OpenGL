#include "application.h"

int main()
{
	app::Application::init_libraris();
	app::Application myApp;
	return myApp.Run();
}