#include "Tile.h"


Tile::Tile(std::shared_ptr<Player> ocupant, ImColor color)
	: m_ocupant(ocupant)
	, m_color(color)
	, m_selected(false)
{
	if (m_ocupant->ocupation() != Player::Ocupant::Empty) {
		m_selected = true;
	}
}

std::shared_ptr<Player> Tile::ocupant() const
{
	return m_ocupant;
}
