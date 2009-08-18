spritesheet_load("effects.png");

function hero_fire_begin()
   log("otake!")
end

function hero_fire()
--   log("attack!");
   if (engine_get_frame() % 3) == 0 then
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

function hero_fire_end()
   log("konec otake!");
end

function hero_focus_begin()
   focus = sprite_create("effects.png", LAYER_BACKGROUND);
   sprite_set_alpha(focus,.6);
   sprite_set_scale(focus,1.5);
   sprite_set_speed(focus,0,0,1);
end

function hero_focus()
   sprite_set_position(focus,hero_x(),hero_y(),0);
end

function hero_focus_end()
   sprite_set_decay(focus,10);
--   sprite_set_scale_speed(focus,-0.01);
end