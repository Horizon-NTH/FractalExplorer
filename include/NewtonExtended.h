#pragma once

#include "Include.h"
#include "Fractal.h"

class Newton;

class NewtonExtended final : public Fractal
{
	friend class Newton;

public:
	NewtonExtended();
	NewtonExtended(const NewtonExtended& newtonExtended);
	explicit NewtonExtended(const hgui::dvec2& center);
	explicit NewtonExtended(const std::shared_ptr<Newton>& fractal);

	[[nodiscard]] const hgui::dvec2& get_center() const;
	[[nodiscard]] double get_offset() const;

private:
	hgui::dvec2 m_center;
	double m_offset;
	std::shared_ptr<hgui::dvec2> m_mousePosition;

	void init();
};
