#include "Board.h"
#include "Player.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>

Board::Board()
	: m_gameOver(false)
{
	std::shared_ptr<Player> playerOne(new Player(1,
											  "User",
											  Player::Ocupant::White,
											  "../images/White.png", 80, 80));
	std::shared_ptr<Player> playerTwo(new Player(2,
											  "PC",
											  Player::Ocupant::Black,
											  "../images/Black.png", 80, 80));
	m_players.push_back(playerOne);
	m_players.push_back(playerTwo);
	m_currentPlayer = m_players.front();

	initTiles();
}

void Board::render()
{
	if (m_gameOver) {
		ImGui::Begin("Game Over");
		int user = 0;
		int pc = 0;
		for(auto &line : m_tiles) {
			for(auto &tile : line) {
				if (tile.ocupant() == m_players.front()) {
					user++;
				} else if (tile.ocupant() == m_players.back()) {
					pc++;
				}
			}
		}
		auto winner = user > pc ? m_players.front() : m_players.back();
		auto winnerScore = user > pc ? user : pc;
		auto loser = user > pc ? m_players.back() : m_players.front();
		auto loserScore = user > pc ? pc : user;
		std::stringstream ss;
		ss << "Game Over the winner is " << winner->name() << " with " << winnerScore << " points.\n"
		   << "The loser is " << loser->name() << " with " << loserScore << " points.";
		ImGui::Text("%s", ss.str().c_str());
		if (ImGui::Button("Play again")) {
			m_gameOver = false;
			initTiles();
		}
		ImGui::End();
	}

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
						m_tiles[y][x].setOcupant(m_currentPlayer);
						m_currentPlayer->increaseScoreBy(changeOponentTiles(x, y));
						m_currentPlayer = m_currentPlayer == m_players.front() ? m_players.back() : m_players.front();
						findBestTileCapture();
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

void Board::initTiles()
{
	std::shared_ptr<Player> defaultPlayer(new Player(-1, "Empty", Player::Ocupant::Empty, "", 0, 0));

	ImVec4 green = (ImVec4)ImColor::HSV(0.3f, 0.6f, 0.6f, 0.5f);
	ImVec4 orange = (ImVec4)ImColor::HSV(1.1f, 0.6f, 0.6f, 0.5f);

	m_tiles.clear();
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

Player::Ocupant Board::middleTilePlayer(int y, int x)
{
	if (y == m_tiles.size()/2 - 1 || y == m_tiles.size()/2) {
		if (x == y) {
			return Player::Ocupant::Black;
		}
		else if (x == m_tiles.size()/2 - 1 || x == m_tiles.size()/2) {
			return Player::Ocupant::White;
		}
	}
	return Player::Ocupant::Empty;
}

int Board::changeOponentTiles(int x, int y)
{
	int score = 0;

	auto changeTiles = [this, &score] (int x, int y, Direction dir) {
		score = checkDirection(x, y, {x, y}, dir);
		if (score) {
			int tmpx = x + dir.x;
			int tmpy = y + dir.y;
			while (m_tiles[tmpy][tmpx].ocupant() != m_currentPlayer) {
				m_tiles[tmpy][tmpx].setOcupant(m_currentPlayer);
				tmpx += dir.x;
				tmpy += dir.y;
			}
		}
	};

	changeTiles(x, y, {-1, 0});
	changeTiles(x, y, {1, 0});
	changeTiles(x, y, {0, -1});
	changeTiles(x, y, {0, 1});
	m_currentPlayer->increaseScoreBy(score);

	return score;
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

int Board::isTilePlayable(int x, int y)
{
	if (m_tiles[y][x].ocupant()->ocupation() == Player::Ocupant::Empty) {
		int score = checkDirection(x, y, {x, y}, {-1, 0})
				+ checkDirection(x, y, {x, y}, {1, 0})
				+ checkDirection(x, y, {x, y}, {0, -1})
				+ checkDirection(x, y, {x, y}, {0, 1});
		return score;
	}
	return 0;
}

int Board::checkDirection(int x, int y, const Direction &init, const Direction &dir)
{
	int score = 0;

	std::function<bool(int, int)> check = [this, &init, &dir, &score, &check] (int x, int y) {
		x += dir.x;
		y += dir.y;
		if (tileExists(x, y)) {
			if (m_tiles[y][x].belongsToOponent(m_currentPlayer)) {
				score++;
				return check(x, y);
			}
			else if (m_tiles[y][x].belongsToUs(m_currentPlayer)) {
				return (std::abs(init.x - x) + std::abs(init.y - y)) > 1;
			}
		}
		return false;
	};

	if (!check(x, y)) {
		score = 0;
	}
	return score;
}

bool Board::tileExists(int x, int y)
{
	return x >= 0 && x < m_tiles.size() && y >= 0 && y < m_tiles.size();
}

void Board::findBestTileCapture()
{
	int bestScore = 0;
	int bestX = -1;
	int bestY = -1;

	for (size_t i = 0; i < m_tiles.size(); i++) {
		for (size_t j = 0; j < m_tiles.size(); j++) {
			int score = isTilePlayable(j, i);
			if (score > bestScore) {
				bestScore = score;
				bestX = j;
				bestY = i;
			}
		}
	}

	if (bestY != -1) {
		m_tiles[bestY][bestX].setOcupant(m_currentPlayer);
		changeOponentTiles(bestX, bestY);
	}
	else {
		m_gameOver = true;
	}

	m_currentPlayer->increaseScoreBy(bestScore);
	m_currentPlayer = m_currentPlayer == m_players.front() ? m_players.back() : m_players.front();
	if (!isGamePlayable()) {
		m_gameOver = true;
	}
}

bool Board::isGamePlayable()
{
	for (size_t i = 0; i < m_tiles.size(); i++) {
		for (size_t j = 0; j < m_tiles.size(); j++) {
			if (isTilePlayable(j, i) > 0) {
				return true;
			}
		}
	}
	return false;
}
