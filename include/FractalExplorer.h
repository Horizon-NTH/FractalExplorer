#pragma once

#include "Include.h"
#include "Mandelbrot.h"

class FractalExplorer
{
public:
	FractalExplorer();

	void start();
	void render(FractalsType fractal);
	void option_menu(FractalsType fractal);

private:
	std::shared_ptr<hgui::kernel::Window> m_window;
	std::shared_ptr<hgui::kernel::Monitor> m_monitor;
	std::shared_ptr<hgui::kernel::Font> m_font;
	std::shared_ptr<hgui::kernel::Font> m_titleFont;
	std::shared_ptr<Fractal> m_fractal;

	std::vector<std::shared_ptr<hgui::kernel::Label>> m_texts;
	std::vector<std::shared_ptr<hgui::kernel::Button>> m_buttons;

	void set_main_menu();
};