#include "Board.h"
#include "Player.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <algorithm>
#include <iostream>
#include <functional>

Board::Board()
{

	std::shared_ptr<Player> playerOne(new Player(1,
											  "Player1",
											  Player::Ocupant::White,
											  "../images/White.png", 80, 80));
	std::shared_ptr<Player> playerTwo(new Player(2,
											  "Player2",
											  Player::Ocupant::Black,
											  "../images/Black.png", 80, 80));
	m_players.push_back(playerOne);
	m_players.push_back(playerTwo);
	m_currentPlayer = m_players.front();

	std::shared_ptr<Player> defaultPlayer(new Player(-1, "Empty", Player::Ocupant::Empty, "", 0, 0));
	ImVec4 green = (ImVec4)ImColor::HSV(0.3f, 0.6f, 0.6f, 0.5f);
	ImVec4 orange = (ImVec4)ImColor::HSV(1.1f, 0.6f, 0.6f, 0.5f);

	m_tiles.resize(8);
	for (int y = 0; y < m_tiles.size(); y++) {
		std::vector<Tile> row;
		for (size_t x = 0; x < m_tiles.size(); x++) {
			ImVec4 color;
			if (y % 2) {
				color = x % 2 ? green : orange;
			}
			else {
				color = x % 2 ? orange : green;
			}

			Tile tile(defaultPlayer, color);
			if (middleTilePlayer(y, x) != Player::Ocupant::Empty) {
				tile.setOcupant(*std::find_if(m_players.begin(), m_players.end(), [&](std::shared_ptr<Player> p) {
					return p->ocupation() == middleTilePlayer(y, x);
				}));
			}
			row.emplace_back(tile);
		}
		m_tiles[y] = row;
	}
}

void Board::render()
{
	ImGui::Begin("Board");

	for (int y = 0; y < m_tiles.size(); y++) {
		for (int x = 0; x < m_tiles.front().size(); x++) {
			if (x > 0) {
				ImGui::SameLine(x*84+8);
			}
			int id = y * m_tiles.size() + x;
			ImGui::PushID(id);

			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)m_tiles[y][x].color());
			if (m_tiles[y][x].selected() || middleTilePlayer(y, x) != Player::Ocupant::Empty) {
				setButtonColor(x, y);
				int h = m_tiles[y][x].ocupant()->textureHeight();
				int w = m_tiles[y][x].ocupant()->textureWidth();
				ImGui::ImageButton((void *)(intptr_t)m_tiles[y][x].ocupant()->texture(), ImVec2(w-8, h-6));
				ImGui::PopStyleColor(2);
			}
			else {
				setButtonColor(x, y);
				if (ImGui::Button("", ImVec2(80, 80))) {
					if (isTilePlayable(x, y)) {
						m_tiles[y][x].select();
						m_tiles[y][x].setOcupant(m_currentPlayer);
						changeOponentTiles(m_tiles, x, y);
						m_currentPlayer = m_currentPlayer == m_players.front() ? m_players.back() : m_players.front();
					}
				}
				ImGui::PopStyleColor(2);
			}
			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}
	}
	ImGui::End();
}

Player::Ocupant Board::middleTilePlayer(int y, int x)
{
	if (y == 3 || y == 4) {
		if (x == y) {
			return Player::Ocupant::Black;
		}
		else if (x == 3 || x == 4){
			return Player::Ocupant::White;
		}
	}
	return Player::Ocupant::Empty;
}

void Board::changeOponentTiles(Tiles &tiles, int x, int y)
{
	auto checkTile = [this, &tiles] (int x, int y) -> bool {
		return tiles[y][x].ocupant()->ocupation() != m_currentPlayer->ocupation() && tiles[y][x].ocupant()->ocupation() != Player::Ocupant::Empty;
	};

	if (x > 0) {
		if (checkTile(x - 1, y)) {
			tiles[y][x - 1].setOcupant(m_currentPlayer);
			changeOponentTiles(tiles, x - 1, y);
		}
	}
	if (y > 0) {
		if (checkTile(x, y - 1)) {
			tiles[y - 1][x].setOcupant(m_currentPlayer);
			changeOponentTiles(tiles, x, y - 1);
		}
	}
	if (x < tiles.size() - 1) {
		if (checkTile(x + 1, y)) {
			tiles[y][x + 1].setOcupant(m_currentPlayer);
			changeOponentTiles(tiles, x + 1, y);
		}
	}
	if (y < tiles.size() - 1) {
		if (checkTile(x, y + 1)) {
			tiles[y + 1][x].setOcupant(m_currentPlayer);
			changeOponentTiles(tiles, x, y + 1);
		}
	}
}

void Board::setButtonColor(int x, int y)
{
	if (isTilePlayable(x, y)) {
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
	}
	else {
		
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)m_tiles[y][x].color());
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)m_tiles[y][x].color());
	}
}

bool Board::isTilePlayable(int x, int y)
{
	if (m_tiles[y][x].ocupant()->ocupation() == Player::Ocupant::Empty) {
		return (checkDirection(x - 1, y, {-1, 0})
				|| checkDirection(x + 1, y, {1, 0})
				|| checkDirection(x, y - 1, {0, -1})
				|| checkDirection(x, y + 1, {0, -1}));
	}
	return false;
}

bool Board::checkDirection(int x, int y, Direction dir) {
	if (tileExists(x, y)) {
		if (m_tiles[y][x].belongsToOponent(m_currentPlayer)) {
			return true;
		}
		else if (m_tiles[y][x].belongsToUs(m_currentPlayer)) {
			return false;
		}

		bool ret = checkDirection(x + dir.x, y + dir.y, dir);
		if (ret) {
			m_currentPlayer->increaseScore();
		}
		return ret;
	}
	return false;
};

bool Board::tileExists(int x, int y)
{
	return x > 0 && x < m_tiles.size() - 1 && y > 0 && y < m_tiles.size() - 1;
}
