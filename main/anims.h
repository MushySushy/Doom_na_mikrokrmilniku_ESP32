const anim_t anims_zombie[][max_anim_num]={
		{{255,0}},					//do nothing
		{{0,12},{1,24},{255,0}},	//asleep
		{{0,5},{1,10},{2,15},{3,20},{255,0}},					//walking
		{{4,5},{5,10},{255,0}},					//ranged attack
		{{255,0}},					//melee attack
		{{7,4},{8,8},{9,12},{10,16},{11,20},{255,0}},			//dead
		{{12,4},{13,8},{14,12},{15,16},{16,20},{17,24},{18,28},{19,32},{20,36},{255,0}},					//gibed
		{{6,5},{255,0}},										//pain
		{{10,4},{9,8},{8,12},{7,16},{6,20},{255,0}},			//raise
};

const anim_t anims_shotguner[][max_anim_num]={
		{{255,0}},					//do nothing
		{{0,12},{1,24},{255,0}},	//asleep
		{{0,5},{1,10},{2,15},{3,20},{255,0}},					//walking
		{{4,5},{5,10},{255,0}},					//ranged attack
		{{255,0}},					//melee attack
		{{7,4},{8,8},{9,12},{10,16},{11,20},{255,0}},			//dead
		{{12,4},{13,8},{14,12},{15,16},{16,20},{17,24},{18,28},{19,32},{20,36},{255,0}},					//gibed
		{{6,5},{255,0}},										//pain
		{{10,4},{9,8},{8,12},{7,16},{6,20},{255,0}},			//raise
};

const anim_t anims_imp[][max_anim_num]={
		{{255,0}},					//do nothing
		{{0,12},{1,24},{255,0}},	//asleep
		{{0,5},{1,10},{2,15},{3,20},{255,0}},					//walking
		{{4,5},{5,10},{6,14},{255,0}},					//ranged attack
		{{4,5},{5,10},{6,14},{255,0}},					//melee attack
		{{8,4},{9,8},{10,12},{11,16},{12,20},{255,0}},			//dead
		{{13,4},{14,8},{15,12},{16,16},{17,20},{18,24},{19,28},{20,32},{255,0}},					//gibed
		{{7,5},{255,0}},										//pain
		{{12,4},{11,8},{10,12},{9,16},{8,20},{255,0}},			//raise

};

const anim_t anims_baron_fireball[][max_anim_num]={
		{{255,0}},					//do nothing
		{{255,0}},	//asleep
		{{0,5},{1,10},{255,0}},					//walking
		{{255,0}},					//ranged attack
		{{255,0}},					//melee attack
		{{2,4},{3,8},{4,12},{255,0}},			//dead
		{{255,0}},					//gibed

};

const anim_t anims_rocket[][max_anim_num]={
		{{255,0}},					//do nothing
		{{255,0}},	//asleep
		{{0,255},{255,0}},					//walking
		{{255,0}},					//ranged attack
		{{255,0}},					//melee attack
		{{1,6},{2,12},{3,16},{255,0}},			//dead
		{{255,0}},					//gibed

};

const anim_t anims_plasma_bolt[][max_anim_num]={
		{{255,0}},					//do nothing
		{{255,0}},	//asleep
		{{0,10},{1,20},{255,0}},					//walking
		{{255,0}},					//ranged attack
		{{255,0}},					//melee attack
		{{2,2},{3,6},{4,10},{5,14},{255,0}},			//dead
		{{255,0}},					//gibed

};

const anim_t anims_bfg_ball[][max_anim_num]={
		{{255,0}},					//do nothing
		{{255,0}},	//asleep
		{{0,10},{1,20},{255,0}},					//walking
		{{255,0}},					//ranged attack
		{{255,0}},					//melee attack
		{{2,4},{3,12},{4,20},{5,28},{6,36},{7,40},{255,0}},			//dead
		{{255,0}},					//gibed

};

const anim_t anims_base_potion[][max_anim_num]={
		{{0,10},{1,20},{2,30},{3,40},{255,0}},
};

//more bit urejeno kot v enum actor_types
const anim_t (*actor_anims_base[])[max_anim_num]={NULL,anims_zombie,anims_shotguner,anims_imp,anims_baron_fireball,anims_rocket,anims_plasma_bolt,anims_bfg_ball,anims_base_potion};




const anim_t anim_base_fist[][max_anim_num]={
		{{255,0}},		//idle
		{{1,4},{2,8},{3,12},{2,16},{1,20},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};

const anim_t anim_base_chainsaw[][max_anim_num]={
		{{0,5},{1,10},{255,0}},		//idle
		{{2,3},{3,6},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};

const anim_t anim_base_pistol[][max_anim_num]={
		{{255,0}},		//idle
		{{0,3},{1,7},{2,12},{3,14},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};

const anim_t anim_base_shotgun[][max_anim_num]={
		{{255,0}},		//idle
		{{0,1},{1,5},{2,8},{3,12},{4,16},{5,20},{4,24},{3,26},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};

const anim_t anim_base_chaingun[][max_anim_num]={
		{{255,0}},		//idle
		{{1,4},{2,7},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};

const anim_t anim_base_rocket_launcher[][max_anim_num]={
		{{255,0}},		//idle
		{{1,3},{2,7},{3,11},{4,15},{5,17},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};

const anim_t anim_base_plasma[][max_anim_num]={
		{{255,0}},		//idle
		{{3,20},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};

const anim_t anim_base_bfg[][max_anim_num]={
		{{255,0}},		//idle
		{{0,18},{1,24},{2,28},{255,0}},		//firing
		{{255,0}},		//raising
		{{255,0}},		//lowering
};



const anim_t (*gun_anims_base[])[max_anim_num]={anim_base_fist,anim_base_chainsaw,anim_base_pistol,anim_base_shotgun,anim_base_chaingun,anim_base_rocket_launcher,anim_base_plasma,anim_base_bfg};




const anim_t anim_base_poof[]={{0,4},{1,8},{2,12},{3,16},{255,0}};
const anim_t anim_base_bfg_tracer[]={{0,4},{1,8},{2,12},{3,16},{255,0}};


const anim_t *particle_anims_base[]={anim_base_poof,anim_base_bfg_tracer};








