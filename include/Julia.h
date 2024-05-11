#pragma once

#include "Include.h"
#include "Fractal.h"

class JuliaExtended;

class Julia final : public Fractal
{
	friend class JuliaExtended;

public:
	Julia();
	Julia(const Julia& julia);
	explicit Julia(const hgui::vec2& center);
	explicit Julia(const std::shared_ptr<JuliaExtended>& fractal);

	void set_complex(hgui::vec2 complex);

private:
	hgui::vec2 m_center;
	hgui::vec2 m_complex;
	float m_offset;
	std::shared_ptr<hgui::vec2> m_mousePosition;

	void init();
};
