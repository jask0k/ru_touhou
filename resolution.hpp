#ifndef RESOLUTION_HPP
#define RESOLUTION_HPP

#include <SDL/SDL.h> 
#include <iostream>

#include "resmetrics.hpp" 		//сюда забиты всякие размеры экрана и проч.

#define DEFAULT_RESOLUTION RES_640x480
#define DEFAULT_DEPTH 32
#define DEFAULT_FULLSCREEN false

#define BASE_XRES 640
#define BASE_YRES 480
#define BASE_ORIGIN_X 32
#define BASE_ORIGIN_Y 16
#define BASE_GAME_FIELD_WIDTH 384			// TODO: совместить со значениями из конфига
#define BASE_GAME_FIELD_HEIGHT 448



// Список доступных разрешений
enum Resolution
{
	RES_640x480 = 0, RES_800x600, RES_1024x768, RES_1152x864, RES_1280x960, RES_1600x1200, 		// 4:3
	RES_854x480, RES_1280x720, RES_1600x900, RES_1920x1080,										// 16:9
	RES_768x480, RES_1440x900, RES_1680x1050, RES_1920x1200,									// 16:10
	/* RES_720x576, RES_1280x1024,																	// 5:4
	RES_1024x600, */															
	RES_CUSTOM
};


/* Функции класса:
 * 1. Инкапсуляция данных о видеорежиме
 * 2. Пересчет размера и позиции игрового поля при смене разрешения
 * 3. Смена видеорежима
 * 4. Контроль доступных режимов  --  TODO
 *  
 */
class CResolutionManager
{
  private:
  	int		xres, yres;			// фактическое разрешение; необходимо как минимум для получения скриншотов.
  
  	int		width, height;						// размеры вьюпорта
  	int 	colorDepth;							// глубина цвета
  	int		gameFieldOriginX, gameFieldOriginY;	// координаты нижней левой точки игрового поля в пикселях
  	int		gameFieldHeight, gameFieldWidth;	// размеры игровой области, в пикселях;
  												// используются сугубо для установки viewport'а
  												
  	bool	fullscreen;							// true=полноэкранный режим; false=оконный
	bool	widescreen;							// true=широкоформатный режим 
	int		borderWidth;						// ширина в пикселях полосы слева (и справа) от игрового 4:3 прямоугольника в широкоформатнике

  	Resolution	resolution;						// Разрешение игры
  	
	float	scaleX, scaleY;						// Факторы расятжения по координатным осям
												// Обычно они равны, но мало ли...

	SDL_Surface*	screen;
  public:
  	
  	CResolutionManager();
  	~CResolutionManager();

  	bool setValues( Resolution res, int depth, bool fscreen ); 	// Устанавливает значения, но не меняет режим
  	bool setCustomValues( int w, int h, int depth, bool fscreen, bool wscreen );	// Позволяет выставить особое, уличное разрешение
  	
  	bool setMode();												// Устанавливает видеорежим в соответствии с текущими значениями
	bool setMode( Resolution res, int depth, bool fscreen );	// Устанавливает видеорежим
	
  	bool toggleFullscreen();
  	
  	Resolution getResolution()	{ return resolution; }
	bool isWidescreen()	{ return widescreen; }
	bool isFullscreen() { return fullscreen; }
  	
  	int getXRes()		{ return xres; }
  	int getYRes()		{ return yres; }
  	int getWidth() 		{ return width; }
  	int getHeight()		{ return height; }
  	int getDepth()		{ return colorDepth; }
  	int getOriginX()	{ return gameFieldOriginX; }
  	int getOriginY()	{ return gameFieldOriginY; }
  	int getGFHeight()	{ return gameFieldHeight; }
  	int getGFWidth()	{ return gameFieldWidth; }
  	int getBorderW()	{ return borderWidth; }
  	float getScaleX()	{ return scaleX; }
  	float getScaleY()	{ return scaleY; }
  	
};


#endif

