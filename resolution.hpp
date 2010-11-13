#ifndef RESOLUTION_HPP
#define RESOLUTION_HPP

#include <SDL/SDL.h> 
#include <iostream>

#include "resmetrics.hpp" 		//сюда забиты всякие размеры экрана и проч.

#define DEFAULT_RESOLUTION RES_640
#define DEFAULT_DEPTH 32
#define DEFAULT_FULLSCREEN false

#define BASE_XRES 640
#define BASE_YRES 480



/// Список доступных разрешений
enum Resolution
{
	RES_640 = 0, RES_800, RES_1024, RES_1280, RES_1600 		//TODO: add widescreen
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
  	int		width, height;						// размеры окна
  	int 	colorDepth;							// глубина цвета
  	int		gameFieldOriginX, gameFieldOriginY;	// координаты нижней левой точки игрового поля в пикселях
  	int		gameFieldHeight, gameFieldWidth;	// размеры игровой области, в пикселях;
  												// используются сугубо для установки viewport'а
  												
  	bool	fullscreen;							// true=полноэкранный режим; false=оконный
	bool	widescreen;							// true=широкоформатный режим 

  	Resolution	resolution;						// Разрешение игры
  	
	float	scaleX, scaleY;						// Факторы расятжения по координатным осям
												// Обычно они равны, но мало ли...

	SDL_Surface*	screen;
  public:
  	
  	CResolutionManager();
  	~CResolutionManager();
  	
  	bool setMode( Resolution res, int depth, bool fscreen );	// Устанавливает видеорежим


  	bool setValues( Resolution res, int depth, bool fscreen ); 	// Устанавливает значения, но не меняет режим
  	bool setMode();												// Устанавливает видеорежим в соответствии с текущими значениями
  	// костыль для совместимости с чтением конфига
  	// to be unsawn later
  	// or not
  	bool toggleFullscreen();
  	
  	int getWidth() 		{ return width; }
  	int getHeight()		{ return height; }
  	int getDepth()		{ return colorDepth; }
  	int getOriginX()	{ return gameFieldOriginX; }
  	int getOriginY()	{ return gameFieldOriginY; }
  	int getGFHeight()	{ return gameFieldHeight; }
  	int getGFWidth()	{ return gameFieldWidth; }
  	float getScaleX()	{ return scaleX; }
  	float getScaleY()	{ return scaleY; }
  	
};


#endif

