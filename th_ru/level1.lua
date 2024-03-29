-- level1.lua
-- Скрипт первого уровня

-- Инициализация переменных
lives = 5;
graze = 0;
power = 0;
god_mode = 0;
--функция треда врага - позволяет пускать несколько врагов по одному пути
--параметр функции - указатель на врага
--(на самом деле что угодно, движок не смотрит на параметр и послушно передаёт параметр 
--без преобразований в функцию треда)
function lantern_AI(lantern)
--   lantern.sprite.x = -31;--ставим врага за экран слева
--   lantern.sprite.y = GAME_FIELD_HEIGHT-100;--чуть ниже верхней границы экрана
   lantern:bind_AI()
--   log("starting AI thread!")
--   local cond_enemy = lantern
--   print ("enemy pointer is:", lantern)
--   print ("enemy_destroyed:",game.enmanager:enemy_destroyed(lantern))
   lantern:set_speed(1,0);--летим вправо
   lantern:start_animation(1);--машем крыльями
   wait_time(60);--ждём секунду
   lantern:set_speed(0,-1);--летим вниз
   while true do
      local i
      for i = 1, 5 do
	 local bullet = lantern:shoot_at_hero(4-(i*0.2));--стреляем очередью из пяти пуль
	 wait_time(3);
      end
      wait_time(45);--ждём 45 кадров
   end
end

function lantern2_AI(lantern)
   lantern:bind_AI()
--   log("starting AI thread!")
--   local cond_enemy = lantern
--   print ("enemy pointer is:", lantern)
--   print ("enemy_destroyed:",game.enmanager:enemy_destroyed(lantern))
   lantern:set_speed(-1,0);--летим вправо
   lantern:start_animation(1);--машем крыльями
   wait_time(60);--ждём секунду
   lantern:set_speed(0,-1);--летим вниз
   while true do
      local i
      for i = 1, 5 do
	 local bullet = lantern:shoot_at_hero(4-(i*0.2));--стреляем очередью из пяти пуль
	 wait_time(3);
      end
      wait_time(45);--ждём 45 кадров
   end
end

function midboss_AI(midboss)
   midboss:bind_AI()
   midboss.sprite.scale = 2
   midboss:start_animation(1);--машем крыльями
   midboss:set_speed(0,-2)
   wait_time(120)
   midboss.sprite:stop()
   while true do
      local i
      for i = 0,7 do
	 midboss:shoot(4,i*45)
	 game.ebmanager:create_bullet(sample_proto, midboss.sprite.x, midboss.sprite.y, 4,i*45+22.5)
      end
      wait_time(45)
   end
end

function lantern_die(sprite)
   sprite:stop()
   sprite:start_animation(2)
   sprite.decay_timer = 20
   sprite.decay_active=true
   v_score:inc(10)
end


function sample_AI(bullet) --тред, крутящий пули
   while true do
      bullet:stray(5);
      wait_time(1);
   end
end

function sample_AI2(bullet)
   while true do
      bullet:lock_on_hero(0,3)
      wait_time(60)
      bullet:stop()
      wait_time(60)

   end
end

-- Всякие загрузки-шмагрузки
level_ss = game.ssheet:new("level1.png")
lantern_ss = game.ssheet:new("lantern.png");
bullete = game.ssheet:new("bullete.png")
font_ss = game.ssheet:new("fontscore.png")
font1 = game.lmanager:font_load(font_ss)
--label1 = game.label:new(440,410,"SCORE",font1,LAYER_PANEL,0)
--lantern_spawn_sound = game.:new("spawn.wav");
log("Starting first level!");
game.boom_box:play_music("music.xm");
--sample_proto = enbullet_create_proto("bullets.png",0,0,1)
sample_proto = game.ebmanager:create_proto(bullete,3,false,.3,"explosion") --создаём прототип пули
game.ebmanager:set_proto_tint(sample_proto, .781, .5, .25, 1.) --раскрашиваем прототип
sample_proto2 = game.ebmanager:create_proto(bullete,0,false,1,"big_bad") --создаём ещё прототип
wait_time(1); -- Подождать кадр
game.background:set_fog_density(.5,-0.004)-- создаём разрежающийся туман

logo = game.sprite:new(level_ss,game.LAYER_EMBLEM); -- Создание спрайта на слое переднего плана
logo.scale = .5; -- Установка масштабирования спрайта
logo.x = GAME_FIELD_WIDTH/2-10;
logo.y = GAME_FIELD_HEIGHT/2; -- Установка положения спрайта
logo.v_x = 0.1 -- Установка скорости передвижения
logo.alpha = 0.0; -- Установка прозрачности логотипа
logo.v_alpha = 0.01; -- Сгущаем логотип 
wait_time(100); -- Ждём
game.background:set_fog_density(.1);

logo.v_alpha = -0.01; -- Разгущаем его
wait_time(130); -- Ещё ждём
-- game.background:set_fog_density(.7,0);

lantern_proto = game.enmanager:create_enemy_proto(lantern_ss,1,20,sample_proto,"lantern_die")
midboss_proto = game.enmanager:create_enemy_proto(lantern_ss,1,500,sample_proto2,"lantern_die")

for i = 1,5 do
   local enemy = game.enmanager:create_enemy(lantern_proto,-31,GAME_FIELD_HEIGHT-100);
--   print (enemy)
   --   enemy_table[i] = bind_AI(CONTROL_SPRITE,lantern,lantern_AI);
   --  thread_start(lantern_AI,string.format("sprite_destroyed(%d)",lantern),lantern);
   thread_start(lantern_AI,"" , enemy)
   
  enemy = game.enmanager:create_enemy(lantern_proto,GAME_FIELD_WIDTH+31,GAME_FIELD_HEIGHT-100);
   --   enemy_table[i] = bind_AI(CONTROL_SPRITE,lantern,lantern_AI);
  thread_start(lantern2_AI,"",enemy);
--   control_sprite(lantern2_AI, lantern2);
   wait_time(60);
end

wait_time(600)

mid_boss = game.enmanager:create_enemy(midboss_proto, GAME_FIELD_WIDTH/2, GAME_FIELD_HEIGHT+64)
thread_start(midboss_AI, "", mid_boss)
--wait_time(10000);
--[[
wait_cond("game.hero.y>300");
log ("ok!")
for k = 40, 340, 100 do
   sprite = enbullet_create(sample_proto2,k,300,3,0); -- Пускаем пули
   thread_start(sample_AI,string.format("enbullet_destroyed(%d)",sprite),sprite);
   sprite = enbullet_create(sample_proto2,k,300,3,0); -- Пускаем пули
   thread_start(sample_AI2,string.format("enbullet_destroyed(%d)",sprite),sprite);
--   wait_time(30);
end
--]]
--[[
sprite = enbullet_create(69,100,300,3,0); -- Пускаем пули
bind_AI(CONTROL_BULLET,sprite,sample_AI2);
--]]
