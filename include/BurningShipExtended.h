#pragma once

#include "Include.h"
#include "Fractal.h"

class BurningShip;
class FractalExplorer;

class BurningShipExtended final : public Fractal
{
	friend class BurningShip;
	friend class FractalExplorer;

public:
	BurningShipExtended();
	BurningShipExtended(const BurningShipExtended& burningShipExtended);
	explicit BurningShipExtended(const hgui::dvec2& center);
	explicit BurningShipExtended(const std::shared_ptr<BurningShip>& fractal);

	[[nodiscard]] const hgui::dvec2& get_center() const;
	[[nodiscard]] double get_offset() const;

private:
	hgui::dvec2 m_center;
	double m_offset;
	std::shared_ptr<hgui::dvec2> m_mousePosition;

	void init();
};
