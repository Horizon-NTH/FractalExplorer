#pragma once

#include "Include.h"
#include "Fractal.h"

class Julia;
class FractalExplorer;

class JuliaExtended final : public Fractal
{
	friend class Julia;
	friend class FractalExplorer;

public:
	JuliaExtended();
	JuliaExtended(const JuliaExtended& juliaExtended);
	explicit JuliaExtended(const hgui::dvec2& center);
	explicit JuliaExtended(const std::shared_ptr<Julia>& fractal);

	void set_complex(hgui::vec2 complex);

private:
	hgui::dvec2 m_center;
	hgui::vec2 m_complex;
	double m_offset;
	std::shared_ptr<hgui::dvec2> m_mousePosition;

	void init();
};
