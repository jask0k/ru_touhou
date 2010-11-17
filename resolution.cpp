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
	// TODO: сюда можно вставить проверку на существование режима и прочие радости.
	if( res >= RES_CUSTOM || res <= 0 )
	{
#ifdef DEBUG
		std::cerr << "Wrong resolution" << std::endl;
#endif
		return false;			// обрабатывается отдельным методом
	}
	
	resolution = res;
	colorDepth = depth;
	fullscreen = fscreen;	
	
	//выдираем заранее заготовленные значения
	width				= resolution_metrics[res][0];
	height				= resolution_metrics[res][1];
	gameFieldOriginX 	= resolution_metrics[res][2];
	gameFieldOriginY 	= resolution_metrics[res][3];
	gameFieldWidth		= resolution_metrics[res][4];
	gameFieldHeight		= resolution_metrics[res][5];
	borderWidth			= resolution_metrics[res][6];
	
	widescreen = ( borderWidth != 0 );
	
	if( widescreen ) std::cout << "Gone wide" << std::endl;
	
	xres = width + 2 * borderWidth;
	yres = height;
	
	scaleX = ((float)width)/BASE_XRES;
	scaleY = ((float)height)/BASE_YRES;
	
	return true;	
}

// Для любителей (или обладателей) чего покастомнее
// Заодно источник формул для любопытствующих
bool CResolutionManager::setCustomValues( int w, int h, int depth, bool fscreen, bool wscreen )
{
	resolution = RES_CUSTOM;
	colorDepth = depth;
	fullscreen = fscreen;
	widescreen = wscreen;
	
	height = h;
	if( wscreen ) 
	{
		width = h * 4 / 3;
		borderWidth = (w - width) / 2;
	}
	else 
	{
		width = w;
		borderWidth = 0;
	}
	
	xres = w;
	yres = height;
	
	scaleX = ((float)width)/BASE_XRES;
	scaleY = ((float)height)/BASE_YRES;
	
	gameFieldOriginX 	= (int) ((float)BASE_ORIGIN_X * scaleX);
	gameFieldOriginY 	= (int) ((float)BASE_ORIGIN_Y * scaleY);
	gameFieldWidth 		= (int) ((float)BASE_GAME_FIELD_WIDTH *  scaleX);
	gameFieldHeight		= (int) ((float)BASE_GAME_FIELD_HEIGHT * scaleY);
	
	
	return true;
}


bool CResolutionManager::setMode()
{
	if( screen != NULL )			// Освобождаем во избежание утечек
		SDL_FreeSurface( screen );	// Правда, неведомо, будут ли эти утечки.
		
	screen = SDL_SetVideoMode(xres, yres, colorDepth, SDL_OPENGL | (SDL_FULLSCREEN * (int)fullscreen));
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
