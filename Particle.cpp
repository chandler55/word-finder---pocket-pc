#include "Particle.h"

CParticle::CParticle(void)
{
	srand(PocketPC::GetTickCount());
	Build_Tables();
}

CParticle::~CParticle(void)
{
}
inline int rand_range(int _min, int _max)
{
	return (rand()%(_max - _min + 1)) + _min;
}


void CParticle::Init_Reset_Particles() {
	for (int index = 0; index < MAX_PARTICLES; index++) {
		particles[index].state = PARTICLE_STATE_DEAD;
		particles[index].type = PARTICLE_TYPE_FADE;
		particles[index].x = 0;
		particles[index].y = 0;
		particles[index].xv = 0;
		particles[index].yv = 0;
		particles[index].start_color = 0;
		particles[index].end_color = 0;
		particles[index].curr_color = 0;
		particles[index].counter = 0;
		particles[index].max_count = 0;
	} 
} // end Init_Reset_Particles

void CParticle::Build_Tables() {
	int x;

		for(x = 0; x <= 360; x++)
		{
			sin_look[x] = sin(DEG_TO_RAD(x));
			cos_look[x] = cos(DEG_TO_RAD(x));
		}
}

void CParticle::Start_Particle(int type, int color, int count, float x, float y, float xv, float yv) {
	int pindex = -1;

	for (int index = 0; index < MAX_PARTICLES; index++) {
		if (particles[index].state == PARTICLE_STATE_DEAD) {
			pindex = index;
			break;
		}
	}

	if (pindex == -1) // not find one?
		return;

	particles[pindex].state = PARTICLE_STATE_ALIVE;
	particles[pindex].type = type;
	particles[pindex].x = x;
	particles[pindex].y = y;
	particles[pindex].xv = xv;
	particles[pindex].yv = yv;
	particles[pindex].counter = 0;
	particles[pindex].max_count = count;

	switch (color) {
		case PARTICLE_COLOR_RED: {
			particles[pindex].start_color = COLOR_RED_START;
			particles[pindex].end_color = COLOR_RED_END;
		 } break;
		case PARTICLE_COLOR_GREEN: {
			particles[pindex].start_color = COLOR_GREEN_START;
			particles[pindex].end_color = COLOR_GREEN_END;
		 } break;
		case PARTICLE_COLOR_BLUE: {
			particles[pindex].start_color = COLOR_BLUE_START;
			particles[pindex].end_color = COLOR_BLUE_END;
		 } break;
		case PARTICLE_COLOR_WHITE: {
			particles[pindex].start_color = COLOR_WHITE_START;
			particles[pindex].end_color = COLOR_WHITE_END;
		 } break;
	} // end switch

	// what type of particle is being requested
	if (type == PARTICLE_TYPE_FLICKER) {
		particles[pindex].curr_color = rand_range(particles[pindex].start_color,particles[pindex].end_color);
	} else {
		particles[pindex].curr_color = particles[pindex].start_color;
	}
}

void CParticle::Process_Particles(double mvTimer) {
	for (int index = 0; index < MAX_PARTICLES; index++) {
		if (particles[index].state == PARTICLE_STATE_ALIVE) {
			particles[index].x += particles[index].xv * mvTimer * 50;
			particles[index].y += particles[index].yv * mvTimer * 50;

			if (particles[index].type == PARTICLE_TYPE_FLICKER) {
				particles[index].curr_color = rand_range(particles[index].start_color, particles[index].end_color);
			} // end if
			if (++particles[index].counter >= particles[index].max_count) {
				particles[index].state = PARTICLE_STATE_DEAD;
			} // end if
		} else { //	end if
			if (++particles[index].counter >= particles[index].max_count) {
				particles[index].counter = 0;

				// update color
				if (++particles[index].curr_color > particles[index].end_color) {
					particles[index].state = PARTICLE_STATE_DEAD;
				} // end if
			} // end if
		} // end else

		if (particles[index].x > screen_width || particles[index].x < 0 ||
			particles[index].y > screen_height || particles[index].y < 0) {
				particles[index].state = PARTICLE_STATE_DEAD; // kill it
		}
	}
} // end process particles

void CParticle::Draw_Particles(Display* display) {
	for (int index = 0; index < MAX_PARTICLES; index++) {
		if (particles[index].state == PARTICLE_STATE_ALIVE) {
			int x = (int)particles[index].x;
			int y = (int)particles[index].y;

			// test for clip
			if (x >= screen_width || x < 0 || y >= screen_height || y < 0)
				continue;

			display->SetPixel(x, y, particles[index].curr_color);
			display->SetPixel(x+1, y, particles[index].curr_color);
			display->SetPixel(x, y+1, particles[index].curr_color);
			display->SetPixel(x+1, y+1, particles[index].curr_color);
		}
	}
}

void CParticle::Start_Particle_Explosion(int type, int color, int count, int x, int y, int xv, int yv, int num_particles) {
	while (--num_particles >= 0) {
		int ang = rand()%360;
		float vel = (float)2 + rand() %4;

		Start_Particle(type, color, count, (float)x + rand_range(-4,4), (float)y+rand_range(-4,4),(float)xv +cos_look[ang]*vel, 
			(float)yv + sin_look[ang]*vel);
	}
}