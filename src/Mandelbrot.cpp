#include "../include/Mandelbrot.h"
#include "../include/MandelbrotExtended.h"

Mandelbrot::Mandelbrot() :
	Fractal(FractalsType::MANDELBROT, nullptr), m_canva(nullptr),
	m_center({ -0.75f, 0.0f }), m_offset(2.0f),
	m_mousePosition(nullptr)
{
	init();
}

Mandelbrot::Mandelbrot(const std::shared_ptr<MandelbrotExtended>& fractal) :
	Fractal(FractalsType::MANDELBROT, nullptr), m_canva(nullptr),
	m_center(fractal->m_center), m_offset(std::max(std::min(static_cast<float>(fractal->m_offset), 2.0f), 1e-4f)),
	m_mousePosition(nullptr)
{
	fractal->~MandelbrotExtended();
	init();
}

Mandelbrot::~Mandelbrot()
{
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT));
	}
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::KP_SUBSTRACT, hgui::actions::REPEAT)))
	{
			hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::KP_SUBSTRACT, hgui::actions::REPEAT));
	}
}

void Mandelbrot::init()
{
	m_shader = hgui::ShaderManager::create(
		hgui::file_reader("assets/shaders/VertexShaderFractal.glsl"),
		hgui::file_reader("assets/shaders/FragmentShaderMandelbrot.glsl")
	);
	m_canva = hgui::CanvaManager::create(m_shader, hgui::size(100_em), hgui::point(0));
	m_shader->set_vec2("canvaSize", hgui::size(100_em));
	m_shader->use().set_vec2("center", m_center).set_float("offset", m_offset);
	m_canva->bind(hgui::MouseCombinationAction(hgui::inputs::OVER, hgui::buttons::LEFT, hgui::actions::PRESS), [&]()
		{
			m_canva->bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT), [&]()
				{
					if (!m_mousePosition)
					{
						m_mousePosition = std::make_shared<hgui::vec2>(hgui::MouseManager::get_position());
					}
					hgui::vec2 newMousePosition = hgui::MouseManager::get_position();
					hgui::vec2 delta = (newMousePosition - *m_mousePosition);
					delta.x /= hgui::size(100_em).width;
					delta.y /= hgui::size(100_em).height;
					hgui::vec2 newCenter = m_center - delta * m_offset;
					m_mousePosition = std::make_shared<hgui::vec2>(newMousePosition);
					if (newCenter.x > 1.0f || newCenter.x < -1.0f)
					{
						newCenter.x = m_center.x;
					}
					if (newCenter.y > 1.0f || newCenter.y < -1.0f)
					{
						newCenter.y = m_center.y;
					}
					m_center = newCenter;
					m_shader->set_vec2("center", m_center);
				});
		});

	m_canva->bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::RELEASE), [&]()
		{
			m_mousePosition = nullptr;
			m_canva->unbind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT));
		});
	m_canva->bind(hgui::inputs::SCROLL_DOWN, [&]()
		{
			m_offset *= 1.05f;
			m_offset = std::min(m_offset, 2.0f);
			m_shader->set_float("offset", m_offset);
		});
	m_canva->bind(hgui::inputs::SCROLL_UP, [&]()
		{
			m_offset *= 0.95f;
			m_offset = std::max(m_offset, 1e-4f);
			m_shader->set_float("offset", m_offset);
		});
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT), [&]()
		{
			m_offset *= 0.95f;
			m_offset = std::max(m_offset, 1e-4f);
			m_shader->set_float("offset", m_offset);
		});
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::KP_SUBSTRACT, hgui::actions::REPEAT), [&]()
		{
			m_offset *= 1.05f;
			m_offset = std::min(m_offset, 2.0f);
			m_shader->set_float("offset", m_offset);
		});
}
