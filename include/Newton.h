#pragma once

#include "Include.h"
#include "Fractal.h"

class NewtonExtended;

class Newton final : public Fractal
{
	friend class NewtonExtended;

public:
	Newton();
	Newton(const Newton& newton);
	explicit Newton(const hgui::vec2& center);
	explicit Newton(const std::shared_ptr<NewtonExtended>& fractal);

	[[nodiscard]] const hgui::vec2& get_center() const;
	[[nodiscard]] float get_offset() const;

private:
	hgui::vec2 m_center;
	float m_offset;
	std::shared_ptr<hgui::vec2> m_mousePosition;

	void init();
};
