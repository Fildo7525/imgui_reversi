#include "Board.h"
#include "Player.h"
#include "imgui.h"

Board::Board()
{
	std::shared_ptr<Player> defaultPlayer(new Player(-1, "Empty", Player::Ocupant::Empty, "", 0, 0));
	ImVec4 black = (ImVec4)ImColor::HSV(0.3f, 0.6f, 0.6f, 0.5f);
	ImVec4 white = (ImVec4)ImColor::HSV(0.9f, 0.6f, 0.6f, 0.5f);

	m_tiles.resize(8);
	for (int y = 0; y < m_tiles.size(); y++) {
		std::vector<Tile> row;
		for (size_t x = 0; x < m_tiles.size(); x++) {
			ImVec4 color;
			if (y % 2) {
				color = x % 2 ? black : white;
			}
			else {
				color = x % 2 ? white : black;
			}
			row.emplace_back(Tile(defaultPlayer, color));
		}
		m_tiles[y] = row;
	}

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
}

void SelectableColor(ImU32 color)
{
	ImVec2 p_min = ImGui::GetItemRectMin();
	ImVec2 p_max = ImGui::GetItemRectMax();
	ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, color);
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
			if (m_tiles[y][x].selected()) {
				int h = m_tiles[y][x].ocupant()->textureHeight();
				int w = m_tiles[y][x].ocupant()->textureWidth();
				ImGui::ImageButton((void *)(intptr_t)m_tiles[y][x].ocupant()->texture(), ImVec2(w-8, h-6));
			}
			else {
				// if (ImGui::Button("", m_tiles[y][x].selected(), ImGuiSelectableFlags_None, ImVec2(80, 80))) {
				if (ImGui::Button("", ImVec2(80, 80))) {
					if (m_tiles[y][x].ocupant()->ocupation() == Player::Ocupant::Empty) {
						m_tiles[y][x].select();
						m_tiles[y][x].setOcupant(m_currentPlayer);
					}
				}
			}
			// SelectableColor(m_tiles[y][x].color());
			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}
	}
	ImGui::End();
}
