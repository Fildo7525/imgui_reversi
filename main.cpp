#include "Application.h"

#include "Board.h"
#include "Status.h"

int main(int argc, char *argv[])
{
	std::shared_ptr<Application> app = Application::create({
		"BoardGame",
		1150,
		710,
		true,
		false,
		true
	});

	app->addLayer(std::make_shared<Board>(8));
	app->addLayer(std::make_shared<Status>(app->getLayer<Board>("Board")));

	app->run();
	return 0;
}

