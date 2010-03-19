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
   wait_time(60);
   sprite_set_speed(sprite,0,-1,0);
   while true do
      local her_xpos = hero_x();
      local her_ypos = hero_y();
      local i
      for i = 1, 5 do
	 local lan_xpos,lan_ypos
	 lan_xpos,lan_ypos = sprite_get_position(sprite)
	 local bullet = enbullet_create_target(sample_proto,lan_xpos,lan_ypos,4-(i*0.2),her_xpos,her_ypos,0);
	 wait_time(3);
      end
      wait_time(45);
   end
end

function lantern2_AI(sprite)
   sprite_set_position(sprite,GAME_FIELD_WIDTH+31,GAME_FIELD_HEIGHT-100,0);
   sprite_set_speed(sprite,-1,0,0);
   sprite_start_animation(sprite,1,1);
   wait_time(60);
   sprite_set_speed(sprite,0,-1,0);
   while true do
      local her_xpos = hero_x();
      local her_ypos = hero_y();
      local i
      sound_play(lantern_spawn_sound);
      for i = 1, 5 do
	 local lan_xpos,lan_ypos
	 lan_xpos,lan_ypos = sprite_get_position(sprite)
	 local bullet = enbullet_create_target(0,lan_xpos,lan_ypos,4-(i*0.2),her_xpos,her_ypos,0);
	 wait_time(3);
      end
      wait_time(45);
   end
end

function sample_AI(bullet)
   while true do
      enbullet_stray(bullet,5);
      wait_time(1);
   end
end

function sample_AI2(bullet)
   while true do
      enbullet_lock_on_hero(bullet,0,3)
      wait_time(60)
      enbullet_stop(bullet)
      wait_time(60)

   end
end

-- Всякие загрузки-шмагрузки
spritesheet_load("level1.png");
spritesheet_load("lantern.png");
spritesheet_load("bullete.png")
spritesheet_load("fontscore.png")
font1=font_load("fontscore.png")
label1 = label_create(440,410,"SCORE",font1,LAYER_PANEL,0)
lantern_spawn_sound = sound_create("spawn.wav");
log("Starting first level!");
music_play("music.xm");
--sample_proto = enbullet_create_proto("bullets.png",0,0,1)
sample_proto = enbullet_create_proto("bullete.png",3,0,.3,"explosion")
enbullet_set_proto_tint(sample_proto, .781, .5, .25, 1.)
sample_proto2 = enbullet_create_proto("bullete.png",0,0,1,"big_bad")
wait_time(1); -- Подождать кадр

background_set_fog_density(.5,-0.004);

logo = sprite_create("level1.png",LAYER_EMBLEM); -- Создание спрайта на слое переднего плана
sprite_set_scale(logo,.5); -- Установка масштабирования спрайта
sprite_set_position(logo,GAME_FIELD_WIDTH/2-10,GAME_FIELD_HEIGHT/2,0); -- Установка положения спрайта
sprite_set_speed(logo,0.1,0,0); -- Установка скорости передвижения
sprite_set_alpha(logo,0.0); -- Установка прозрачности логотипа
sprite_set_alpha_speed(logo,0.01); -- Сгущаем логотип 
wait_time(100); -- Ждём
background_set_fog_density(.1,0);

sprite_set_alpha_speed(logo,-0.01); -- Разгущаем его
wait_time(130); -- Ещё ждём

for i = 1,5 do
   local lantern = sprite_create("lantern.png",LAYER_ENEMY);
   --   enemy_table[i] = bind_AI(CONTROL_SPRITE,lantern,lantern_AI);
   --  thread_start(lantern_AI,string.format("sprite_destroyed(%d)",lantern),lantern);
   control_sprite(lantern_AI, lantern);
   local lantern2 = sprite_create("lantern.png",LAYER_ENEMY);
   --   enemy_table[i] = bind_AI(CONTROL_SPRITE,lantern,lantern_AI);
   -- thread_start(lantern2_AI,string.format("sprite_destroyed(%d)",lantern2),lantern2);
   control_sprite(lantern2_AI, lantern2);
   wait_time(60);
end
--wait_time(10000);
wait_cond("hero_y()>300");
for k = 40, 340, 100 do
   sprite = enbullet_create(sample_proto2,k,300,3,0); -- Пускаем пули
   thread_start(sample_AI,string.format("enbullet_destroyed(%d)",sprite),sprite);
   sprite = enbullet_create(sample_proto2,k,300,3,0); -- Пускаем пули
   thread_start(sample_AI2,string.format("enbullet_destroyed(%d)",sprite),sprite);
--   wait_time(30);
end
--[[
sprite = enbullet_create(69,100,300,3,0); -- Пускаем пули
bind_AI(CONTROL_BULLET,sprite,sample_AI2);
--]]
