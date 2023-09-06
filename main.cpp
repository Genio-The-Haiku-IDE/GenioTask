#include "Application.h"


int
main(void)
{
	MainApp *app = new MainApp();
	app->Run();
	delete app;
	return 0;
}
