-- hero.lua
-- потом это будет несколько скриптов вида hero_xxx.lua
-- в этом скрипте описываются всякие атаки и ништяки, специфичные для персонажа

spritesheet_load("effects.png"); -- загрузка спрайтового листа
hero_name="ayayaya";

function hero_fire_begin() -- эта функция вызывается при нажатии кнопки атаки
--   log("otake!") -- запись в логе
   begin_frame = engine_get_frame() -- сохраняем номер кадра, в котором начали стрелять
end

function hero_fire() -- эта функция вызывается каждый кадр, когда нажата кнопка атаки
   if ((engine_get_frame()-begin_frame) % 3) == 0 then
      for i = 55,125,5 do
	 sprite = sprite_create("bullets.png", LAYER_HERO_BULLET);
	 sprite_set_position(sprite,hero_x(),hero_y(),i);
	 sprite_set_frame(sprite,8);
	 sprite_set_angle(sprite,15,i);
	 sprite_set_alpha(sprite,.2);
	 sprite_set_scale(sprite,2);
      end
   end
end

function hero_fire_end() -- эта функция вызывается когда отпускается кнопка атаки
--   log("konec otake!");
end

function hero_focus_begin() -- эта функция вызывается когда нажимают кнопку фокуса
   hitbox = sprite_create("bullets.png", LAYER_HERO);
   sprite_set_follow (hitbox, hero_sprite());
   sprite_set_frame(hitbox,1);
   focus = sprite_create("effects.png", LAYER_BACKGROUND);
   sprite_set_alpha(focus,0);
   sprite_set_alpha_speed(focus,0.05);
   sprite_set_alpha_limit(focus,0,.5);
   sprite_set_scale(focus,.01);
   sprite_set_scale_limit(focus,0,1.5);
   sprite_set_speed(focus,0,0,-2);
   sprite_set_follow(focus,hero_sprite());
   sprite_set_scale_speed(focus,0.1);

end

function hero_focus() -- эта функция вызывается каждый кадр во время фокуса
   engine_god_mode(1);
--   particle_set_colour(1,1,1,1);
--   particle_create_to(hero_x(),hero_y(),20);
end

function hero_focus_end() -- эта функция вызывается когда отпускают фокус
   sprite_destroy(hitbox);
   sprite_set_decay(focus,30);
   sprite_set_alpha_speed(focus,-.05);
   sprite_set_scale_speed(focus,-0.1);
end

function hero_graze() -- эта функция вызывается когда герой получает грейз
   graze=graze+1;
end

function hero_die()
   
end
