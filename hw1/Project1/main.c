/*

  Author: Luke Bucher
  Email: lbucher2017@my.fit.edu
  Course: CSE2010
  Section: 04

  Description of this file:




 */
#define _CRT_SECURE_NO_DEPRECATE     //remove before submittal                                                                                    
#include <stdio.h>                                                                                                                                
#include <stdlib.h>                                                                                                                               
#include <string.h>                                                                                                                                                                                                                                                  //PrintMaxWaitTime 
#include "repInteractions.h"
#include "customerInteractions.h"
#include "chatInteractions.h"
 //General Interactions
int parseInputData(char* inputBuffer, struct repList** currentRepList, struct chatList** currentChatList, struct custList** currentHoldList, int* waitTime);
///

///  
struct instructionInput {
	char instruction[128];
	char custName[128];
	char repName[128];
	char holdType[16];
	char timestamp[5];
};

struct instructionInputInit* instructionInputInit(char* systemInput);

int main(int argc, char* argv[]) {
	int waitTime = 0;
	int inputErrorReporting; //amount of time customers were on hold
	struct repList* headRepList = NULL;
	struct chatList* headChatList = NULL;
	struct custList* headCustList = NULL;
	char fileBuffer[255];
	headRepList = repListInit(headRepList);
	FILE* systemInput = fopen(argv[1], "r");

	fseek(systemInput, 0, SEEK_END);
	unsigned long lengthOfFile = (unsigned long)ftell(systemInput);
	if (lengthOfFile > 0) {  //check if the file is empty or not.
		rewind(systemInput);
		do {
			fgets(fileBuffer, sizeof(fileBuffer), systemInput);
			inputErrorReporting = parseInputData(fileBuffer, &headRepList, &headChatList, &headCustList, &waitTime);
			if (inputErrorReporting == -1) {
				printf("\nString was not handled properly during execution.\n");
				exit(-1);
			}
		}

		while (!feof(systemInput));
	}
	else
		exit(-1);
	return 0;
}


/*//////////////////////////////////////////////////////*/
int parseInputData(char* inputBuffer, struct repList** currentRepList, struct chatList** currentChatList, struct custList** currentHoldList, int* waitTime) {
	struct instructionInput* currentInsturction = instructionInputInit(inputBuffer);
	
	if (strcmp(currentInsturction->instruction, "ChatRequest") == 0) {
		///Take in incomming request and check for available rep, if no rep available check if wants to hold or leave
		///If waiting make sure to track the waiting time
		if (strcmp(currentInsturction->holdType, "wait") == 0) {
			printf("%s %s %s %s\n", currentInsturction->instruction, currentInsturction->timestamp, currentInsturction->custName, currentInsturction->holdType);
			if (isRepAvailable((*currentRepList))) {
				*currentChatList = appendChatList((*currentChatList), currentInsturction->custName, (*(*currentRepList)->repName)); //Start 
				printf("%s %s %s %s\n", "RepAssignment", currentInsturction->custName, (*(*currentRepList)->repName), currentInsturction->timestamp);
				*currentRepList = deleteFromRepList((*currentRepList), (*(*currentRepList)->repName));
			}
			else {
				printf("%s %s %s %s\n", currentInsturction->instruction, currentInsturction->timestamp, currentInsturction->custName, currentInsturction->holdType);
				appendCustList((*currentHoldList), currentInsturction->custName, currentInsturction->timestamp); //Put on Hold
			}
		}
		else { //Will Callback
			printf("%s %s %s %s\n", currentInsturction->instruction, currentInsturction->timestamp, currentInsturction->custName, currentInsturction->holdType);
			if (isRepAvailable((*currentRepList))) {
				*currentChatList = appendChatList((*currentChatList), currentInsturction->custName, (*(*currentRepList)->repName)); //Start 
				printf("%s %s %s %s\n", "RepAssignment", currentInsturction->custName, (*(*currentRepList)->repName), currentInsturction->timestamp);
				*currentRepList = deleteFromRepList((*currentRepList), (*(*currentRepList)->repName));
			}
		}
	}
	else if (strcmp(currentInsturction->instruction, "QuitOnHold") == 0) {
		///Confirm a customer that is on the hold list will no longer be on the hold list, Make sure to add the wait time to the overall wait time
		printf("%s %s %s\n", currentInsturction->instruction, currentInsturction->timestamp, currentInsturction->custName);
		*waitTime += deleteFromCustList((*currentHoldList), currentInsturction->custName, currentInsturction->timestamp);
	}

	else if (strcmp(currentInsturction->instruction, "ChatEnded") == 0) { //End current Chat interaction and check if a customer is waiting if a customer is waiting assign a rep 
		printf("%s %s %s %s \n", currentInsturction->instruction, currentInsturction->custName, currentInsturction->repName, currentInsturction->timestamp);
		deleteFromChatList((*currentChatList), currentInsturction->custName);
		if (isCustomerHolding((*currentHoldList))) {
			printf("RepAssignment %s %s %s\n", currentInsturction->custName, currentInsturction->repName, currentInsturction->timestamp);
			appendChatList((*currentChatList), (*currentHoldList)->custName, currentInsturction->repName);
			*waitTime += deleteFromCustList((*currentHoldList), (*currentHoldList)->custName, currentInsturction->timestamp);
		}
		else {
			appendRepList((*currentRepList), currentInsturction->repName);
		}
	}

	else if (strcmp(currentInsturction->instruction, "PrintAvailableRepList") == 0) {
		///Run through the current rep list and print any reps that are not on a chat currently
		printf("%s %s ", "AvailableRepList", currentInsturction->timestamp);
		printAvailableRepList(currentRepList);
	}
	else if (strcmp(currentInsturction->instruction, "PrintMaxWaitTime") == 0) {
		///Print the current overall wait time
		printf("%s %s %d \n", currentInsturction->instruction, currentInsturction->timestamp, *waitTime);
	}
	else
		return -1; //Handle incorrect or errornous input to ensure stability
	free(currentInsturction);
	return 0;
}

struct instructionInputInit* instructionInputInit(char* systemInput) { 
	struct instructionInput* currentInstruction = malloc(sizeof(struct instructionInput));
	memset(currentInstruction->custName, NULL, sizeof(currentInstruction->custName));
	memset(currentInstruction->holdType, NULL, sizeof(currentInstruction->holdType));
	memset(currentInstruction->instruction, NULL, sizeof(currentInstruction->instruction));
	memset(currentInstruction->repName, NULL, sizeof(currentInstruction->repName));
	memset(currentInstruction->timestamp, NULL, sizeof(currentInstruction->timestamp));

	int currentArrayPoint = 0;
	int localArrayPointer = 0;
	while (systemInput[currentArrayPoint] != ' ') {
		currentInstruction->instruction[currentArrayPoint] = systemInput[currentArrayPoint];
		currentArrayPoint++;
	}
	currentInstruction->instruction[currentArrayPoint] = NULL;
	if (strcmp(currentInstruction->instruction, "ChatRequest") == 0) { //Request Time ->Customer -> WaitType
		currentArrayPoint++;
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->timestamp, currentArrayPoint, localArrayPointer);
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->custName, currentArrayPoint, localArrayPointer);
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->holdType, currentArrayPoint, localArrayPointer);
	}
	else if (strcmp(currentInstruction->instruction, "QuitOnHold") == 0) { //quitOnHoldTime -> customer
		currentArrayPoint++;
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->timestamp, currentArrayPoint, localArrayPointer);
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->custName, currentArrayPoint, localArrayPointer);
	}
	else if (strcmp(currentInstruction->instruction, "ChatEnded") == 0) { //customer->rep->endTime
		currentArrayPoint++;
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->custName, currentArrayPoint, localArrayPointer);
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->repName, currentArrayPoint, localArrayPointer);
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->timestamp, currentArrayPoint, localArrayPointer);
	}
	else if (strcmp(currentInstruction->instruction, "PrintMaxWaitTime") == 0 || strcmp(currentInstruction->instruction, "PrintAvailableRepList") == 0) {
		currentArrayPoint++;
		currentArrayPoint = addFieldsToInstruction(systemInput, currentInstruction->timestamp, currentArrayPoint, localArrayPointer);
	}
	return currentInstruction;
}

int addFieldsToInstruction(char* systemInput, char* field, int systemInputPointer, int localInputPointer) { //add string to proper field 
	while (systemInput[systemInputPointer] != ' ') {
		if (systemInput[systemInputPointer] == '\n')
			break;
		field[localInputPointer] = systemInput[systemInputPointer];
		systemInputPointer++;
		localInputPointer++;
	}
	field[localInputPointer] = NULL;
	systemInputPointer++;
	return systemInputPointer;
}