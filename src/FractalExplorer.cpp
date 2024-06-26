#include "../include/FractalExplorer.h"
#include "../include/Mandelbrot.h"
#include "../include/Julia.h"
#include "../include/BurningShip.h"
#include "../include/Newton.h"
#include "../include/MandelbrotExtended.h"
#include "../include/JuliaExtended.h"
#include "../include/BurningShipExtended.h"
#include "../include/NewtonExtended.h"

FractalExplorer::FractalExplorer() : m_window(nullptr),
                                     m_monitor(nullptr),
                                     m_font(nullptr),
                                     m_titleFont(nullptr),
                                     m_fractal(nullptr),
                                     m_inMainMenu(true)
{
	// Library initialization //
	hgui::init();
	// Window creation //
	m_monitor = hgui::MonitorManager::get_primary_monitor();
	m_window = hgui::WindowManager::create("FractalExplorer", m_monitor->get_size(),
	                                       hgui::point(0), hgui::image_loader("assets/textures/logo.png"), nullptr, {hgui::WindowOption(hgui::options::MAXIMIZED, true)});
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
	hgui::kernel::Widget::active({"main_menu"});
	hgui::Renderer::draw({"main_menu"});
	hgui::Renderer::draw({"background"}, hgui::effects::BLURRED);
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
	case FractalsType::BURNINGSHIP:
		m_fractal = std::static_pointer_cast<Fractal>(std::make_shared<BurningShip>());
		set_special_option_menu(fractal);
		break;
	case FractalsType::NEWTON:
		m_fractal = std::static_pointer_cast<Fractal>(std::make_shared<Newton>());
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
	hgui::kernel::Widget::active({"fractal"});
	hgui::Renderer::draw({"fractal"});
	hgui::Renderer::draw({HGUI_TAG_MAIN}, hgui::effects::BLURRED);
}

void FractalExplorer::option_menu()
{
	if (hgui::KeyBoardManager::is_bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE)))
	{
		hgui::KeyBoardManager::unbind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE));
	}
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::RELEASE), [&] { render(FractalsType::SAME); });
	hgui::kernel::Widget::active({"option"});
	hgui::Renderer::draw({"option"});
	hgui::Renderer::draw({"fractal"}, hgui::effects::BLURRED);
}

void FractalExplorer::draw_background()
{
	hgui::TagManager::set_current_tag("background");
	static const std::vector mandelbrotCenters = {
				std::make_pair(hgui::dvec2(0.29803728643142374244, -0.017892934639849415734), 1e-09),
				std::make_pair(hgui::dvec2(0.27236526368881613935, -0.0051226242725265431002), 1e-10),
				std::make_pair(hgui::dvec2(-1.8100958106935862713, -1.2115589437016044164e-12), 1e-11),
				std::make_pair(hgui::dvec2(-0.27977201929735756591, -0.83911902078682876205), 1e-9),
				std::make_pair(hgui::dvec2(-0.77116049982246182015, 0.11535252316027029551), 1e-9),
				std::make_pair(hgui::dvec2(-0.13498701313338801189, 1.000848937830046026), 1e-11)
			};
	static const std::vector burningShipCenters = {
				std::make_pair(hgui::dvec2(-1.3863144447250430602, -0.35540827807523550641), 1e-13),
				std::make_pair(hgui::dvec2(-1.7734196606593537471, -0.041480450372869973052), 1e-12),
				std::make_pair(hgui::dvec2(0.26598765732655199612, -0.25063094727651130667), 1e-05),
				std::make_pair(hgui::dvec2(0.86603598504612222531, -1.5713711680279656147), 1e-13)
			};
	static std::pair<hgui::dvec2, double> center;
	static std::pair<hgui::vec2, double> complex;
	if (const auto mandelbrotExtended = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal); mandelbrotExtended && mandelbrotExtended->m_offset >= center.second)
	{
		mandelbrotExtended->m_offset *= 0.95;
		mandelbrotExtended->m_shader->use().set_double("offset", mandelbrotExtended->m_offset);
	}
	else if (const auto burningShipExtended = std::dynamic_pointer_cast<BurningShipExtended>(m_fractal); burningShipExtended && burningShipExtended->m_offset >= center.second)
	{
		burningShipExtended->m_offset *= 0.95;
		burningShipExtended->m_shader->use().set_double("offset", burningShipExtended->m_offset);
	}
	else if (const auto julia = std::dynamic_pointer_cast<Julia>(m_fractal); julia && glfwGetTime() - complex.second <= 30.)
	{
		const double time = fmod(0.5 * glfwGetTime(), 2. * glm::pi<double>());
		complex.first = 0.7885f * hgui::vec2(static_cast<float>(glm::cos(time)), static_cast<float>(glm::sin(time)));
		julia->set_complex(complex.first);
	}
	else
	{
		if (hgui::random(1. / 3.))
		{
			center = mandelbrotCenters.at(std::rand() % mandelbrotCenters.size());
			m_fractal = std::make_shared<MandelbrotExtended>(center.first);
		}
		else if (hgui::random(1. / 3.))
		{
			center = burningShipCenters.at(std::rand() % burningShipCenters.size());
			m_fractal = std::make_shared<BurningShipExtended>(center.first);
		}
		else
		{
			complex.second = glfwGetTime();
			m_fractal = std::make_shared<Julia>();
		}
	}
	hgui::TaskManager::program(std::chrono::milliseconds(50), [&] { draw_background(); });
}

void FractalExplorer::set_main_menu()
{
	hgui::TagManager::set_current_tag("main_menu");

	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                hgui::TaskManager::deprogram();
			                                                m_inMainMenu = false;
			                                                render(FractalsType::MANDELBROT);
		                                                }, hgui::size(25_em, 10_em), hgui::point(0), nullptr, HGUI_COLOR_BLUE, 100, true, "Mandelbrot", m_font, HGUI_COLOR_WHITE));
	m_buttons.back()->set_position(hgui::point(50_em, 40_em) - m_buttons.back()->get_size() / 2 + hgui::point(0, 12.5_em));

	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                hgui::TaskManager::deprogram();
			                                                m_inMainMenu = false;
			                                                render(FractalsType::JULIA);
		                                                }, hgui::size(25_em, 10_em), hgui::point(0), nullptr, HGUI_COLOR_BLUE, 100, true, "Julia", m_font, HGUI_COLOR_WHITE));
	m_buttons.back()->set_position(hgui::point(50_em, 40_em) - m_buttons.back()->get_size() / 2 + hgui::point(0, 25_em));

	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                hgui::TaskManager::deprogram();
			                                                m_inMainMenu = false;
			                                                render(FractalsType::BURNINGSHIP);
		                                                }, hgui::size(25_em, 10_em), hgui::point(0), nullptr, HGUI_COLOR_BLUE, 100, true, "Burning Ship", m_font, HGUI_COLOR_WHITE));
	m_buttons.back()->set_position(hgui::point(50_em, 40_em) - m_buttons.back()->get_size() / 2 + hgui::point(0, 37.5_em));

	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                hgui::TaskManager::deprogram();
			                                                m_inMainMenu = false;
			                                                render(FractalsType::NEWTON);
		                                                }, hgui::size(25_em, 10_em), hgui::point(0), nullptr, HGUI_COLOR_BLUE, 100, true, "Newton", m_font, HGUI_COLOR_WHITE));
	m_buttons.back()->set_position(hgui::point(50_em, 40_em) - m_buttons.back()->get_size() / 2 + hgui::point(0, 50_em));

	const auto version = hgui::LabelManager::create("V 4.0", hgui::point(0), m_font);
	m_texts.push_back(version);
	const auto title = hgui::LabelManager::create("FractalExplorer", hgui::point(0), m_titleFont, false, {200u, HGUI_COLOR_BLUE, 1.0f});
	m_texts.push_back(title);
	const auto txt1 = hgui::LabelManager::create("Choose the fractal you want", hgui::point(0), m_font);
	m_texts.push_back(txt1);
	const auto txt2 = hgui::LabelManager::create("to explore.", hgui::point(0), m_font);
	m_texts.push_back(txt2);
	m_callback = [=, this]
		{
			version->set_height(static_cast<unsigned>(hgui::size(3_em).height));
			txt1->set_height(static_cast<unsigned>(hgui::size(4_em).height));
			txt1->set_position(hgui::point(50_em) - txt1->get_size() / 2 - hgui::point(0, 12.5_em));
			txt2->set_height(static_cast<unsigned>(hgui::size(4_em).height));
			txt2->set_position(hgui::point(50_em) - txt2->get_size() / 2 - hgui::point(0, 7.5_em));
			version->set_position(hgui::point(99_em - version->get_size().width, 1_em));
			title->set_height(static_cast<unsigned>(hgui::size(25_em).height));
			title->set_position(hgui::point(50_em) - title->get_size() / 2 - hgui::point(0, 30_em));
			if (m_inMainMenu)
				hgui::TagManager::set_current_tag("background");
			else
				hgui::TagManager::set_current_tag("fractal");
			if (const auto mandelbrot = std::dynamic_pointer_cast<Mandelbrot>(m_fractal))
			{
				m_fractal = std::make_shared<Mandelbrot>(*mandelbrot);
			}
			else if (const auto mandelbrotExtended = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal))
			{
				m_fractal = std::make_shared<MandelbrotExtended>(*mandelbrotExtended);
			}
			else if (const auto julia = std::dynamic_pointer_cast<Julia>(m_fractal))
			{
				m_fractal = std::make_shared<Julia>(*julia);
			}
			else if (const auto juliaExtended = std::dynamic_pointer_cast<JuliaExtended>(m_fractal))
			{
				m_fractal = std::make_shared<JuliaExtended>(*juliaExtended);
			}
			else if (const auto burningShip = std::dynamic_pointer_cast<BurningShip>(m_fractal))
			{
				m_fractal = std::make_shared<BurningShip>(*burningShip);
			}
			else if (const auto burningShipExtended = std::dynamic_pointer_cast<BurningShipExtended>(m_fractal))
			{
				m_fractal = std::make_shared<BurningShipExtended>(*burningShipExtended);
			}
			else if (const auto newton = std::dynamic_pointer_cast<Newton>(m_fractal))
			{
				m_fractal = std::make_shared<Newton>(*newton);
			}
			else if (const auto newtonExtended = std::dynamic_pointer_cast<NewtonExtended>(m_fractal))
			{
				m_fractal = std::make_shared<NewtonExtended>(*newtonExtended);
			}
		};
	m_callback();
	m_window->set_size_callback(m_callback);
}

void FractalExplorer::set_option_menu()
{
	hgui::TagManager::set_current_tag("option");
	const auto BUTTON_SIZE = hgui::point(10_em).set_reference(hgui::reference::HEIGHT);
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                render(FractalsType::SAME);
		                                                }, hgui::size(BUTTON_SIZE), grid(1, 1),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/option.png")), HGUI_COLOR_WHITE, 50));
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
			                                                m_inMainMenu = true;
			                                                hgui::Renderer::set_draw_callback(nullptr);
			                                                start();
		                                                }, hgui::size(BUTTON_SIZE), grid(1, 2),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/home.png")), HGUI_COLOR_WHITE, 50.f));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                hgui::end();
		                                                }, hgui::size(BUTTON_SIZE), grid(1, 3),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/exit.png")), HGUI_COLOR_WHITE, 50.f));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                hgui::TagManager::set_current_tag("fractal");
			                                                if (auto mandelbrot = std::dynamic_pointer_cast<Mandelbrot>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<MandelbrotExtended>(mandelbrot);
			                                                }
			                                                else if (auto julia = std::dynamic_pointer_cast<Julia>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<JuliaExtended>(julia);
			                                                }
			                                                else if (auto burningShip = std::dynamic_pointer_cast<BurningShip>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<BurningShipExtended>(burningShip);
			                                                }
			                                                else if (auto newton = std::dynamic_pointer_cast<Newton>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<NewtonExtended>(newton);
			                                                }
		                                                }, hgui::size(BUTTON_SIZE), grid(2, 1),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/zoom.png")), HGUI_COLOR_WHITE, 50.f));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                hgui::TagManager::set_current_tag("fractal");
			                                                if (auto mandelbrotExtended = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<Mandelbrot>(mandelbrotExtended);
			                                                }
			                                                else if (auto juliaExtended = std::dynamic_pointer_cast<JuliaExtended>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<Julia>(juliaExtended);
			                                                }
			                                                else if (auto burningShipExtended = std::dynamic_pointer_cast<BurningShipExtended>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<BurningShip>(burningShipExtended);
			                                                }
			                                                else if (auto newtonExtended = std::dynamic_pointer_cast<NewtonExtended>(m_fractal))
			                                                {
				                                                m_fractal = std::make_shared<Newton>(newtonExtended);
			                                                }
		                                                }, hgui::size(BUTTON_SIZE), grid(2, 2),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/perf.png")), HGUI_COLOR_WHITE, 50.f));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                if (m_fractal)
			                                                {
				                                                m_fractal->get_shader()->use().set_int("color", 0);
			                                                }
		                                                }, hgui::size(BUTTON_SIZE), grid(3, 1),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/star.png")), HGUI_COLOR_WHITE, 50.f));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                if (m_fractal)
			                                                {
				                                                m_fractal->get_shader()->use().set_int("color", 1);
			                                                }
		                                                }, hgui::size(BUTTON_SIZE), grid(3, 2),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/wave.png")), HGUI_COLOR_WHITE, 50.f));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                if (m_fractal)
			                                                {
				                                                m_fractal->get_shader()->use().set_int("color", 2);
			                                                }
		                                                }, hgui::size(BUTTON_SIZE), grid(3, 3),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/water.png")), HGUI_COLOR_WHITE, 50.f));
	m_buttons.push_back(hgui::ButtonManager::create([&]
		                                                {
			                                                if (m_fractal)
			                                                {
				                                                m_fractal->get_shader()->use().set_int("color", 3);
			                                                }
		                                                }, hgui::size(BUTTON_SIZE), grid(3, 4),
	                                                hgui::TextureManager::create(hgui::image_loader("assets/textures/rainbow.png")), HGUI_COLOR_WHITE, 50.f));
}

void FractalExplorer::set_special_option_menu(const FractalsType fractal)
{
	m_sliders.clear();
	m_optionTexts.clear();
	hgui::Renderer::set_draw_callback(nullptr);
	hgui::TagManager::set_current_tag("option");
	const auto BUTTON_HEIGHT = hgui::size(hgui::size(15_em).set_reference(hgui::reference::HEIGHT)).em_height;
	switch (fractal)
	{
	case FractalsType::BURNINGSHIP:
		[[fallthrough]];
	case FractalsType::NEWTON:
		[[fallthrough]];
	case FractalsType::MANDELBROT:
		{
			auto function = [&]
				{
					if (m_optionTexts.size() >= 2)
					{
						hgui::dvec2 center;
						double offset = 2.;
						if (const auto mandelbrot = std::dynamic_pointer_cast<Mandelbrot>(m_fractal))
						{
							center = mandelbrot->get_center();
							offset = mandelbrot->get_offset();
						}
						else if (const auto mandelbrotExtended = std::dynamic_pointer_cast<MandelbrotExtended>(m_fractal))
						{
							center = mandelbrotExtended->get_center();
							offset = mandelbrotExtended->get_offset();
						}
						else if (const auto burningShip = std::dynamic_pointer_cast<BurningShip>(m_fractal))
						{
							center = burningShip->get_center();
							offset = burningShip->get_offset();
						}
						else if (const auto burningShipExtended = std::dynamic_pointer_cast<BurningShipExtended>(m_fractal))
						{
							center = burningShipExtended->get_center();
							offset = burningShipExtended->get_offset();
						}
						else if (const auto newton = std::dynamic_pointer_cast<Newton>(m_fractal))
						{
							center = newton->get_center();
							offset = newton->get_offset();
						}
						else if (const auto newtonExtended = std::dynamic_pointer_cast<NewtonExtended>(m_fractal))
						{
							center = newtonExtended->get_center();
							offset = newtonExtended->get_offset();
						}
						std::stringstream ss;
						ss << "Center : " << center;
						m_optionTexts.at(0)->set_text(ss.str());
						ss.str("");
						ss << "Offset : " << offset;
						m_optionTexts.at(1)->set_text(ss.str());
					}
				};
			const auto center = hgui::LabelManager::create("Center : (-0.75, 0.00)", grid(4, 1), m_font, false);
			m_optionTexts.push_back(center);
			const auto offset = hgui::LabelManager::create("Offset : 2", grid(5, 1) - hgui::point(0, 5_em), m_font, false);
			m_optionTexts.push_back(offset);
			auto callback = [=, this]
				{
					m_callback();
					center->set_height(static_cast<unsigned>(hgui::size(5_em).height));
					offset->set_height(static_cast<unsigned>(hgui::size(5_em).height));
				};
			hgui::Renderer::set_draw_callback(function);
			callback();
			m_window->set_size_callback(callback);
		}
		break;
	case FractalsType::JULIA:
		{
			constexpr hgui::kernel::Ranges range = {
						.min = -2.f,
						.max = 2.f,
						.precision = 6u
					};
			auto function = [&]
				{
					if (m_optionTexts.size() >= 2)
					{
						if (const auto julia = std::dynamic_pointer_cast<Julia>(m_fractal); julia && m_sliders.size() >= 2)
						{
							julia->set_complex(hgui::vec2(m_sliders.at(0)->get_value(), m_sliders.at(1)->get_value()));
						}
						else if (const auto juliaExtended = std::dynamic_pointer_cast<JuliaExtended>(m_fractal); juliaExtended && m_sliders.size() >= 2)
						{
							juliaExtended->set_complex(hgui::vec2(m_sliders.at(0)->get_value(), m_sliders.at(1)->get_value()));
						}
						std::stringstream ss;
						ss << "Re(c) = " << m_sliders.at(0)->get_value();
						m_optionTexts.at(0)->set_text(ss.str());
						ss.str("");
						ss << "Im(c) = " << m_sliders.at(1)->get_value();
						m_optionTexts.at(1)->set_text(ss.str());
					}
				};
			const auto re = hgui::LabelManager::create("Re(c) = 0", grid(4, 1) + hgui::point(0, BUTTON_HEIGHT / 9.f), m_font, false);
			m_optionTexts.push_back(re);
			m_sliders.push_back(hgui::SliderManager::create(range, hgui::size(20_em, BUTTON_HEIGHT / 3.f), grid(5, 2) + hgui::point(0, BUTTON_HEIGHT / 8.f), hgui::color("424242"), hgui::color("#097fe0"), HGUI_COLOR_WHITE, function));
			m_sliders.back()->set_value(0.f);
			const auto im = hgui::LabelManager::create("Im(c) = 0", grid(6, 1) + hgui::point(0, BUTTON_HEIGHT / 9.f), m_font, false);
			m_optionTexts.push_back(im);
			m_sliders.push_back(hgui::SliderManager::create(range, hgui::size(20_em, BUTTON_HEIGHT / 3.f), grid(7, 2) + hgui::point(0, BUTTON_HEIGHT / 8.f), hgui::color("424242"), hgui::color("#097fe0"), HGUI_COLOR_WHITE, function));
			m_sliders.back()->set_value(0.f);
			auto callback = [=, this]
				{
					m_callback();
					im->set_height(static_cast<unsigned>(hgui::size(5_em).height));
					re->set_height(static_cast<unsigned>(hgui::size(5_em).height));
				};
			callback();
			m_window->set_size_callback(callback);
		}
		break;
	default: ;
	}
}

hgui::point FractalExplorer::grid(const int row, const int columns)
{
	const auto BUTTON_SIZE = hgui::point(10_em).set_reference(hgui::reference::HEIGHT);
	const auto GAP = hgui::point(1_em).set_reference(hgui::reference::HEIGHT);
	hgui::point pos = GAP;
	pos.em_x += (columns - 1) * hgui::point(BUTTON_SIZE + GAP).em_x;
	pos.em_y += (row - 1) * hgui::point(BUTTON_SIZE + GAP).em_y;
	return pos;
}
