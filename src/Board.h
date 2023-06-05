#pragma once

#include "Layer.h"
#include "Player.h"
#include "Tile.h"

#include <memory>
#include <vector>

class Board
	: public Layer
{
public:
	using Tiles = std::vector<std::vector<Tile>>;
	Board();
	void render() override;

private:
	struct Direction {
		int x;
		int y;
	};

	/**
	 * @brief Detects wether the coordinates are in the middle of the board and who is the owner.
	 *
	 * @param y Y coordiante.
	 * @param x X coordinate.
	 * @return Ocupant of the tile.
	 */
	Player::Ocupant middleTilePlayer(int y, int x);
	void changeOponentTiles(Tiles &tiles, int x, int y);
	void setButtonColor(int x, int y);
	bool isTilePlayable(int x, int y);

	bool checkDirection(int x, int y, Direction dir);
	bool tileExists(int x, int y);

private:
	std::shared_ptr<Player> m_currentPlayer;
	std::vector<std::shared_ptr<Player>> m_players;
	Tiles m_tiles;
};
