#pragma once

#include "Board.h"
#include "imgui.h"

#include <memory>
#include <set>

class Status
	: public Layer
{
public:
	explicit Status(std::shared_ptr<Board> board);
	void render() override;

private:
	std::shared_ptr<Board> m_board;
	std::set<std::pair<int, int>> m_scores;
};
