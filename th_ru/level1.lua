for j = 0, 100, 5 do
   for i= 0,360-45,45 do
      sprite = sprite_create("bullets.png",1);
      sprite_set_position(sprite,j+10,100);
      sprite_set_angle(sprite,2,i);
   end
   wait(20)
end

