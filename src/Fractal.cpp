#include "../include/Fractal.h"

Fractal::Fractal(FractalsType type, const std::shared_ptr<hgui::kernel::Shader>& shader) :
	m_type(type), m_shader(shader)
{
}

Fractal::~Fractal()
{
}

const std::shared_ptr<hgui::kernel::Shader>& Fractal::get_shader() const
{
	return m_shader;
}

const FractalsType& Fractal::get_type() const
{
	return m_type;
}
