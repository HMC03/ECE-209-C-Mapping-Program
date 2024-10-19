
#include "Street.h"
#include <stdlib.h>
#include <string.h>


/* create a new street and initialize */
/* returns pointer to newly-allocated struct street */
/* returns null if error: (a) name too long, (b) points in wrong order */
Street * newStreet(const char *name, const Point * start, const Point *end) {
   if(strlen(name) > 15) return NULL;
   if((start->north != end->north) && (start->east != end->east)) return NULL;
   if((start->north < end->north) || (start->east > end->east)) return NULL;
   if(start->north < 0 || start->east < 0 || end->north < 0 || end->east < 0) return NULL;

   Street *s = malloc(sizeof(Street)); //creates & initializes a new street
   strcpy(s->name,name);
   s->start = *start;
   s->end = *end;

   return s;
}

/* returns the distance from start to end */
int streetLength(const Street *s) {
   int l = 0;

   if((s->start.north != s->end.north) && (s->start.east != s->end.east)) {//Checks for diagonal streets just in case
       l = -1;
       return l;
   }
   if(s->start.north != s->end.north) l = abs(s->end.north - s->start.north);
   if(s->start.east != s->end.east) l = abs(s->end.east - s->start.east);

   return l;
}

/* returns a literal string that gives the direction of a street */
/* 0 for west-to-east, 1 for north-to-south */
/* note: west-to-east is the same as east-to-west, and ditto for north-south */
int streetDirection(const Street *s) {
    int d = -1;

    //Checks for diagonal streets just in case
    if((s->start.north != s->end.north) && (s->start.east != s->end.east)) return d;

    if(s->start.north != s->end.north) d = 1; //North-South
    if(s->start.east != s->end.east) d = 0; //West-East

    return d;
}
