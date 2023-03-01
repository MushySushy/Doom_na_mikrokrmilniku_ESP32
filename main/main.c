#include <sdkconfig.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/i2s_std.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

/*
#include "../zvok/snd_exp.h"
#include "../zvok/snd_tadi.h"
#include "../zvok/snd_sin.h"
#include "../zvok/snd_door.h"
*/


#define SCREENSHOTS 1
#define TARGET_FPS 30
#define TRANSPARENT_SURF_BACKS 1
#define SKYBOX_REPEAT_NUM 3
#define MAP_TO_LOAD informativni

#define HFOV_CONST 1.7320508075688767	//tan(120/2°)
#define VFOV_CONST 0.7673269879789604	//tan(hfov*w/h)  (naceloma naj velja vfov=hfov*w/h)
#define VFOV_CONST_NOHUD 0.49314542603130407


#define w 240
#define h 150
#define poly_max_points 8
#define max_actor_count 256
#define max_particle_count 64
#define max_anim_num 10
#define player_speed 1./16

#define melee_range 2.5

#define pi 3.141592

typedef unsigned char byte;


enum keys{key_space=' ', key_up=1, key_left=2, key_down=3, key_right=4, key_shift=5, key_ctrl=6, key_esc=7, key_enter=8, key_f5=9};

#define color(r,g,b)((b<<11)+(g<<5)+r)	//mišljeno le za literale

#define mk_tx(tx,flags)((tx_t){(uint16_t*)tx,sizeof(tx[0])/sizeof(uint16_t),sizeof(tx)/sizeof(tx[0]),(byte)flags})

#define rand_ang_range(x)((doom_rand()-127)*x/128)	//x=kot med line of sight in max
#define rand_dmg_range(min,max,inc)((doom_rand()%((max-min)/inc+1))*inc+min)

enum core1_control_values{CORE1_ACK, CORE1_DRAW_STATIC_SURFS, CORE1_DRAW_DYNAMIC_SURFS, CORE1_DRAW_ACTORS, CORE1_DRAW_PARTICLES, CORE1_DRAW_GUN, CORE1_CALCULATE_COLISIONS, CORE1_CLEAR_Z_BUFFER, CORE1_COPY_SCREEN};

enum surf_flags{IS_WALL=1, IS_SOLID=2, SKYBOX=4, TEMPORARY=8, IS_MOVING=128};

enum actor_flags{CAN_THINK=1, SHOOTABLE=2, SOLID=4, FRAGILE=8, JUST_HIT=16, JUST_ATTACKED=32, /*JUST_BOOPED=64,*/ DELETE_ME=128};

enum texture_flags{MIRRORED=1};

enum trigger_flags{TRIGGER_FLAG_DISABLE=1<<15,TRIGGER_FLAG_ACTIVE=1<<14,TRIGGER_FLAG_SWITCH_ON=1<<13,TRIGGER_FLAG_WAITING=1<<12};

enum guns{GUN_FIST=0, GUN_CHAINSAW=1, GUN_PISTOL=2, GUN_SHOTGUN=3, GUN_CHAINGUN=4, GUN_ROCKET=5, GUN_PLASMA=6, GUN_BFG=7};
enum ammo{AMMO_BULLET=0, AMMO_SHELL=1, AMMO_ROCKET=2, AMMO_CELL=3, AMMO_NONE=4};
enum gun_state{GUN_STATE_IDLE, GUN_STATE_FIRING, GUN_STATE_RAISING, GUN_STATE_LOWERING};
enum hud_face_state{HF_STATE_LOOKL, HF_STATE_LR, HF_STATE_LF, HF_STATE_TL, HF_STATE_TR, HF_STATE_KILL, HF_STATE_GRIN, HF_STATE_AU, HF_STATE_DEAD, HF_STATE_GOD};

typedef enum{PARTICLE_POOF, PARTICLE_BLOOD} particle_type_t;

const int gun_ammo_type[]={AMMO_NONE, AMMO_NONE, AMMO_BULLET, AMMO_SHELL, AMMO_BULLET, AMMO_ROCKET, AMMO_CELL, AMMO_CELL};

enum gamestates{GAMESTATE_RUNNING, GAMESTATE_SCREEN_MELT, GAMESTATE_TITLE_SCREEN, GAMESTATE_PAUSE, GAMESTATE_CREDITS};


//----------actor states----------//

//here, sprites, anims, thinkers
typedef enum __attribute__ ((__packed__)){
	ACTOR_PLAYER,
	ACTOR_ZOMBIE,
	ACTOR_SHOTGUNER,
	ACTOR_IMP,
	ACTOR_IMP_FIREBALL,
	ACTOR_ROCKET,
	ACTOR_PLASMA_BOLT,
	ACTOR_BFG_BALL,
	ACTOR_POTION,
} actor_type_t;

typedef enum __attribute__ ((__packed__)){
	ACTOR_STATE_DO_NOTHING,
	ACTOR_STATE_IDLE,
	ACTOR_STATE_WALKING,
	ACTOR_STATE_RANGED_ATTACK,
	ACTOR_STATE_MELEE_ATTACK,
	ACTOR_STATE_DEAD,
	ACTOR_STATE_GIBED,
	ACTOR_STATE_PAIN,
	ACTOR_STATE_RAISE,
} actor_state_t;

typedef enum __attribute__ ((__packed__)){
	TRIGGER_MODE_DO_NOTHING,
	TRIGGER_MODE_MOVE_SINGLE,
	TRIGGER_MODE_MOVE_AUTOCLOSE,
	TRIGGER_MODE_MOVE_REPEATABLE,
	TRIGGER_MODE_SPAWNER,
	TRIGGER_MODE_TELEPORTER,

	TRIGGER_INPUT_MODE_COLISION=0b00000000,
	TRIGGER_INPUT_MODE_PRESS=0b01000000,
	TRIGGER_INPUT_MODE_SHOOT=0b10000000,
	TRIGGER_INPUT_MODE_AUTO=0b11000000,
} trigger_type_t;


//----------structs----------//



typedef struct{
	float x,y,z;
} point_t;

typedef struct{
	float x,y,z;
	uint16_t tx,ty;
} vert_t;

typedef struct{
	uint16_t *p;
	uint16_t width,height;
	byte flags;
} tx_t;

typedef struct{
	byte num;
	byte counter;
} anim_t;

typedef struct{
	byte size;
	vert_t points[poly_max_points];
	point_t normal;
	tx_t tx;
	byte flags;
} polygon_t;


typedef struct actor_t actor_t;
struct actor_t{
	point_t pos;
	point_t vel;
	double roth;
	float width, height;
	actor_type_t type;
	actor_state_t state;
	actor_t *source;
	actor_t *target;
	int16_t anim_timer;
	int16_t walk_timer;
	byte reaction_time;
	int16_t health;
	byte flags;
};

typedef const struct{
	point_t p1,p2;
	trigger_type_t type;
	uint16_t move_time;
	point_t vel;
	//const polygon_t *const* surfs;		//NULL terminated, do not remove const
	//const actor_t *actors;		//terminates if actor type = actor_player, do not remove const
	const void *targets;
	tx_t tx_on; 	//do not draw if .p=NULL
	tx_t tx_off; 	//do not draw if .p=NULL
} trigger_t;

typedef struct{
	point_t pos;
	point_t vel;
	particle_type_t type;
	int16_t anim_timer;
} particle_t;

typedef const struct{
	char *s;
	void (*func)();
} cheat_t;

typedef const struct{
	int x,y;
	char *txt;
	void (*func)();
} textbox_t;

typedef const struct{
	const polygon_t *static_surfs;
	int static_surf_num;
	const polygon_t *dynamic_surfs;
	int dynamic_surf_num;
	const trigger_t *triggers;
	int trigger_num;
	const actor_t *actors;
	int actor_num;
} map_t;


//----------globals----------//

volatile bool vsync_trigger=false;
volatile unsigned int gametick=0;

volatile byte core1_control_byte=CORE1_ACK;

int gamestate;

uint16_t screen_buf[h][w];
uint16_t screen_out_buf[h][w];

uint16_t (*screen)[w]=screen_buf;
uint16_t (*screen_out)[w]=screen_out_buf;

float z_buffer[h][w];

const polygon_t *static_surfs;
int static_surf_num;
polygon_t *dynamic_surfs;
int dynamic_surf_num;
const trigger_t *triggers;
int trigger_num;
const textbox_t *textboxes;
int textbox_num;

textbox_t *selected_textbox;

uint16_t *trigger_rtf;		//trigger run-time flags

actor_t *actors[max_actor_count];
particle_t *particles[max_particle_count];

actor_t *player=NULL;
float player_rotv=pi/2;


char key_buf[32];
int key_buf_len;

bool inputs[128];
bool inputs_old[128];

char *motd=NULL;
int16_t motd_timer=0;

int pickup_screen_tint_timer=0;

int screen_melt_timer=0;
int screen_melt_offsets[w];


bool allow_freelook=true;
bool allow_drawhud=true;
bool allow_draw_fps=true;
bool allow_no_clip=false;
bool allow_godmode=false;

float fps_old=0;

float player_armor_prot_value=1./2;
int16_t player_armor=200;

uint16_t player_max_ammo[]={200,50,50,200};
uint16_t player_current_ammo[]={100,25,25,100};
uint16_t player_owned_guns=-1;
byte player_held_gun=GUN_PISTOL;
byte player_target_gun=GUN_PISTOL;
byte player_gun_state=GUN_STATE_IDLE;
byte player_gun_anim_timer=0;
int player_gun_y_offset=0;

void switch_gamestate(int state);


//----------assets----------//

#include "../slike/textures.h"
#include "sprites.h"
#include "anims.h"
#include "../maps/maps.h"


//----------funcs----------//

#include "fast_math.c"
#include "colision_funcs.c"
#include "thinkersC.c"
#include "thinkersP.c"
#include "draw_funcs.c"

#include "input_funcs.c"

void start_game();
void resume_game();
void open_settings();
void close_settings();
void quit();


const textbox_t textboxes_start[]={
	{
		w/2+8, h/2-4,
		"start",
		start_game
	},
	{
		w/2+8, h/2-4+12,
		"settings",
		open_settings
	},
	{
		w/2+8, h/2-4+24,
		"quit",
		quit
	}
};

const textbox_t textboxes_pause[]={
	{
		w/2, h/2,
		"continue",
		resume_game
	},
	{
		w/2, h/2+12,
		"reset",
		NULL
	}
};

const textbox_t textboxes_settings[]={
	{
		60, h/2,
		"ne obstaja se",
		close_settings
	},
};

const textbox_t textboxes_quit[]={
	{
		40, h/2,
		"there is no escape",
		close_settings
	},
};

void start_game(){
	switch_gamestate(GAMESTATE_SCREEN_MELT);
}

void resume_game(){
	switch_gamestate(GAMESTATE_RUNNING);
}

void open_settings(){
	textboxes=textboxes_settings;
	textbox_num=sizeof(textboxes_settings)/sizeof(textboxes_settings[0]);
	selected_textbox=textboxes;
}

void close_settings(){
	textboxes=textboxes_start;
	textbox_num=sizeof(textboxes_start)/sizeof(textboxes_start[0]);
	selected_textbox=textboxes;
}

void quit(){
	textboxes=textboxes_quit;
	textbox_num=sizeof(textboxes_quit)/sizeof(textboxes_quit[0]);
	selected_textbox=textboxes;
}



void load_map(const map_t map){
	static_surfs=map.static_surfs;
	static_surf_num=map.static_surf_num;

	dynamic_surf_num=map.dynamic_surf_num;
	dynamic_surfs=malloc(dynamic_surf_num*sizeof(polygon_t));
	for(int i=0;i<dynamic_surf_num;i++) dynamic_surfs[i]=map.dynamic_surfs[i];

	triggers=map.triggers;
	trigger_num=map.trigger_num;

	trigger_rtf=calloc(trigger_num, sizeof(uint16_t));

	for(int i=0;i<map.actor_num;i++){
		actors[i]=malloc(sizeof(actor_t));
		*actors[i]=map.actors[i];
		actors[i]->anim_timer=doom_rand()&7;
		if(!actors[i]->type) player=actors[i];
	}

}

void switch_gamestate(int state){
	switch(state){
		case GAMESTATE_RUNNING:
			textboxes=NULL;
			textbox_num=0;
			selected_textbox=NULL;

			update_hud(screen, 1);
			update_hud(screen_out, 1);

			break;

		case GAMESTATE_SCREEN_MELT:
			screen_melt_timer=h-1;
			for(int i=0;i<w;i++) screen_melt_offsets[i]=-(doom_rand()&31);

			load_map(MAP_TO_LOAD);

			clear_z_buffer();

			core1_control_byte=CORE1_DRAW_STATIC_SURFS;
			draw_static_surfs_until_done();
			while(core1_control_byte!=CORE1_ACK);

			core1_control_byte=CORE1_DRAW_DYNAMIC_SURFS;
			draw_dynamic_surfs_until_done();
			while(core1_control_byte!=CORE1_ACK);

			core1_control_byte=CORE1_DRAW_ACTORS;
			draw_actors_until_done();
			while(core1_control_byte!=CORE1_ACK);

			for(int i=0;i<trigger_num;i++) if(triggers[i].tx_on.p) draw_trigger(triggers+i);

			draw_crosshair();
			draw_held_gun();

			clear_z_buffer();

			update_hud(screen, 1);
			memcpy(screen_out[h-32], tx_title_screen[h-32], 32*w*2);

			break;

		case GAMESTATE_TITLE_SCREEN:
			textboxes=textboxes_start;
			textbox_num=sizeof(textboxes_start)/sizeof(textboxes_start[0]);
			selected_textbox=textboxes;

			break;

		case GAMESTATE_PAUSE:
			memcpy(screen,screen_out,w*h*2);
			for(int y=0;y<h;y++) for(int x=0;x<w;x++) screen[y][x]=(screen[y][x]>>1)&0b0111101111101111;


			textboxes=textboxes_pause;
			textbox_num=sizeof(textboxes_pause)/sizeof(textboxes_pause[0]);
			selected_textbox=textboxes;

			break;

		default:
			break;
	}

	gamestate=state;
}

/*
int n=0;
int s=0;
size_t bog_je_mrtu=0;
i2s_chan_handle_t zakaj;
uint16_t *ubime=snd_door;
*/


void core0_func(void *args){
	uint64_t frame_start_time;
	uint64_t frame_end_time;

	switch_gamestate(GAMESTATE_TITLE_SCREEN);

    while(1){
		timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &frame_start_time);		//fps timer
		get_keyboard_inputs();
		apply_player_inputs();

		switch(gamestate){
			case GAMESTATE_RUNNING:
				check_cheats();

				//trigger check
				for(int i=0;i<trigger_num;i++) trigger_check(triggers+i);

				//actor thinkers
				for(int i=0;i<max_actor_count;i++) if(actors[i]) do_a_think(actors[i]);

				//rendering
				core1_control_byte=CORE1_DRAW_DYNAMIC_SURFS;
				draw_dynamic_surfs_until_done();
				while(core1_control_byte!=CORE1_ACK);

				core1_control_byte=CORE1_DRAW_STATIC_SURFS;
				draw_static_surfs_until_done();
				while(core1_control_byte!=CORE1_ACK);

				core1_control_byte=CORE1_DRAW_ACTORS;
				draw_actors_until_done();
				while(core1_control_byte!=CORE1_ACK);

				core1_control_byte=CORE1_DRAW_PARTICLES;
				draw_particles_until_done();
				while(core1_control_byte!=CORE1_ACK);

				for(int i=0;i<trigger_num;i++) if(triggers[i].tx_on.p) draw_trigger(triggers+i);

				draw_held_gun();

				if(allow_drawhud){
					draw_crosshair();
					motd_logic();
					update_hud(screen, 0);
					screen_tint_logic();
				}

				if(allow_draw_fps) drawf(1,10, "fps %.2f", fps_old);

				clear_z_buffer();

				break;

			case GAMESTATE_SCREEN_MELT:
				for(int i=0;i<6;i++){
					bool done=true;

					for(int x=0;x<w;x++){
						if(screen_melt_offsets[x]>=0 && screen_melt_offsets[x]<h){
							for(int y=h-1;y>screen_melt_offsets[x];y--) screen_out[y][x]=screen_out[y-1][x];
							screen_out[screen_melt_offsets[x]][x]=screen[screen_melt_offsets[x]][x];
							done=false;
						}
						screen_melt_offsets[x]++;
					}


					if(done) switch_gamestate(GAMESTATE_RUNNING);
				}

				break;

			case GAMESTATE_TITLE_SCREEN:
				memcpy(screen, tx_title_screen, h*w*2);

				for(int i=0;i<textbox_num;i++) draw_textbox(textboxes+i);
				selected_textbox_logic();

				break;

			case GAMESTATE_PAUSE:
				memcpy(screen_out, screen, h*w*2);

				void *temp=screen;
				screen=screen_out;
				screen_out=temp;

				for(int i=0;i<textbox_num;i++) draw_textbox(textboxes+i);
				selected_textbox_logic();

				temp=screen;
				screen=screen_out;
				screen_out=temp;

				break;

		}

    	//cajt ka smo dejansko rabl za frame izracunat
		timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &frame_end_time);
		float actual_fps=40000000./(frame_end_time-frame_start_time);
		fps_old=actual_fps;

		//spin do target fps-a
		while(frame_end_time < frame_start_time + 40000000/TARGET_FPS) timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &frame_end_time);


		//spin do konca trenutnega frejma (da ne zbrisemo draw bufferja na sredini risanja)
		while(!vsync_trigger);
		vsync_trigger=false;

		if(gamestate!=GAMESTATE_SCREEN_MELT && gamestate!=GAMESTATE_PAUSE){
			void *temp=screen;
			screen=screen_out;
			screen_out=temp;
    	}

		gametick++;
	}
}



void core1_func(void* args){
	while(1){
		switch(core1_control_byte){
			case CORE1_ACK:
				break;
			case CORE1_DRAW_STATIC_SURFS:
				draw_static_surfs_until_done();
				core1_control_byte=CORE1_ACK;
				break;
			case CORE1_DRAW_DYNAMIC_SURFS:
				draw_dynamic_surfs_until_done();
				core1_control_byte=CORE1_ACK;
				break;
			case CORE1_DRAW_ACTORS:
				draw_actors_until_done();
				core1_control_byte=CORE1_ACK;
				break;
			case CORE1_DRAW_PARTICLES:
				draw_particles_until_done();
				core1_control_byte=CORE1_ACK;
				break;
			case CORE1_DRAW_GUN:
				draw_held_gun();
				core1_control_byte=CORE1_ACK;
				break;
			case CORE1_CLEAR_Z_BUFFER:
				clear_z_buffer();
				core1_control_byte=CORE1_ACK;
				break;
			case CORE1_COPY_SCREEN:
				//memcpy(screen_out_buf, screen, w*h*2);
				core1_control_byte=CORE1_ACK;
				break;
		}
	}
}


bool vsync_hit(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx){
	vsync_trigger=true;
	return false;
}

bool refill_bb(esp_lcd_panel_handle_t panel, void *bounce_buf, int pos_px, int len_bytes, void *user_ctx){
	//int line_m=(pos_px/len_bytes)&7;
	//if(line_m==3 || line_m==5) return false;	//ne kopiri po nepotrebnm

	uint16_t *bb=(uint16_t*)bounce_buf;
	uint16_t *s=screen_out[(h*pos_px)/(400*640)];
	uint16_t *s_end=s+w;

	uint16_t color;
	while(s!=s_end){
		color=*s++;
		*bb++=color;
		*bb++=color;
		*bb++=color;

		color=*s++;
		*bb++=color;
		*bb++=color;
		*bb++=color;

		color=*s++;
		*bb++=color;
		*bb++=color;
	}

	return false;
}

bool sound_buf_empty(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx){
	//static int n=0;

	/*
	uint16_t *buf=(uint16_t*)event->data;
	for(int i=0;i<event->size;i++){
		//buf[i]=6900;
		buf[i]=snd_door[n++];
		//if(n>=sizeof(snd_exp)) n=0;
	}
	*/

	//s=buf;

	//i2s_channel_write(handle,&snd_door,event->size,&bog_je_mrtu,1000);
	//n+=event->size;

	return false;
}

bool najebu_si(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx){

	return false;
}


void app_main(void){
	//u glaunm sam inicializacija razlicnih periferalou

	//GPIO-ji
	for(int i=0;i<19;i++) gpio_set_direction(i,GPIO_MODE_OUTPUT);

	for(int i=19;i<22;i++){
		gpio_set_direction(i,GPIO_MODE_INPUT);
		gpio_pullup_en(i);
	}

	gpio_pullup_dis(21);
	gpio_pulldown_dis(21);

	for(int i=35;i<49;i++){
		gpio_set_direction(i,GPIO_MODE_INPUT);
		gpio_pulldown_en(i);
	}

	gpio_set_direction(40,GPIO_MODE_OUTPUT);
	gpio_set_direction(41,GPIO_MODE_OUTPUT);
	gpio_set_direction(42,GPIO_MODE_OUTPUT);




	//LCD
    esp_lcd_panel_handle_t panel = NULL;
    esp_lcd_rgb_panel_config_t panel_config = {
    	.clk_src=LCD_CLK_SRC_DEFAULT,
        .data_width=16,
        .disp_gpio_num=-1,
        .pclk_gpio_num=0,
        .vsync_gpio_num=18,
        .hsync_gpio_num=17,
        .de_gpio_num=-1,
        .data_gpio_nums={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
		.bounce_buffer_size_px=640,

	    .timings={
	    	//gej shit
	        //.pclk_hz = 2*6293750,
	        //.h_res = w,
	        //.v_res = 2*h,
	        //.hsync_back_porch = 24,
	        //.hsync_front_porch = 8,
	        //.hsync_pulse_width = 48,
	        //.vsync_back_porch = 35,
	        //.vsync_front_porch = 12,
	        //.vsync_pulse_width = 2,


	    	/*
	    	//mau zmansamo da ne rabmo 2x vecji frame buffer
			//.pclk_hz=4*6293750,
			.pclk_hz=4*6293750,
			.h_res=w,
			.v_res=h,
			.hsync_back_porch=2*24+(640-w)/2,
			.hsync_front_porch=2*8+(640-w)/2,
			.hsync_pulse_width=2*48,
			.vsync_back_porch=35+(400-h)/2,
			.vsync_front_porch=12+(400-h)/2,
			.vsync_pulse_width=2,
			*/


			//.pclk_hz=800*449*41,		//ne vem zakaj, ampak moj monitor noce laufat mn ka 41 fps
			.pclk_hz=800*449*63,		//stable >60
	    	.h_res=640,
			.v_res=400,
			.hsync_back_porch=48,
			.hsync_front_porch=16,
			.hsync_pulse_width=96,
			.vsync_back_porch=35,
			.vsync_front_porch=12,
			.vsync_pulse_width=2,


			.flags.hsync_idle_low=0,
			.flags.vsync_idle_low=0,
	    },
		.flags.no_fb=1,

    };

    esp_lcd_rgb_panel_event_callbacks_t lcd_callbacks=(esp_lcd_rgb_panel_event_callbacks_t){&vsync_hit,&refill_bb};

    esp_lcd_new_rgb_panel(&panel_config, &panel);
    esp_lcd_panel_reset(panel);
    esp_lcd_panel_init(panel);
    esp_lcd_rgb_panel_register_event_callbacks(panel,&lcd_callbacks,NULL);


    /*
    //(GPIO continued) vsync interrupt (vsync pin je fizicno povezan s pinom 21 kr se mi res ne da ukvarjat s callbacki)
    gpio_set_intr_type(19, GPIO_INTR_NEGEDGE);
    gpio_intr_enable(19);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(19, on_vsync, NULL);
    */



	//UART (za tipkovnco)
	uart_config_t uart_config_keyboard={
		.baud_rate=12500,
		.data_bits=UART_DATA_8_BITS,
		.parity=UART_PARITY_ODD,
		.stop_bits=UART_STOP_BITS_1,
		.flow_ctrl=UART_HW_FLOWCTRL_DISABLE,
		.source_clk=GPTIMER_CLK_SRC_XTAL,
	};

	uart_param_config(2, &uart_config_keyboard);
	uart_set_pin(2, -1, 21, -1, -1);
	uart_driver_install(2, 1024, 0, 0, NULL, 0);


	#if SCREENSHOTS
		//UART (za screenshote)
		uart_config_t uart_config_screenshot={
			.baud_rate=115200*20,
			.data_bits=UART_DATA_8_BITS,
			.parity=UART_PARITY_DISABLE,
			.stop_bits=UART_STOP_BITS_1,
			.flow_ctrl=UART_HW_FLOWCTRL_DISABLE,
			.source_clk=GPTIMER_CLK_SRC_XTAL,
		};

		uart_param_config(1, &uart_config_screenshot);
		uart_set_pin(1, 43, 44, -1, -1);
		uart_driver_install(1, 1024, 0, 0, NULL, 0);
	#endif

	/*
	//i2s (zvok)
	i2s_chan_handle_t handle;
	i2s_chan_config_t chan_cfg=I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
	i2s_new_channel(&chan_cfg, &handle, NULL);

	i2s_std_config_t cfg={
		.clk_cfg=I2S_STD_CLK_DEFAULT_CONFIG(11025),
		.slot_cfg=I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
		.gpio_cfg={
			.mclk=-1,
			.bclk=40,
			.ws=41,
			.dout=42,
			.din=-1,
			.invert_flags={
				.mclk_inv=false,
				.bclk_inv=false,
				.ws_inv=false,
			},
		},
	};


	const i2s_event_callbacks_t i2s_callbacks={
		.on_recv = NULL,
		.on_send_q_ovf = NULL,
		.on_sent=sound_buf_empty,
		.on_send_q_ovf=najebu_si,
	};


	i2s_channel_init_std_mode(handle, &cfg);
	//i2s_channel_register_event_callback(handle,&i2s_callbacks,NULL);
	i2s_channel_enable(handle);

	zakaj=handle;

	printf("begin\n");


	const uint16_t fuck[]={
		0b1100110000000000,0b1100110000000000,0b1100110000000000,0b1100110000000000,0b1100110000000000,0b1100110000000000,0b1100110000000000,0b1100110000000000,0b1100110000000000,0b1100110000000000,
	};


	size_t uwu=0;
	//while(1) i2s_channel_write(handle,&snd_door,sizeof(snd_door),&uwu,1000);

	//printf("done %d\n",uwu);


	int fuck=0;

	while(1){
		i2s_channel_write(zakaj,snd_door+fuck,300,&bog_je_mrtu,1000);
		fuck+=150;
		if(fuck>sizeof(snd_door)/2-150) fuck=0;
	}
	*/


	//TIMER (za omejevanje fps-a (kr je tickrate odvisn od njega))
	timer_config_t timer_config={
		.alarm_en=TIMER_ALARM_DIS,
		.counter_en=true,
		.intr_type=TIMER_INTR_LEVEL,
		.counter_dir=TIMER_COUNT_UP,
		.auto_reload=TIMER_AUTORELOAD_DIS,
		.divider=2,
		.clk_src=TIMER_SRC_CLK_APB
	};
	timer_init(TIMER_GROUP_0, TIMER_0, &timer_config);


	//spraumo core1 na delo
	TaskHandle_t task_core1_func;
	xTaskCreatePinnedToCore(core1_func,"core1_func",16000,NULL,5,&task_core1_func,1);

	//efektivno jump u main game loop
	TaskHandle_t task_core0_func;
	xTaskCreatePinnedToCore(core0_func,"core0_func",32000,NULL,5,&task_core0_func,0);
}
