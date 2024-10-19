/* Definition of Map abstract data type */
/* Program 2, ECE 209 Fall 2023         */
/* NOTHING in this file may be changed  */

#ifndef MAP_H
#define MAP_H

#include "Street.h"

/* struct map must be defined in map.c */
typedef struct map * Map;

/* allocate and initialize a new Map */
/* if error, returns NULL */
/* possible errors:
 * name longer than 15 characters
 */
Map createMap(const char *name);

/* returns map name string */
/* do not copy, just return pointer to string */
const char *mapName(Map m);

/* add a new street to Map */
/* return 1 to acknowledge that street has been added */
/* return 0 if error */
/* errors that must be checked:
 * s has the same name as another street
 * s overlaps with another street in the same direction
 */
int mapAddStreet(Map m, Street * s);

/* create and return a list of all streets in the map */
/* must be sorted alphabetically by name */
/* must create a new list -- may be deallocated by caller */
StreetList * mapAllStreets(Map m);

/* create and return a list of all north-south streets */
/* must be sorted from west to east, then by name */
/* must create a new list -- may be deallocated by caller */
StreetList * mapNSStreets(Map m);

/* create and return a list of all west-east streets */
/* most be sorted from north to south, then by name */
/* must create a new list -- may be deallocated by caller */
StreetList * mapWEStreets(Map m);

/* find street by name */
/* does not create a new street, just returns a pointer */
/* returns NULL if street not in map */
Street * mapFindStreet(Map m, const char *name);

/* create and return a list of streets that intersect with given street */
/* if s is a north-south street, the return list must be sorted from north to south */
/* if s is a west-east street, the return list must be sorted from west to east */
/* must create a new list -- may be deallocated by caller */
StreetList * mapCrossStreets(Map m, const Street *s);


#endif
