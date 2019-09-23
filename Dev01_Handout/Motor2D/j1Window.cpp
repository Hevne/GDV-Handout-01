#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"

#include "SDL/include/SDL.h"


j1Window::j1Window() : j1Module()
{
	window = NULL;
	screen_surface = NULL;
	name.create("window");
}

// Destructor
j1Window::~j1Window()
{
}

// Called before render is available
bool j1Window::Awake(pugi::xml_node* n)
{
	//Eudald 6: Definim un string sobre el qual carregarem l'atribut de title del xml 
	p2SString title; 

	if(n!=nullptr) {	//Eudald 7: Comprovem que el node que sobre el que s'ha carregat el document estigui funcionant correctament com a cursor a algún element del xml, en aquest cas a <config>
		if (!n->child("modules").child(name.GetString()).empty())	//Eudald 8: Comprovem que existeix l'element <modules> com a fill de <config> dins de l'xml i que aquest té un fill anomenat com el mòdul finestra: "window" declarat més amunt
		{
			node = &n->child("modules").child(name.GetString());	//Eudald 9: Coloquem el node del modul com a cursor sobre el child de l'element <modules> anomenat <window> de l'xml
																	//Eudald 10: Fem que l'string title prengui el valor del child de <window> anomenat <TITLE> accedint a un dels seus atributs i obtenint el valor de l'atribut "value"
		}

	LOG("Init SDL window & surface");
	bool ret = true;



	title.create(node->child("TITLE").attribute("value").value());
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{

		//Create window

		Uint32 flags = SDL_WINDOW_SHOWN;



		xml_width = node->child("WIDTH").attribute("value").as_int();
		xml_height = node->child("HEIGHT").attribute("value").as_int();
		xml_scale = node->child("SCALE").attribute("value").as_int();

		xml_fullscreen = node->child("FULLSCREEN").attribute("value").as_bool();
		xml_borderless = node->child("BORDERLESS").attribute("value").as_bool();
		xml_resizable = node->child("RESIZABLE").attribute("value").as_bool();
		xml_fullscreen_window = node->child("FULLSCREEN_WINDOW").attribute("value").as_bool();



		if (xml_fullscreen)

		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (xml_borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (xml_resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (xml_fullscreen_window)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		//TODO 7: Move "Todo 4" code to the awake method on the window module
		//Pass the title as a variable when creating the window

		window = SDL_CreateWindow(title.GetString(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, xml_width, xml_height, flags);
		//App->win->SetTitle(node->child("modules").child("window").child("title").child_value());
		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	return ret;

	}
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	SDL_SetWindowTitle(window, new_title);
}

void j1Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->xml_width;
	height = this->xml_height;
}

uint j1Window::GetScale() const
{
	return xml_scale;
}