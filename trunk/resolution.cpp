#include "resolution.hpp"

/// Устанавливает значения по умолчанию
CResolutionManager::CResolutionManager()
{
	setValues( DEFAULT_RESOLUTION, DEFAULT_DEPTH, DEFAULT_FULLSCREEN );
	screen = NULL;
}

CResolutionManager::~CResolutionManager() 
{
	if( screen != NULL )			
		SDL_FreeSurface( screen );	
}


bool CResolutionManager::setMode( Resolution res, int depth, bool fscreen )
{
	return setValues( res, depth, fscreen ) && setMode();
}

bool CResolutionManager::setValues( Resolution res, int depth, bool fscreen )
{
	resolution = res;
	colorDepth = depth;
	fullscreen = fscreen;
	// TODO: сюда можно вставить проверку на существование режима и прочие радости.
	
	
	//выдираем заранее заготовленные значения
	width				= resolution_metrics[res][0];
	height				= resolution_metrics[res][1];
	gameFieldOriginX 	= resolution_metrics[res][2];
	gameFieldOriginY 	= resolution_metrics[res][3];
	gameFieldWidth		= resolution_metrics[res][4];
	gameFieldHeight		= resolution_metrics[res][5];
	
	scaleX = ((float)width)/BASE_XRES;
	scaleY = ((float)height)/BASE_YRES;
	
	return true;	
}

bool CResolutionManager::setMode()
{
	if( screen != NULL )			// Освобождаем во избежание утечек
		SDL_FreeSurface( screen );	// Правда, неведомо, будут ли эти утечки.
		
	screen = SDL_SetVideoMode(width, height, colorDepth, SDL_OPENGL | (SDL_FULLSCREEN * (int)fullscreen));
	if( !screen ) 
	{
		std::cerr << "Failed to set up resolution" << std::endl;
		return false;		
	}
	
	SDL_ShowCursor(SDL_DISABLE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	
	
	SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 ); 	// врубаем vsync; needs testing
	
	
	return true;
}

bool CResolutionManager::toggleFullscreen()
{
	fullscreen = !fullscreen;
	return (bool) ( SDL_WM_ToggleFullScreen( screen ) );
}
