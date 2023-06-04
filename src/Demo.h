#pragma once

#include "Layer.h"

class Demo
	: public Layer
{
public:
	Demo() = default;
	void render() override;
private:
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;
};
