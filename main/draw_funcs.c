#define hud_h sizeof(tx_background)/sizeof(tx_background[0])

void clear_z_buffer(){
	for(int y=0;y<h;y++) for(int x=0;x<w;x++) z_buffer[y][x]=1./0;
}


void screen_flush(uint16_t color){
	for(int y=0;y<h;y++) for(int x=0;x<w;x++) screen[y][x]=color;
}

void draw_dot(point_t p, int a, uint16_t color){
	int x1=(int)p.x;
	int y1=(int)p.y;
  if(x1+a>w || x1<0 || y1+a>h || y1<0) return;
  for(int y=y1; y<y1+a; y++) for(int x=x1; x<x1+a; x++) screen[y][x]=color;
}

void draw_line(point_t p1, point_t p2, uint16_t color){
	int x1=(int)p1.x;
	int y1=(int)p1.y;
	int x2=(int)p2.x;
	int y2=(int)p2.y;

  if(x1<0 || x1>w || x2<0 || x2>w || y1<0 || y1>h || y2<0 || y2>h) return;

  if(x1==x2){
    if(y1>y2){
      int temp=y1;
      y1=y2;
      y2=temp;
    }
    for(int y=y1;y<y2;y++) screen[y][x1]=color;
    return;
  }

  if(y1==y2){
    if(x1>x2){
      int temp=x1;
      x1=x2;
      x2=temp;
    }
    for(int x=x1;x<x2;x++) screen[y1][x]=color;
    return;
  }

  double k=((double)(y2-y1))/(x2-x1);
  double n=y1-k*x1;

  if(y1>y2){
    int temp=y1;
    y1=y2;
    y2=temp;
  }

  if(x1>x2){
    int temp=x1;
    x1=x2;
    x2=temp;
  }

  if(k<1 && k>-1) for(int x=x1;x<x2;x++) screen[(int)(k*x+n)][x]=color;
  else for(int y=y1;y<y2;y++) screen[y][(int)((y-n)/k)]=color;

}

//generacija s pythonom: list((round((1+cos(pi*i/240))/2,8) for i in range(240)))
const float skybox_distortion_values[w]={1.0, 0.99995716, 0.99982866, 0.99961452, 0.99931477, 0.99892946, 0.99845867, 0.99790246, 0.99726095, 0.99653423, 0.99572243, 0.99482569, 0.99384417, 0.99277803, 0.99162745, 0.99039264, 0.9890738, 0.98767116, 0.98618496, 0.98461545, 0.98296291, 0.98122762, 0.97940987, 0.97750997, 0.97552826, 0.97346506, 0.97132075, 0.96909567, 0.96679021, 0.96440478, 0.96193977, 0.95939561, 0.95677273, 0.95407159, 0.95129264, 0.94843637, 0.94550326, 0.94249382, 0.93940856, 0.936248, 0.9330127, 0.92970321, 0.92632008, 0.92286391, 0.91933528, 0.91573481, 0.91206309, 0.90832078, 0.9045085, 0.90062691, 0.89667667, 0.89265847, 0.88857298, 0.88442092, 0.88020298, 0.8759199, 0.87157241, 0.86716125, 0.86268719, 0.85815097, 0.85355339, 0.84889523, 0.84417729, 0.83940037, 0.8345653, 0.82967291, 0.82472402, 0.8197195, 0.8146602, 0.80954697, 0.80438071, 0.7991623, 0.79389263, 0.7885726, 0.78320312, 0.77778512, 0.77231952, 0.76680726, 0.76124928, 0.75564654, 0.75, 0.74431062, 0.73857938, 0.73280726, 0.72699525, 0.72114435, 0.71525555, 0.70932987, 0.70336832, 0.69737193, 0.69134172, 0.68527872, 0.67918397, 0.67305853, 0.66690343, 0.66071973, 0.6545085, 0.64827079, 0.64200767, 0.63572022, 0.62940952, 0.62307665, 0.61672268, 0.61034872, 0.60395585, 0.59754516, 0.59111776, 0.58467475, 0.57821723, 0.57174631, 0.5652631, 0.5587687, 0.55226423, 0.54575081, 0.53922955, 0.53270156, 0.52616798, 0.51962991, 0.51308847, 0.5065448, 0.5, 0.4934552, 0.48691153, 0.48037009, 0.47383202, 0.46729844, 0.46077045, 0.45424919, 0.44773577, 0.4412313, 0.4347369, 0.42825369, 0.42178277, 0.41532525, 0.40888224, 0.40245484, 0.39604415, 0.38965128, 0.38327732, 0.37692335, 0.37059048, 0.36427978, 0.35799233, 0.35172921, 0.3454915, 0.33928027, 0.33309657, 0.32694147, 0.32081603, 0.31472128, 0.30865828, 0.30262807, 0.29663168, 0.29067013, 0.28474445, 0.27885565, 0.27300475, 0.26719274, 0.26142062, 0.25568938, 0.25, 0.24435346, 0.23875072, 0.23319274, 0.22768048, 0.22221488, 0.21679688, 0.2114274, 0.20610737, 0.2008377, 0.19561929, 0.19045303, 0.1853398, 0.1802805, 0.17527598, 0.17032709, 0.1654347, 0.16059963, 0.15582271, 0.15110477, 0.14644661, 0.14184903, 0.13731281, 0.13283875, 0.12842759, 0.1240801, 0.11979702, 0.11557908, 0.11142702, 0.10734153, 0.10332333, 0.09937309, 0.0954915, 0.09167922, 0.08793691, 0.08426519, 0.08066472, 0.07713609, 0.07367992, 0.07029679, 0.0669873, 0.063752, 0.06059144, 0.05750618, 0.05449674, 0.05156363, 0.04870736, 0.04592841, 0.04322727, 0.04060439, 0.03806023, 0.03559522, 0.03320979, 0.03090433, 0.02867925, 0.02653494, 0.02447174, 0.02249003, 0.02059013, 0.01877238, 0.01703709, 0.01538455, 0.01381504, 0.01232884, 0.0109262, 0.00960736, 0.00837255, 0.00722197, 0.00615583, 0.00517431, 0.00427757, 0.00346577, 0.00273905, 0.00209754, 0.00154133, 0.00107054, 0.00068523, 0.00038548, 0.00017134, 0.00004284};

void draw_triangle(vert_t *p1, vert_t *p2, vert_t *p3, tx_t *texture, bool is_skybox){
	if(p1->x==p2->x && p2->x==p3->x) return;

	float a,b,c,d;
	point_t cross_v=fast_cross(vert_sub(p1,p2),vert_sub(p1,p3));
	a=cross_v.x;
	b=cross_v.y;
	c=cross_v.z;

	d=-a*p1->x-b*p1->y-c*p1->z;
	float inv_neg_c=-1/c;


	//sort
	if(p1->y > p2->y){
		void *temp=p1;
		p1=p2;
		p2=temp;
	}
	if(p2->y > p3->y){
		void *temp=p2;
		p2=p3;
		p3=temp;
	}
	if(p1->y > p2->y){
		void *temp=p1;
		p1=p2;
		p2=temp;
	}

	int x1=(int)p1->x;
	int y1=(int)p1->y;
	float z1=1/p1->z;
	float u1=p1->tx*z1;
	float v1=p1->ty*z1;
	int x2=(int)p2->x;
	int y2=(int)p2->y;
	float z2=1/p2->z;
	float u2=p2->tx*z2;
	float v2=p2->ty*z2;
	int x3=(int)p3->x;
	int y3=(int)p3->y;
	float z3=1/p3->z;
	float u3=p3->tx*z3;
	float v3=p3->ty*z3;

	uint16_t *tx=texture->p;
	int tx_w=texture->width;
	int tx_h=texture->height;

	float k1=(float)(y2-y1)/(x2-x1);
	float k2=(float)(y3-y2)/(x3-x2);
	float k3=(float)(y1-y3)/(x1-x3);

	float n1=y1-k1*x1;
	float n2=y2-k2*x2;
	float n3=y3-k3*x3;

	int t1,t2;

	if(!is_skybox){
		float ui1,ui2,vi1,vi2,zi1,zi2;
		float p;

		p=(float)(y2-y1)/(y3-y1);

		float u_intr=u3*p+u1*(1-p);
		float v_intr=v3*p+v1*(1-p);
		float z_intr=z3*p+z1*(1-p);

		float x_mid=x3*p+x1*(1-p);


		//zgorn trikotnik
		for(int y=y1;y<y2;y++){
			t1=(x1==x2)?x1:(y-n1)/k1;
			t2=(x1==x3)?x1:(y-n3)/k3;

			if(t1>t2){
			  int temp=t1;
			  t1=t2;
			  t2=temp;
			}

			p=(float)(y-y1)/(y2-y1);

			ui1=u2*p+u1*(1-p);
			vi1=v2*p+v1*(1-p);
			zi1=z2*p+z1*(1-p);
			ui2=u_intr*p+u1*(1-p);
			vi2=v_intr*p+v1*(1-p);
			zi2=z_intr*p+z1*(1-p);


			p+=.01;	//grdo ampak veš da me prou boli kurac
			if(x2*p+x1*(1-p) < x_mid*p+x1*(1-p)){
				float temp=ui1;
				ui1=ui2;
				ui2=temp;

				temp=vi1;
				vi1=vi2;
				vi2=temp;

				temp=zi1;
				zi1=zi2;
				zi2=temp;
			}


			//da, ful konstant je ampak tole je tko 1.5x hitrej ka brez
			float dist_const1=a*inv_neg_c;
			float dist_const2=(b*y+d)*inv_neg_c;
			float p_const=1./(t2-t1);
			float u_const=ui1-ui2;
			float v_const=vi1-vi2;
			float z_const=zi1-zi2;
			float dist=dist_const1*t1+dist_const2;
			p=0;

			for(int x=t1;x<t2;x++){
				dist+=dist_const1;
				p+=p_const;
				if(dist<z_buffer[y][x]){
					float zi=1/(z_const*p+zi2);

					uint16_t color=tx[(((int)((v_const*p+vi2)*zi))%tx_h)*tx_w+((int)((u_const*p+ui2)*zi))%tx_w];
					if(color!=0b1111111111100000){
						z_buffer[y][x]=dist;
						screen[y][x]=color;
					}
				}
			}
		}

		//spodn trikotnik
		for(int y=y2;y<y3;y++){
			t1=(x3==x2)?x3:(y-n2)/k2;
			t2=(x3==x1)?x3:(y-n3)/k3;

			if(t1>t2){
			  int temp=t1;
			  t1=t2;
			  t2=temp;
			}

			p=(float)(y-y2)/(y3-y2);

			ui1=u3*p+u2*(1-p);
			vi1=v3*p+v2*(1-p);
			zi1=z3*p+z2*(1-p);
			ui2=u3*p+u_intr*(1-p);
			vi2=v3*p+v_intr*(1-p);
			zi2=z3*p+z_intr*(1-p);


			p+=.01;	//grdo ampak veš da me prou boli kurac
			if(x2*p+x1*(1-p) < x_mid*p+x1*(1-p)){
				float temp=ui1;
				ui1=ui2;
				ui2=temp;

				temp=vi1;
				vi1=vi2;
				vi2=temp;

				temp=zi1;
				zi1=zi2;
				zi2=temp;
			}


			//da, ful konstant je ampak tole je tko 1.5x hitrej ka brez
			float dist_const1=a*inv_neg_c;
			float dist_const2=(b*y+d)*inv_neg_c;
			float p_const=1./(t2-t1);
			float u_const=ui1-ui2;
			float v_const=vi1-vi2;
			float z_const=zi1-zi2;
			float dist=dist_const1*t1+dist_const2;
			p=0;

			for(int x=t1;x<t2;x++){
				dist+=dist_const1;
				p+=p_const;
				if(dist<z_buffer[y][x]){
					float zi=1/(z_const*p+zi2);

					uint16_t color=tx[(((int)((v_const*p+vi2)*zi))%tx_h)*tx_w+((int)((u_const*p+ui2)*zi))%tx_w];
					if(color!=0b1111111111100000){
						z_buffer[y][x]=dist;
						screen[y][x]=color;
					}
				}
			}
		}
	}
	else{
		float u1_ang=player->roth-pi/3+2*pi;
		float u1=(u1_ang)*SKYBOX_REPEAT_NUM*tx_w/(2*pi);
		float du=SKYBOX_REPEAT_NUM*tx_w/3;

		float p;

		float c=1./w;
		float dist_const1=a*inv_neg_c;

		//zgorn trikotnik
		for(int y=y1;y<y2;y++){
			t1=(x1==x2)?x1:(y-n1)/k1;
			t2=(x1==x3)?x1:(y-n3)/k3;

			if(t1>t2){
			  int temp=t1;
			  t1=t2;
			  t2=temp;
			}

			float dist_const2=(b*y+d)*inv_neg_c;

			for(int x=t1;x<t2;x++){
				float dist=dist_const1*x+dist_const2;
				if(dist<z_buffer[y][x]){
					/* legacy approximation; nimam srca da bi kr vrgu stran :(
					p=x*c;
					p-=.5;
					float p2=p*p;
					p=.5+p*(1.5708+p2*(-2.58386+1.27508*p2));	//bog pomagi temu y=(1-cos(pi*x))/2
					*/
					z_buffer[y][x]=dist;
					screen[y][x]=tx[y*tx_w+((int)(u1+du*skybox_distortion_values[x]))%tx_w];
				}
			}
		}

		//spodn trikotnik
		for(int y=y2;y<y3;y++){
			t1=(x3==x2)?x3:(y-n2)/k2;
			t2=(x3==x1)?x3:(y-n3)/k3;

			if(t1>t2){
			  int temp=t1;
			  t1=t2;
			  t2=temp;
			}


			float dist_const2=(b*y+d)*inv_neg_c;

			for(int x=t1;x<t2;x++){
				float dist=dist_const1*x+dist_const2;
				if(dist<z_buffer[y][x]){
					z_buffer[y][x]=dist;
					screen[y][x]=tx[y*tx_w+((int)(u1+du*skybox_distortion_values[x]))%tx_w];
				}
			}
		}
	}
}


void draw_polygon(polygon_t *poly){
	for(int i=1;i<poly->size-1;i++) draw_triangle(&poly->points[0], &poly->points[i], &poly->points[i+1], &poly->tx, poly->flags & SKYBOX);
}


void clip_polygon(polygon_t *poly0){
	polygon_t masohizem;
	polygon_t *poly1=&masohizem;

	vert_t p_old;
	vert_t p;
	vert_t presecisce;
	float t;
	float ch=1/HFOV_CONST;
	float cv=1/VFOV_CONST;
	float dx,dy,dz;

	for(int n=0;n<2;n++){
		//x
		poly1->size=0;
		p_old=poly0->points[poly0->size-1];
		for(int i=0;i<poly0->size;i++){
			p=poly0->points[i];

			if((p.z>=-ch*p.x)!=(p_old.z>=-ch*p_old.x)){
				dx=(p_old.x-p.x);
				dy=(p_old.y-p.y);
				dz=(p_old.z-p.z);

				t=-(ch*p.x+p.z)/(ch*dx+dz);
				presecisce=(vert_t){dx*t+p.x, dy*t+p.y, dz*t+p.z, p.tx*(1-t)+p_old.tx*t, p.ty*(1-t)+p_old.ty*t};
				poly1->points[poly1->size++]=presecisce;
			}

			if(p.z>=-ch*p.x) poly1->points[poly1->size++]=p;

			p_old=p;
		}

		//y
		poly0->size=0;
		p_old=poly1->points[poly1->size-1];
		for(int i=0;i<poly1->size;i++){
			p=poly1->points[i];

			if((p.z>=-cv*p.y)!=(p_old.z>=-cv*p_old.y)){
				dx=(p_old.x-p.x);
				dy=(p_old.y-p.y);
				dz=(p_old.z-p.z);

				t=-(cv*p.y+p.z)/(cv*dy+dz);
				presecisce=(vert_t){dx*t+p.x, dy*t+p.y, dz*t+p.z, p.tx*(1-t)+p_old.tx*t, p.ty*(1-t)+p_old.ty*t};
				poly0->points[poly0->size++]=presecisce;
			}

			if(p.z>=-cv*p.y) poly0->points[poly0->size++]=p;

			p_old=p;
		}

		ch*=-1;
		cv*=-1;

		if(allow_drawhud) cv=-1/VFOV_CONST_NOHUD;
	}
}


/*
float get_dist_from_origin_unsqrted(point_t *p){
	return p->x*p->x + p->y*p->y + p->z*p->z;
}
*/

point_t get_poly_center(polygon_t *poly){
	point_t p={0,0,0};

	for(int i=0;i<poly->size;i++){
		p.x+=poly->points[i].x;
		p.y+=poly->points[i].y;
		p.z+=poly->points[i].z;
	}
	p.x/=poly->size;
	p.y/=poly->size;
	p.z/=poly->size;

	return p;
}

void draw_surf(polygon_t *pp){
	if(!(pp->flags & IS_WALL)) if(player->pos.z+player->height==pp->points[0].z) return;

	#if TRANSPARENT_SURF_BACKS
	if(!(pp->flags & TEMPORARY)){
		if(pp->flags & IS_WALL){
			if(pp->points[0].x==pp->points[1].x){
				if(fast_sign(pp->normal.x)!=fast_sign(player->pos.x-pp->points[0].x)) return;
			}
			else{
				float k=(pp->points[1].y-pp->points[0].y)/(pp->points[1].x-pp->points[0].x);
				if(((pp->points[0].x+pp->normal.x)*k < pp->normal.y)!=(k*(player->pos.x-pp->points[0].x) < player->pos.y-pp->points[0].y)) return;
			}
		}
		else{
			//if((pp->normal.z>pp->points[0].z)!=((player->pos.z+player->height)>pp->points[0].z)) return;
		}
	}
	#endif


	polygon_t poly=*pp;

	float sinh=fast_sin(player->roth-pi/2);
	float cosh=fast_cos(player->roth-pi/2);
	float sinv=fast_sin(player_rotv);
	float cosv=fast_cos(player_rotv);

	//camera transform
	for(int i=0;i<poly.size;i++){
		//move
		poly.points[i].x-=player->pos.x;
		poly.points[i].y-=player->pos.y;
		poly.points[i].z-=player->pos.z+player->height;

		float temp;
		temp=poly.points[i].x;

		//rotz
		poly.points[i].x=poly.points[i].x*cosh+poly.points[i].y*sinh;
		poly.points[i].y=-temp*sinh+poly.points[i].y*cosh;

		temp=poly.points[i].y;

		//rotx
		poly.points[i].y=poly.points[i].y*cosv-poly.points[i].z*sinv;
		poly.points[i].z=temp*sinv+poly.points[i].z*cosv;
	}


	clip_polygon(&poly);


	//move to image space
	for(int i=0; i<poly.size;i++){
		if(poly.points[i].z){
			poly.points[i].x/=poly.points[i].z*HFOV_CONST;
			poly.points[i].y/=poly.points[i].z*VFOV_CONST;
		}
		//poly.points[j].z=((poly.points[j].z)*2-100)/(100);
	}


	//move to screen space
	for(int i=0; i<poly.size;i++){
			poly.points[i].x*=w/2;
			poly.points[i].y*=h/2;

			poly.points[i].x+=w/2;
			poly.points[i].y+=h/2;
	}

	draw_polygon(&poly);
}

void draw_static_surfs_until_done(){
	for(int i=xPortGetCoreID();i<static_surf_num;i+=2) draw_surf(static_surfs+i);
}

void draw_dynamic_surfs_until_done(){
	for(int i=xPortGetCoreID();i<dynamic_surf_num;i+=2) draw_surf(dynamic_surfs+i);
}

void draw_actor(actor_t *actor){
	polygon_t poly={.size=4,.flags=IS_WALL|TEMPORARY};

	if(!actor->type) return;

	float rot=atan2(actor->pos.y-player->pos.y,actor->pos.x-player->pos.x);
	if(rot<0) rot+=2*pi;

	float relative_rot=actor->roth-rot;
	if(relative_rot<0) relative_rot+=2*pi;

	int rot_sector=(int)((relative_rot+pi/8)*(4/pi))&7;

	int new_timer_value=0;

	int anim_num=0;
	for(int i=0;actor_anims_base[actor->type][actor->state][i].num!=255;i++){
		anim_num=actor_anims_base[actor->type][actor->state][i].num;
		if(actor->anim_timer < actor_anims_base[actor->type][actor->state][i].counter){
			new_timer_value=actor->anim_timer;
			break;
		}
	}

	if(actor->state!=ACTOR_STATE_DEAD && actor->state!=ACTOR_STATE_GIBED) actor->anim_timer=new_timer_value;
	if(actor->anim_timer>1000) actor->anim_timer=1000;

	anim_num=(anim_num>=actor_sprite_rot_limit[actor->type])?anim_num+7*actor_sprite_rot_limit[actor->type]:anim_num*8+rot_sector;

	tx_t tx=actor_sprite_base[actor->type][anim_num];


	//1 unit=32 px

	poly.tx=tx;

	poly.points[0]=(vert_t){-tx.width/32., 0, 0, 					0,tx.height-1};
	poly.points[1]=(vert_t){tx.width/32., 0, 0, 					tx.width,tx.height-1};
	poly.points[2]=(vert_t){tx.width/32., 0, tx.height/16., 		tx.width-1,0};
	poly.points[3]=(vert_t){-tx.width/32., 0, tx.height/16.,		0,0};


	for(int i=0;i<4;i++){
		rotate_point_2d(&poly.points[i],pi/2-rot);

		add_to_point(&poly.points[i],&actor->pos);

		if(tx.flags & 1){
			poly.points[i].tx=tx.width-poly.points[i].tx;
		}
	}

	draw_surf(&poly);
}

void draw_actors_until_done(){
	for(int i=xPortGetCoreID();i<max_actor_count;i+=2) if(actors[i]) draw_actor(actors[i]);
}


void draw_particle(particle_t *particle){
	polygon_t poly={.size=4,.flags=IS_WALL|TEMPORARY};

	float rot=atan2(particle->pos.x-player->pos.x,particle->pos.y-player->pos.y);
	if(rot<0) rot+=2*pi;

	int anim_num=-1;
	for(int i=0;particle_anims_base[particle->type][i].num!=255;i++) if(particle->anim_timer < particle_anims_base[particle->type][i].counter){
		anim_num=particle_anims_base[particle->type][i].num;
		break;
	}

	if(anim_num==-1){
		for(int i=0;i<max_particle_count;i++) if(particles[i]==particle){
			particles[i]=NULL;
			free(particle);
			return;
		}
	}

	tx_t tx=particle_sprite_base[particle->type][anim_num];

	particle->anim_timer++;

	poly.tx=tx;

	poly.points[0]=(vert_t){-tx.width/32., 0, 0, 					0,tx.height};
	poly.points[1]=(vert_t){tx.width/32., 0, 0, 					tx.width,tx.height};
	poly.points[2]=(vert_t){tx.width/32., 0, tx.height/16., 		tx.width,0};
	poly.points[3]=(vert_t){-tx.width/32., 0, tx.height/16.,		0,0};


	for(int i=0;i<4;i++){
		rotate_point_2d(&poly.points[i],rot);

		add_to_point(&poly.points[i],&particle->pos);
	}

	draw_surf(&poly);
}

void draw_particles_until_done(){
	for(int i=xPortGetCoreID();i<max_particle_count;i+=2) if(particles[i]) draw_particle(particles[i]);
}


void draw_trigger(trigger_t *trigger){
	polygon_t poly={.size=4,.flags=IS_WALL|TEMPORARY};

	int id=((int)trigger-(int)triggers)/sizeof(trigger_t);

	tx_t tx=(trigger_rtf[id] & TRIGGER_FLAG_SWITCH_ON)?trigger->tx_on:trigger->tx_off;

	poly.tx=tx;

	poly.points[0]=(vert_t){trigger->p1.x, trigger->p1.y, trigger->p1.z, 0, tx.height};
	poly.points[1]=(vert_t){trigger->p2.x, trigger->p2.y, trigger->p1.z, tx.width, tx.height};
	poly.points[2]=(vert_t){trigger->p2.x, trigger->p2.y, trigger->p2.z, tx.width, 0};
	poly.points[3]=(vert_t){trigger->p1.x, trigger->p1.y, trigger->p2.z, 0, 0};

	draw_surf(&poly);
}



void draw_held_gun(){
	static int plasma_gun_anim_num=1;
	int new_timer_value=0;

	int anim_num=0;
	for(int i=0;gun_anims_base[player_held_gun][player_gun_state][i].num!=255;i++){
		anim_num=gun_anims_base[player_held_gun][player_gun_state][i].num;
		if(player_gun_anim_timer < gun_anims_base[player_held_gun][player_gun_state][i].counter){
			new_timer_value=player_gun_anim_timer;
			break;
		}
	}

	player_gun_anim_timer=new_timer_value;

	if(!allow_drawhud) return;

	if(player_held_gun==GUN_PLASMA && player_gun_state==GUN_STATE_FIRING && inputs[key_ctrl] && player_gun_anim_timer<4){
		if(!(gametick&8)) plasma_gun_anim_num=(doom_rand()&1)+1;;
		anim_num=plasma_gun_anim_num;
		player_gun_anim_timer=2;
	}

	tx_t tx=gun_sprite_base[player_held_gun][anim_num];


	int nyoom=(int)(get_mag_2d(&player->vel)*30);
	static int sway_x=0;
	static int sway_y=0;

	if(nyoom && player_gun_state==GUN_STATE_IDLE){
		sway_x=(int)(nyoom*fast_sin((gametick&63)*(2*pi/64)));
		sway_y=(int)(nyoom*fast_fabs(fast_cos((gametick&63)*(2*pi/64))));
	}

	//drawing
	if(tx.height-player_gun_y_offset-sway_y>hud_h) for(int y=0;y<tx.height-player_gun_y_offset-sway_y-hud_h; y++) for(int x=0;x<tx.width;x++) if(tx.p[y*tx.width+x]!=0b1111111111100000) screen[h-tx.height+y+player_gun_y_offset+sway_y][w/2-tx.width/2+x+sway_x]=tx.p[y*tx.width+x];
}





const tx_t char_textures_normal[]={mk_tx(tx_space,0),mk_tx(tx_klicaj,0),mk_tx(tx_citat,0),mk_tx(tx_hash,0),mk_tx(tx_dolar,0),mk_tx(tx_procent,0),mk_tx(tx_and,0),mk_tx(tx_apostrof,0),mk_tx(tx_uklepaj,0),mk_tx(tx_zaklepaj,0),mk_tx(tx_zvezda,0),mk_tx(tx_plus,0),mk_tx(tx_vejica,0),mk_tx(tx_minus,0),mk_tx(tx_pika,0),mk_tx(tx_slash,0),mk_tx(tx_0,0),mk_tx(tx_1,0),mk_tx(tx_2,0),mk_tx(tx_3,0),mk_tx(tx_4,0),mk_tx(tx_5,0),mk_tx(tx_6,0),mk_tx(tx_7,0),mk_tx(tx_8,0),mk_tx(tx_9,0),mk_tx(tx_space,0),mk_tx(tx_space,0),mk_tx(tx_space,0),mk_tx(tx_space,0),mk_tx(tx_space,0),mk_tx(tx_space,0),mk_tx(tx_space,0),mk_tx(tx_A,0),mk_tx(tx_B,0),mk_tx(tx_C,0),mk_tx(tx_D,0),mk_tx(tx_E,0),mk_tx(tx_F,0),mk_tx(tx_G,0),mk_tx(tx_H,0),mk_tx(tx_I,0),mk_tx(tx_J,0),mk_tx(tx_K,0),mk_tx(tx_L,0),mk_tx(tx_M,0),mk_tx(tx_N,0),mk_tx(tx_O,0),mk_tx(tx_P,0),mk_tx(tx_Q,0),mk_tx(tx_R,0),mk_tx(tx_S,0),mk_tx(tx_T,0),mk_tx(tx_U,0),mk_tx(tx_V,0),mk_tx(tx_W,0),mk_tx(tx_X,0),mk_tx(tx_Y,0),mk_tx(tx_Z,0)};
const tx_t char_textures_big[]={mk_tx(tx_big_0,0),mk_tx(tx_big_1,0),mk_tx(tx_big_2,0),mk_tx(tx_big_3,0),mk_tx(tx_big_4,0),mk_tx(tx_big_5,0),mk_tx(tx_big_6,0),mk_tx(tx_big_7,0),mk_tx(tx_big_8,0),mk_tx(tx_big_9,0)};
const tx_t char_textures_smol_light[]={mk_tx(tx_smol_light_0,0),mk_tx(tx_smol_light_1,0),mk_tx(tx_smol_light_2,0),mk_tx(tx_smol_light_3,0),mk_tx(tx_smol_light_4,0),mk_tx(tx_smol_light_5,0),mk_tx(tx_smol_light_6,0),mk_tx(tx_smol_light_7,0),mk_tx(tx_smol_light_8,0),mk_tx(tx_smol_light_9,0)};


int draw_char(int x0, int y0, char c, tx_t *font, bool draw_from_right){
	tx_t tx=font[(int)c];

	if(draw_from_right) x0-=tx.width;

	for(int y=0;y<tx.height;y++) for(int x=0;x<tx.width;x++) if(tx.p[y*tx.width+x]!=0b1111111111100000) screen[y0+y][x0+x]=tx.p[y*tx.width+x];

	return tx.width;
}


void draw_string(int x0, int y0, char *string){
	if(!string) return;
	for(int i=0;string[i];i++){
		x0+=draw_char(x0, y0, string[i]-(('a'<=string[i] && string[i]<='z')?64:32), char_textures_normal, 0)+1;
		if(x0>w-10){
			x0=1;
			y0+=10;
		}
	}
}


void draw_num(int x0, int y0, int num, const tx_t *font){
	char string[8]={0,0,0,0,0,0,0,0};
	sprintf(string,"%d",num);
	for(int i=7;i>=0;i--) if(string[i] && string[i]!='-') x0-=draw_char(x0, y0, string[i]-0x30, font, 1);
}


void drawf(int x0, int y0, char *string, ...){
	char formated_string[256];
	va_list args;
	va_start(args,string);
	vsprintf(formated_string, string, args);
	draw_string(x0, y0, formated_string);
}

int str_len(char *str){
	int n=0;
	while(*str){
		int c=*str;
		c-=('a'<=c && c<='z')?64:32;
		n+=char_textures_normal[c].width+1;
		str++;
	}
	return n;
}


void draw_hud_seg(int x0, int y0, int x1, int y1){
	for(int y=y0;y<y1;y++) for(int x=x0;x<x1;x++) screen[y+(h-hud_h)][x]=tx_background[y][x];
}

void copy_to_other_screen(int x0, int y0, int x1, int y1){
	for(int y=y0;y<y1;y++) for(int x=x0;x<x1;x++) screen_out[y+(h-hud_h)][x]=screen[y+(h-hud_h)][x];
}

void update_hud(uint16_t (*target_screen)[w], bool draw_all){
	static int health_old=-1,armor_old=-1,current_ammo_old=-1,owned_guns_old=0;
	static uint16_t ammo_old[6];
	static uint16_t max_ammo_old[6];
	static uint16_t face_num,face_count,face_dir,rampage_counter,face_num_old;

	if(draw_all) owned_guns_old=player_owned_guns;

	bool switch_screens=screen!=target_screen;

	if(switch_screens){
		void *temp=screen;
		screen=screen_out;
		screen_out=temp;
	}

	if(draw_all) draw_hud_seg(0,0,w,hud_h);


	//health
	if(player->health!=health_old || draw_all){
		draw_hud_seg(37,1,69,18);
		draw_num(69,h-hud_h+2,player->health,char_textures_big);
		copy_to_other_screen(37,1,69,18);
	}

	//armor
	if(player_armor!=armor_old || draw_all){
		draw_hud_seg(134,1,167,18);
		draw_num(167,h-hud_h+2,player_armor,char_textures_big);
		copy_to_other_screen(134,1,167,18);
	}

	//currnet ammo
	if(player_current_ammo[gun_ammo_type[player_held_gun]]!=current_ammo_old || draw_all){
		draw_hud_seg(0,1,35,18);
		if(player_held_gun>1) draw_num(34,h-hud_h+2,player_current_ammo[gun_ammo_type[player_held_gun]],char_textures_big);
		copy_to_other_screen(0,1,35,18);
	}

	//small ammo
	for(int i=0;i<4;i++){
		if(player_current_ammo[i]!=ammo_old[i] || player_max_ammo[i]!=max_ammo_old[i] || draw_all){
			draw_hud_seg(202,2,238,26);
			for(int i=0;i<4;i++){
				draw_num(w-23,h-hud_h+i*6+2,player_current_ammo[i],char_textures_smol_light);		//current ammo
				draw_num(w-3,h-hud_h+i*6+2,player_max_ammo[i],char_textures_smol_light);			//max ammo
			}
			copy_to_other_screen(202,2,238,26);
			break;
		}
	}

	//owned guns
	if(owned_guns_old!=player_owned_guns || draw_all){
		draw_hud_seg(83,2,105,17);
		for(int i=2;i<5;i++) if(player_owned_guns&(1<<i)) draw_num(87+i*9-18,h-hud_h+2,i,char_textures_smol_light);
		for(int i=5;i<8;i++) if(player_owned_guns&(1<<i)) draw_num(87+i*9-9*5,h-hud_h+12,i,char_textures_smol_light);
		copy_to_other_screen(83,2,105,17);
	}



	const int face_priority[]={1,1,1, 5,5, 5, 8, 7, 9, 4};

	if(inputs[key_ctrl]) rampage_counter++;
	else rampage_counter=0;


	//hud face
	int blood_offset=(100-player->health)/20;
	if(blood_offset<0) blood_offset=0;


	int new_face_num=0;

	//dead
	if(player->state==ACTOR_STATE_DEAD){
		new_face_num=8;
		face_count=1;
		blood_offset=0;
	}

	//grin
	else if(owned_guns_old!=player_owned_guns && face_priority[face_num]<8){
		new_face_num=6;
		face_count=60;
	}

	//au
	else if(health_old-player->health>20 && face_priority[face_num]<7){
		new_face_num=7;
		face_count=20;
	}

	//damage
	else if(health_old-player->health>0 && face_priority[face_num]<5){
		new_face_num=5;
		face_count=30;
	}

	//rampage
	else if(rampage_counter>60 && face_priority[face_num]<6){
		new_face_num=5;
		face_count=1;
	}

	//godmode
	else if(allow_godmode && face_priority[face_num]<5){
		new_face_num=9;
		face_count=1;
		blood_offset=0;
	}

	//default
	else if(face_priority[face_num]<2){
		new_face_num=face_dir;
		face_count=1;
	}

	if(!(gametick&15)) face_dir=doom_rand()%3;

	if(face_priority[new_face_num]>=face_priority[face_num] || !face_count--) face_num=new_face_num;

	tx_t face=hud_face_sprite_base[5*face_num+blood_offset];


	if(face_num_old!=face_num || draw_all){
		draw_hud_seg(107,1,133,27);
		for(int y=0;y<face.height;y++) for(int x=0;x<face.width;x++) if(face.p[(y-1)*face.width+x]!=0b1111111111100000 && y>face.height-hud_h) screen[h-face.height+y][w/2-face.width/2+x]=face.p[(y-1)*face.width+x];
		copy_to_other_screen(107,1,133,27);
	}



	health_old=player->health;
	armor_old=player_armor;
	current_ammo_old=player_current_ammo[gun_ammo_type[player_held_gun]];
	owned_guns_old=player_owned_guns;
	face_num_old=face_num;
	for(int i=0;i<6;i++){
		ammo_old[i]=player_current_ammo[i];
		max_ammo_old[6]=player_max_ammo[i];
	}



	if(switch_screens){
		void *temp=screen;
		screen=screen_out;
		screen_out=temp;
	}
}


void tint_screen_red(int jakost){
	for(int y=0;y<h-hud_h;y++) for(int x=0;x<w;x++){
		int other=screen[y][x];
		int red=other & 0b11111;
		other&=0b1111111111100000;
		red=((red-32)>>jakost)+32;
		screen[y][x]=other|red;
	}
}

void tint_screen_pickup(){
	for(int y=0;y<h-hud_h;y++) for(int x=0;x<w;x++){
		int other=screen[y][x];
		int red=other & 0b0000000000011111;
		int grn=other & 0b0000011111100000;
		int blu=other & 0b1111100000000000;
		grn+=((64<<5)-grn)>>2;
		grn&=0b0000011111100000;
		red+=(32-red)>>2;
		screen[y][x]=red|grn|blu;
	}
}

void screen_tint_logic(){
	static float screen_tint=0;
	static int player_health_old=0;

	screen_tint+=(player_health_old-player->health)/4.;
	if(pickup_screen_tint_timer){
		tint_screen_pickup();
		pickup_screen_tint_timer--;
	}
	else{
		if(screen_tint>1){
			tint_screen_red((int)(screen_tint));
			screen_tint*=.9;
		}
		else screen_tint=0;
	}

	player_health_old=player->health;
}

void draw_crosshair(){
	for(int i=-3;i<-1;i++){
		screen[h/2][w/2+i]=color(0,0,0);
		screen[h/2+i][w/2]=color(0,0,0);
	}
	for(int i=2;i<4;i++){
		screen[h/2][w/2+i]=color(0,0,0);
		screen[h/2+i][w/2]=color(0,0,0);
	}
}


void motd_logic(){
	if(motd_timer>0){
		draw_string(1,1,motd);
		motd_timer--;
	}
	else motd=NULL;
}


void draw_textbox(textbox_t *textbox){
	draw_string(textbox->x, textbox->y, textbox->txt);
}

void selected_textbox_logic(){
	tx_t tx=(gametick&8)?mk_tx(tx_text_skull_off,0):mk_tx(tx_text_skull_on,0);

	//int d=str_len(selected_textbox->txt);

	for(int y=0;y<tx.height;y++) for(int x=0;x<tx.width;x++) if(tx.p[y*tx.width+x]!=0b1111111111100000){
		screen[y+selected_textbox->y-6][x+selected_textbox->x-tx.width-7]=tx.p[y*tx.width+x];
		//screen[y+selected_textbox->y-6][x+selected_textbox->x+d+1]=tx.p[y*tx.width+x];
	}

	if(inputs[key_space] && !inputs_old[key_space]) selected_textbox->func();
}








