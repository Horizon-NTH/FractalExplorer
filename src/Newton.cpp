#include "../include/Newton.h"
#include "../include/NewtonExtended.h"

Newton::Newton() : Fractal(FractalsType::NEWTON, nullptr),
                   m_center(0.0f, 0.0f),
                   m_offset(2.0f),
                   m_mousePosition(nullptr)
{
	init();
}

Newton::Newton(const Newton& newton) : Fractal(FractalsType::NEWTON, nullptr),
                                       m_center(newton.m_center),
                                       m_offset(std::clamp(newton.m_offset, 1e-4f, 2.f)),
                                       m_mousePosition(nullptr)
{
	init();
}

Newton::Newton(const hgui::vec2& center) : Fractal(FractalsType::NEWTON, nullptr),
                                           m_center(hgui::vec2(std::clamp(center.x, -2.f, 2.f), std::clamp(center.y, -2.f, 2.f))),
                                           m_offset(2.0f),
                                           m_mousePosition(nullptr)
{
	init();
}

Newton::Newton(const std::shared_ptr<NewtonExtended>& fractal) : Fractal(FractalsType::NEWTON, nullptr),
                                                                 m_center(fractal->m_center),
                                                                 m_offset(std::clamp(static_cast<float>(fractal->m_offset), 1e-4f, 2.f)),
                                                                 m_mousePosition(nullptr)
{
	init();
}

const hgui::vec2& Newton::get_center() const
{
	return m_center;
}

float Newton::get_offset() const
{
	return m_offset;
}

void Newton::init()
{
	m_shader = hgui::ShaderManager::create(
		hgui::file_reader("assets/shaders/VertexShaderFractal.glsl"),
		hgui::file_reader("assets/shaders/FragmentShaderNewton.glsl")
	);
	m_canvas = hgui::CanvasManager::create(m_shader, hgui::size(100_em), hgui::point(0));
	m_shader->use().set_vec2("center", m_center).set_float("offset", m_offset)
	        .set_vec2("canvasSize", hgui::size(100_em));
	m_canvas->bind(hgui::MouseCombinationAction(hgui::inputs::OVER, hgui::buttons::LEFT, hgui::actions::PRESS), [&]()
		               {
			               if (!m_canvas->is_bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT)))
			               {
				               m_canvas->bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT), [&]()
					                              {
						                              if (!m_mousePosition)
						                              {
							                              m_mousePosition = std::make_shared<hgui::vec2>(static_cast<hgui::kernel::Vector<float, 2>>(hgui::MouseManager::get_position()));
						                              }
						                              hgui::vec2 newMousePosition = static_cast<hgui::kernel::Vector<float, 2>>(hgui::MouseManager::get_position());
						                              const hgui::vec2 delta = (newMousePosition - *m_mousePosition);
						                              delta.x /= hgui::size(100_em).width;
						                              delta.y /= hgui::size(100_em).height;
						                              const hgui::vec2 newCenter = m_center - delta * m_offset;
						                              m_mousePosition = std::make_shared<hgui::vec2>(newMousePosition);
						                              m_center = hgui::vec2(std::clamp(newCenter.x, -2.f, 2.f), std::clamp(newCenter.y, -2.f, 2.f));
						                              m_shader->set_vec2("center", m_center);
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
			               m_offset *= 1.05f;
			               m_offset = std::min(m_offset, 2.0f);
			               m_shader->set_float("offset", m_offset);
		               });
	m_canvas->bind(hgui::inputs::SCROLL_UP, [&]()
		               {
			               m_offset *= 0.95f;
			               m_offset = std::max(m_offset, 1e-4f);
			               m_shader->set_float("offset", m_offset);
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
			                            m_offset *= 0.95f;
			                            m_offset = std::max(m_offset, 1e-4f);
			                            m_shader->set_float("offset", m_offset);
		                            });
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::KP_SUBTRACT, hgui::actions::REPEAT), [&]()
		                            {
			                            m_offset *= 1.05f;
			                            m_offset = std::min(m_offset, 2.0f);
			                            m_shader->set_float("offset", m_offset);
		                            });
}
