#include <assert.h>
#include "common.h"
#include "point.h"
#include <math.h>
#include <stdbool.h>

void
point_translate(struct point *p, double x, double y)
{
	p->x = p->x + x;
	p->y = p->y + y;
}

double
point_distance(const struct point *p1, const struct point *p2)
{
	return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

int
point_compare(const struct point *p1, const struct point *p2)
{
	struct point* origin = (struct point*)malloc(sizeof(struct point));
	origin->x = 0; origin->y = 0;
	double p1Length = point_distance(origin, p1);
	double p2Length = point_distance(origin, p2);

	if(p1Length < p2Length){
		return -1;
	}
	else if(p1Length > p2Length){
		return 1;
	}
	else{
		return 0;
	}
}
