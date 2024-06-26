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
	MandelbrotExtended(const MandelbrotExtended& mandelbrotExtended);
	explicit MandelbrotExtended(const hgui::dvec2& center);
	explicit MandelbrotExtended(const std::shared_ptr<Mandelbrot>& fractal);

	[[nodiscard]] const hgui::dvec2& get_center() const;
	[[nodiscard]] double get_offset() const;

private:
	hgui::dvec2 m_center;
	double m_offset;
	std::shared_ptr<hgui::dvec2> m_mousePosition;

	void init();
};
