#pragma once

#include "Player.h"
#include "imgui.h"

#include <memory>

class Tile
{
public:
	explicit Tile(std::shared_ptr<Player> ocupant, ImColor color);

	void select() { m_selected = true; }
	void unselect() { m_selected = false; }

	void setOcupant(std::shared_ptr<Player> ocupant) { m_ocupant = ocupant; }

	ImColor color() const { return m_color; }
	bool selected() const { return m_selected; }
	std::shared_ptr<Player> ocupant() const;
private:
	bool m_selected;
	ImColor m_color;
	std::shared_ptr<Player> m_ocupant;
};
