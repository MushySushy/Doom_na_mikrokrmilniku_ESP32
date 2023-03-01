point_t line_line_presecisce(point_t *p1, point_t *p2, point_t *p3, point_t *p4){
	float k1=(p2->y-p1->y)/(p2->x-p1->x);
	float n1=p1->y-p1->x*k1;

	float k2=(p4->y-p3->y)/(p4->x-p3->x);
	float n2=p3->y-p3->x*k2;

	float presek;
	bool check_x=true;

	if(k1==k2) return (point_t){0,0,0};
	else if(p1->x==p2->x){
		presek=p1->x;
		check_x=false;
	}
	else if(p3->x==p4->x){
		presek=p3->x;
		check_x=false;
	}
	else presek=(n2-n1)/(k1-k2);

	float presek_y=presek*k1+n1;

	return (point_t){presek,presek_y,check_x?(((p1->x<=presek) == (presek<=p2->x)) && ((p3->x<=presek) == (presek<=p4->x))):(((p1->y<=presek_y) == (presek_y<=p2->y)) && ((p3->y<=presek_y) == (presek_y<=p4->y)))};
}

bool line_colides_square(point_t *p1, point_t *p2, float x, float y, float r){
	float x1=p1->x;
	float y1=p1->y;
	float x2=p2->x;
	float y2=p2->y;

	float k=(y2-y1)/(x2-x1);
	float n=y1-k*x1;

	if(x2<x1){
		float temp=x2;
		x2=x1;
		x1=temp;
	}

	if(y2<y1){
		float temp=y2;
		y2=y1;
		y1=temp;
	}


	float presek;

	if(x1==x2){if((y1-r<y && y<y2+r) && (x-r<x1 && x1<x+r)) return true;}
	else if(y1==y2){if((x1-r<x && x<x2+r) && (y-r<y1 && y1<y+r)) return true;}
	else{
		presek=((y+r)-n)/k;	//zgorna
		if((x-r <= presek && presek <= x+r) && (x1 <= presek && presek <= x2)) return true;

		presek=((y-r)-n)/k;	//spodna
		if((x-r <= presek && presek <= x+r) && (x1 <= presek && presek <= x2)) return true;

		presek=k*(x+r)+n;	//desna
		if((y-r <= presek && presek <= y+r) && (y1 <= presek && presek <= y2)) return true;

		presek=k*(x-r)+n;	//leva
		if((y-r <= presek && presek <= y+r) && (y1 <= presek && presek <= y2)) return true;
	}

	return false;
}

bool actor_colides_polygon(actor_t* actor, polygon_t* poly){
	if(!(poly->flags & IS_SOLID)) return false;

	if(poly->flags & IS_WALL){
		//hitr vertikaln colision
		if((poly->points[2].z < actor->pos.z) || (actor->pos.z+actor->height < poly->points[0].z)) return false;

		return line_colides_square(&poly->points[0],&poly->points[1], actor->pos.x, actor->pos.y, actor->width/2);
	}

	else{
		//hitr vertikaln colision
		if(!(actor->pos.z < poly->points[0].z && poly->points[0].z < actor->pos.z+actor->height)) return false;


		/* a je sploh kje bliz poligona na y		CE TO SE KDEJ UPORABM ZA OPTIMIZACIJO RABS PLAYER Y-U PRISTET SE PLAYER WIDTH/2
		float max=poly->points[0].y;
		float min=poly->points[0].y;
		for(int i=1;i<poly->size;i++){
			if(poly->points[i].y<min) min=poly->points[i].y;
			if(poly->points[i].y>max) max=poly->points[i].y;
		}

		if(!((min<actor->y) && (actor->y<max))) return false;
		*/

		//a je centr playerja u poligonu
		float k,n, p;
		int p_num=0;
		int i_old=poly->size-1;


		for(int i=0;i<poly->size;i++){
			if(poly->points[i].x == poly->points[i_old].x) p=poly->points[i].x;

			else{
				k=(poly->points[i_old].x-poly->points[i].x)/(poly->points[i_old].y-poly->points[i].y);
				n=poly->points[i].x-k*poly->points[i].y;

				p=k*actor->pos.y+n;
			}

			p_num+=(p<actor->pos.x) && (((poly->points[i].y < actor->pos.y) && (actor->pos.y < poly->points[i_old].y)) || ((poly->points[i_old].y < actor->pos.y) && (actor->pos.y < poly->points[i].y)));


			i_old=i;
		}

		if(p_num & 1) return true;


		//colision player kvadrata s robovi poligona
		i_old=poly->size-1;

		for(int i=0;i<poly->size;i++){
			if(line_colides_square(&poly->points[i], &poly->points[i_old], actor->pos.x, actor->pos.y, actor->width/2)) return true;
			i_old=i;
		}

		return false;
	}

}

void push_actor_out_of_surf(actor_t* actor, polygon_t* poly){
	if(poly->flags & IS_WALL){
		vert_t p1=poly->points[0];
		vert_t p2=poly->points[1];

		if(p1.x==p2.x) actor->pos.x=p1.x+poly->normal.x*actor->width/2;
		else if(p1.y==p2.y) actor->pos.y=p1.y+poly->normal.y*actor->width/2;
		else{
			float k=(p2.y-p1.y)/(p2.x-p1.x);
			float n=p1.y-k*p1.x;


			int prava_stran=fast_sign(k*(p1.x+poly->normal.x)+n-(p1.y+poly->normal.y));

			float force=0,force_temp;
			float x,y;
			float inv_sqrt=fast_inv_sqrt(k*k+1);

			x=actor->pos.x+actor->width/2;
			y=actor->pos.y+actor->width/2;
			if(fast_sign(k*x+n-y)!=prava_stran){
				//force_temp=fast_fabs((-k/n)*x+(1/n)*y-1)*fast_inv_sqrt(k*k/(n*n)+1/(n*n));
				force_temp=fast_fabs(y-(k*x+n))*inv_sqrt;
				if(force<force_temp) force=force_temp;
			}

			x=actor->pos.x+actor->width/2;
			y=actor->pos.y-actor->width/2;
			if(fast_sign(k*x+n-y)!=prava_stran){
				force_temp=fast_fabs(y-(k*x+n))*inv_sqrt;
				if(force<force_temp) force=force_temp;
			}

			x=actor->pos.x-actor->width/2;
			y=actor->pos.y+actor->width/2;
			if(fast_sign(k*x+n-y)!=prava_stran){
				force_temp=fast_fabs(y-(k*x+n))*inv_sqrt;
				if(force<force_temp) force=force_temp;
			}

			x=actor->pos.x-actor->width/2;
			y=actor->pos.y-actor->width/2;
			if(fast_sign(k*x+n-y)!=prava_stran){
				force_temp=fast_fabs(y-(k*x+n))*inv_sqrt;
				if(force<force_temp) force=force_temp;
			}

			actor->pos.x+=poly->normal.x*force;
			actor->pos.y+=poly->normal.y*force;
		}
	}

	else{
		if(poly->normal.z==1){
			actor->pos.z+=.2;
			if(poly->points[0].z < actor->pos.z) actor->pos.z=poly->points[0].z;
		}
		else{
			actor->pos.z=poly->points[0].z-(actor->height+.1);
		}
	}

}


bool actor_colides_actor(actor_t *actor1, actor_t *actor2){
	float x1=actor1->pos.x-actor1->width/2;
	float x2=actor1->pos.x+actor1->width/2;
	float x3=actor2->pos.x-actor2->width/2;
	float x4=actor2->pos.x+actor2->width/2;

	float y1=actor1->pos.y-actor1->width/2;
	float y2=actor1->pos.y+actor1->width/2;
	float y3=actor2->pos.y-actor2->width/2;
	float y4=actor2->pos.y+actor2->width/2;

	float z1=actor1->pos.z+actor1->height;
	float z2=actor1->pos.z;
	float z3=actor2->pos.z+actor2->height;
	float z4=actor2->pos.z;

	if(actor1->width >= actor2->width) return ((x1<=x3 && x3<=x2) || (x1<=x4 && x4<=x2)) && ((y1<=y3 && y3<=y2) || (y1<=y4 && y4<=y2)) && ((z4<=z1 && z1<=z3) || (z4<=z2 && z2<=z3));
	else return ((x3<=x2 && x2<=x4) || (x3<=x1 && x1<=x4)) && ((y3<=y2 && y2<=y4) || (y3<=y1 && y1<=y4)) && ((z4<=z1 && z1<=z3) || (z4<=z2 && z2<=z3));
}


void push_actor_out_of_actor(actor_t *pusher, actor_t *pushee){
	if(allow_no_clip && (pusher==player || pushee==player)) return;

	float dx=pushee->pos.x-pusher->pos.x;
	float dy=pushee->pos.y-pusher->pos.y;

	if(fast_fabs(dx)>fast_fabs(dy)) pushee->pos.x=pusher->pos.x+fast_sign(dx)*(pushee->width+pusher->width)/2;
	else pushee->pos.y=pusher->pos.y+fast_sign(dy)*(pushee->width+pusher->width)/2;
}

void actor_hit_wall(actor_t *actor, polygon_t *surf);

void world_colision(actor_t *actor){
	polygon_t *boops[8];
	int n=0;

	for(int i=0;i<static_surf_num;i++) if(actor_colides_polygon(actor,static_surfs+i)){
		if(static_surfs[i].flags & IS_WALL) boops[n++]=static_surfs+i;
		else push_actor_out_of_surf(actor,static_surfs+i);
	}

	for(int i=0;i<dynamic_surf_num;i++) if(actor_colides_polygon(actor,dynamic_surfs+i)){
		if(dynamic_surfs[i].flags & IS_WALL) boops[n++]=dynamic_surfs+i;
		else push_actor_out_of_surf(actor,dynamic_surfs+i);
	}

	if(!n) return;
	if(n==1){
		actor_hit_wall(actor,boops[0]);
		return;
	}

	polygon_t *best=boops[0];
	float best_v=1;
	float dot;
	point_t v;

	for(int i=0;i<n;i++){
		v=point_sub(boops[i]->points+1,boops[i]->points);

		dot=fast_fabs(fast_dot_2d(&player->vel,&v)/(get_mag_2d(&v)*get_mag_2d(&player->vel)));

		if(best_v>dot){
			best=boops[i];
			best_v=dot;
		}
	}

	actor_hit_wall(actor,best);

	best=(best==boops[0])?boops[1]:boops[0];

	if(actor_colides_polygon(actor,best)) push_actor_out_of_surf(actor,best);
}







