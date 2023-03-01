void register_actor(actor_t *actor){
	for(int i=0;i<max_actor_count;i++) if(!actors[i]){
		actors[i]=actor;
		return;
	}
}

void deregister_actor(actor_t *actor){
	for(int i=0;i<max_actor_count;i++) if(actors[i]==actor){
		actors[i]=NULL;
		return;
	}
}

void register_particle(particle_t *particle){
	for(int i=0;i<max_particle_count;i++) if(!particles[i]){
		particles[i]=particle;
		return;
	}
}

void deregister_particle(particle_t *particle){
	for(int i=0;i<max_particle_count;i++) if(particles[i]==particle){
		particles[i]=NULL;
		return;
	}
}

int get_actor_num(){
	int n=0;
	for(int i=0;i<max_actor_count;i++) if(actors[i]) n++;
	return n;
}

const byte rng_table[]={
    0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
    74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
    95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
    52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
    149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
    145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0,
    175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
    25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
    94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
    136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
    135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
    80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
    24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
    145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
    28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
    71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
    17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
    197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
    120, 163, 236, 249
};

byte doom_rand(){
	static byte index=0;
	return rng_table[index++];
}

void spawn_particle(point_t *pos, particle_type_t type){
	particle_t *particle=malloc(sizeof(particle_t));
	*particle=(particle_t){
		.pos=*pos,
		.vel=(point_t){0,0,0},
		.type=type,
		.anim_timer=0,
	};

	register_particle(particle);
}



void switch_state(actor_t *actor, actor_state_t state){
	actor->state=state;
	actor->anim_timer=1;
	actor->vel=(point_t){0,0,0};
}


void poglej_tarco(actor_t *actor){
	if(actor->target) actor->roth=atan2(actor->target->pos.y-actor->pos.y,actor->target->pos.x-actor->pos.x);
}

void kill_player(actor_t *actor){
	player->health=0;
	switch_state(actor, ACTOR_STATE_DEAD);
}

void kill_actor(actor_t *actor){
	//projectiles
	if(actor->flags & FRAGILE){
		int anim_num=actor_anims_base[actor->type][actor->state][0].num;
		anim_num=(anim_num>=actor_sprite_rot_limit[actor->type])?anim_num+7*actor_sprite_rot_limit[actor->type]:anim_num*8;
		int h1=actor_sprite_base[actor->type][anim_num].height/32;

		anim_num=actor_anims_base[actor->type][ACTOR_STATE_DEAD][0].num;
		anim_num=(anim_num>=actor_sprite_rot_limit[actor->type])?anim_num+7*actor_sprite_rot_limit[actor->type]:anim_num*8;
		int h2=actor_sprite_base[actor->type][anim_num].height/32;

		actor->pos.z-=h2-h1;


		switch_state(actor, ACTOR_STATE_DEAD);
		actor->flags&=~SOLID;
	}

	//monsters
	else{
		if(actor->type){
			if(actor_anims_base[actor->type][ACTOR_STATE_GIBED][0].num && actor->health<-30) switch_state(actor, ACTOR_STATE_GIBED);
			else switch_state(actor, ACTOR_STATE_DEAD);
			actor->flags&=~(SHOOTABLE | SOLID);
		}
		else kill_player(actor);
	}
}

bool roll_for_pain_state(actor_t *actor){
	return true;
}

void try_pain_state(actor_t *actor){
	if(roll_for_pain_state(actor)){
		switch_state(actor,ACTOR_STATE_PAIN);
		actor->flags|=JUST_HIT;
	}
}


bool try_attack(actor_t *actor){
	if(actor->reaction_time>0) return false;

	if(actor->flags & JUST_HIT){
		actor->flags&=~JUST_HIT;
		return true;
	}

	if(actor->flags & JUST_ATTACKED){
		actor->flags&=~JUST_ATTACKED;
		return false;
	}


	float dist=get_actor_dist_doom(actor,actor->target)-1.92;
	if(dist>6) dist=6;

	return doom_rand()>=dist*33.33;
}


void damage_actor(actor_t *inflictor, actor_t *recipient, int dmg){
	if(recipient==player && allow_godmode) return;
	if(recipient->state==ACTOR_STATE_DEAD || recipient->state==ACTOR_STATE_GIBED) return;

	if(recipient->type){
		recipient->health-=dmg;
		if(inflictor->source!=recipient) recipient->target=(inflictor->source)?inflictor->source:inflictor;
		try_pain_state(recipient);
	}

	else{
		if(player_armor>dmg*player_armor_prot_value){
			player_armor-=dmg*player_armor_prot_value;
			player->health-=dmg*(1-player_armor_prot_value);
		}
		else{
			dmg-=player_armor;
			player_armor=0;
			player->health-=dmg;
		}

	}

	if(recipient->health<=0){
		kill_actor(recipient);
	}
}

void push_away(actor_t *pusher, actor_t *loser_getting_pushed){
	loser_getting_pushed->vel=point_sub(&loser_getting_pushed->pos, &pusher->pos);
	normalize_2d(&loser_getting_pushed->vel);
	loser_getting_pushed->vel.x/=4;
	loser_getting_pushed->vel.y/=4;
	loser_getting_pushed->vel.z=0;
}

void explode(actor_t *actor, int dmg){
	printf("boom\n");
	for(int i=0;i<max_actor_count;i++) if(actors[i])
		if(actors[i]!=actor  && (actors[i]->flags & SHOOTABLE))
			if(get_actor_dist_2d_unsqrtd(actor,actors[i])<14){
				damage_actor(actor,actors[i],dmg);
				printf("player: %p, cunt: %p\n",player,actors[i]);
				if(actors[i]==player || actors[i]->state==ACTOR_STATE_DEAD || actors[i]->state==ACTOR_STATE_GIBED) push_away(actor, actors[i]);
			}
}



void fire_hitscan(actor_t *actor, float ang_offset, int dmg){
	poglej_tarco(actor);

	point_t p_end={100*fast_cos(actor->roth+ang_offset)+actor->pos.x,100*fast_sin(actor->roth+ang_offset)+actor->pos.y,0};

	actor_t *najblizji_actor=NULL;
	float actor_dist=1./0;

	for(int i=0;i<max_actor_count;i++) if(actors[i])
		if((actors[i]!=actor) && (actors[i]->flags & SHOOTABLE))
			if(line_colides_square(&actor->pos, &p_end, actors[i]->pos.x, actors[i]->pos.y, actors[i]->width/2)){
				float temp=(actor->pos.x-actors[i]->pos.x)*(actor->pos.x-actors[i]->pos.x) + (actor->pos.y-actors[i]->pos.y)*(actor->pos.y-actors[i]->pos.y);
				if(temp<actor_dist){
					actor_dist=temp;
					najblizji_actor=actors[i];
				}
			}


	point_t najblizji_presek;
	float surf_dist=1./0;
	for(int i=0;i<static_surf_num;i++) if((static_surfs[i].flags & IS_WALL) && (static_surfs[i].flags & IS_SOLID)){
		point_t p=line_line_presecisce(&actor->pos,&p_end,&static_surfs[i].points[0],&static_surfs[i].points[1]);

		if(p.z){
			float temp=(actor->pos.x-p.x)*(actor->pos.x-p.x) + (actor->pos.y-p.y)*(actor->pos.y-p.y);
			if(temp<surf_dist){
				surf_dist=temp;
				najblizji_presek=p;
			}
		}
	}
	for(int i=0;i<dynamic_surf_num;i++) if((dynamic_surfs[i].flags & IS_WALL) && (dynamic_surfs[i].flags & IS_SOLID)){
		point_t p=line_line_presecisce(&actor->pos,&p_end,&dynamic_surfs[i].points[0],&dynamic_surfs[i].points[1]);

		if(p.z){
			float temp=(actor->pos.x-p.x)*(actor->pos.x-p.x) + (actor->pos.y-p.y)*(actor->pos.y-p.y);
			if(temp<surf_dist){
				surf_dist=temp;
				najblizji_presek=p;
			}
		}
	}

	najblizji_presek.z=player->pos.z+player->height;

	actor->flags|=JUST_ATTACKED;

	if(najblizji_actor || surf_dist!=1./0){
		if(actor_dist<surf_dist) damage_actor(actor, najblizji_actor, dmg);
		else spawn_particle(&najblizji_presek, PARTICLE_POOF);
	}
}


void fire_projectile(actor_t *actor, actor_type_t type){
	poglej_tarco(actor);

	actor_t *projectile=malloc(sizeof(actor_t));
	*projectile=(actor_t){
		.pos=actor->pos,
		.vel=actor->type?point_sub(&actor->target->pos,&actor->pos):(point_t){fast_cos(player->roth)*.8,fast_sin(player->roth)*.8,0},
		.roth=actor->roth,
		.width=1,
		.height=1,
		.type=type,
		.state=ACTOR_STATE_WALKING,
		.health=1,
		.source=actor,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=300,
		.flags=CAN_THINK | FRAGILE | SOLID

	};

	projectile->pos.z+=actor->height*3/4;

	if(actor->type){
		normalize_3d(&projectile->vel);
		projectile->vel.x/=4;
		projectile->vel.y/=4;
		projectile->vel.z/=4;
	}

	if(!actor->type) add_to_point(&projectile->pos,&projectile->vel);

	register_actor(projectile);

	actor->flags|=JUST_ATTACKED;
}

void initiate_ranged_attack(actor_t *actor){
	switch_state(actor, ACTOR_STATE_RANGED_ATTACK);
	poglej_tarco(actor);
}

void initiate_melee_attack(actor_t *actor){
	switch_state(actor, ACTOR_STATE_MELEE_ATTACK);
	poglej_tarco(actor);
}

bool in_melee_range(actor_t *actor){
	return get_actor_dist_2d_unsqrtd(actor,actor->target)<melee_range*melee_range;
}

void try_melee_attack(actor_t *actor){
	if(actor->target) if(in_melee_range(actor)) initiate_melee_attack(actor);
}

void walk(actor_t *actor){
	switch_state(actor, ACTOR_STATE_WALKING);
	actor->walk_timer=doom_rand()%32+32;

	point_t vect=point_sub(&actor->target->pos,&actor->pos);
	rotate_point_2d(&vect, (doom_rand()%3-1)*(pi/4));	//zavrt za -pi/4, 0 al pi/4
	normalize_2d(&vect);
	vect.x/=16;
	vect.y/=16;
	vect.z=0;


	actor->roth=atan2(vect.y,vect.x);
	actor->vel=vect;

}

void look_for_player(actor_t *actor){
	if(actor->pos.y < player->pos.y) actor->target=player;

	/*
	if(actor->roth==pi/2){
		if(actor->pos.y >= player->pos.y) actor->target=player;
	}
	else if(actor->roth==3*pi/2){
		if(actor->pos.y <= player->pos.y) actor->target=player;
	}
	else{
		float k=fast_tan(actor->roth-pi/2);

	}
	*/
}

void delete_actor(actor_t *actor){
	actor->flags|=DELETE_ME;
}

//ce zadane nastavi target na stvarco ki jo je zadelo in izvedi seppuku
void projectile_hit_check(actor_t *actor){
	for(int i=0;i<max_actor_count;i++) if(actors[i])
		if((actors[i]->flags & SHOOTABLE) && (actors[i]!=actor && actors[i]!=actor->source))
			if(actor_colides_actor(actor, actors[i])){
				actor->target=actors[i];
				kill_actor(actor);
				return;
			}
}

void actor_hit_wall(actor_t *actor, polygon_t *surf){
	if(actor->flags & FRAGILE) kill_actor(actor);
	else{
		push_actor_out_of_surf(actor,surf);
		if(actor->state==ACTOR_STATE_WALKING) walk(actor);
	}
}


void player_punch(int damage){
	point_t p_end=(point_t){20*fast_sin(2*pi-player->roth),20*fast_cos(2*pi-player->roth),0};

	actor_t *najblizji_actor=NULL;
	float actor_dist=1./0;

	for(int i=1;i<max_actor_count;i++) if(actors[i])
		if((actors[i]->flags & SHOOTABLE))
			if(line_colides_square(&player->pos, &p_end, actors[i]->pos.x, actors[i]->pos.y, actors[i]->width/2)){
				float temp=(player->pos.x-actors[i]->pos.x)*(player->pos.x-actors[i]->pos.x) + (player->pos.y-actors[i]->pos.y)*(player->pos.y-actors[i]->pos.y);
				if(temp<actor_dist){
					actor_dist=temp;
					najblizji_actor=actors[i];
				}
			}

	if(najblizji_actor && actor_dist<4) damage_actor(player, najblizji_actor, damage);
}


void fire_bfg_tracers(actor_t *actor){
	for(int i=0;i<40;i++){
		int dmg=0;
		for(int j=0;j<15;j++) dmg+=(doom_rand()&8)+1;
		fire_hitscan(actor->source,i*(pi/80)-pi/4,dmg);
	}
}


void player_fire_gun(){
	switch(player_held_gun){
		case GUN_FIST:
			player_punch(rand_dmg_range(2,20,2));
			break;

		case GUN_CHAINSAW:
			player_punch(rand_dmg_range(2,20,2));
			break;

		case GUN_PISTOL:
			if(player_current_ammo[AMMO_BULLET]){
				fire_hitscan(player,rand_ang_range(5.6*pi/180),rand_dmg_range(5,15,5));
				player_current_ammo[AMMO_BULLET]--;
			}
			break;

		case GUN_SHOTGUN:
			if(player_current_ammo[AMMO_SHELL]){
				for(int i=0;i<5;i++) fire_hitscan(player,rand_ang_range(5.6*pi/180),rand_dmg_range(5,15,5));
				player_current_ammo[AMMO_SHELL]--;
			}
			break;

		case GUN_CHAINGUN:
			if(player_current_ammo[AMMO_BULLET]){
				fire_hitscan(player,rand_ang_range(5.6*pi/180),rand_dmg_range(5,15,5));
				player_current_ammo[AMMO_BULLET]--;
			}
			break;

		case GUN_ROCKET:
			if(player_current_ammo[AMMO_ROCKET]){
				fire_projectile(player,ACTOR_ROCKET);
				player_current_ammo[AMMO_ROCKET]--;
			}
			break;

		case GUN_PLASMA:
			if(player_current_ammo[AMMO_CELL]){
				fire_projectile(player,ACTOR_PLASMA_BOLT);
				player_current_ammo[AMMO_CELL]--;
			}
			break;

		case GUN_BFG:
			if(player_current_ammo[AMMO_CELL]>=40){
				fire_projectile(player,ACTOR_BFG_BALL);
				player_current_ammo[AMMO_CELL]-=40;
			}
			break;
	}
}

void trigger_action(trigger_t *trigger){
	int id=((int)trigger-(int)triggers)/sizeof(trigger_t);
	if(trigger_rtf[id] & TRIGGER_FLAG_DISABLE) return;

	switch(trigger->type & 0b00111111){
		case TRIGGER_MODE_DO_NOTHING: break;

		case TRIGGER_MODE_SPAWNER:
			for(actor_t *actor_template=(const actor_t*)trigger->targets;actor_template->type!=255;actor_template++){
				actor_t *actor=malloc(sizeof(actor_t));
				*actor=*actor_template;
				register_actor(actor);
			};
			trigger_rtf[id]|=TRIGGER_FLAG_DISABLE;
			break;

		case TRIGGER_MODE_MOVE_SINGLE:
			if(!(trigger_rtf[id] & TRIGGER_FLAG_ACTIVE)){
				trigger_rtf[id]|=TRIGGER_FLAG_ACTIVE | TRIGGER_FLAG_SWITCH_ON | trigger->move_time;
			}

			for(uint16_t *i=trigger->targets;*i!=65535;i++) for(int j=0;j<dynamic_surfs[*i].size;j++) add_to_point(&(dynamic_surfs[*i].points[j]),&(trigger->vel));

			if(!(--trigger_rtf[id] & 0b111111111111)) trigger_rtf[id]|=TRIGGER_FLAG_DISABLE;

			break;

		case TRIGGER_MODE_MOVE_REPEATABLE:
			if(!(trigger_rtf[id] & TRIGGER_FLAG_ACTIVE)){
				trigger_rtf[id]|=TRIGGER_FLAG_ACTIVE | trigger->move_time;
				trigger_rtf[id]^=TRIGGER_FLAG_SWITCH_ON;
			}

			for(uint16_t *i=trigger->targets;*i!=65535;i++) for(int j=0;j<dynamic_surfs[*i].size;j++){
				if(trigger_rtf[id] & TRIGGER_FLAG_SWITCH_ON) add_to_point(&(dynamic_surfs[*i].points[j]),&(trigger->vel));
				else sub_from_point(&(dynamic_surfs[*i].points[j]),&(trigger->vel));
			}

			if(!(--trigger_rtf[id] & 0b111111111111)) trigger_rtf[id]&=~TRIGGER_FLAG_ACTIVE;

			break;

		case TRIGGER_MODE_MOVE_AUTOCLOSE:
			if(!(trigger_rtf[id] & TRIGGER_FLAG_ACTIVE)){
				if(dynamic_surfs[*(uint16_t*)trigger->targets].flags&IS_MOVING) return;
				dynamic_surfs[*(uint16_t*)trigger->targets].flags|=IS_MOVING;
				trigger_rtf[id]|=TRIGGER_FLAG_ACTIVE | TRIGGER_FLAG_SWITCH_ON | trigger->move_time;
			}

			if(!(trigger_rtf[id] & TRIGGER_FLAG_WAITING)) for(uint16_t *i=trigger->targets;*i!=65535;i++) for(int j=0;j<dynamic_surfs[*i].size;j++){
				if(trigger_rtf[id] & TRIGGER_FLAG_SWITCH_ON) add_to_point(&(dynamic_surfs[*i].points[j]),&(trigger->vel));
				else sub_from_point(&(dynamic_surfs[*i].points[j]),&(trigger->vel));
			}

			if(!(--trigger_rtf[id] & 0b111111111111)){
				if(trigger_rtf[id] & TRIGGER_FLAG_SWITCH_ON){
					trigger_rtf[id]&=~TRIGGER_FLAG_SWITCH_ON;
					trigger_rtf[id]|=TRIGGER_FLAG_WAITING | 120;
				}
				else{
					if(trigger_rtf[id] & TRIGGER_FLAG_WAITING){
						trigger_rtf[id]&=~TRIGGER_FLAG_WAITING;
						trigger_rtf[id]|=trigger->move_time;
					}
					else{
						trigger_rtf[id]&=~TRIGGER_FLAG_ACTIVE;
						dynamic_surfs[*(uint16_t*)trigger->targets].flags&=~IS_MOVING;
					}
				}
			}

			break;
	}
}

void trigger_check(trigger_t *trigger){
	if(trigger_rtf[((int)trigger-(int)triggers)/sizeof(trigger_t)] & TRIGGER_FLAG_ACTIVE) trigger_action(trigger);

	else switch(trigger->type & 0b11000000){
		case TRIGGER_INPUT_MODE_COLISION:
			if(line_colides_square(&trigger->p1,&trigger->p2,player->pos.x,player->pos.y,player->width/2)) trigger_action(trigger);
			break;
		case TRIGGER_INPUT_MODE_PRESS:
			if(inputs[' ']) if(line_colides_square(&trigger->p1,&trigger->p2,player->pos.x,player->pos.y,player->width/2)) trigger_action(trigger);
			break;
		case TRIGGER_INPUT_MODE_SHOOT:
			break;
		case TRIGGER_INPUT_MODE_AUTO:
			trigger_action(trigger);
			break;
	}
}


void general_think(actor_t *actor){
	add_to_point(&actor->pos,&actor->vel);	//velocety aplication

	actor->walk_timer--;

	if(actor->reaction_time>0 && actor->target) actor->reaction_time--;


	if(actor->flags & SOLID){
		//actor-actor collision
		if(!(actor->flags & FRAGILE)) for(int i=0;i<max_actor_count;i++) if(actors[i] && actors[i]!=actor) if((actors[i]->flags & SOLID) && !(actors[i]->flags & FRAGILE)) if(actor_colides_actor(actor,actors[i])) push_actor_out_of_actor(actors[i],actor);

		//actor-world colision
		if(actor!=player || !allow_no_clip) world_colision(actor);
	}
}






