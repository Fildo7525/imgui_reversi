#include "Player.h"
#include "Image.h"

Player::Player(int id, const std::string &name, Ocupant ocupation, const std::string &texturePath, int width, int height)
	: m_score(0)
	, m_id(id)
	, m_name(name)
	, m_ocupation(ocupation)
	, m_width(width)
	, m_height(height)
{
	int h;
	int w;
	LoadTextureFromFile(texturePath.c_str(), &m_texture, &w, &h);
}

int Player::score() const
{
	return m_score;
}

int Player::id() const
{
	return m_id;
}

const std::string &Player::name() const
{
	return m_name;
}

Player::Ocupant Player::ocupation() const
{
	return m_ocupation;
}

