#pragma once

#include <string>
#include <GL/gl.h>

class Player
{
public:
	enum class Ocupant {
		Empty,
		Black,
		White
	};

	explicit Player(int id, const std::string &name, Ocupant ocupation, const std::string &texturePath, int width, int height);
	int score() const;
	int id() const;
	const std::string &name() const;
	Ocupant ocupation() const;
	GLuint texture() const { return m_texture; }
	int textureWidth() const { return m_width; }
	int textureHeight() const { return m_height; }

private:
	int m_score;
	int m_id;
	std::string m_name;
	Ocupant m_ocupation;
	int m_width;
	int m_height;
	GLuint m_texture;
};
