#include "Application.h"

#include "Board.h"
#include "Status.h"

int main(int argc, char *argv[])
{
	Application *app = new Application({
			"BoardGame",
			1150,
			710,
			false,
			false,
			"/usr/share/fonts/BS/BitstromWeraNerdFont-Regular.ttf"
		});

	app->addLayer(std::make_shared<Board>(8));

	std::shared_ptr<Board> board = std::static_pointer_cast<Board>(app->getLayer("Board"));
	std::shared_ptr<Status> status(new Status(board));
	app->addLayer(status);

	app->run();
	return 0;
}

