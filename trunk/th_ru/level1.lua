lives = 5;
graze = 0;
power = 0;
god_mode = 0;

spritesheet_load("level1.png");
log("Starting first level!");


logo = sprite_create("level1.png",LAYER_EMBLEM);
sprite_set_scale(logo,.5);
sprite_set_position(logo,GAME_FIELD_WIDTH/2-10,GAME_FIELD_HEIGHT/2,0);
sprite_set_speed(logo,0.1,0,0);
sprite_set_alpha(logo,0.01);
sprite_set_alpha_speed(logo,0.01);
wait(100);
sprite_set_alpha_speed(logo,-0.01);
wait(130);
for j = 0, 100 do
   for i= 0,360-45,45 do
	for k = 10, 310, 100 do
      sprite = enbullet_create_hero(1,k,300,2,i);
--      sprite_set_position(sprite,k,300,0);
--      sprite_set_angle(sprite,5,i);
      end
   end
   wait(12);
end
