#pragma once

#include "Include.h"
#include "Fractal.h"

class BurningShipExtended;

class BurningShip final : public Fractal
{
	friend class BurningShipExtended;

public:
	BurningShip();
	BurningShip(const BurningShip& burningShip);
	explicit BurningShip(const hgui::vec2& center);
	explicit BurningShip(const std::shared_ptr<BurningShipExtended>& fractal);

	[[nodiscard]] const hgui::vec2& get_center() const;
	[[nodiscard]] float get_offset() const;

private:
	hgui::vec2 m_center;
	float m_offset;
	std::shared_ptr<hgui::vec2> m_mousePosition;

	void init();
};
