int fast_sign(float x){
	return (x<0)?-1:1;
}

float fast_fabs(float x){
	//return x & 1<<31
	return (x>=0)?x:-x;
}

float fast_fmod(double x, double y){
	return x-((int)(x/y))*y;
}

float fast_sin_0_to_pi(double x){
	x-=pi/2;
	return 1-x*x/2+x*x*x*x/24-x*x*x*x*x*x/720;
}

float fast_sin(double x){
	x=fast_fmod(x+2*pi,2*pi);
	if(x<pi) return fast_sin_0_to_pi(x);
	return -fast_sin_0_to_pi(x-pi);
}

float fast_cos(double x){
	return fast_sin(x+pi/2);
}

float fast_tan(double x){
	return fast_sin(x)/fast_cos(x);
}

//only works well for -.5<x<.5
/*
float fast_acos_limited(float x){
	return pi/2-x-x*x*x*(1/5.3);
}
*/


//hvala quake
float fast_inv_sqrt(const float n){
   static union{int i; float f;} u;
   u.i = 0x5F375A86 - (*(int*)&n >> 1);
   return (3 - n * u.f * u.f) * u.f * 0.5;
}

float fast_sqrt(const float n){
   static union{int i; float f;} u;
   u.i = 0x5F375A86 - (*(int*)&n >> 1);
   return (3 - n * u.f * u.f) * u.f * 0.5 * n;
}

point_t point_add(point_t *p1, point_t *p2){
	return (point_t){
		p1->x+p2->x,
		p1->y+p2->y,
		p1->z+p2->z,
	};
}

point_t point_sub(point_t *p1, point_t *p2){
	return (point_t){
		p1->x-p2->x,
		p1->y-p2->y,
		p1->z-p2->z,
	};
}

point_t vert_add(vert_t *p1, vert_t *p2){
	return (point_t){
		p1->x+p2->x,
		p1->y+p2->y,
		p1->z+p2->z,
	};
}

point_t vert_sub(vert_t *p1, vert_t *p2){
	return (point_t){
		p1->x-p2->x,
		p1->y-p2->y,
		p1->z-p2->z,
	};
}

float fast_dot_2d(point_t *p1, point_t *p2){
	return p1->x*p2->x+p1->y*p2->y;
}


point_t fast_cross(point_t a, point_t b){
	return (point_t){
		a.y*b.z-a.z*b.y,
		a.z*b.x-a.x*b.z,
		a.x*b.y-a.y*b.x,
	};
}

void add_to_point(point_t *p1, point_t *p2){
	p1->x+=p2->x;
	p1->y+=p2->y;
	p1->z+=p2->z;
}

void sub_from_point(point_t *p1, point_t *p2){
	p1->x-=p2->x;
	p1->y-=p2->y;
	p1->z-=p2->z;
}

void rotate_point_2d(point_t *p, float theta){
	float temp=p->x;
	p->x = p->x*fast_cos(theta)+p->y*fast_sin(theta);
	p->y = -temp*fast_sin(theta)+p->y*fast_cos(theta);
}

void normalize_3d(point_t *p){
	float inv_sqrt=fast_inv_sqrt(p->x*p->x + p->y*p->y + p->z*p->z);
	p->x*=inv_sqrt;
	p->y*=inv_sqrt;
	p->z*=inv_sqrt;
}

void normalize_2d(point_t *p){
	float inv_sqrt=fast_inv_sqrt(p->x*p->x + p->y*p->y);
	p->x*=inv_sqrt;
	p->y*=inv_sqrt;
}

float get_mag_2d(point_t *p){
	return fast_sqrt(p->x*p->x + p->y*p->y);
}

/*
void normalize_3d(point_t *p){
	float inv_sqrt=fast_inv_sqrt(p->x*p->x + p->y*p->y + p->z*p->z);
	p->x*=inv_sqrt;
	p->y*=inv_sqrt;
	p->z*=inv_sqrt;
}
*/

float get_actor_dist_2d_unsqrtd(actor_t *actor1, actor_t *actor2){
	float dx=actor1->pos.x-actor2->pos.x;
	float dy=actor1->pos.y-actor2->pos.y;

	return dx*dx+dy*dy;
}


float get_actor_dist_doom(actor_t *actor1, actor_t *actor2){
	float dx=fast_fabs(actor1->pos.x-actor2->pos.x);
	float dy=fast_fabs(actor1->pos.y-actor2->pos.y);

	return (dx>dy)?dx:dy;
}




