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
