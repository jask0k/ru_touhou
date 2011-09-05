function explosion(sprite)
   sprite:stop();
   sprite.v_scale=.1;
   sprite.v_alpha = -.1;
   sprite:set_decay(10);
end

function big_bad(sprite)
   sprite:stop();
   sprite.v_r=10;
   sprite.v_scale=-.1;
   sprite.v_alpha = -.1;
   sprite:set_decay(10);
end

function control_sprite(func, sprite)
   thread_start(func,string.format("sprite_destroyed(%d)",sprite),sprite);
end
