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
	Board();
	void render() override;

private:
	Player::Ocupant middleTilePlayer(int y, int x);

private:
	std::shared_ptr<Player> m_currentPlayer;
	std::vector<std::shared_ptr<Player>> m_players;
	std::vector<std::vector<Tile>> m_tiles;
};
