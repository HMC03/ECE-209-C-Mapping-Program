#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Street.h"
#include "Map.h"

Map readMap(const char *filename);
void printStreet(const Street *);
void printStreets(const StreetList *);
void printHelp();
void deleteList(StreetList *);  /* deallocate all of list's nodes */

/* main function is user interface */
/* read an initial map file if name is given as argument */
/* then enter user command loop */
int main(int argc, char **argv) {

    Map m = NULL;
    StreetList * streets = NULL;
    Street *s = NULL;

    char command = 'q';
    char input[30] = "";

    printf("Commands:\n");
    printHelp();

    /* if name is provided, read map file */
    if (argc == 2) {
        m = readMap(argv[1]);
        if (m) {
            printf("MAP: %s\n", mapName(m));
        }
    }

    while (1) {   /* user interface */
        printf("\nCommand: "); fflush(stdout);
        scanf("%c", &command);  /* get one-letter command */
        if (command == 'q') {
            break;
        }
        else if (command == 'a') {
            if (!m) {
                printf("ERROR: Must read a map first\n");
            }
            else {
                printf("All streets in %s\n", mapName(m));
                streets = mapAllStreets(m);
                printStreets(streets);
                deleteList(streets);
                free(streets); streets = NULL;
                printf("\nWest-to-east streets:\n");
                streets = mapWEStreets(m);
                printStreets(streets);
                deleteList(streets);
                free(streets); streets = NULL;
                printf("\nNorth-to-south streets:\n");
                streets = mapNSStreets(m);
                printStreets(streets);
                deleteList(streets);
                free(streets); streets = NULL;
            }
        }
        else if (command == 'p') {
            if (!m) {
                printf("ERROR: Must read a map first\n");
            }
            else {
                scanf("%29s", input);
                s = mapFindStreet(m, input);
                if (s) printStreet(s);
                else {
                    printf("Street %s not found.\n", input);
                }
            }
        }
        else if (command == 'c') {
            if (!m) {
                printf("ERROR: Must read a map first\n");
            }
            else {
                scanf("%29s", input);
                s = mapFindStreet(m, input);
                if (!s) {
                    printf("Street %s not found.\n", input);
                }
                else {
                    streets = mapCrossStreets(m, s);
                    printf("Streets that intersect %s:\n", input);
                    printStreets(streets);
                    deleteList(streets);
                    free(streets); streets = NULL;
                }
            }
        }
        else if (command == 'm') {
            m = NULL;  /* delete old map */
            /* this will cause a memory leak, but we won't worry about it */
            scanf("%29s", input);
            m = readMap(input);
            if (!m) {
                printf("ERROR: Unable to read map %s\n", input);
            }
            else {
                printf("New map: %s\n", mapName(m));
            }
        }
        else if (command == 'h') {
            printHelp();
        }
        else {
            printf("ERROR: Unknown command %c\n", command);
        }
        /* remove any extra characters from command */
        do {
            scanf("%c", &command);
        } while (command != '\n');

    }

    return EXIT_SUCCESS;
}

/* read map file -- create map data structure and add each street */
Map readMap(const char *filename) {
    Map new = NULL;
    char mapName[20];
    char streetName[50]; /* in case of illegal street name */
    Point start, end;
    FILE* fp;
    char *p;
    int status;
    Street * s;

    if(strlen(filename) > 19) {
        printf("ERROR: File name %s too long (> 19 chars)\n", filename);
        return NULL;
    }
    fp = fopen(filename, "r");
    if (!fp) {
        printf("ERROR: Unable to open file %s\n:", filename);
        return NULL;
    }

    do {
        strcpy(mapName, filename);
        p = strchr(mapName, '.');
        *p = '\0';
        new = createMap(mapName);
        if (!new) {
            printf("ERROR: Unable to create map\n");
            break;
        }

        while(fscanf(fp, "%49s", streetName) != EOF) {
            status = fscanf(fp, " (%d,%d) (%d,%d)", &start.east, &start.north, &end.east, &end.north);
            if (status != 4) {
                printf("ERROR: failed reading start/end points for %s\n", streetName);
                break;
            }
            s = newStreet(streetName, &start, &end);
            if (!s) {
                printf("ERROR: Unable to create street %s\n", streetName);
                break;
            }
            status = mapAddStreet(new, s);
            if (!status) {
                printf("ERROR: Failed adding street %s to map\n", streetName);
                break;
            }
        }

    } while (0);

    fclose(fp);
    return new;
}

void printHelp() {
    printf("m = read map file\n");
    printf("a = print all streets\n");
    printf("p = print named street\n");
    printf("c = print cross streets\n");
    printf("h = help\n");
    printf("q = quit\n");
    printf("\n");
    return;
}

void printStreet(const Street *s) {
    const char *dir = "";
    if (s) {
        if (streetDirection(s) == 0) dir = "WE";
        else dir = "NS";
        printf("%s: (%d,%d)->(%d,%d) [%s, %d blocks]",
               s->name, s->start.east, s->start.north, s->end.east, s->end.north,
               dir, streetLength(s));
    }
    return;
}

void printStreets(const StreetList * list) {
    const StreetNode *p;
    for (p = list->head; p; p = p->next) {
        printStreet(p->street);
        printf("\n");
    }
    return;
}

void deleteList(StreetList *list) {
    StreetNode *p = list->head;
    StreetNode *next;
    while (p) {
        next = p->next;
        free(p);  /* deallocate node */
        p = next;
    }
    // does not deallocate the list struct itself
    list->head = list->tail = NULL;
    return;
}