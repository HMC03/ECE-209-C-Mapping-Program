/* Implementation of Map abstract data type */

#include "Map.h"
#include <string.h>
#include <stdlib.h>

//Function Definitions-----------
int abcName(const char * a, const char * b); //Returns a 1 if string 'a' comes first alphabetically, and a -1 if string 'b' does

struct map {
   char name[16];
   StreetList * streets;
};

Map createMap(const char *name) {

    if(strlen(name) > 15) return NULL; //name is too long

    StreetList * emptyList = malloc(sizeof(StreetList)); //define and initialize an empty list
    emptyList->head = NULL;
    emptyList->tail = NULL;

    Map m = malloc(sizeof(Map)); //define and initialize a empty map
    strcpy(m->name,name);
    m->streets = emptyList;

    return m;
}

const char *mapName(Map m) {
    return m->name;
}

/* add a new street to Map */
/* return 1 to acknowledge that street has been added */
/* return 0 if error (see spec) */
int mapAddStreet(Map m, Street * s) {
    if(m->streets->head == NULL){ //If map is empty
        StreetNode * newStreet = malloc(sizeof(StreetNode)); //Define & initialize Street
        newStreet->street = s;
        newStreet->next = NULL;

        m->streets->head = newStreet; //Assign to head and tail of list
        m->streets->tail = newStreet;

        return 1;
    }

    StreetNode * listHead = m->streets->head; //Copy of the head node for modulation
    Street * existingStreet = m->streets->head->street; //For abbreviation/understanding purposes

    while(listHead != NULL) {//Steps through the maps street list until loop reaches end
        if(strcmp(s->name, existingStreet->name) == 0) return 0; //checks for same street name

        if(streetDirection(s) == streetDirection(existingStreet)){ //Checks for overlap
            if(streetDirection(s) == 1){
                if(s->start.east == existingStreet->start.east){
                    if(s->start.north <= existingStreet->start.north) {
                        if(s->end.north >= existingStreet->start.north) return 0;
                    }
                    if(s->start.north <= existingStreet->end.north) {
                        if(s->end.north >= existingStreet->end.north) return 0;
                    }
                }
            }
            if(streetDirection(s) == 0){
                if(s->start.north == existingStreet->start.north){
                    if(s->start.east <= existingStreet->start.east){
                        if(s->end.east >= existingStreet->start.east) return 0;
                    }
                    if(s->start.east >= existingStreet->end.east){
                        if(s->end.east <= existingStreet->end.east) return 0;
                    }
                }
            }
        }

        listHead = listHead->next;
    }

    StreetNode * newStreet = malloc(sizeof(StreetNode)); //Define & initialize Street
    newStreet->street = s;
    newStreet->next = m->streets->head;

    m->streets->head = newStreet; //Assign to head


    return 1;
}

/* create and return a list of all streets in the map */
/* must be sorted alphabetically by name */
/* must create a new list -- may be deallocated by caller */
struct streetList * mapAllStreets(Map m) {

    StreetList * abcStreets = malloc(sizeof(StreetList)); //Define and initialize alphabetically ordered list of streets(abcStreets)
    if(m->streets->head == NULL) { //Initialize abcStreets if mList of streets is empty
        abcStreets->head = NULL;
        abcStreets->tail = NULL;

        return abcStreets;
    }
    StreetNode * mListHead = malloc(sizeof(StreetNode)); //For Abbreviation/understanding/modulation purposes
    mListHead->street = m->streets->head->street;
    mListHead->next = m->streets->head->next;

    StreetNode * mListTail = malloc(sizeof(StreetNode)); //For Abbreviation/understanding/modulation purposes
    mListTail->street = m->streets->tail->street;
    mListTail->next = NULL;

    if(m->streets->head->next == NULL){ //Initialize abcStreets if mList of streets only has one street
        abcStreets->head = mListHead;
        abcStreets->tail = mListHead;

        return abcStreets;
    }
    //Initialize abcStreets to tail of mList when mList contains multiple streets
    abcStreets->head = mListTail;
    abcStreets->tail = mListTail;

    while(mListHead->next != NULL){ //Checks if loop reaches the last node of mList(already accounted for)
        StreetNode * tempNode = malloc(sizeof(StreetNode)); //Define & initialize temporary node
        tempNode->street = mListHead->street;
        tempNode->next = NULL;

        if(abcStreets->head->next == NULL){ //If there is only one street in abcStreets
            if(abcName(abcStreets->head->street->name,tempNode->street->name) < 0){ //If tempNode's street comes first
                tempNode->next = abcStreets->head;
                abcStreets->head = tempNode;
            }
            if(abcName(abcStreets->head->street->name,tempNode->street->name) > 0){ //If tempNode's street goes last
                tempNode->next = NULL;
                abcStreets->head->next = tempNode;
                abcStreets->tail = tempNode;
            }
        }
        else{ //There are multiple streets in abcStreets
            StreetNode * abcP = abcStreets->head; //Points to first node in alphabetical list
            StreetNode * abcPrev = NULL;

            while(abcName(abcP->street->name,tempNode->street->name) > 0){ //Moves abcP to point where tempNode fits alphabetically
                abcPrev = abcP;
                abcP = abcP->next;

                if(abcP == NULL) break;
            }
            if(abcPrev == NULL){ //If tempNode's street comes first
                tempNode->next = abcStreets->head;
                abcStreets->head = tempNode;
            }
            else if(abcP != NULL){ //If tempNode's street goes in middle
                tempNode->next = abcP;
                abcPrev->next = tempNode;
            }
            else{ //If tempNode's street goes last
                tempNode->next = NULL;
                abcPrev->next = tempNode;
                abcStreets->tail = tempNode;
            }
        }
        mListHead = mListHead->next; //Go next street node in mList
    }

    return abcStreets;
}

/* return a list of all north-south streets */
/* must be sorted west to east, then by name */
/* must create a new list -- may be deallocated by caller */
StreetList * mapNSStreets(Map m) {
    if(m->streets->head == NULL) return m->streets; //if map is empty

    StreetList * abcStreets = mapAllStreets(m); //alphabetically ordered streets

    StreetList * cbaNS = malloc(sizeof(StreetList)); //Reverse alphabetical order of the [NS] streets
    cbaNS->head = NULL;
    cbaNS->tail = NULL;

    StreetNode * abcP = abcStreets->head; //For stepping through abcStreets

    while(abcP != NULL){ //Step through abcStreets
        StreetNode * tempNode = malloc(sizeof(StreetNode));
        tempNode->street = abcP->street;
        tempNode->next = NULL;

        if(streetDirection(tempNode->street) == 1 ){ //Put street on cbaNS when it's North-South
            if(cbaNS->head == NULL){ //If this is the first street to go on
                tempNode->next = NULL;
                cbaNS->head = tempNode;
                cbaNS->tail = tempNode;
            }
            else{ //There is already at least one street on cbaNS
                tempNode->next = cbaNS->head;
                cbaNS->head = tempNode;
            }
        }

        abcP = abcP->next;
    }

    if(cbaNS->head == NULL) return cbaNS; //if there are no [NS] streets

    StreetList * nsStreets = malloc(sizeof(StreetList)); //Final North to South list
    nsStreets->head = NULL;
    nsStreets->tail = NULL;

    StreetNode * cbaP1 = cbaNS->head; //For stepping through cbaNS

    StreetNode * eastMost = malloc(sizeof(StreetNode)); //Represents the east most street in cbaNS
    eastMost->street = cbaNS->head->street;
    eastMost->next = NULL;

    StreetNode * mostNext = cbaNS->head->next; //For alphabetizing purposes(if there are more with the same east)

    while(cbaP1 != NULL){ //eastMost becomes the east most street
        if(eastMost->street->start.east < cbaP1->street->start.east){
            eastMost->street = cbaP1->street;
            eastMost->next = NULL;

            mostNext = cbaP1->next; //update mostNext
        }
        cbaP1 = cbaP1->next;
    }

    cbaP1 = cbaNS->head; //reset cbaP2 pointer to find eastLeast

    StreetNode * eastLeast = malloc(sizeof(StreetNode)); //Represents the least east street in cbsNS
    eastLeast->street = cbaNS->head->street;
    eastLeast->next = NULL;

    StreetNode * leastNext = cbaNS->head->next; //For alphabetizing purposes(if there are more with the same east)

    while(cbaP1 != NULL){ //eastLeast becomes the lease east street
        if(eastLeast->street->start.east > cbaP1->street->start.east){
            eastLeast->street = cbaP1->street;
            eastLeast->next = NULL;

            leastNext = cbaP1->next; //Update leastNext
        }
        cbaP1 = cbaP1->next;
    }

    nsStreets->head = eastMost; //Assign the eastMost node as the first node in nsStreets
    nsStreets->tail = eastMost;

    while(mostNext != NULL){ //Check for more streets with the same east
        StreetNode * tempNext = malloc(sizeof(StreetNode)); //Node I can assign to nsStreets
        tempNext->street = mostNext->street;
        tempNext->next = NULL;

        if(tempNext->street->start.east == eastMost->street->start.east){
            tempNext->next = nsStreets->head;
            nsStreets->head = tempNext;
        }

        mostNext = mostNext->next;
    }

    if(eastMost->street->start.east == eastLeast->street->start.east) return nsStreets; //If there was only one value of east for all [NS] streets

    StreetNode * eastMid = malloc(sizeof(StreetNode)); //Node that I can assign the [NS] with the "next greatest" east value
    eastMid->street = eastLeast->street; //Initialize the street to the least eastern street
    eastMid->next = NULL;

    StreetNode * midNext = eastLeast->next; //For alphabetizing purposes(if there are more with the same east)

    cbaP1 = cbaNS->head; //reset pointer to beginning of cbaNS

    while(cbaP1 != NULL){ //Assigns eastMid to the [NS] street with the second-greatest east value
        if(cbaP1->street->start.east > eastMid->street->start.east){ //Greater than lowest/last
            if(cbaP1->street->start.east < eastMost->street->start.east){ //Lesser than the highest
                eastMid->street = cbaP1->street;
                eastMid->next = NULL;

                midNext = cbaP1->next; //Update midNext
            }
        }

        cbaP1 = cbaP1->next;
    }

    while(eastMid->street->start.east != eastLeast->street->start.east){ //while the next most eastern street is not the least eastern street
        StreetNode * tempMid = malloc(sizeof(StreetNode)); //Node that I can assign to nsStreets
        tempMid->street = eastMid->street;
        tempMid->next = NULL;

        tempMid->next = nsStreets->head; //Pushing tempMid onto ncStreets
        nsStreets->head = tempMid;

        while(midNext != NULL){ //Check for more streets with the same east
            StreetNode * tempNext = malloc(sizeof(StreetNode)); //Node I can assign to nsStreets
            tempNext->street = midNext->street;
            tempNext->next = NULL;

            if(tempNext->street->start.east == eastMid->street->start.east){
                tempNext->next = nsStreets->head;
                nsStreets->head = tempNext;
            }

            midNext = midNext->next; //Update midNext
        }

        cbaP1 = cbaNS->head; //reset pointer to beginning of cbaNS

        StreetNode * eastMid2 = malloc(sizeof(StreetNode)); //The [NS] street with the next largest east value
        eastMid2->street = eastLeast->street;
        eastMid2->next = NULL;

        while(cbaP1 != NULL){ //Set eastMid2 to the [NS] street with the next largest east value
            if(cbaP1->street->start.east > eastMid2->street->start.east){ //Greater than lowest or last [NS] east
                if(cbaP1->street->start.east < eastMid->street->start.east){ //Lesser than last eastMid
                    eastMid2->street = cbaP1->street;
                    eastMid2->next = NULL;

                    midNext = cbaP1->next; //Update midNext
                }
            }

            cbaP1 = cbaP1->next; //Update cbaP1
        }

        eastMid->next = eastMid2->next; //Update eastMid
        eastMid->street = eastMid2->street;
    }

    eastLeast->next = nsStreets->head; //Push the eastLeast node onto nsStreets
    nsStreets->head = eastLeast;

    while(leastNext != NULL){ //Check for more streets with the same east
        StreetNode * tempNext = malloc(sizeof(StreetNode)); //Node I can assign to nsStreets
        tempNext->street = leastNext->street;
        tempNext->next = NULL;

        if(tempNext->street->start.east == eastLeast->street->start.east){
            tempNext->next = nsStreets->head;
            nsStreets->head = tempNext;
        }

        leastNext = leastNext->next;
    }

    return nsStreets;
}

/* return a list of all east-west streets */
/* most be sorted from north to south, then by name */
/* must create a new list -- may be deallocated by caller */
StreetList * mapWEStreets(Map m) {
    if(m->streets->head == NULL) return m->streets; //if map is empty

    StreetList * abcStreets = mapAllStreets(m); //alphabetically ordered streets

    StreetList * cbaWE = malloc(sizeof(StreetList)); //Reverse alphabetical order of the [WE] streets
    cbaWE->head = NULL;
    cbaWE->tail = NULL;

    StreetNode * abcP = abcStreets->head; //For stepping through abcStreets

    while(abcP != NULL){ //Step through abcStreets
        StreetNode * tempNode = malloc(sizeof(StreetNode));
        tempNode->street = abcP->street;
        tempNode->next = NULL;

        if(streetDirection(tempNode->street) == 0 ){ //Put street on cbaNS when it's West-East
            if(cbaWE->head == NULL){ //If this is the first street to go on
                tempNode->next = NULL;
                cbaWE->head = tempNode;
                cbaWE->tail = tempNode;
            }
            else{ //There is already at least one street on cbaWE
                tempNode->next = cbaWE->head;
                cbaWE->head = tempNode;
            }
        }

        abcP = abcP->next;
    }

    if(cbaWE->head == NULL) return cbaWE; //if there are no [WE] streets

    StreetList * weStreets = malloc(sizeof(StreetList)); //Final West to East list
    weStreets->head = NULL;
    weStreets->tail = NULL;

    StreetNode * cbaP1 = cbaWE->head; //For stepping through cbaWE

    StreetNode * northLeast = malloc(sizeof(StreetNode)); //Represents the least northern street in cbaWE
    northLeast->street = cbaWE->head->street;
    northLeast->next = NULL;

    StreetNode * nextLeast = cbaWE->head->next; //For alphabetizing purposes(if there are more with the same north)

    while(cbaP1 != NULL){ //northLeast becomes the least north street
        if(northLeast->street->start.north > cbaP1->street->start.north){
            northLeast->street = cbaP1->street;
            northLeast->next = NULL;

            nextLeast = cbaP1->next; //update nextLeast
        }
        cbaP1 = cbaP1->next;
    }

    cbaP1 = cbaWE->head; //reset cbaP2 pointer to find northMost

    StreetNode * northMost = malloc(sizeof(StreetNode)); //Represents the most northern street in cbaWE
    northMost->street = cbaWE->head->street;
    northMost->next = NULL;

    StreetNode * nextMost = cbaWE->head->next; //For alphabetizing purposes(if there are more with the same east)

    while(cbaP1 != NULL){ //northMost becomes the most northern street
        if(northMost->street->start.north < cbaP1->street->start.north){
            northMost->street = cbaP1->street;
            northMost->next = NULL;

            nextMost = cbaP1->next; //Update mostNext
        }
        cbaP1 = cbaP1->next;
    }

    weStreets->head = northLeast; //Assign the eastMost node as the first node in nsStreets
    weStreets->tail = northLeast;

    while(nextLeast != NULL){ //Check for more streets with the same north
        StreetNode * tempNext = malloc(sizeof(StreetNode)); //Node I can assign to weStreets
        tempNext->street = nextLeast->street;
        tempNext->next = NULL;

        if(tempNext->street->start.north == northLeast->street->start.north){
            tempNext->next = weStreets->head;
            weStreets->head = tempNext;
        }

        nextLeast = nextLeast->next;
    }

    if(northLeast->street->start.east == northMost->street->start.east) return weStreets; //If there was only one value of north for all [WE] streets

    StreetNode * northMid = malloc(sizeof(StreetNode)); //Node that I can assign the [WE] streets with the "next lowest" north value
    northMid->street = northMost->street; //Initialize the street to the most northern street
    northMid->next = NULL;

    StreetNode * midNext = northMost->next; //For alphabetizing purposes(if there are more with the same north)

    cbaP1 = cbaWE->head; //reset pointer to beginning of cbaWE

    while(cbaP1 != NULL){ //Assigns northMid to the [WE] street with the second-lowest north value
        if(cbaP1->street->start.north < northMid->street->start.north){ //Lower than greatest/last
            if(cbaP1->street->start.north > northLeast->street->start.north){ //Greater than the lowest
                northMid->street = cbaP1->street;
                northMid->next = NULL;

                midNext = cbaP1->next; //Update midNext
            }
        }

        cbaP1 = cbaP1->next;
    }

    while(northMid->street->start.north != northMost->street->start.north){ //while the next least northern street is not the most northern street
        StreetNode * tempMid = malloc(sizeof(StreetNode)); //Node that I can assign to weStreets
        tempMid->street = northMid->street;
        tempMid->next = NULL;

        tempMid->next = weStreets->head; //Pushing tempMid onto weStreets
        weStreets->head = tempMid;

        while(midNext != NULL){ //Check for more streets with the same east
            StreetNode * tempNext = malloc(sizeof(StreetNode)); //Node I can assign to weStreets
            tempNext->street = midNext->street;
            tempNext->next = NULL;

            if(tempNext->street->start.north == northMid->street->start.north){ //Push tempNext onto weStreets if it has the same north
                tempNext->next = weStreets->head;
                weStreets->head = tempNext;
            }

            midNext = midNext->next; //Update midNext
        }

        cbaP1 = cbaWE->head; //reset pointer to beginning of cbaWE

        StreetNode * northMid2 = malloc(sizeof(StreetNode)); //The [WE] street with the next smallest north value
        northMid2->street = northMost->street;
        northMid2->next = NULL;

        while(cbaP1 != NULL){ //Set northMid2 to the [WE] street with the next smallest north value
            if(cbaP1->street->start.north < northMid2->street->start.north){ //Lesser than highest or last [WE] north
                if(cbaP1->street->start.north > northMid->street->start.north){ //Greater than last northMid
                    northMid2->street = cbaP1->street;
                    northMid2->next = NULL;

                    midNext = cbaP1->next; //Update midNext
                }
            }

            cbaP1 = cbaP1->next; //Update cbaP1
        }

        northMid->next = northMid2->next; //Update eastMid
        northMid->street = northMid2->street;
    }

    northMost->next = weStreets->head; //Push the northMost node onto weStreets
    weStreets->head = northMost;

    while(nextMost != NULL){ //Check for more streets with the same north
        StreetNode * tempNext = malloc(sizeof(StreetNode)); //Node I can assign to weStreets
        tempNext->street = nextMost->street;
        tempNext->next = NULL;

        if(tempNext->street->start.north == northMost->street->start.north){
            tempNext->next = weStreets->head;
            weStreets->head = tempNext;
        }

        nextMost = nextMost->next;
    }

    return weStreets;
}

/* find street by name */
/* does not create a new street, just returns a pointer */
/* returns NULL if street not in map */
Street * mapFindStreet(Map m, const char *name) {
    StreetNode * nodeP = m->streets->head; //Pointer for navigation through street nodes

    while(nodeP != NULL){
        if(abcName(nodeP->street->name, name) == 0){
            return nodeP->street;
        }
        nodeP = nodeP->next;
    }

    return NULL;
}

/* return a list of streets that intersect with given street */
/* sorted by west-to-east or north-to-south, depending on direction of street */
/* must create a new list -- may be deallocated by caller */
StreetList * mapCrossStreets(Map m, const Street *s) {
    StreetList * intersects = malloc(sizeof(StreetList));
    intersects->head = NULL;
    intersects->tail = NULL;

    if(streetDirection(s) == 1){ //Street is North-South
        StreetList * weStreets = mapWEStreets(m); //west to east streets in north to south order

        if(weStreets->head == NULL) return intersects; //If there are no west to east streets

        StreetList * reverseWE = malloc(sizeof(StreetList)); //west to east streets in south to north order
        reverseWE->head = NULL;
        reverseWE->tail = NULL;

        StreetNode * weP = weStreets->head; //Pointer to navigate through weStreets

        while(weP != NULL){ //flip weStreets and put them in reverseWE
            StreetNode * tempNode = malloc(sizeof(StreetNode));
            tempNode->street = weP->street;
            tempNode->next = NULL;

            if(reverseWE->head == NULL){ //First tempNode allocation
                tempNode->next = NULL;
                reverseWE->head = tempNode;
                reverseWE->tail = tempNode;
            }
            else{ //Already tempNode on reverseWE
                tempNode->next = reverseWE->head;
                reverseWE->head = tempNode;
            }

            weP = weP->next;
        }

        StreetNode * reverseP = reverseWE->head; //Pointer to navigate through reverseWE

        while(reverseP != NULL){ //Put streets that intersect into "intersects"
            StreetNode * tempNode = malloc(sizeof(StreetNode));
            tempNode->street = reverseP->street;
            tempNode->next = NULL;

            if(reverseP->street->start.north <= s->start.north){ //If same north
                if(reverseP->street->start.north >= s->end.north){
                    if(reverseP->street->start.east <= s->start.east){ //If intersects
                        if(reverseP->street->end.east >= s->start.east){
                            tempNode->next = intersects->head;
                            intersects->head = tempNode;

                            if(intersects->tail == NULL){ //If this is the first street to be added
                                intersects->tail = tempNode;
                            }
                        }
                    }
                }
            }

            reverseP = reverseP->next;
        }

    }

    if(streetDirection(s) == 0){ //Street is West-East
        StreetList * nsStreets = mapNSStreets(m); //north to south streets in west to east order

        if(nsStreets->head == NULL) return intersects; //If there are no north to south streets

        StreetList * reverseNS = malloc(sizeof(StreetList)); //north to south streets in east to west order
        reverseNS->head = NULL;
        reverseNS->tail = NULL;

        StreetNode * nsP = nsStreets->head; //Pointer to navigate through nsStreets

        while(nsP != NULL){ //flip nsStreets and put them in reverseNS
            StreetNode * tempNode = malloc(sizeof(StreetNode));
            tempNode->street = nsP->street;
            tempNode->next = NULL;

            if(reverseNS->head == NULL){ //First tempNode allocation
                tempNode->next = NULL;
                reverseNS->head = tempNode;
                reverseNS->tail = tempNode;
            }
            else{ //Already tempNode on reverseNS
                tempNode->next = reverseNS->head;
                reverseNS->head = tempNode;
            }

            nsP = nsP->next;
        }

        StreetNode * reverseP = reverseNS->head; //Pointer to navigate through reverseNS

        while(reverseP != NULL){ //Put streets that intersect into "intersects"
            StreetNode * tempNode = malloc(sizeof(StreetNode));
            tempNode->street = reverseP->street;
            tempNode->next = NULL;

            if(reverseP->street->start.east >= s->start.east){ //If same east
                if(reverseP->street->start.east <= s->end.east){
                    if(reverseP->street->start.north >= s->start.north){ //If intersects
                        if(reverseP->street->end.north <= s->start.north){
                            tempNode->next = intersects->head;
                            intersects->head = tempNode;

                            if(intersects->tail == NULL){ //If this is the first street to be added
                                intersects->tail = tempNode;
                            }
                        }
                    }
                }
            }

            reverseP = reverseP->next;
        }
    }

    return intersects;
}

int abcName(const char * a, const char * b){
    int val = 0;
    int i = 0;

    while((a[i] != '\0') && (b[i] != '\0')){
        if(a[i] < b[i]){
            val = 1;
            return val;
        } //a comes alphabetically first

        if(a[i] > b[i]){
            val = -1;
            return val;
        } //b comes alphabetically first

        i +=1;
    }//Steps through both strings until end of one of them, or they differ

    if(a[i] == b[i]) return val; //same name(both a[i] & b[i] are '\0'), returns 0

    if(a[i] == '\0') val = 1; //'a' ended first and thus is alphabetically first
    if(b[i] == '\0') val = -1;//'b' ended first and thus is alphabetically first

    return val;
}