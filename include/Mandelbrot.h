#pragma once

#include "Include.h"
#include "Fractal.h"
#include "MandelbrotExtended.h"

class Mandelbrot : public Fractal
{
	friend class MandelbrotExtended;

public:
	Mandelbrot();
	Mandelbrot(const std::shared_ptr<MandelbrotExtended>& fractal);
	~Mandelbrot() override;

private:
	std::shared_ptr<hgui::kernel::Canva> m_canva;

	hgui::vec2 m_center;
	float m_offset;
	std::shared_ptr<hgui::vec2> m_mousePosition;

	void init();
};