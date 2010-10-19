#pragma once
#include <stdlib.h>
#include <math.h>
#include <PocketFrog/PocketFrog.h>
using namespace Frog;

#define PARTICLE_STATE_DEAD 0
#define PARTICLE_STATE_ALIVE 1

#define PARTICLE_TYPE_FLICKER 0
#define PARTICLE_TYPE_FADE 1

#define MAX_PARTICLES 128

#define PARTICLE_COLOR_RED 0
#define PARTICLE_COLOR_BLUE 1
#define PARTICLE_COLOR_GREEN 2
#define PARTICLE_COLOR_WHITE 3

#define COLOR_RED_START 63488
#define COLOR_RED_END 63488
#define COLOR_GREEN_START 2016
#define COLOR_GREEN_END 2016
#define COLOR_BLUE_START 31
#define COLOR_BLUE_END 31
#define COLOR_WHITE_START 131071
#define COLOR_WHITE_END 131071

#define screen_width 320
#define screen_height 240

#define PI 3.14159265

#define DEG_TO_RAD(ang) ((float)(PI* (ang) / 180))


struct Particle {
	float x, y;
	float xv, yv;
	float life;
	int start_color;
	int end_color;
	int curr_color;
	int type;
	int state;
	int counter;
	int max_count;
};

class CParticle
{
	float sin_look[361];
	float cos_look[361];

	Particle particles[MAX_PARTICLES]; // particles for engine
public:
	CParticle(void);
public:
	~CParticle(void);
	void Init_Reset_Particles();
	void Start_Particle(int type, int color, int count, float x, float y, float xv, float yv);
	void Process_Particles(double mvTimer);
	void Draw_Particles(Display* display);
	void Start_Particle_Explosion(int type, int color, int count, int x, int y, int xv, int yv, int num_particles);
	void Build_Tables();
};

