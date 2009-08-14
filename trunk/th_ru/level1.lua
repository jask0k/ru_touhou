spritesheet_load("level1.png");
log("Starting first level!");
logo = sprite_create("level1.png",LAYER_EMBLEM);
sprite_set_position(logo,0,300,0);
sprite_set_speed(logo,0.5,0,0);
wait(20);
for j = 0, 100 do
   for i= 0,360-45,45 do
	for k = 10, 310, 100 do
      sprite = sprite_create("bullets.png",LAYER_ENEMY_BULLET);
      sprite_set_position(sprite,k,300,0);
      sprite_set_angle(sprite,5,i);
      end
   end
   wait(12);
end

