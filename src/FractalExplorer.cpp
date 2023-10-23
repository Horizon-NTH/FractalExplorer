#include "../include/FractalExplorer.h"

FractalExplorer::FractalExplorer() :
	m_monitor(nullptr), m_window(nullptr),
	m_font(nullptr), m_titleFont(nullptr), m_texts(), m_buttons(),
	m_fractal(nullptr)
{
	//LIbrary initialization//
	hgui::init();
	//Winodw creation//
	m_monitor = hgui::MonitorManager::get_primary_monitor();
	m_window = hgui::WindowManager::create("FractalExplorer", m_monitor->get_size(),
		hgui::point(0), hgui::image_loader("assets/textures/logo.png"), nullptr, { hgui::WindowOption(hgui::options::MAXIMAIZED, true) });
	//Tag creation//
	hgui::TagManager::create_tag("main_menu");
	hgui::TagManager::create_tag("fractal");
	hgui::TagManager::create_tag("option");
	//Load Font
	m_font = hgui::FontManager::create("assets/fonts/montserrat/Montserrat-Regular.otf");
	m_titleFont = hgui::FontManager::create("assets/fonts/great-vibes/GreatVibes-Regular.otf");
	//Binding
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), hgui::end);
	//OptionMenu
	hgui::TagManager::set_current_tag("option");
	constexpr int BUTTON_SIZE = 100;
	m_buttons.push_back(hgui::ButtonManager::create([&]() 
		{
			render(FractalsType::SAME);
		}, hgui::size(BUTTON_SIZE), hgui::point(5_em),
		hgui::TextureManager::create(hgui::image_loader("assets/textures/option.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]() 
		{
			if (auto fractal = std::dynamic_pointer_cast<Mandelbrot>(m_fractal))
			{
				m_fractal = std::static_pointer_cast<Fractal>(std::make_shared<MandelbrotExtended>(fractal));
			}
		}, hgui::size(BUTTON_SIZE), hgui::point(5_em, 5_em + BUTTON_SIZE),
		hgui::TextureManager::create(hgui::image_loader("assets/textures/zoom.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]() 
		{
			if (auto fractal = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal))
			{
				m_fractal = std::make_shared<Mandelbrot>(fractal);
			}
		}, hgui::size(BUTTON_SIZE), hgui::point(5_em + BUTTON_SIZE),
		hgui::TextureManager::create(hgui::image_loader("assets/textures/perf.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]() 
		{
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 0);
			}
		}, hgui::size(100), hgui::point(5_em, 5_em + 2 * BUTTON_SIZE),
		hgui::TextureManager::create(hgui::image_loader("assets/textures/wave.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]() 
		{ 
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 1);
			}
		}, hgui::size(100), hgui::point(5_em + BUTTON_SIZE, 5_em + 2 * BUTTON_SIZE),
		nullptr, hgui::color(1.0, 0.0), static_cast<float>(INT_MAX), "HSV", m_font, HGUI_COLOR_WHITE));
	m_buttons.push_back(hgui::ButtonManager::create([&]() 
		{
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 2);
			}
		}, hgui::size(100), hgui::point(5_em + 2 * BUTTON_SIZE, 5_em + 2 * BUTTON_SIZE),
		nullptr, hgui::color(1.0, 0.0), static_cast<float>(INT_MAX), "HSL", m_font, HGUI_COLOR_WHITE));
	m_buttons.push_back(hgui::ButtonManager::create([&]() 
		{
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 3);
			}
		}, hgui::size(100), hgui::point(5_em + 3 * BUTTON_SIZE, 5_em + 2 * BUTTON_SIZE),
		nullptr, hgui::color(1.0, 0.0), static_cast<float>(INT_MAX), "LCH", m_font, HGUI_COLOR_WHITE));
}

void FractalExplorer::start()
{
	set_main_menu();

	hgui::Widget::active({ "main_menu" });
	hgui::Renderer::draw({ "main_menu" });
	hgui::Renderer::loop();
}

void FractalExplorer::render(FractalsType fractal)
{
	hgui::TagManager::set_current_tag("fractal");
	switch (fractal)
	{
	case FractalsType::MANDELBROT:
		m_fractal = std::static_pointer_cast<Fractal>(std::make_shared<Mandelbrot>());
		break;
	case FractalsType::JULIA:
		break;
	default:
		break;
	}
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE));
	}
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), [&]() { option_menu(fractal); });
	hgui::Widget::active({ "fractal" });
	hgui::Renderer::draw({ "fractal" });
	hgui::Renderer::draw({ HGUI_TAG_MAIN }, hgui::effects::BLURRED);
}

void FractalExplorer::option_menu(FractalsType fractal)
{
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE));
	}
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), [&]() { render(FractalsType::SAME); });
	hgui::Widget::active({ "option" });
	hgui::Renderer::draw({ "option" });
	hgui::Renderer::draw({ "fractal" }, hgui::effects::BLURRED);
}

void FractalExplorer::set_main_menu()
{
	hgui::TagManager::set_current_tag("main_menu");
	m_texts.push_back(hgui::LabelManager::create("V 1.0", hgui::point(0), m_font,
		{ 25u,  HGUI_COLOR_WHITE, 1.0f }));
	m_texts.back()->set_position(hgui::point(99_em - m_texts.back()->get_size().width, 0));
	m_texts.push_back(hgui::LabelManager::create("FractalExplorer", hgui::point(0), m_titleFont,
		{ 200u, HGUI_COLOR_BLUE, 1.0f }));
	m_texts.back()->set_position(hgui::point(50_em) - m_texts.back()->get_size() / 2 - hgui::point(0, 30_em));
	m_texts.push_back(hgui::LabelManager::create("Choose the fractal you want", hgui::point(0), m_font,
		{ 40u, HGUI_COLOR_WHITE, 1.0f }));
	m_texts.back()->set_position(hgui::point(50_em) - m_texts.back()->get_size() / 2 - hgui::point(0, 15_em));
	m_texts.push_back(hgui::LabelManager::create("to explore.", hgui::point(0), m_font,
		{ 40u, HGUI_COLOR_WHITE, 1.0f }));
	m_texts.back()->set_position(hgui::point(50_em) - m_texts.back()->get_size() / 2 - hgui::point(0, 12_em) +
		hgui::point(0, m_texts[2]->get_size().height));

	m_buttons.push_back(hgui::ButtonManager::create(std::bind(&FractalExplorer::render, this, FractalsType::MANDELBROT), hgui::size(20_em, 10_em), hgui::point(0), nullptr, HGUI_COLOR_BLUE, 1000, "Mandelbrot", m_font, HGUI_COLOR_WHITE));
	m_buttons.back()->set_position(hgui::point(50_em) - m_buttons.back()->get_size() / 2 + hgui::point(0, 10_em));
	auto start = [this](const hgui::keys& key, const hgui::actions& action)
	{
		if (action == hgui::actions::PRESS && key != hgui::keys::ESCAPE)
		{
			m_texts.clear();
		}
	};
	hgui::KeyBoardManager::bind_keycallback(start);
}
