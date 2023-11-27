#pragma once

#include "Include.h"
#include "Fractal.h"

class MandelbrotExtended;

class Mandelbrot final : public Fractal
{
	friend class MandelbrotExtended;

public:
	Mandelbrot();
	explicit Mandelbrot(const std::shared_ptr<MandelbrotExtended>& fractal);

private:
	hgui::vec2 m_center;
	float m_offset;
	std::shared_ptr<hgui::vec2> m_mousePosition;

	void init();
};
