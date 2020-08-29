#pragma once
//Chat List interactions
struct chatList* appendChatList(struct chatList** currentChatList, char* custName, char* repName); //parse the Current ChatReuest and Add a rep to the chatList
void deleteFromChatList(struct chatList** currentChatList, char* custName); //parse the ChatEnded request and add rep the the repList and check if customers are still holding
/// <summary>
/// 
struct chatList{ //list of current Customers interacting with reps and time
char custName[128];
char repName[128];
struct chatList* next;

};

struct chatList* appendChatList(struct chatList** currentChatList, char* custName, char* repName) {
	struct chatList* newNode = (struct repList*)malloc(sizeof(struct repList));         //Create Node of List that will be added                                  
	struct chatList* lookupNode = currentChatList;                                          //set the working list head into a pointer for finding list
	strcpy(newNode->custName, custName);                                                 //Append Current Rep into List
	strcpy(newNode->repName, repName);                                                 //Append Current Rep into List
	newNode->next = NULL;                                                              //Set the new node to be the end of list 
	if (lookupNode == NULL) {                                                          //If list is already empty add new node directly to the front of the list 
		currentChatList = newNode;                                                         //
		return currentChatList;                                                            //
	}                                                                                  //
	while (lookupNode->next != NULL) {                                                 //Check through the list until the end of the list 
		lookupNode = lookupNode->next;
	}                                                                                  //
	lookupNode->next = newNode;                                                        //Append new node and data to end of list
	return currentChatList;
}

void deleteFromChatList(struct chatList** currentChatList, char* custName) {
    struct chatList* toDelete = currentChatList;
    struct chatList* previousNode = NULL;
    while ((strcmp(toDelete->custName, custName) != 0)) {
        if (toDelete->next != NULL) {
            previousNode = toDelete;
            toDelete = toDelete->next;
        }
        else
            return;
    }
    if (previousNode == NULL) {
        toDelete = toDelete->next;
        return;
    }
    previousNode->next = toDelete->next;
    toDelete->next = NULL;
    return;
}