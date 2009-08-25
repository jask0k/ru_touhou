-- level1.lua
-- Скрипт первого уровня

-- Инициализация переменных
lives = 5;
graze = 0;
power = 0;
god_mode = 0;

-- Всякие загрузки-шмагрузки
spritesheet_load("level1.png");
log("Starting first level!");
wait(1); -- Подождать кадр

logo = sprite_create("level1.png",LAYER_EMBLEM); -- Создание спрайта на слое переднего плана
sprite_set_scale(logo,.5); -- Установка масштабирования спрайта
sprite_set_position(logo,GAME_FIELD_WIDTH/2-10,GAME_FIELD_HEIGHT/2,0); -- Установка положения спрайта
sprite_set_speed(logo,0.1,0,0); -- Установка скорости передвижения
sprite_set_alpha(logo,0.0); -- Установка прозрачности логотипа
sprite_set_alpha_speed(logo,0.01); -- Сгущаем логотип 
wait(100); -- Ждём
sprite_set_alpha_speed(logo,-0.01); -- Разгущаем его
wait(130); -- Ещё ждём
for j = 0, 100 do
   for i= -2,2,1 do
      for k = 40, 340, 100 do
	 sprite = enbullet_create_hero(69,k,300,3,i*5); -- Пускаем пули
      end
   end
   wait(30);
end
