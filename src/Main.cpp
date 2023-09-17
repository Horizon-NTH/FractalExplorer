#include <hgui/HorizonGUI.h>

int main()
{
	hgui::init();
	auto& monitor = hgui::MonitorManager::get_primary_monitor();
	auto window = hgui::WindowManager::create("FractalExplorer", monitor->get_size(), glm::vec2(0), nullptr, nullptr, { hgui::WindowOption(hgui::options::MAXIMAIZED, true) });
	auto shader = hgui::ShaderManager::create(hgui::file_reader("assets/shaders/VertexShaderFractal.glsl"), hgui::file_reader("assets/shaders/FragmentShaderFractal.glsl"));
	shader->set_vec2("canvaSize", monitor->get_size());
	auto canva = hgui::CanvaManager::create(shader, monitor->get_size(), hgui::point(0));
#ifndef EXTENDED
	glm::vec2 center = glm::vec2(-0.75f, 0.0f);
	shader->set_vec2("center", center);
	float offset = 2.0f;
	shader->set_float("offset", offset);
#endif // !EXTENDED
#ifdef EXTENDED
	glm::dvec2 center = glm::dvec2(0);
	shader->set_dvec2("center", center);
	shader->set_dvec2("center", center);
	double offset = 2.0f;
	shader->set_double("offset", offset);
#endif // EXTENDED
	static std::shared_ptr<glm::vec2> lastMousePosition = nullptr;
	canva->bind(hgui::MouseCombinationAction(hgui::inputs::OVER, hgui::buttons::LEFT, hgui::actions::PRESS), [&]()
		{
			canva->bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT), [&]()
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
					if (newCenter.x > 1.0 || newCenter.x < -1.0)
					{
						newCenter.x = center.x;
					}
					if (newCenter.y > 1.0 || newCenter.y < -1.0)
					{
						newCenter.y = center.y;
					}
					std::cout << hgui::point(center) << std::endl;
					center = newCenter;
#ifndef EXTENDED
					shader->set_vec2("center", center);
#endif // !EXTENDED
#ifdef EXTENDED
					shader->set_dvec2("center", center);
#endif // EXTENDED
				});
		});
	canva->bind(hgui::MouseCombinationAction(hgui::inputs::OVER, hgui::buttons::LEFT, hgui::actions::RELEASE), [&]()
		{
			lastMousePosition = nullptr;
			canva->unbind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::REPEAT));
		});
	canva->bind(hgui::inputs::SCROLL_DOWN, [&]()
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
	canva->bind(hgui::inputs::SCROLL_UP, [&]()
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
	hgui::Renderer::draw();
	hgui::Widget::active();
	hgui::Renderer::set_background_color(HGUI_COLOR_WHITE);
	hgui::Renderer::loop();
	return 0;
}