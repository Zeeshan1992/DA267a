#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include"single_linked_list.h"



int addElement(struct linkedList* list, int value){
	struct linkedListElement* temp1 = malloc(sizeof(struct linkedListElement)); // create a new element, allocating memory
	temp1->data = value; // set value of elements
	temp1->next = NULL; // set next pointer to null
	if (list->first == NULL) //if the list is empty
	{
		list->first = temp1; //setting the first element to the recently created element
		return list->first->data;
	} if (list->first->data >= value) // if the value is equal to or lower than the data from first element
	{
		temp1->next = list->first;
		list->first = temp1;
	}
	else{
		struct linkedListElement* temp2 = list->first;
		while(temp2->next!= NULL && temp2->next->data <= value){ // iterates the list until it point to the last item or if data is bigger than value
			temp2 = temp2->next;
		}
		temp1->next = temp2->next;
		temp2->next = temp1; // setting the next pointer to the recently created element
		return temp2->next->data;
	}
	return INT_MIN;
}

void initLinkedList(struct linkedList* list) {
	list->first = NULL;
}

int removeFirstElement(struct linkedList* list){
	if (list->first == NULL)
	{
		return INT_MIN;
	}
	struct linkedListElement* temp = list->first;
	list->first = temp->next; // first now points to second element
	free(temp);
	return INT_MIN;
}


int removeLastElement(struct linkedList* list){
		struct linkedListElement* ptr = list->first; 
		struct linkedListElement* temp;
		while (ptr->next->next!=NULL)   // iterates the list until it point to the second to last item
		{
			ptr = ptr->next;
		}
		temp = ptr->next;
		ptr->next = NULL;
		free(temp);
		return INT_MIN;
} 

void deleteList(struct linkedList* list){
	struct linkedListElement* ptr = list->first;
	struct linkedListElement* temp;
	while(ptr != NULL){
		temp = ptr->next;
		free(ptr);
		ptr = temp;
	}
	initLinkedList(list);
}
