--Здесь всякая инициализация и прочее

-- spritesheet_load("fontg.png");
-- spritesheet_load("aya_2.png");
-- spritesheet_load("bullets.png");
ss_ui=game.ssheet:new("panel_labels.png");
font_texture=game.ssheet:new("fontscore.png");
font_ui=game.lmanager:font_load(font_texture);
l_score = game.sprite:new(ss_ui,game.LAYER_UI);
l_score.x= 640-138
l_score.y=480-48

v_score = game.var:new("score", 100, 0, font_ui, 640-138, 480-77, game.LAYER_PANEL);

l_lives = game.sprite:new(ss_ui,game.LAYER_UI);
l_lives.frame=3;
l_lives.x= 640-138
l_lives.y=480-106

l_spell = game.sprite:new(ss_ui,game.LAYER_UI);
l_spell.frame=4;
l_spell.x= 640-138
l_spell.y=480-138

l_points = game.sprite:new(ss_ui,game.LAYER_UI);
l_points.frame=1;
l_points.x= 640-138
l_points.y=480-170

l_graze = game.sprite:new(ss_ui,game.LAYER_UI);
l_graze.frame=2;
l_graze.x= 640-138
l_graze.y=480-202

l_power = game.sprite:new(ss_ui,game.LAYER_UI);
l_power.frame=5;
l_power.x= 640-138
l_power.y=480-234

l_powerbar = game.sprite:new(ss_ui,game.LAYER_UI);
l_powerbar.frame=6;
l_powerbar.x= 640-64
l_powerbar.y=480-234