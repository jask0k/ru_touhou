-- level1.lua
-- Скрипт первого уровня

-- Инициализация переменных
lives = 5;
graze = 0;
power = 0;
god_mode = 0;

function lantern_AI(sprite, table)
   sprite_set_position(sprite,-31,GAME_FIELD_HEIGHT-100,0);
   sprite_set_speed(sprite,1,0,0);
   sprite_start_animation(sprite,1,1);
   wait(32);
   while true do
      local her_xpos = hero_x();
      local her_ypos = hero_y();
      local i
      for i = 1, 5 do
	 local lan_xpos,lan_ypos
	 lan_xpos,lan_ypos = sprite_get_position(sprite)
	 local bullet = enbullet_create_target(0,lan_xpos,lan_ypos,4,her_xpos,her_ypos,0);
	 wait(10);
      end
      wait(10);
   end
end

function sample_AI(bullet, table) -- исскуственный интеллект для управления спрайтами
-- в первом параметре функции - хендл врага, управляемого интеллектом
-- во втором - таблица, которую выдают каждому управляемому объекту
   while true do
      enbullet_stray(bullet,5);
      wait(1);
   end
end

function sample_AI2(bullet, table)
   while true do
      enbullet_lock_on_hero(bullet,0,3)
      wait(60)
      enbullet_stop(bullet)
      wait(60)

   end
end

-- Всякие загрузки-шмагрузки
spritesheet_load("level1.png");
spritesheet_load("lantern.png");
log("Starting first level!");
wait(1); -- Подождать кадр

background_set_fog_density(.6,-0.005);

logo = sprite_create("level1.png",LAYER_EMBLEM); -- Создание спрайта на слое переднего плана
sprite_set_scale(logo,.5); -- Установка масштабирования спрайта
sprite_set_position(logo,GAME_FIELD_WIDTH/2-10,GAME_FIELD_HEIGHT/2,0); -- Установка положения спрайта
sprite_set_speed(logo,0.1,0,0); -- Установка скорости передвижения
sprite_set_alpha(logo,0.0); -- Установка прозрачности логотипа
sprite_set_alpha_speed(logo,0.01); -- Сгущаем логотип 
wait(100); -- Ждём
background_set_fog_density(.1,0);

sprite_set_alpha_speed(logo,-0.01); -- Разгущаем его
wait(130); -- Ещё ждём


for i = 1,5 do
   lantern = sprite_create("lantern.png",LAYER_ENEMY);
   bind_AI(CONTROL_SPRITE,lantern,lantern_AI);
   wait(60);
end

--[[
for k = 40, 340, 100 do
   sprite = enbullet_create(69,k,300,3,0); -- Пускаем пули
   bind_AI(CONTROL_BULLET,sprite,sample_AI);
--   sprite = enbullet_create(69,k,300,3,0); -- Пускаем пули
--   bind_AI(CONTROL_BULLET,sprite,sample_AI2);
--   wait(30);
end

sprite = enbullet_create(69,100,300,3,0); -- Пускаем пули
bind_AI(CONTROL_BULLET,sprite,sample_AI2);
--]]