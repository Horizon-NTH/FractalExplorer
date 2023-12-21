#pragma once

#include "Include.h"
#include "Fractal.h"

class Mandelbrot;
class FractalExplorer;

class MandelbrotExtended final : public Fractal
{
	friend class Mandelbrot;
	friend class FractalExplorer;

public:
	MandelbrotExtended();
	MandelbrotExtended(const hgui::dvec2& center);
	explicit MandelbrotExtended(const std::shared_ptr<Mandelbrot>& fractal);

	const hgui::vec2& get_center() const;
	float get_offset() const;

private:
	hgui::dvec2 m_center;
	double m_offset;
	std::shared_ptr<hgui::dvec2> m_mousePosition;

	void init();
};
