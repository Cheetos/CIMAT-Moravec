#ifndef _POINTLIST_H
#define _POINTLIST_H

typedef struct stPointList
{
	int x;
	int y;
	struct stPointList *next;
}pointList;

int Add_Point(int x, int y, pointList *);
int Remove_Point(pointList *);

#endif
