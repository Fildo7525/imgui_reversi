#pragma once

#include <iostream>
#include "imgui.h"

class Layer
{
public:
	virtual ~Layer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}

	virtual void render() {}
};
