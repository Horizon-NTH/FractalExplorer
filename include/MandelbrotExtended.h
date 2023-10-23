#pragma once

#include "Include.h"
#include "Fractal.h"
#include "Mandelbrot.h"

class MandelbrotExtended : public Fractal
{
	friend class Mandelbrot;

public:
	MandelbrotExtended();
	MandelbrotExtended(const std::shared_ptr<Mandelbrot>& fractal);
	~MandelbrotExtended() override;

private:
	std::shared_ptr<hgui::kernel::Canva> m_canva;

	hgui::dvec2 m_center;
	double m_offset;
	std::shared_ptr<hgui::dvec2> m_mousePosition;

	void init();
};