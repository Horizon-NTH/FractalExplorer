#include "../include/FractalExplorer.h"

FractalExplorer::FractalExplorer() :
	m_monitor(nullptr), m_window(nullptr),
	m_font(nullptr), m_titleFont(nullptr), m_texts()
{
	//LIbrary initialization//
	hgui::init();
	//Winodw creation//
	m_monitor = hgui::MonitorManager::get_primary_monitor();
	m_window = hgui::WindowManager::create("FractalExplorer", m_monitor->get_size(),
		hgui::point(0), hgui::image_loader("assets/textures/logo.png"), m_monitor);
	//Tag creation//
	hgui::TagManager::create_tag("main_menu");
	hgui::TagManager::create_tag("fractal");
	//Load Font
	m_font = hgui::FontManager::create("assets/fonts/montserrat/Montserrat-Regular.otf");
	m_titleFont = hgui::FontManager::create("assets/fonts/great-vibes/GreatVibes-Regular.otf");
	//Binding
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::PRESS), hgui::end);
}

void FractalExplorer::start()
{
	set_main_menu();

	hgui::Widget::active({ "main_menu" });
	hgui::Renderer::draw({ "main_menu" });
	hgui::Renderer::loop();
}

void FractalExplorer::set_main_menu()
{
	hgui::TagManager::set_current_tag("main_menu");
	m_texts.push_back(hgui::LabelManager::create("V 1.0", hgui::point(0), m_font, 
		{ 12u,  HGUI_COLOR_WHITE, 1.0f }));
	m_texts[0]->set_position(hgui::point(100_em - m_texts[0]->get_size().width, 0));
	m_texts.push_back(hgui::LabelManager::create("FractalExplorer", hgui::point(0), m_titleFont, 
		{ 160u, HGUI_COLOR_BLUE, 1.0f }));
	m_texts[1]->set_position(hgui::point(50_em) - m_texts[1]->get_size() / 2 - hgui::point(0, 10_em));
	m_texts.push_back(hgui::LabelManager::create("Choose the fractal you want", hgui::point(0), m_font, 
		{ 40u, HGUI_COLOR_WHITE, 1.0f }));
	m_texts[2]->set_position(hgui::point(50_em) - m_texts[2]->get_size() / 2 + hgui::point(0, 15_em));
	m_texts.push_back(hgui::LabelManager::create("to explore.", hgui::point(0), m_font, 
		{ 40u, HGUI_COLOR_WHITE, 1.0f }));
	m_texts[3]->set_position(hgui::point(50_em) - m_texts[3]->get_size() / 2 + 
		hgui::point(0, m_texts[1]->get_size().height));
	auto start = [this](const hgui::keys& key, const hgui::actions& action)
	{
		if (action == hgui::actions::PRESS && key != hgui::keys::ESCAPE)
		{
			m_texts.clear();
		}
	};
	hgui::KeyBoardManager::bind_keycallback(start);
}
