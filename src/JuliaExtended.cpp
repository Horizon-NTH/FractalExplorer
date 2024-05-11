#include "../include/JuliaExtended.h"
#include "../include/Julia.h"

JuliaExtended::JuliaExtended() :
	Fractal(FractalsType::JULIA, nullptr),
	m_center(0.),
	m_offset(2.0),
	m_mousePosition(nullptr)
{
	init();
}

JuliaExtended::JuliaExtended(const JuliaExtended& juliaExtended) :
	Fractal(FractalsType::JULIA, nullptr),
	m_center(juliaExtended.m_center),
	m_offset(std::clamp(juliaExtended.m_offset, 1.e-13, 2.0)),
	m_mousePosition(nullptr)
{
	init();
	set_complex(juliaExtended.m_complex);
}

JuliaExtended::JuliaExtended(const hgui::dvec2& center) :
	Fractal(FractalsType::JULIA, nullptr),
	m_center(hgui::vec2(std::clamp(center.x, -2., 2.), std::clamp(center.y, -2., 2.))),
	m_offset(2.0),
	m_mousePosition(nullptr)
{
	init();
}

JuliaExtended::JuliaExtended(const std::shared_ptr<Julia>& fractal) :
	Fractal(FractalsType::JULIA, nullptr),
	m_center(fractal->m_center),
	m_offset(std::clamp(static_cast<double>(fractal->m_offset), 1.e-13, 2.0)),
	m_mousePosition(nullptr)
{
	init();
	set_complex(fractal->m_complex);
}

void JuliaExtended::set_complex(hgui::vec2 complex)
{
	m_complex = hgui::vec2(std::clamp(complex.x, -2.f, 2.f), std::clamp(complex.y, -2.f, 2.f));
	m_shader->use().set_vec2("complex", m_complex);
}

void JuliaExtended::init()
{
	m_shader = hgui::ShaderManager::create(
		hgui::file_reader("assets/shaders/VertexShaderFractalExtended.glsl"),
		hgui::file_reader("assets/shaders/FragmentShaderJuliaExtended.glsl")
	);
	m_canvas = hgui::CanvasManager::create(m_shader, hgui::size(100_em), hgui::point(0));
	m_shader->use().set_dvec2("center", m_center).set_double("offset", m_offset)
	        .set_vec2("canvasSize", hgui::size(100_em))
	        .set_vec2("complex", m_complex);
	m_canvas->bind(hgui::MouseCombinationAction(hgui::inputs::OVER, hgui::buttons::LEFT, hgui::actions::PRESS), [&]()
		{
			if (!m_canvas->is_bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT)))
			{
				m_canvas->bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT), [&]()
					{
						if (!m_mousePosition)
						{
							m_mousePosition = std::make_shared<hgui::dvec2>(static_cast<hgui::kernel::Vector<double, 2>>(hgui::MouseManager::get_position()));
						}
						hgui::dvec2 newMousePosition = static_cast<hgui::kernel::Vector<double, 2>>(hgui::MouseManager::get_position());
						const hgui::dvec2 delta = (newMousePosition - *m_mousePosition);
						delta.x /= hgui::size(100_em).width;
						delta.y /= hgui::size(100_em).height;
						const hgui::dvec2 newCenter = m_center - delta * m_offset;
						m_mousePosition = std::make_shared<hgui::dvec2>(newMousePosition);
						m_center = hgui::dvec2(std::clamp(newCenter.x, -2., 2.), std::clamp(newCenter.y, -2., 2.));
						m_shader->set_dvec2("center", m_center);
					});
			}
		});
	m_canvas->bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::RELEASE), [&]()
		{
			m_mousePosition = nullptr;
			if (m_canvas->is_bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT)))
			{
				m_canvas->unbind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT));
			}
		});
	m_canvas->bind(hgui::inputs::SCROLL_DOWN, [&]()
		{
			m_offset *= 1.05;
			m_offset = std::min(m_offset, 2.0);
			m_shader->set_double("offset", m_offset);
		});
	m_canvas->bind(hgui::inputs::SCROLL_UP, [&]()
		{
			m_offset *= 0.95;
			m_offset = std::max(m_offset, 1e-13);
			m_shader->set_double("offset", m_offset);
		});
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT));
	}
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::KP_SUBTRACT, hgui::actions::REPEAT)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::KP_SUBTRACT, hgui::actions::REPEAT));
	}
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT), [&]()
		{
			m_offset *= 0.95;
			m_offset = std::max(m_offset, 1e-13);
			m_shader->set_double("offset", m_offset);
		});
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::KP_SUBTRACT, hgui::actions::REPEAT), [&]()
		{
			m_offset *= 1.05;
			m_offset = std::min(m_offset, 2.0);
			m_shader->set_double("offset", m_offset);
		});
}
