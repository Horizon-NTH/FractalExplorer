#pragma once

#include "Include.h"

enum class FractalsType
{
	MANDELBROT,
	JULIA,
	BURNINGSHIP,
	SAME
};

class Fractal
{
public:
	Fractal(FractalsType type, const std::shared_ptr<hgui::kernel::Shader>& shader);
	virtual ~Fractal() = default;

	[[nodiscard]] const std::shared_ptr<hgui::kernel::Shader>& get_shader() const;
	[[nodiscard]] const FractalsType& get_type() const;

protected:
	FractalsType m_type;
	std::shared_ptr<hgui::kernel::Shader> m_shader;
	std::shared_ptr<hgui::kernel::Canvas> m_canvas;
};
