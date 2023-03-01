void thinker_player(actor_t *actor){
	actor->target=NULL;
	actor->vel.x*=.88;
	actor->vel.y*=.88;

	if(fast_fabs(player->vel.x)<.01) player->vel.x=0;
	if(fast_fabs(player->vel.y)<.01) player->vel.y=0;

	if(actor->state==ACTOR_STATE_DEAD){
		if(player->height>.1) player->height-=.075;
	}
	else player->height=2;

	if(player->state!=ACTOR_STATE_DEAD){
		switch(player_gun_state){
			case GUN_STATE_IDLE:
				if(inputs[key_ctrl] && player_current_ammo[gun_ammo_type[player_held_gun]]){
					for(int i=0;i<max_actor_count;i++) if(actors[i]) if(!actors[i]->target && ((actors[i]->flags & (FRAGILE | CAN_THINK))==CAN_THINK)) actors[i]->target=player;

					player_gun_state=GUN_STATE_FIRING;
					player_gun_anim_timer=0;
					break;
				}
				if(player_target_gun!=player_held_gun) player_gun_state=GUN_STATE_LOWERING;
				break;

			case GUN_STATE_FIRING:
				if(player_gun_anim_timer==0){
					if(!inputs[key_ctrl]) player_gun_state=GUN_STATE_IDLE;
					if(player_held_gun!=player_target_gun) player_gun_state=GUN_STATE_LOWERING;
				}
				if(player_gun_anim_timer==1 || (player_held_gun==GUN_PLASMA && gametick%3 && inputs[key_ctrl] && player_gun_anim_timer<4)) player_fire_gun();
				break;

			case GUN_STATE_RAISING:
				player_gun_y_offset-=2;
				if(player_gun_y_offset<=0) player_gun_state=GUN_STATE_IDLE;
				break;

			case GUN_STATE_LOWERING:
				player_gun_y_offset+=2;
				if(player_gun_y_offset>sizeof(tx_background)/sizeof(tx_background[0])-3){
					player_held_gun=player_target_gun;
					player_gun_state=GUN_STATE_RAISING;
				}
				break;
		}

		player_gun_anim_timer++;
	}
	else player_gun_state=GUN_STATE_IDLE;
}

void thinker_zombie(actor_t *actor){
	switch(actor->state){
		case ACTOR_STATE_DO_NOTHING:
			break;

		case ACTOR_STATE_IDLE:
			if(actor->target){
				if(actor->target->state==ACTOR_STATE_DEAD || actor->target->state==ACTOR_STATE_GIBED){
					actor->target=NULL;
					break;
				}

				if(try_attack(actor)) initiate_ranged_attack(actor);
				else walk(actor);

			}
			else look_for_player(actor);
			break;

		case ACTOR_STATE_WALKING:
			if(!actor->walk_timer) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_RANGED_ATTACK:
			if(actor->anim_timer==9) fire_hitscan(actor,rand_ang_range(22*pi/180),rand_dmg_range(3,15,3));
			if(actor->anim_timer==0) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_MELEE_ATTACK:
			switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_DEAD:
			actor->vel.x*=.88;
			actor->vel.y*=.88;
			break;

		case ACTOR_STATE_GIBED:
			actor->vel.x*=.88;
			actor->vel.y*=.88;
			break;

		case ACTOR_STATE_PAIN:
			if(actor->anim_timer==0) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_RAISE:
			break;

		default:
			break;
	}
}

void thinker_shotguner(actor_t *actor){
	switch(actor->state){
		case ACTOR_STATE_DO_NOTHING:
			break;

		case ACTOR_STATE_IDLE:
			if(actor->target){
				if(actor->target->state==ACTOR_STATE_DEAD || actor->target->state==ACTOR_STATE_GIBED){
					actor->target=NULL;
					break;
				}

				if(try_attack(actor)) initiate_ranged_attack(actor);
				else walk(actor);

			}
			else look_for_player(actor);
			break;

		case ACTOR_STATE_WALKING:
			if(!actor->walk_timer) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_RANGED_ATTACK:
			if(actor->anim_timer==9) for(int i=0;i<2;i++) fire_hitscan(actor,rand_ang_range(22*pi/180),rand_dmg_range(3,15,3));
			if(actor->anim_timer==0) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_MELEE_ATTACK:
			switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_DEAD:
			actor->vel.x*=.88;
			actor->vel.y*=.88;
			break;

		case ACTOR_STATE_GIBED:
			actor->vel.x*=.88;
			actor->vel.y*=.88;
			break;

		case ACTOR_STATE_PAIN:
			if(actor->anim_timer==0) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_RAISE:
			break;

		default:
			break;
	}
}

void thinker_imp(actor_t *actor){
	switch(actor->state){
		case ACTOR_STATE_DO_NOTHING:
			break;

		case ACTOR_STATE_IDLE:
			if(actor->target){
				if(actor->target->state==ACTOR_STATE_DEAD || actor->target->state==ACTOR_STATE_GIBED){
					actor->target=NULL;
					break;
				}

				if(try_attack(actor)) initiate_ranged_attack(actor);
				else walk(actor);

				try_melee_attack(actor);

			}
			else look_for_player(actor);
			break;

		case ACTOR_STATE_WALKING:
			if(!actor->walk_timer) switch_state(actor, ACTOR_STATE_IDLE);
			try_melee_attack(actor);
			break;

		case ACTOR_STATE_RANGED_ATTACK:
			if(actor->anim_timer==10) fire_projectile(actor, ACTOR_IMP_FIREBALL);
			if(actor->anim_timer==0) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_MELEE_ATTACK:
			if(actor->anim_timer==10){
				if(in_melee_range(actor)) damage_actor(actor,actor->target,rand_dmg_range(3,24,3));
				else fire_projectile(actor, ACTOR_IMP_FIREBALL);
			}
			if(actor->anim_timer==0) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_DEAD:
			actor->vel.x*=.88;
			actor->vel.y*=.88;
			break;

		case ACTOR_STATE_GIBED:
			actor->vel.x*=.88;
			actor->vel.y*=.88;
			break;

		case ACTOR_STATE_PAIN:
			if(actor->anim_timer==0) switch_state(actor, ACTOR_STATE_IDLE);
			break;

		case ACTOR_STATE_RAISE:
			break;

		default:
			break;
	}
}

void thinker_baron_fireball(actor_t *actor){
	if(actor->walk_timer<=0) delete_actor(actor);	//temporary memory leak prevention

	if(actor->state==ACTOR_STATE_WALKING) projectile_hit_check(actor);

	if(actor->state==ACTOR_STATE_DEAD){
		//if(!actor->anim_timer) delete_actor(actor);
		if(actor->anim_timer>11) delete_actor(actor);

		if(actor->anim_timer==1 && actor->target) damage_actor(actor,actor->target,rand_dmg_range(3,24,3));
	}
}

void thinker_rocket(actor_t *actor){
	if(actor->walk_timer<=0) delete_actor(actor);	//temporary memory leak prevention

	if(actor->state==ACTOR_STATE_WALKING) projectile_hit_check(actor);

	if(actor->state==ACTOR_STATE_DEAD){
		//if(!actor->anim_timer) delete_actor(actor);
		if(actor->anim_timer>15) delete_actor(actor);

		if(actor->anim_timer==1) explode(actor,rand_dmg_range(20,160,20));
	}
}

void thinker_plasma_bolt(actor_t *actor){
	if(actor->walk_timer<=0) delete_actor(actor);	//temporary memory leak prevention

	if(actor->state==ACTOR_STATE_WALKING) projectile_hit_check(actor);

	if(actor->state==ACTOR_STATE_DEAD){
		//if(!actor->anim_timer) delete_actor(actor);
		if(actor->anim_timer>11) delete_actor(actor);

		if(actor->anim_timer==1 && actor->target) damage_actor(actor,actor->target,rand_dmg_range(5,40,5));
	}
}

void thinker_bfg_ball(actor_t *actor){
	if(actor->walk_timer<=0) delete_actor(actor);	//temporary memory leak prevention

	if(actor->state==ACTOR_STATE_WALKING) projectile_hit_check(actor);

	if(actor->state==ACTOR_STATE_DEAD){
		//if(!actor->anim_timer) delete_actor(actor);
		if(actor->anim_timer>40) delete_actor(actor);

		if(actor->anim_timer==20){
			if(actor->target) damage_actor(actor,actor->target,rand_dmg_range(100,800,100));
			fire_bfg_tracers(actor);
		}
	}
}

void thinker_potion(actor_t *actor){
	actor->state=ACTOR_STATE_DO_NOTHING;
	if(actor_colides_actor(player,actor)){
		player->health++;
		pickup_screen_tint_timer=6;
		if(player->health>200) player->health=200;
		actor->flags|=DELETE_ME;
	}
}


const void (*thinkers[])(actor_t*)={thinker_player,thinker_zombie,thinker_shotguner,thinker_imp,thinker_baron_fireball,thinker_rocket,thinker_plasma_bolt,thinker_bfg_ball,thinker_potion};

void do_a_think(actor_t *actor){
	if(thinkers[actor->type]) thinkers[actor->type](actor);	//specific think

	actor->anim_timer++;

	if(actor->flags & CAN_THINK) general_think(actor);	//general think

	if(actor->flags & DELETE_ME){
		deregister_actor(actor);
		free(actor);
	}
}
