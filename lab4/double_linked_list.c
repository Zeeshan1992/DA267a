#include<stdlib.h>
#include<limits.h>
#include"double_linked_list.h"




int addElement(struct linkedList* list, int value){
	struct linkedListElement* newElement = malloc(sizeof(struct linkedListElement)); //creating a new element for list
	newElement->data = value;
	newElement->next = NULL;  //default next to null
	newElement->previous = NULL; //default prev to null
	if (list->first == NULL) // if list is empty
	{
		list->first = newElement;  //set the new element as list.first
		return list->first->data;
	} 
	if (list->first->data >= value) //sorting out list, smallest value to be list.first
	{
		list->first->previous = newElement; // set the current first.previous to point at the new element
		newElement->next = list->first; // the new elements next pointer should point to the current first
		list->first = newElement; //now that the new links have been set lets put this new element as the first
		return list->first->data;
	}
	else
	{
		struct linkedListElement* temp = list->first;
		while(temp->next != NULL && temp->next->data <= value){ //iterates the list
			temp = temp->next;
		}
		newElement->previous = temp; // set new elements prev to temp
		newElement->next = temp->next; // point new element next to the next element in list null if its at end
		if (newElement->next != NULL) //if newElement->next = null program will crash
		{
			newElement->next->previous = newElement; // change the element which prev pointed to temp to newelement	
		} 
		temp->next = newElement; // setting the next pointer of temp to the recently created element
		return temp->next->data;
	}
  return INT_MIN;
} 


void initLinkedList(struct linkedList* list) {
  list->first = NULL;
  list->last = NULL;
}


int removeFirstElement(struct linkedList* list){
	if (list->first == NULL)
	{
		return INT_MIN;
	} else{
		struct linkedListElement* temp = list->first;
		list->first = list->first->next;
		list->first->previous = NULL;
		free(temp);
	}
	
  return INT_MIN;
} 


int removeLastElement(struct linkedList* list) {
	struct linkedListElement* ptr;
	struct linkedListElement* temp;
	ptr = list->first;
	while (ptr->next->next != NULL)
	{
		ptr->previous = ptr;
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






