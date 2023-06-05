#include "Status.h"
#include "imgui.h"

Status::Status(std::shared_ptr<Board> board)
	: Layer("Status")
	, m_board(board)
{
}

void Status::render()
{
	ImGui::Begin("Status", NULL, m_windowFlags);
	ImGui::Text("Status");
	ImGui::Text("Current player: %s", m_board->currentPlayer()->name().c_str());

	auto players = m_board->players();
	for (auto &player: players) {
		ImGui::Text("Player: %s Score: %d", player->name().c_str(), player->score());
	}

	if (m_board->isGameOver()) {
		m_scores.insert({players.front()->score(), players.back()->score()});
	}

	{
		if (ImGui::RadioButton("Small", &m_board->boardSize(), 6)) {
			m_board->setBoardSize(6);
		}
		if (ImGui::RadioButton("Medium", &m_board->boardSize(), 8)) {
			m_board->setBoardSize(8);
		}
		if (ImGui::RadioButton("Large", &m_board->boardSize(), 12)) {
			m_board->setBoardSize(12);
		}
	}

	{
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX());
		ImGui::BeginChild("Red", ImVec2(ImGui::GetWindowSize().x*0.8, 100), true, ImGuiWindowFlags_None);
		for (auto &score: m_scores) {
			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
			ImGui::Text("User: %d\tPC: %d", score.first, score.second);
			ImGui::PopStyleColor();
		}
		ImGui::EndChild();
	}

	ImGui::End();
}
