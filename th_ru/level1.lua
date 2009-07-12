for j = 0, 100 do
   for i= 0,360-45,45 do
	for k = 10, 310, 100 do
      sprite = sprite_create("bullets.png",LAYER_ENEMY_BULLET);
      sprite_set_position(sprite,k,300);
      sprite_set_angle(sprite,5,i);
      end
   end
   wait(12)
end

