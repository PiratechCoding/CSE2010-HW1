#pragma once
struct repList* repListInit(struct repList* workingList);   //Fill the rep list with all reps that are avaialbe before start
struct repList* appendRepList(struct repList* workingList, char* repName); //Add a Rep to the end of the list
struct repList* deleteFromRepList(struct repList* workingList, char* nameToDelete); //Take a rep out of the avaiable queue
int isRepAvailable(struct repList* workingList);                              //Return if Rep is not on Call 
void printAvailableRepList(struct repList* workingList);                      //Go through list and print all reps in queue

struct repList {     //Hold the current availble rep for customers, if the rep is not on the list they are not availble
    char *repName[1];
    struct repList* next;
};

int isRepAvailable(struct repList* workingList) {
    if (workingList == NULL)
        return 0;
    else
        return 1;
}
 struct repList* repListInit(struct repList* workingList) {                             //Fill the rep list with all reps that are avaialbe before start
     //Memeory cleanup all fields are 2d arrays
     char* repNamesStart[5][1] = { "Alice", "Bob", "Carol", "David", "Emily" };
     for (int i = 0; i < 5; i++){                                                       //Loop through 5 times to add names to the list
      workingList =  appendRepList(workingList, repNamesStart[i]);                           //add each name to the end of the list
     }
     return workingList;
}

 struct repList* appendRepList(struct repList* workingList, char** repName) {       //Add a Rep to the end of the list
     struct repList* newNode = malloc(sizeof(struct repList));         //Create Node of List that will be added                                  
     struct repList* lookupNode = workingList;                                          //set the working list head into a pointer for finding list
     strcpy(newNode->repName, repName);                                                 //Append Current Rep into List
     newNode->next = NULL;                                                              //Set the new node to be the end of list 
     if (lookupNode == NULL) {                                                          //If list is already empty add new node directly to the front of the list 
         workingList = newNode;                                                         //
         return workingList;                                                            //
     }                                                                                  //
     while (lookupNode->next != NULL) {                                                 //Check through the list until the end of the list 
         lookupNode = lookupNode->next;
     }                                                                                  //
     lookupNode->next = newNode;                                                        //Append new node and data to end of list
     return workingList;
 }

 void printAvailableRepList(struct repList** workingList) {                              //Go through list and print all reps in queue
     struct repList* movingPointer = (*workingList);
     while (movingPointer->next != NULL) {
         printf("%s ", (*movingPointer->repName));
         movingPointer = movingPointer->next;
     }
     printf("%s \n", (*movingPointer->repName));
 }
 struct repList* deleteFromRepList(struct repList** workingList, char* nameToDelete) {        //Take a rep out of the avaiable queue
     struct repList* toDelete = workingList;
     struct repList* previousNode = NULL;
     while ((strcmp((*toDelete->repName), nameToDelete) != 0)) {
         if (toDelete->next != NULL) {
             previousNode = toDelete;
             toDelete = toDelete->next;
         }
         else
             return workingList;
     }
     if (previousNode == NULL) {
         toDelete = toDelete->next;
         return toDelete;
     }
     previousNode->next = toDelete->next;
     free(toDelete);
     return workingList;
 }