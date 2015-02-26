/**
    CSC209 A2 Spring 2015
    visualization.c
    Purpose: Visualizes the structs used in A2 of CSC209 

    @author Samir Patel
    @version 1.1 25/02/15
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"
extern struct block *allocated_list;
extern struct block *freelist;

void print_size_line(int size){
	int i;
	printf("        -  Size:");
	if(size>99999){
		
		for (i = 0; i < 13; i++){
			if(i == 1){
				printf("%d", size);
			}
			else{
				printf(" ");
			}
		}
	}
	else if(size>9999){
		for (i = 0; i < 14; i++){
			if(i == 1){
				printf("%d", size);
			}
			else{
				printf(" ");
			}
		}
	}
	else if(size>999){
		for (i = 0; i < 15; i++){
			if(i == 1){
				printf("%d", size);
			}
			else{
				printf(" ");
			}
		}
	}	
	else if(size>99){
		for (i = 0; i < 16; i++){
			if(i == 1){
				printf("%d", size);
			}
			else{
				printf(" ");
			}
		}
	}
	else if(size>9){
		for (i = 0; i < 17; i++){
			if(i == 1){
				printf("%d", size);
			}
			else{
				printf(" ");
			}
		}
	}
	else{
		for (i = 0; i < 18; i++){
			if(i == 1){
				printf("%d", size);
			}
			else{
				printf(" ");
			}
		}
	}
	printf("-\n");

}

void print_null_line(){
	printf("        -          NULL           -\n");
}

void print_full_line(){
	printf("        ---------------------------\n");
}

void print_blank_line(){
	printf("        -                         -\n");
}

void print_address_line(struct block *list){
	printf("        - Address: %p -\n", list->addr);
}

void print_name_line(char* name){
	printf("        -        %s         -\n", name);
}

void print_null_box(){
	print_full_line();
	print_blank_line();
	print_full_line();
	print_blank_line();
	print_full_line();
	print_blank_line();
	print_full_line();
	print_null_line();
	print_full_line();	
}

void print_box(struct block *list){
	print_full_line();
	print_blank_line();
	print_full_line();
	print_address_line(list);
	print_full_line();
	print_size_line(list->size);
	print_full_line();
	print_blank_line();
	print_full_line();
}

void print_final_box(struct block *list){
	print_full_line();
	print_blank_line();
	print_full_line();
	print_address_line(list);
	print_full_line();
	print_size_line(list->size);
	print_full_line();
	print_null_line();
	print_full_line();
}

void print_name_box(struct block *list, char* name){

	print_full_line();
	print_name_line(name);
	print_full_line();
	print_address_line(list);
	print_full_line();
	print_size_line(list->size);
	print_full_line();
	print_null_line();
	print_full_line();
	
}

void print_final_name_box(struct block *list, char* name){
	print_full_line();
	print_name_line(name);
	print_full_line();
	print_address_line(list);
	print_full_line();
	print_size_line(list->size);
	print_full_line();
	print_null_line();
	print_full_line();
}

void print_arrow(){
	printf("                     |\n                     |\n                     |\n                     V\n");
}
	

void print_list_display(struct block *list, char* name){
	int i = 0;
	if (list == NULL){
		print_null_box();
	}

	while(list != NULL){
		if (i > 0 && list->next != NULL){
			print_box(list);
		}
		else if(i>0 && list ->next == NULL){
			print_final_box(list);
		}
		else if (list->next != NULL){
			print_name_box(list, name);		
		}
		else{
			print_final_name_box(list, name);
		}
	
		list = list->next;
		i++;
		if (list != NULL){
			print_arrow();
		}
	}
	printf("\n");
}

void print_free_display(){
	print_list_display(freelist, "FreeList");
}

void print_allocated_display(){
	print_list_display(allocated_list, "Alocated");
}

