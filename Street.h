/* Data structures and functions for street */
/* Program 2 - Fall 2023                    */
/* NOTHING in this file may be changed      */

#ifndef STREET_H
#define STREET_H

/* represents a point on the map */
/* map origin is (0,0) in the left bottom corner */
/* "east" is number of blocks to the right of the origin */
/* "north" is the number of blocks above the origin */
struct point {
    int east;   /* x-coordinate of map point */
    int north;  /* y-coordinate of map point */
};
typedef struct point Point;

/* represents a street */
/* for a west-to-east street, start must be the western-most point */
/* for a north-to-south street, start must be the northern-most point */
struct street {
    char name[16];  /* street name is up to 15 characters */
    struct point start;
    struct point end;
};
typedef struct street Street;


/* create a new street and initialize */
/* returns pointer to newly-allocated struct street */
/* if there's an error, returns NULL */
/* possible errors:
 * name longer than 15 characters
 * any negative values in point coordinates
 * points in the wrong order -- must go west-to-east or north-to-south
 */
Street * newStreet(const char *name, const Point * start, const Point *end);

/* returns the distance from start to end */
int streetLength(const Street *s);

/* returns 0 for west-to-east, 1 for north-to-south */
int streetDirection(const Street *s);

/* used to build a list of streets */
struct streetNode {
   Street * street;  /* data -- pointer to street data */
   struct streetNode * next;  /* pointer to next item in list */
};
typedef struct streetNode StreetNode;

/* a struct that holds a list of streets */
struct streetList {
   StreetNode * head;
   StreetNode * tail;
};
typedef struct streetList StreetList;

#endif
