#include <iostream>
#include "CAMApplication/CAMApplication.h"

int main()
{
	CAMageddon::CAMApplication app;
	app.Init();
	app.Run();

	return EXIT_SUCCESS;
}