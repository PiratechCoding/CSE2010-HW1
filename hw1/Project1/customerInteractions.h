#pragma once
//Customer List interatcion
struct custList* appendCustList(struct custList** currentHoldList, char* custName, char* timeStamp); // will parse the PutOnHold request when no reps are ready
struct custList* deleteFromCustList(struct custList** currentHoldList, char* custName, char* instructionTimeStamp, int waitTime); //will handle reps exiting chats and assigning holding customers with them return amount of time waiting
int isCustomerHolding(struct custList* currentHoldList);
int convertToMinutes(char* timeStamp);
/// <summary>
/// 
struct custList { //List of current customers in queue and time started to wait
	char custName[128];
	char timeStamp[5];
	struct custList* next;
};

int convertToMinutes(char* timeStamp) {
	int currentTime = atoi(timeStamp);
	int timeInMinutes = 0;
	timeInMinutes = currentTime % 100;
	currentTime /= 100;
	currentTime *= 60;
	timeInMinutes += currentTime;
	return timeInMinutes;
}

struct custList* appendCustList(struct custList** currentHoldList, char* custName, char* timeStamp){
	struct custList* newNode = malloc(sizeof(struct custList));         //Create Node of List that will be added                                  
	struct custList* lookupNode = currentHoldList;                                          //set the working list head into a pointer for finding list
	strcpy(newNode->custName, custName);                                                 //Append Current Rep into List
	strcpy(newNode->timeStamp, timeStamp);                                                 //Append Current Rep into List
	newNode->next = NULL;                                                              //Set the new node to be the end of list 
	if (lookupNode == NULL) {                                                          //If list is already empty add new node directly to the front of the list 
		currentHoldList = newNode;                                                         //
		return currentHoldList;                                                            //
	}                                                                                  //
	while (lookupNode->next != NULL) {                                                 //Check through the list until the end of the list 
		lookupNode = lookupNode->next;
	}                                                                                  //
	lookupNode->next = newNode;                                                        //Append new node and data to end of list
	return currentHoldList;
}

struct custList* deleteFromCustList(struct custList** currentHoldList, char* custName,char*instructionTimeStamp, int* waitTime, int isQuitting) {
	struct custList* toDelete = *currentHoldList;
	struct custList* previousNode = NULL;
	int timeInMinutes = 0;
	while ((strcmp(toDelete->custName , custName) != 0)) {
		if (toDelete->next != NULL) {
			previousNode = toDelete;
			toDelete = toDelete->next;
		}
		else
			return currentHoldList;
	}
	if (previousNode == NULL) {
		timeInMinutes = convertToMinutes(instructionTimeStamp)-convertToMinutes(toDelete->timeStamp);
		if(*waitTime < timeInMinutes && isQuitting == 0)
			*waitTime = timeInMinutes;
		return currentHoldList = toDelete->next;
	}
	previousNode->next = toDelete->next;
	timeInMinutes = convertToMinutes(instructionTimeStamp) - convertToMinutes(toDelete->timeStamp);
	if (*waitTime < timeInMinutes && isQuitting == 0)
		*waitTime = timeInMinutes;
	return currentHoldList = toDelete->next; 
}

int isCustomerHolding(struct custList* currentHoldingList) {
	if (currentHoldingList == NULL)
		return 0;
	else
		return 1;
}