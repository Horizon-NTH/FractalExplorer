#include "../include/FractalExplorer.h"

FractalExplorer::FractalExplorer() :
	m_window(nullptr),
	m_monitor(nullptr),
	m_font(nullptr),
	m_titleFont(nullptr),
	m_fractal(nullptr)
{
	// Library initialization //
	hgui::init();
	// Window creation //
	m_monitor = hgui::MonitorManager::get_primary_monitor();
	m_window = hgui::WindowManager::create("FractalExplorer", m_monitor->get_size(),
		hgui::point(0), hgui::image_loader("assets/textures/logo.png"), nullptr, { hgui::WindowOption(hgui::options::MAXIMIZED, true) });
	// Tag creation //
	hgui::TagManager::create_tag("main_menu");
	hgui::TagManager::create_tag("fractal");
	hgui::TagManager::create_tag("background");
	hgui::TagManager::create_tag("option");
	//Load Font //
	m_font = hgui::FontManager::create("assets/fonts/montserrat/Montserrat-Regular.otf");
	m_titleFont = hgui::FontManager::create("assets/fonts/great-vibes/GreatVibes-Regular.otf");
	// Binding //
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), hgui::end);
	// Option menu //
	set_option_menu();
	// Main Menu //
	set_main_menu();
}

void FractalExplorer::start()
{
	draw_background();
	hgui::Widget::active({ "main_menu" });
	hgui::Renderer::draw({ "main_menu" });
	hgui::Renderer::draw({ "background" }, hgui::effects::BLURRED);
	hgui::Renderer::loop();
}

void FractalExplorer::render(const FractalsType fractal)
{
	hgui::TagManager::set_current_tag("fractal");
	switch (fractal)
	{
	case FractalsType::MANDELBROT:
		m_fractal = std::static_pointer_cast<Fractal>(std::make_shared<Mandelbrot>());
		set_special_option_menu(fractal);
		break;
	case FractalsType::JULIA:
		m_fractal = std::static_pointer_cast<Fractal>(std::make_shared<Julia>());
		set_special_option_menu(fractal);
		break;
	case FractalsType::SAME:
		break;
	}
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE));
	}
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), [&] { option_menu(); });
	hgui::Widget::active({ "fractal" });
	hgui::Renderer::draw({ "fractal" });
	hgui::Renderer::draw({ HGUI_TAG_MAIN }, hgui::effects::BLURRED);
}

void FractalExplorer::option_menu()
{
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE));
	}
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), [&] { render(FractalsType::SAME); });
	hgui::Widget::active({ "option" });
	hgui::Renderer::draw({ "option" });
	hgui::Renderer::draw({ "fractal" }, hgui::effects::BLURRED);
}

void FractalExplorer::draw_background()
{
	hgui::TagManager::set_current_tag("background");
	static const std::vector<std::pair<hgui::dvec2, double>> centers = {
		std::make_pair(hgui::dvec2(0.29803728643142374244, -0.017892934639849415734), 1e-9),
		std::make_pair(hgui::dvec2(0.27236526368881613935, -0.0051226242725265431002), 1e-10),
		std::make_pair(hgui::dvec2(-1.8100958106935862713, -1.2115589437016044164e-12), 1e-11),
		std::make_pair(hgui::dvec2(-0.27977201929735756591, -0.83911902078682876205), 1e-9),
		std::make_pair(hgui::dvec2(-0.77116049982246182015, 0.11535252316027029551), 1e-9),
		std::make_pair(hgui::dvec2(-0.13498701313338801189, 1.000848937830046026), 1e-11)
	};
	static std::pair<hgui::dvec2, double> center;
	static std::pair<hgui::vec2, double> complex;
	if (const auto fractal = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal); fractal && fractal->m_offset >= center.second)
	{
		fractal->m_offset *= 0.95;
		fractal->m_shader->use().set_double("offset", fractal->m_offset);
	}
	else if (const auto fractal = std::dynamic_pointer_cast<Julia>(m_fractal); fractal && glfwGetTime() - complex.second <= 30.)
	{
		double time = fmod(0.5 * glfwGetTime(), 2. * glm::pi<double>());
		complex.first = 0.7885f * hgui::vec2(static_cast<float>(glm::cos(time)), static_cast<float>(glm::sin(time)));
		fractal->set_complex(complex.first);
	}
	else
	{
		if (hgui::random(0.5))
		{
			center = centers.at(std::rand() % centers.size());
			m_fractal = std::make_shared<MandelbrotExtended>(center.first);
		}
		else
		{
			complex.second = glfwGetTime();
			m_fractal = std::make_shared<Julia>();
		}
	}
	hgui::TaskManager::program(std::chrono::milliseconds(50), [this] { draw_background(); });
}

void FractalExplorer::set_main_menu()
{
	hgui::TagManager::set_current_tag("main_menu");

	m_buttons.push_back(hgui::ButtonManager::create([&]
		{
			hgui::TaskManager::deprogram();
			render(FractalsType::MANDELBROT);
		}, hgui::size(20_em, 10_em), hgui::point(0), nullptr, HGUI_COLOR_BLUE, 1000, "Mandelbrot", m_font, HGUI_COLOR_WHITE));
	m_buttons.back()->set_position(hgui::point(50_em) - m_buttons.back()->get_size() / 2 + hgui::point(0, 10_em));

	m_buttons.push_back(hgui::ButtonManager::create([&]
		{
			hgui::TaskManager::deprogram();
			render(FractalsType::JULIA);
		}, hgui::size(20_em, 10_em), hgui::point(0), nullptr, HGUI_COLOR_BLUE, 1000, "Julia", m_font, HGUI_COLOR_WHITE));
	m_buttons.back()->set_position(hgui::point(50_em) - m_buttons.back()->get_size() / 2 + hgui::point(0, 25_em));

	m_texts.push_back(hgui::LabelManager::create("V 2.0", hgui::point(0), m_font,
		{ 25u, HGUI_COLOR_WHITE, 1.0f }));
	m_texts.back()->set_position(hgui::point(99_em - m_texts.back()->get_size().width, 1_em));
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
}

void FractalExplorer::set_option_menu()
{
	constexpr int BUTTON_SIZE = 100;
	constexpr int GAP = 15;
	const auto get_position = [](const int row, const int columns) -> hgui::point
		{
			return { 5_em + (columns - 1) * (BUTTON_SIZE + GAP), 5_em + (row - 1) * (BUTTON_SIZE + GAP) };
		};
	hgui::TagManager::set_current_tag("option");
	m_buttons.push_back(hgui::ButtonManager::create([&]()
		{
			render(FractalsType::SAME);
		}, hgui::size(BUTTON_SIZE), get_position(1, 1),
			hgui::TextureManager::create(hgui::image_loader("assets/textures/option.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		{
			if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE)))
			{
				hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE));
			}
			hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), hgui::end);
			if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT)))
			{
				hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::KP_ADD, hgui::actions::REPEAT));
			}
			if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::KP_SUBTRACT, hgui::actions::REPEAT)))
			{
				hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::KP_SUBTRACT, hgui::actions::REPEAT));
			}
			m_fractal = nullptr;
			start();
		}, hgui::size(BUTTON_SIZE), get_position(1, 2),
			hgui::TextureManager::create(hgui::image_loader("assets/textures/home.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		{
			hgui::end();
		}, hgui::size(BUTTON_SIZE), get_position(1, 3),
			hgui::TextureManager::create(hgui::image_loader("assets/textures/exit.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]()
		{
			hgui::TagManager::set_current_tag("fractal");
			if (auto fractal = std::dynamic_pointer_cast<Mandelbrot>(m_fractal))
			{
				m_fractal = std::make_shared<MandelbrotExtended>(fractal);
			}
			else if (auto fractal = std::dynamic_pointer_cast<Julia>(m_fractal))
			{
				m_fractal = std::make_shared<JuliaExtended>(fractal);
			}
		}, hgui::size(BUTTON_SIZE), get_position(2, 1),
			hgui::TextureManager::create(hgui::image_loader("assets/textures/zoom.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]()
		{
			hgui::TagManager::set_current_tag("fractal");
			if (auto fractal = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal))
			{
				m_fractal = std::make_shared<Mandelbrot>(fractal);
			}
			else if (auto fractal = std::dynamic_pointer_cast<JuliaExtended>(m_fractal))
			{
				m_fractal = std::make_shared<Julia>(fractal);
			}
		}, hgui::size(BUTTON_SIZE), get_position(2, 2),
			hgui::TextureManager::create(hgui::image_loader("assets/textures/perf.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]()
		{
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 0);
			}
		}, hgui::size(100), get_position(3, 1),
			hgui::TextureManager::create(hgui::image_loader("assets/textures/wave.png"))));
	m_buttons.push_back(hgui::ButtonManager::create([&]()
		{
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 1);
			}
		}, hgui::size(100), get_position(3, 2),
			nullptr, hgui::color(1.0, 0.0), static_cast<float>(INT_MAX), "HSV", m_font, HGUI_COLOR_WHITE));
	m_buttons.push_back(hgui::ButtonManager::create([&]()
		{
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 2);
			}
		}, hgui::size(100), get_position(3, 3),
			nullptr, hgui::color(1.0, 0.0), static_cast<float>(INT_MAX), "HSL", m_font, HGUI_COLOR_WHITE));
	m_buttons.push_back(hgui::ButtonManager::create([&]()
		{
			if (m_fractal)
			{
				m_fractal->get_shader()->use().set_int("color", 3);
			}
		}, hgui::size(100), get_position(3, 4),
			nullptr, hgui::color(1.0, 0.0), static_cast<float>(INT_MAX), "LCH", m_font, HGUI_COLOR_WHITE));
}

void FractalExplorer::set_special_option_menu(const FractalsType fractal)
{
	m_sliders.clear();
	m_optionTexts.clear();  
	hgui::Renderer::set_draw_callback(nullptr);
	constexpr int BUTTON_HEIGHT = 100;
	constexpr int GAP = 15;
	const auto get_position = [](const int row, const int columns) -> hgui::point
		{
			return { 5_em + (columns - 1) * (BUTTON_HEIGHT + GAP), 5_em + (row - 1) * (BUTTON_HEIGHT + GAP) };
		};
	hgui::TagManager::set_current_tag("option");
	switch (fractal)
	{
	case FractalsType::MANDELBROT:
	{
		auto function = [&]()
			{
				hgui::dvec2 center;
				double offset{2.};
				if (auto mandelbrot = std::dynamic_pointer_cast<Mandelbrot>(m_fractal))
				{
					center = mandelbrot->get_center();
					offset = mandelbrot->get_offset();
				}
				else if (auto mandelbrot = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal))
				{
					center = mandelbrot->get_center();
					offset = mandelbrot->get_offset();
				}
				std::stringstream ss;
				ss << "Center : (" << center.x << ", " << center.y << ")";
				m_optionTexts.at(0)->set_text(ss.str());
				ss.str("");
				ss << "Offset : " << offset;
				m_optionTexts.at(1)->set_text(ss.str());
			};
		m_optionTexts.push_back(hgui::LabelManager::create("Center : (-0.75, 0.00)", get_position(4, 1), m_font));
		m_optionTexts.back()->set_height(40u);
		m_optionTexts.push_back(hgui::LabelManager::create("Offset : 2", get_position(5, 1), m_font));
		m_optionTexts.back()->set_height(40u);
		hgui::Renderer::set_draw_callback(function);
	}
		break;
	case FractalsType::JULIA:
	{
		hgui::kernel::Ranges range = {
			.min = -2.f,
			.max = 2.f,
			.precision = 6u
		};
		auto function = [&]()
			{
				if (auto julia = std::dynamic_pointer_cast<Julia>(m_fractal); julia && m_sliders.size() >= 2)
				{
					julia->set_complex(hgui::vec2(m_sliders.at(0)->get_value(), m_sliders.at(1)->get_value()));
				}
				else if (auto julia = std::dynamic_pointer_cast<JuliaExtended>(m_fractal); julia && m_sliders.size() >= 2)
				{
					julia->set_complex(hgui::vec2(m_sliders.at(0)->get_value(), m_sliders.at(1)->get_value()));
				}
				std::stringstream ss;
				ss << "Re(c) = " << m_sliders.at(0)->get_value();
				m_optionTexts.at(0)->set_text(ss.str());
				ss.str("");
				ss << "Im(c) = " << m_sliders.at(1)->get_value();
				m_optionTexts.at(1)->set_text(ss.str());
			};
		m_optionTexts.push_back(hgui::LabelManager::create("Re(c) = -2", get_position(4, 1) + hgui::point(0, 10), m_font));
		m_optionTexts.back()->set_height(40u);
		m_sliders.push_back(hgui::SliderManager::create(range, hgui::size(20_em, BUTTON_HEIGHT / 2.f), get_position(4, 1) + hgui::point(m_texts.back()->get_size().width + 15 * GAP, 0), hgui::color(66) / 255.f, hgui::color(9, 127, 224) / 255.f, HGUI_COLOR_WHITE, function));
		m_optionTexts.push_back(hgui::LabelManager::create("Im(c) = -2", get_position(5, 1) + hgui::point(0, 10), m_font));
		m_optionTexts.back()->set_height(40u);
		m_sliders.push_back(hgui::SliderManager::create(range, hgui::size(20_em, BUTTON_HEIGHT / 2.f), get_position(5, 1) + hgui::point(m_texts.back()->get_size().width + 15 * GAP, 0), hgui::color(66) / 255.f, hgui::color(9, 127, 224) / 255.f, HGUI_COLOR_WHITE, function));
	}
	break;
	case FractalsType::SAME:
		break;
	default:
		break;
	}
}
