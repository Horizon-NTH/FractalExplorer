#include <hgui/HorizonGUI.h>

int main()
{
	hgui::init();
	auto& monitor = hgui::MonitorManager::get_primary_monitor();
	hgui::WindowManager::create("FractalExplorer", monitor->get_size(), glm::vec2(0), nullptr, { hgui::WindowOption(hgui::options::MAXIMAIZED, true) });
	auto& shader = hgui::ResourceManager::load_shader("fractal", hgui::file_reader("VertexShaderFractal.glsl"), hgui::file_reader("FragmentShaderFractal.glsl"));
	shader->set_vec2("canvaSize", monitor->get_size());
	hgui::CanvaManager::create("fractal", shader, monitor->get_size(), glm::vec2(0));
#ifndef EXTENDED
	glm::vec2 center = glm::vec2(0.75, -1.25);
	shader->set_vec2("center", center);
	float offset = 2.0f;
	shader->set_float("offset", offset);
#endif // !EXTENDED
#ifdef EXTENDED
	glm::dvec2 center = glm::dvec2(0);
	shader->set_dvec2("center", center);
	double offset = 2.0f;
	shader->set_double("offset", offset);
#endif // EXTENDED
	static std::shared_ptr<glm::vec2> lastMousePosition = nullptr;
	hgui::Widget::bind(hgui::CanvaManager::get("fractal"), hgui::MouseCombinationAction(hgui::inputs::OVER, hgui::buttons::LEFT, hgui::actions::PRESS), [&]()
		{
			hgui::Widget::bind(hgui::CanvaManager::get("fractal"), hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT), [&]()
				{
					if (!lastMousePosition)
					{
						lastMousePosition = std::make_shared<glm::vec2>(hgui::MouseManager::get_position().x, hgui::MouseManager::get_position().y);
					}
					hgui::point mouse = hgui::MouseManager::get_position();
					glm::vec2 newMousePosition(mouse.x, mouse.y);
					decltype(center) delta = decltype(center)(newMousePosition - *lastMousePosition) / decltype(center)(monitor->get_size().width, monitor->get_size().height);
					decltype(center) newCenter = center - delta * offset;
					lastMousePosition = std::make_shared<glm::vec2>(newMousePosition);
					if (newCenter.x > 2.0 || newCenter.x < -2.0)
					{
						newCenter.x = center.x;
					}
					if (newCenter.y > 2.0 || newCenter.y < -2.0)
					{
						newCenter.y = center.y;
					}
					std::cout << hgui::point(center) << " --> " << hgui::point(newCenter) << std::endl;
					center = newCenter;
#ifndef EXTENDED
					shader->set_vec2("center", center);
#endif // !EXTENDED
#ifdef EXTENDED
					shader->set_dvec2("center", center);
#endif // EXTENDED
				});
		});
	hgui::Widget::bind(hgui::CanvaManager::get("fractal"), hgui::MouseCombinationAction(hgui::inputs::OVER, hgui::buttons::LEFT, hgui::actions::RELEASE), [&]()
		{
			lastMousePosition = nullptr;
			hgui::Widget::unbind(hgui::CanvaManager::get("fractal"), hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT));
		});
	hgui::Widget::bind(hgui::CanvaManager::get("fractal"), hgui::inputs::SCROLL_DOWN, [&]()
		{
			offset *= 1.05f;

#ifndef EXTENDED
			offset = std::min(offset, 2.0f);
			shader->set_float("offset", offset);
#endif // !EXTENDED
#ifdef EXTENDED
			offset = std::min(offset, 2.0);
			shader->set_double("offset", offset);
#endif // EXTENDED
		});
	hgui::Widget::bind(hgui::CanvaManager::get("fractal"), hgui::inputs::SCROLL_UP, [&]()
		{
			offset *= 0.95f;
#ifndef EXTENDED
			offset = std::max(offset, 1e-4f);
			shader->set_float("offset", offset);
#endif // !EXTENDED
#ifdef EXTENDED
			offset = std::max(offset, 1e-13);
			shader->set_double("offset", offset);
#endif // EXTENDED
		});
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT), [&]()
		{
			offset *= 0.95;
#ifndef EXTENDED
			offset = std::max(offset, 1e-4f);
			shader->set_float("offset", offset);
#endif // !EXTENDED
#ifdef EXTENDED
			offset = std::max(offset, 1e-13);
			shader->set_double("offset", offset);
#endif // EXTENDED
		});
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::KP_SUBSTRACT, hgui::actions::REPEAT), [&]()
		{
			offset *= 1.05;
#ifndef EXTENDED
			offset = std::min(offset, 2.0f);
			shader->set_float("offset", offset);
#endif // !EXTENDED
#ifdef EXTENDED
			offset = std::min(offset, 2.0);
			shader->set_double("offset", offset);
#endif // EXTENDED
		});
	hgui::RenderManager::draw();
	hgui::Widget::active();
	hgui::RenderManager::set_background_color(HGUI_COLOR_WHITE);
	hgui::RenderManager::loop();
	return 0;
}