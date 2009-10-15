-- level1.lua
-- Скрипт первого уровня

-- Инициализация переменных
lives = 5;
graze = 0;
power = 0;
god_mode = 0;

function lantern_AI(sprite)
   sprite_set_position(sprite,-31,GAME_FIELD_HEIGHT-100,0);
   sprite_set_speed(sprite,1,0,0);
   sprite_start_animation(sprite,1,1);
   wait(60);
   sprite_set_speed(sprite,0,-1,0);
   while true do
      local her_xpos = hero_x();
      local her_ypos = hero_y();
      local i
      for i = 1, 5 do
	 local lan_xpos,lan_ypos
	 lan_xpos,lan_ypos = sprite_get_position(sprite)
	 local bullet = enbullet_create_target(sample_proto,lan_xpos,lan_ypos,4-(i*0.2),her_xpos,her_ypos,0);
	 wait(3);
      end
      wait(45);
   end
end

function lantern2_AI(sprite)
   sprite_set_position(sprite,GAME_FIELD_WIDTH+31,GAME_FIELD_HEIGHT-100,0);
   sprite_set_speed(sprite,-1,0,0);
   sprite_start_animation(sprite,1,1);
   wait(60);
   sprite_set_speed(sprite,0,-1,0);
   while true do
      local her_xpos = hero_x();
      local her_ypos = hero_y();
      local i
      for i = 1, 5 do
	 local lan_xpos,lan_ypos
	 lan_xpos,lan_ypos = sprite_get_position(sprite)
	 local bullet = enbullet_create_target(0,lan_xpos,lan_ypos,4-(i*0.2),her_xpos,her_ypos,0);
	 wait(3);
      end
      wait(45);
   end
end

function sample_AI(bullet) -- исскуственный интеллект для управления спрайтами
-- в параметре функции - хендл чего-то, управляемого интеллектом
   while true do
      enbullet_stray(bullet,5);
      wait(1);
   end
end

function sample_AI2(bullet)
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
lantern_spawn_sound = sound_create("spawn.wav");
log("Starting first level!");
music_play("music.xm");
sample_proto = enbullet_create_proto("bullets.png",0,0,1)
sample_proto2 = enbullet_create_proto("bullets.png",3,0,2)
wait(1); -- Подождать кадр

background_set_fog_density(.5,-0.004);

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
   local lantern = sprite_create("lantern.png",LAYER_ENEMY);
   --   enemy_table[i] = bind_AI(CONTROL_SPRITE,lantern,lantern_AI);
   thread_start(lantern,string.format("sprite_destroyed(%d)",lantern),lantern_AI);
   local lantern2 = sprite_create("lantern.png",LAYER_ENEMY);
   --   enemy_table[i] = bind_AI(CONTROL_SPRITE,lantern,lantern_AI);
   thread_start(lantern2,string.format("sprite_destroyed(%d)",lantern2),lantern2_AI);
   sound_play(lantern_spawn_sound);
   wait(60);
end
--wait(10000);
for k = 40, 340, 100 do
   sprite = enbullet_create(sample_proto2,k,300,3,0); -- Пускаем пули
   thread_start(sprite,"nil",sample_AI);
   sprite = enbullet_create(sample_proto2,k,300,3,0); -- Пускаем пули
   thread_start(sprite,"nil",sample_AI2);
--   wait(30);
end
--[[
sprite = enbullet_create(69,100,300,3,0); -- Пускаем пули
bind_AI(CONTROL_BULLET,sprite,sample_AI2);
--]]
