-- hero.lua
-- потом это будет несколько скриптов вида hero_xxx.lua
-- в этом скрипте описываются всякие атаки и ништяки, специфичные для персонажа

effects = game.ssheet:new("effects.png"); -- загрузка спрайтового листа
hero_name="ayayaya";

hero_spritesheet = game.ssheet:new("mokou.png")
hero_scale = 0.25

game.hbmanager:load_spritesheet(game.ssheet:new("bullete.png"));

main_proto = game.hbmanager:create_proto("", 1, .3, false, 1, 1, 0, 0, .2);
log(main_proto)

function hero_fire_begin() -- эта функция вызывается при нажатии кнопки атаки
--   log("otake!") -- запись в логе
   begin_frame = game.engine.frames -- сохраняем номер кадра, в котором начали стрелять
end

function hero_fire() -- эта функция вызывается каждый кадр, когда нажата кнопка атаки
   if ((game.engine.frames-begin_frame) % 3) == 0 then -- стреляем через два кадра, это нужно, 
                                                       -- чтоб создать ощущение движения пуль:
                                                       -- стрельба в каждом кадре создаёт ощущение
                                                       -- неподвижной струйки из пуль для глаза,
                                                       -- стрельба через кадр создаёт ощущение
                                                       -- _мигающей_ неподвижной струйки
      for i = 55,125,5 do
	 bullet = game.hbullet:new(main_proto,10,i,0,0) -- наверно, здесь будет работать сборщик мусора
	 --не будет, чтоб объекты собирались им, нужно вызывать особую функцию толуа
      end
   end
end

function hero_fire_end() -- эта функция вызывается когда отпускается кнопка атаки
--   log("konec otake!");
end

function hero_focus_begin() -- эта функция вызывается когда нажимают кнопку фокуса
--   hitbox = game.sprite:create("bullets.png", LAYER_HERO);
--   sprite_set_follow (hitbox, hero_sprite());
--   sprite_set_frame(hitbox,1);
   focus = game.sprite:new(effects, game.LAYER_BACKGROUND)
   focus.x = game.hero.sprite.x
   focus.y = game.hero.sprite.y
   focus.follow = game.hero.sprite
   focus.alpha = 0
   focus.v_alpha = 0.05
   focus.max_alpha = .5
   focus.scale = .01
   focus.max_scale = 1.5
   focus.v_r = -2
   focus.v_scale = .1

end

function hero_focus() -- эта функция вызывается каждый кадр во время фокуса
--   engine_god_mode(1);
--   particle_set_colour(1,1,1,1);
--   particle_create_to(hero_x(),hero_y(),20);
end

function hero_focus_end() -- эта функция вызывается когда отпускают фокус
--   sprite_destroy(hitbox);
   focus.decay = 30
   focus.v_alpha = -.05;
   focus.v_scale = -0.1;
end

function hero_graze() -- эта функция вызывается когда герой получает грейз
   graze=graze+1;
end

function hero_die()
   
end
