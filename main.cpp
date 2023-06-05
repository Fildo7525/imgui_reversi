#include "Application.h"

#include "Board.h"
#include "Demo.h"

int main(int argc, char *argv[])
{
	Application *app = new Application({
			"BoardGame",
			1150,
			710,
			false,
			false,
			"/usr/share/fonts/BS/BitstromWeraNerdFont-Regular.ttf"
		},
		Application::RenderBackend::WaitEvents
	);

	app->addLayer(std::make_shared<Board>());
	
	app->run();
	return 0;
}

