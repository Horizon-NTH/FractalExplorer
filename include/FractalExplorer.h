#pragma once

#include "Include.h"

class FractalExplorer
{
public:
	FractalExplorer();
	
	void start();

private:
	std::shared_ptr<hgui::kernel::Window> m_window;
	std::shared_ptr<hgui::kernel::Monitor> m_monitor;
	std::shared_ptr<hgui::kernel::Font> m_font;
	std::shared_ptr<hgui::kernel::Font> m_titleFont;

	std::vector<std::shared_ptr<hgui::kernel::Label>> m_texts;

	void set_main_menu();
};

