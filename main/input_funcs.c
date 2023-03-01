const char ps2_to_ascii[256]={0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 96, 0, 0, 0, 5, 0, 6, 113, 49, 0, 0, 0, 122, 115, 97, 119, 50, 0, 0, 99, 120, 100, 101, 52, 51, 0, 0, 32, 118, 102, 116, 114, 53, 0, 0, 110, 98, 104, 103, 121, 54, 0, 0, 0, 109, 106, 117, 55, 56, 0, 0, 44, 107, 105, 111, 48, 57, 0, 0, 46, 47, 108, 59, 112, 45, 0, 0, 0, 39, 0, 91, 61, 0, 0, 0, 0, 0, 93, 0, 92, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3, 0, 4, 1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void get_keyboard_inputs(){
	for(int i=0;i<128;i++) inputs_old[i]=inputs[i];

	uart_get_buffered_data_len(2, (size_t*)&key_buf_len);
	key_buf_len=uart_read_bytes(2, key_buf, key_buf_len, 1);

	bool polarity=true;
	for(int i=0;i<key_buf_len;i++){
		if(key_buf[i]==0xe0) continue;
		if(key_buf[i]==0xf0){
			polarity=false;
			continue;
		}

		inputs[(int)ps2_to_ascii[(int)key_buf[i]]]=polarity;
		polarity=true;
	}
}


void apply_player_inputs(){
	if(gamestate==GAMESTATE_RUNNING){
		if(player->state!=ACTOR_STATE_DEAD){
			//rotational inputs
			if(inputs[key_left]) player->roth+=.015*(220/TARGET_FPS);
			if(inputs[key_right]) player->roth-=.015*(220/TARGET_FPS);
			if(allow_freelook && !textboxes){
				if(inputs[key_up]) player_rotv-=.015*(220/TARGET_FPS);
				if(inputs[key_down]) player_rotv+=.015*(220/TARGET_FPS);
			}

			//camera limits
			if(player->roth<0) player->roth+=2*pi;
			else if(player->roth>2*pi) player->roth-=2*pi;
			if(player_rotv<0) player_rotv=0;
			else if(player_rotv>pi) player_rotv=pi;

			//movement inputs
			if(inputs['w']){
				player->vel.x+=fast_cos(player->roth)*player_speed;
				player->vel.y+=fast_sin(player->roth)*player_speed;
			}

			if(inputs['s']){
				player->vel.x-=fast_cos(player->roth)*player_speed;
				player->vel.y-=fast_sin(player->roth)*player_speed;
			}

			if(inputs['d']){
				player->vel.x+=fast_cos(player->roth-pi/2)*player_speed;
				player->vel.y+=fast_sin(player->roth-pi/2)*player_speed;
			}

			if(inputs['a']){
				player->vel.x+=fast_cos(player->roth+pi/2)*player_speed;
				player->vel.y+=fast_sin(player->roth+pi/2)*player_speed;
			}

			//if(inputs[key_space]) player->pos.z+=.2;
			//if(inputs[key_shift]) player->pos.z-=.2;

			for(int i=1;i<8;i++) if(inputs[i+0x30] && (player_owned_guns & 1<<i)) player_target_gun=i;
			if(inputs['1'] && (player_held_gun==GUN_CHAINSAW || !(player_owned_guns&(1<<GUN_CHAINSAW)))) player_target_gun=GUN_FIST;
		}
	}
	else{
		if(inputs[key_up] && !inputs_old[key_up] && selected_textbox!=textboxes) selected_textbox--;
		if(inputs[key_down] && !inputs_old[key_down] && selected_textbox!=(textboxes+textbox_num-1)) selected_textbox++;
	}

	if(inputs[key_esc] && !inputs_old[key_esc]){
		int fuckme=gamestate;
		if(fuckme==GAMESTATE_RUNNING) switch_gamestate(GAMESTATE_PAUSE);
		if(fuckme==GAMESTATE_PAUSE) switch_gamestate(GAMESTATE_RUNNING);

	}
}

void set_motd(char *s, int t){
	motd=s;
	motd_timer=t;
}


void test(){
	static const char s[]="test";
	printf("test\n");
	trigger_action(e1m1_triggers+1);

	set_motd(s,60);
}


void uwu(){
	static const char s[]="owo";
	for(int i=0;i<max_actor_count;i++) if(actors[i]){
		if(actors[i]->type) damage_actor(player,actors[i],1000);
	}

	set_motd(s,60);
}

void freelook(){
	static const char s_on[]="bepis";
	static const char s_off[]="unbepis";

	allow_freelook=!allow_freelook;
	if(!allow_freelook) player_rotv=pi/2;

	set_motd(allow_freelook?s_on:s_off,60);
}

void rez(){
	static const char s[]="mazohist";

	for(int i=0;i<max_actor_count;i++) if(actors[i]) if(!(actors[i]->flags & FRAGILE)){
		actors[i]->state=ACTOR_STATE_IDLE;
		actors[i]->health=100;
		actors[i]->flags|=SHOOTABLE | SOLID;
	}

	set_motd(s,60);
}

void mushud(){
	static const char s_on[]="ok tuki mas";
	static const char s_off[]="haha tega nikol naus vidu";

	allow_drawhud=!allow_drawhud;

	if(allow_drawhud){
		update_hud(screen, 1);
		update_hud(screen_out, 1);
	}

	set_motd(allow_drawhud?s_on:s_off,60);
}

void idkfa(){
	static const char s[]="very happy ammo added";

	for(int i=0;i<4;i++) player_current_ammo[i]=player_max_ammo[i];

	set_motd(s,60);
}

void musfps(){
	static const char s[]="ok nerd";

	allow_draw_fps=!allow_draw_fps;

	set_motd(s,60);
}

void idclip(){
	static const char s_on[]="no clipping mode on";
	static const char s_off[]="no clipping mode off";

	allow_no_clip=!allow_no_clip;

	set_motd(allow_no_clip?s_on:s_off,60);
}

void iddqd(){
	static const char s_on[]="degreelessness mode on";
	static const char s_off[]="degreelessness mode off";

	allow_godmode=!allow_godmode;

	set_motd(allow_godmode?s_on:s_off,60);
}

void screenshot(){
	static const char s[]="screenshot taken";
	const char handshake[]="OwO";	//ni za kej šiška :)

	uart_write_bytes(1, handshake, sizeof(handshake)-1);
	uart_write_bytes(1, screen_out, h*w*2);

	set_motd(s,60);
}



const cheat_t cheats[]={
		{"musfreelook",freelook},
		{"mushud",mushud},
		{"uwu",uwu},
		{"rey",rez},	//jebas amerièane
		{"test",test},
		{"idkfa",idkfa},
		{"musfps",musfps},
		{"idclip",idclip},
		{"idspispopd",idclip},
		{"iddqd",iddqd},
		#if SCREENSHOTS
		{"\x09",screenshot},
		#endif

};

byte cheats_progress[sizeof(cheats)/sizeof(cheat_t)];

void check_cheats(){
	for(int i=0;i<sizeof(inputs)/sizeof(inputs[0]);i++) if(inputs[i] && !inputs_old[i]) for(int j=0;j<sizeof(cheats)/sizeof(cheat_t);j++){
		if(cheats[j].s[cheats_progress[j]]==i) cheats_progress[j]++;
		else cheats_progress[j]=0;

		if(!cheats[j].s[cheats_progress[j]]){
			cheats_progress[j]=0;
			cheats[j].func();
		}

	}
}









