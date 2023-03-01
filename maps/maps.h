/*
 * vrstni red tock pri steni:
 * 1. in 2. morta met isti z
 * 1. in 3. morta met razlicn z
 *
 * (levo spodi, desno spodi, desno zgori, levo zgori)
*/

/*
const polygon_t debug_map[]={
		{
			4,
			{
				{0,5,0, 0,99},
				{5,0,0, 99,99},
				{5,0,5, 99,0},
				{0,5,5, 0,0},
			},
			{-.7071,-.7071,0},
			mk_tx(tx_erik, 0),
			IS_WALL | IS_SOLID,
		},
		{
			3,
			{
				{0,0,0, 0,99},
				{5,0,0, 99,99},
				{0,5,0, 0,0},
			},
			{0,0,1},
			mk_tx(tx_erik, 0),
			IS_SOLID,
		},
		{
			4,
			{
				{0,-5,-1, 0,99},
				{5,-5,-1, 99,99},
				{5,0,-1, 99,0},
				{0,0,-1, 0,0},
			},
			{0,0,1},
			mk_tx(tx_cooler_revenant, 0),
			IS_SOLID,
		},
		{
			4,
			{
				{-6,0,0, 0,99},
				{-3,0,0, 99,99},
				{-3,0,3, 99,0},
				{-6,0,3, 0,0},
			},
			{0,-1,0},
			mk_tx(tx_cooler_revenant, 0),
			IS_WALL | IS_SOLID,
		},
		{
			4,
			{
				{-3,0,0, 0,99},
				{-3,3,0, 99,99},
				{-3,3,3, 99,0},
				{-3,0,3, 0,0},
			},
			{1,0,0},
			mk_tx(tx_cooler_revenant, 0),
			IS_WALL | IS_SOLID,
		},
};
*/


const polygon_t e1m1_static_surfs[]={
	{
		4,
		{
			{0,0,0, 0,0},
			{0,40,0, 0,15*64},
			{40,40,0, 15*64,15*64},
			{40,0,0, 15*64,0},
		},
		{0,0,1},
		mk_tx(tx_floor0, 0),
		IS_SOLID,
	},
	{
		4,
		{
			{0,0,6, 0,0},
			{0,40,6, 0,15*64},
			{40,40,6, 15*64,15*64},
			{40,0,6, 15*64,0},
		},
		{0,0,-1},
		mk_tx(tx_skybox1, 0),
		IS_SOLID | SKYBOX,
	},
	{
		4,
		{
			{0,0,0, 0,128},
			{0,40,0, 8*64,128},
			{0,40,6, 8*64,0},
			{0,0,6, 0,0},
		},
		{1,0,0},
		mk_tx(tx_wall0, 0),
		IS_SOLID | IS_WALL,
	},
	{
		4,
		{
			{40,40,0, 0,128},
			{40,0,0, 8*64,128},
			{40,0,6, 8*64,0},
			{40,40,6, 0,0},
		},
		{-1,0,0},
		mk_tx(tx_wall0, 0),
		IS_SOLID | IS_WALL,
	},
	{
		4,
		{
			{0,0,0, 0,128},
			{40,0,0, 8*64,128},
			{40,0,6, 8*64,0},
			{0,0,6, 0,0},
		},
		{0,1,0},
		mk_tx(tx_wall0, 0),
		IS_SOLID | IS_WALL,
	},
	{
		4,
		{
			{40,40,0, 0,128},
			{0,40,0, 8*64,128},
			{0,40,6, 8*64,0},
			{40,40,6, 0,0},
		},
		{0,-1,0},
		mk_tx(tx_wall0, 0),
		IS_SOLID | IS_WALL,
	},

	{
		4,
		{
			{.2,18,1, 0,100},
			{.2,22,1, 100,100},
			{.2,22,5, 100,0},
			{.2,18,5, 0,0},
		},
		{1,0,0},
		mk_tx(tx_cooler_revenant, 0),
		IS_WALL,
	},

	{
		4,
		{
			{20,18,0, 0,100},
			{20,22,0, 100,100},
			{20,22,4, 100,0},
			{20,18,4, 0,0},
		},
		{-1,0,0},
		mk_tx(tx_cooler_revenant, 0),
		IS_WALL | IS_SOLID,
	},
	{
		4,
		{
			{20,18,0, 0,100},
			{24,18,0, 100,100},
			{24,18,4, 100,0},
			{20,18,4, 0,0},
		},
		{0,-1,0},
		mk_tx(tx_cooler_revenant, 0),
		IS_WALL | IS_SOLID,
	},
	{
		4,
		{
			{24,18,0, 0,100},
			{20,22,0, 100,100},
			{20,22,4, 100,0},
			{24,18,4, 0,0},
		},
		{.7071067811865476,.7071067811865476,0},
		mk_tx(tx_cooler_revenant, 0),
		IS_WALL | IS_SOLID,
	},
};

const polygon_t e1m1_dynamic_surfs[]={
	{
		4,
		{
			{8,12,0, 0,128},
			{8,7,0, 128,128},
			{8,7,5, 128,0},
			{8,12,5, 0,0},
		},
		{-1,0,0},
		mk_tx(tx_door,0),
		IS_SOLID | IS_WALL
	},
	{
		4,
		{
			{9,12,0, 0,128},
			{9,7,0, 128,128},
			{9,7,5, 128,0},
			{9,12,5, 0,0},
		},
		{1,0,0},
		mk_tx(tx_door,1),
		IS_SOLID | IS_WALL
	}
};


const trigger_t e1m1_triggers[]={
	{
		.p1={.1,10,1.5},
		.p2={.1,11,2.5},
		.type=TRIGGER_MODE_MOVE_AUTOCLOSE | TRIGGER_INPUT_MODE_PRESS,
		.move_time=30,
		.vel={0,0,3./30},
		.targets=(const uint16_t[]){0,1,-1},
		.tx_on=mk_tx(tx_switch0_on,0),
		.tx_off=mk_tx(tx_switch0_off,0),
	},
	{
		.p1={1,.5,0},
		.p2={5,.5,2},
		.type=TRIGGER_MODE_SPAWNER | TRIGGER_INPUT_MODE_COLISION,
		.targets=(const actor_t[]){{.pos={3,10,0},.width=2,.height=2,.type=ACTOR_IMP,.state=ACTOR_STATE_IDLE,.health=100,.flags=CAN_THINK | SHOOTABLE | SOLID},{.pos={9,10,0},.width=2,.height=2,.type=ACTOR_ZOMBIE,.state=ACTOR_STATE_IDLE,.health=100,.flags=CAN_THINK | SHOOTABLE | SOLID},{.type=255}},
	},
};

const actor_t e1m1_actors[]={
	{
		.pos={2,2,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_PLAYER,
		.state=ACTOR_STATE_IDLE,
		.health=100,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={3*1,30,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_IMP,
		.state=ACTOR_STATE_IDLE,
		.health=60,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={3*2,30,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_IMP,
		.state=ACTOR_STATE_IDLE,
		.health=60,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={3*3,30,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_IMP,
		.state=ACTOR_STATE_IDLE,
		.health=60,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={10,10,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	}
};


map_t e1m1={
	e1m1_static_surfs,
	sizeof(e1m1_static_surfs)/sizeof(e1m1_static_surfs[0]),
	e1m1_dynamic_surfs,
	sizeof(e1m1_dynamic_surfs)/sizeof(e1m1_dynamic_surfs[0]),
	e1m1_triggers,
	sizeof(e1m1_triggers)/sizeof(trigger_t),
	e1m1_actors,
	sizeof(e1m1_actors)/sizeof(actor_t),
};


const polygon_t informativni_static_surfs[]={
		{
			4,
			{
				{8,.2,1, 0,100},
				{12,.2,1, 100,100},
				{12,.2,5, 100,0},
				{8,.2,5, 0,0},
			},
			{0,1,0},
			mk_tx(tx_cooler_revenant, 0),
			IS_WALL,
		},

		{
			4,
			{
				{0,0,0, 0,0},
				{0,20,0, 0,8*64},
				{20,20,0, 8*64,8*64},
				{20,0,0, 8*64,0},
			},
			{0,0,1},
			mk_tx(tx_floor0, 0),
			IS_SOLID,
		},
		{
			4,
			{
				{0,0,6, 0,0},
				{0,20,6, 0,8*64},
				{20,20,6, 8*64,8*64},
				{20,0,6, 8*64,0},
			},
			{0,0,-1},
			mk_tx(tx_ceil1, 0),
			IS_SOLID,
		},
		{
			4,
			{
				{0,0,0, 0,128},
				{0,20,0, 8*64,128},
				{0,20,6, 8*64,0},
				{0,0,6, 0,0},
			},
			{1,0,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{20,20,0, 0,128},
				{20,0,0, 8*64,128},
				{20,0,6, 8*64,0},
				{20,20,6, 0,0},
			},
			{-1,0,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{0,0,0, 0,128},
				{20,0,0, 8*64,128},
				{20,0,6, 8*64,0},
				{0,0,6, 0,0},
			},
			{0,1,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},

		{
			4,
			{
				{0,20,4.5, 0,128*1.5/6},
				{20,20,4.5, 8*64,128*1.5/6},
				{20,20,6, 8*64,0},
				{0,20,6, 0,0},
			},
			{0,-1,0},
			mk_tx(tx_wall0, 0),
			IS_WALL,
		},
		{
			4,
			{
				{0,20,0, 0,128},
				{7.5,20,0, 7.5*64*8/20,128},
				{7.5,20,4.5, 7.5*64*8/20,128*1.5/6},
				{0,20,4.5, 0,128*1.5/6},
			},
			{0,-1,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{12.5,20,0, 8*64*12.5/20,128},
				{20,20,0, 8*64,128},
				{20,20,4.5, 8*64,128*1.5/6},
				{12.5,20,4.5, 8*64*12.5/20,128*1.5/6},
			},
			{0,-1,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},





		{
			4,
			{
				{7.5,20,0, 8*64*7.5/20,8*64},
				{12.5,20,0, 8*64*12.5/20,8*64},
				{12.5,30,0, 8*64*12.5/20,8*64+10*64*8/20},
				{7.5,30,0, 8*64*7.5/20,8*64+10*64*8/20},
			},
			{0,0,1},
			mk_tx(tx_floor0, 0),
			IS_SOLID,
		},

		{
			4,
			{
				{7.5,20,4.5, 8*64*7.5/20,8*64},
				{12.5,20,4.5, 8*64*12.5/20,8*64},
				{12.5,24.5,4.5, 8*64*12.5/20,8*64+4.5*64*8/20},
				{7.5,24.5,4.5, 8*64*7.5/20,8*64+4.5*64*8/20},
			},
			{0,0,-1},
			mk_tx(tx_ceil1, 0),
			IS_SOLID,
		},
		{
			4,
			{
				{7.5,25.5,4.5, 8*64*7.5/20,8*64+5.5*64*8/20},
				{12.5,25.5,4.5, 8*64*12.5/20,8*64+5.5*64*8/20},
				{12.5,30,4.5, 8*64*12.5/20,8*64+10*64*8/20},
				{7.5,30,4.5, 8*64*7.5/20,8*64+10*64*8/20},
			},
			{0,0,-1},
			mk_tx(tx_ceil1, 0),
			IS_SOLID,
		},

		{
			4,
			{
				{7.5,20,0, 0,128},
				{7.5,30,0, 10*64*8/20,128},
				{7.5,30,4.5, 10*64*8/20,128*1.5/6},
				{7.5,20,4.5, 0,128*1.5/6},
			},
			{1,0,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{12.5,30,0, 0,128},
				{12.5,20,0, 10*64*8/20,128},
				{12.5,20,4.5, 10*64*8/20,128*1.5/6},
				{12.5,30,4.5, 0,128*1.5/6},
			},
			{-1,0,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},





		{
			4,
			{
				{-10,30,0, 0+25.6*2/3,30*8*64/20},
				{-10,60,0, 25.6*2/3+40*8*64/20,30*8*64/20},
				{30,60,0, 25.6*2/3+40*8*64/20,0},
				{30,30,0, 0+25.6*2/3,0},
			},
			{0,0,1},
			mk_tx(tx_floor0, 0),
			IS_SOLID,
		},
		{
			4,
			{
				{-10,30,6, 0,0},
				{-10,60,6, 0,8*64},
				{30,60,6, 8*64,8*64},
				{30,30,6, 8*64,0},
			},
			{0,0,-1},
			mk_tx(tx_skybox1, 0),
			IS_SOLID | SKYBOX,
		},
		{
			4,
			{
				{-10,30,0, 0,128},
				{-10,60,0, 8*64,128},
				{-10,60,3, 8*64,0},
				{-10,30,3, 0,0},
			},
			{1,0,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{-10,30,3, 0,128},
				{-10,60,3, 8*64,128},
				{-10,60,6, 8*64,0},
				{-10,30,6, 0,0},
			},
			{1,0,0},
			mk_tx(tx_skybox1, 0),
			IS_SOLID | IS_WALL | SKYBOX,
		},
		{
			4,
			{
				{30,60,0, 0,128},
				{30,30,0, 8*64,128},
				{30,30,3, 8*64,0},
				{30,60,3, 0,0},
			},
			{-1,0,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{30,60,3, 0,128},
				{30,30,3, 8*64,128},
				{30,30,6, 8*64,0},
				{30,60,6, 0,0},
			},
			{-1,0,0},
			mk_tx(tx_skybox1, 0),
			IS_SOLID | IS_WALL | SKYBOX,
		},
		{
			4,
			{
				{-10,60,0, 0,128},
				{30,60,0, 8*64,128},
				{30,60,3, 8*64,0},
				{-10,60,3, 0,0},
			},
			{0,-1,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{-10,60,3, 0,128},
				{30,60,3, 8*64,128},
				{30,60,6, 8*64,0},
				{-10,60,6, 0,0},
			},
			{0,-1,0},
			mk_tx(tx_skybox1, 0),
			IS_SOLID | IS_WALL | SKYBOX,
		},

		{
			4,
			{
				{-10,30,4.5, 0,128*1.5/6},
				{30,30,4.5, 8*64,128*1.5/6},
				{30,30,6, 8*64,0},
				{-10,30,6, 0,0},
			},
			{0,1,0},
			mk_tx(tx_wall0, 0),
			IS_WALL,
		},
		{
			4,
			{
				{-10,30,0, 0,128},
				{7.5,30,0, 7.5*64*8/20,128},
				{7.5,30,4.5, 7.5*64*8/20,128*1.5/6},
				{-10,30,4.5, 0,128*1.5/6},
			},
			{0,1,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
		{
			4,
			{
				{12.5,30,0, 8*64*12.5/20,128},
				{30,30,0, 8*64,128},
				{30,30,4.5, 8*64,128*1.5/6},
				{12.5,30,4.5, 8*64*12.5/20,128*1.5/6},
			},
			{0,1,0},
			mk_tx(tx_wall0, 0),
			IS_SOLID | IS_WALL,
		},
};

const polygon_t informativni_dynamic_surfs[]={
	{
		4,
		{
			{7.5,24.5,0, 0,128},
			{12.5,24.5,0, 128,128},
			{12.5,24.5,4.5, 128,128/10},
			{7.5,24.5,4.5, 0,128/10},
		},
		{0,-1,0},
		mk_tx(tx_door, 0),
		IS_SOLID | IS_WALL,
	},
	{
		4,
		{
			{12.5,25.5,0, 0,128},
			{7.5,25.5,0, 128,128},
			{7.5,25.5,4.5, 128,128/10},
			{12.5,25.5,4.5, 0,128/10},
		},
		{0,1,0},
		mk_tx(tx_door, 0),
		IS_SOLID | IS_WALL,
	},
	{
		4,
		{
			{7.5,24.5,0, 0,16},
			{12.5,24.5,0, 5*8,16},
			{12.5,25.5,0, 5*8,0},
			{7.5,25.5,0, 0,0},
		},
		{0,0,-1},
		mk_tx(tx_door_floor, 0),
		0,
	},
};

const trigger_t informativni_triggers[]={
	{
		.p1={7.5,24.45,1.5},
		.p2={12.5,24.45,2.5},
		.type=TRIGGER_MODE_MOVE_AUTOCLOSE | TRIGGER_INPUT_MODE_PRESS,
		.move_time=30,
		.vel={0,0,4.5/30},
		.targets=(const uint16_t[]){0,1,2,-1},
		.tx_on={NULL,0,0},
		.tx_off={NULL,0,0},
	},
	{
		.p1={7.5,25.55,1.5},
		.p2={12.5,25.55,2.5},
		.type=TRIGGER_MODE_MOVE_AUTOCLOSE | TRIGGER_INPUT_MODE_PRESS,
		.move_time=30,
		.vel={0,0,4.5/30},
		.targets=(const uint16_t[]){0,1,2,-1},
		.tx_on={NULL,0,0},
		.tx_off={NULL,0,0},
	},
};

const actor_t informativni_actors[]={
	{
		.pos={10,2,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_PLAYER,
		.state=ACTOR_STATE_IDLE,
		.health=100,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={-5,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_ZOMBIE,
		.state=ACTOR_STATE_IDLE,
		.health=20,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={-2,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_ZOMBIE,
		.state=ACTOR_STATE_IDLE,
		.health=20,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={1,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_ZOMBIE,
		.state=ACTOR_STATE_IDLE,
		.health=20,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={7,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_IMP,
		.state=ACTOR_STATE_IDLE,
		.health=60,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={10,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_IMP,
		.state=ACTOR_STATE_IDLE,
		.health=60,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={13,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_IMP,
		.state=ACTOR_STATE_IDLE,
		.health=60,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={19,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_SHOTGUNER,
		.state=ACTOR_STATE_IDLE,
		.health=30,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={22,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_SHOTGUNER,
		.state=ACTOR_STATE_IDLE,
		.health=30,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
	{
		.pos={25,50,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_SHOTGUNER,
		.state=ACTOR_STATE_IDLE,
		.health=30,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.walk_timer=0,
		.reaction_time=8,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},

	{
		.pos={18,17,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	},
	{
		.pos={18,15,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	},
	{
		.pos={18,13,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	},
	{
		.pos={18,11,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	},
	{
		.pos={18,9,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	},
	{
		.pos={18,7,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	},
	{
		.pos={18,4,0},
		.vel={0,0,0},
		.width=1,
		.height=1,
		.type=ACTOR_POTION,
		.state=ACTOR_STATE_DO_NOTHING,
		.anim_timer=0,
		.flags=0,
	},
};




map_t informativni={
	informativni_static_surfs,
	sizeof(informativni_static_surfs)/sizeof(informativni_static_surfs[0]),
	informativni_dynamic_surfs,
	sizeof(informativni_dynamic_surfs)/sizeof(informativni_dynamic_surfs[0]),
	informativni_triggers,
	sizeof(informativni_triggers)/sizeof(trigger_t),
	informativni_actors,
	sizeof(informativni_actors)/sizeof(actor_t),
};



const polygon_t raziskovalna_static_surfs[]={
	/*
	{
		4,
		{
			{0,5,1, 0,100},
			{4,5,1, 100,100},
			{4,5,5, 100,0},
			{0,5,5, 0,0},
		},
		{0,-1,0},
		mk_tx(tx_cooler_revenant, 0),
		IS_WALL,
	},
	*/
	{
		4,
		{
			{0,10,1, 0,100},
			{4,10,1, 100,100},
			{4,10,5, 100,0},
			{0,10,5, 0,0},
		},
		{0,-1,0},
		mk_tx(tx_erik, 0),
		IS_WALL,
	},
};

const actor_t raziskovalna_actors[]={
	{
		.pos={0,0,0},
		.vel={0,0,0},
		.roth=pi/2,
		.width=2,
		.height=2,
		.type=ACTOR_PLAYER,
		.state=ACTOR_STATE_IDLE,
		.health=100,
		.source=NULL,
		.target=NULL,
		.anim_timer=0,
		.flags=CAN_THINK | SHOOTABLE | SOLID,
	},
};

map_t raziskovalna={
	raziskovalna_static_surfs,
	sizeof(raziskovalna_static_surfs)/sizeof(raziskovalna_static_surfs[0]),
	NULL,
	0,
	NULL,
	0,
	raziskovalna_actors,
	sizeof(raziskovalna_actors)/sizeof(raziskovalna_actors[0]),
};





