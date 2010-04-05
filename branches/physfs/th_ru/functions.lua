function explosion(sprite)
   sprite_set_speed(sprite,0,0,0);
   sprite_set_scale_speed(sprite,.1);
   sprite_set_alpha_speed(sprite, -.1);
   sprite_set_decay(sprite,10);
end

function big_bad(sprite)
   sprite_set_speed(sprite,0,0,5);
   sprite_set_scale_speed(sprite,-.1);
   sprite_set_alpha_speed(sprite, -.1);
   sprite_set_decay(sprite,10);
end

function control_sprite(func, sprite)
   thread_start(func,string.format("sprite_destroyed(%d)",sprite),sprite);
end