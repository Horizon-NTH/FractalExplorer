#pragma once

#include "Include.h"
#include "Fractal.h"

class MandelbrotExtended;

class Mandelbrot final : public Fractal
{
	friend class MandelbrotExtended;

public:
	Mandelbrot();
	Mandelbrot(const Mandelbrot& mandelbrot);
	explicit Mandelbrot(const hgui::vec2& center);
	explicit Mandelbrot(const std::shared_ptr<MandelbrotExtended>& fractal);

	[[nodiscard]] const hgui::vec2& get_center() const;
	[[nodiscard]] float get_offset() const;

private:
	hgui::vec2 m_center;
	float m_offset;
	std::shared_ptr<hgui::vec2> m_mousePosition;

	void init();
};
