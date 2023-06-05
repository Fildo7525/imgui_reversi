#include "Board.h"
#include "Player.h"
#include "imgui.h"

#include <algorithm>

Board::Board()
{

	std::shared_ptr<Player> playerOne(new Player(1,
											  "Player1",
											  Player::Ocupant::White,
											  "../images/White.png", 80, 80));
	std::shared_ptr<Player> playerTwo(new Player(2,
											  "Player1",
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
				int h = m_tiles[y][x].ocupant()->textureHeight();
				int w = m_tiles[y][x].ocupant()->textureWidth();
				ImGui::ImageButton((void *)(intptr_t)m_tiles[y][x].ocupant()->texture(), ImVec2(w-8, h-6));
			}
			else {
				if (ImGui::Button("", ImVec2(80, 80))) {
					if (m_tiles[y][x].ocupant()->ocupation() == Player::Ocupant::Empty) {
						m_tiles[y][x].select();
						m_tiles[y][x].setOcupant(m_currentPlayer);
					}
				}
			}
			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}
	}
	ImGui::End();
}

/**
 * @brief Detects wether the coordinates are in the middle of the board and who is the owner.
 *
 * @param y Y coordiante.
 * @param x X coordinate.
 * @return Ocupant of the tile.
 */
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
