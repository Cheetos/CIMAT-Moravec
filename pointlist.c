#include "pointlist.h"

int Add_Point(int x, int y, pointList *h)
{
	pointList *p = malloc(sizeof(pointList));
	
	p->x = x;
	p->y = y;
	p->next = h->next;
	h->next = p;
	
	return 1;
}

int Remove_Point(pointList *h)
{
	pointList *temp = h->next;
	h->next = temp->next;
	
	free(temp);
	
	return 1;
}
