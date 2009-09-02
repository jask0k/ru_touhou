-- level1.lua
-- Скрипт первого уровня

-- Инициализация переменных
lives = 5;
graze = 0;
power = 0;
god_mode = 0;

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



for k = 40, 340, 100 do
   sprite = enbullet_create(69,k,300,3,0); -- Пускаем пули
   bind_AI(CONTROL_BULLET,sprite,sample_AI);
--   sprite = enbullet_create(69,k,300,3,0); -- Пускаем пули
--   bind_AI(CONTROL_BULLET,sprite,sample_AI2);
--   wait(30);
end

sprite = enbullet_create(69,100,300,3,0); -- Пускаем пули
bind_AI(CONTROL_BULLET,sprite,sample_AI2);
