#include <assert.h>
#include <stdio.h>
#include "Elevator_system.h"
/////////////////////////////////////////////////////////////////////
void status(Elevator *elev)
{
	assert(elev);
	uint8_t i=0;
	while(elev->Elev_ID[i]!=0)
		{
			printf("\n\nElevator ID: %d\n Elevator is on %d level\n Elevator target is %d level",elev->Elev_ID[i],elev->present_lvl[i],elev->target_lvl[i]);
			i++;
		}
}
/////////////////////////////////////////////////////////////////////
void init(Elevator *elev)
{
	elev->number_of_elev=0;//initialization 
	elev->request_num =0;
	elev->elev_request_num=0;
}
/////////////////////////////////////////////////////////////////////
uint8_t find_idx(uint16_t *tab, uint8_t n, uint16_t name)
{
	//finding location of elevator index in tables
	//each elevator has its own index in every table used in program!
	//for ex: 
	//ID: 1002 has its index = 0 in each table,
	//ID: 1234 has its index = 4 (...),
	//ID: 1342 has its index = 2 (...)
  uint8_t c, index = 0;
  for (c = 0; c < n; c++)
    {
	 	if (tab[c] == name)
    	{
        break;//if you match any ID get out 
    	}
  	}
	index = c;
	//if you wont match any index, the loop will set index value to the last one, 
	//to create set new ID in table
   return index;
}
/////////////////////////////////////////////////////////////////////
bool update(Elevator *elev,uint16_t name, uint8_t present, uint8_t target)
{
	assert(elev);
	assert(name);
	assert(present);
	assert(target);

if(elev && (name!=0) &&( present!=0 )&&( target!=0 ))
{
if(elev->number_of_elev == 0)
{
	elev->Elev_ID[elev->number_of_elev] = name;
	elev->present_lvl[elev->number_of_elev] = present;
	elev->target_lvl[elev->number_of_elev] = target;
	++elev->number_of_elev;
	//first elevator making
	return true;
}
else
{

uint8_t new_index=0;

//matching any existing ID or creating new one
new_index = find_idx(elev->Elev_ID,elev->number_of_elev,name);
elev->Elev_ID[new_index] = name;
elev->present_lvl[new_index] = present;
elev->target_lvl[new_index] = target;
	
	if(new_index>elev->number_of_elev-1)
	{
		++elev->number_of_elev;
	}	
return true;

}
}
else return false;

}
/////////////////////////////////////////////////////////////////////
bool pickup(Elevator *elev,uint16_t name,uint8_t request_floor)
{
assert(elev);
if(elev && (name!=0) &&( request_floor!=0 ))
{
uint8_t i=0;

while(name!=elev->Elev_ID[i])
{	
	i++;
	if(i>elev->number_of_elev)
	{	
		printf("\nID: %d doesn't exist in base",name);
		break;
		return false;
	}
}


elev->request_stack[i][elev->request_num] = request_floor;

++elev->request_num;
return true;
}
else return false;

}
/////////////////////////////////////////////////////////////////////
void show_pickup_request_queue(Elevator *elev)
{
assert(elev);
	printf("\n\nRequested floors queue:\n");

for(uint8_t j=0;j<elev->number_of_elev;j++)
{//jedziemy po 3 windach:
	for (uint8_t i=0;i<(elev->request_num);i++)
	{
		if(elev->request_stack[j][i]!=0)
		{
		//important thing - as all values in tables are initialized with zero value at the beggining
		//and we dont know if we will be using all of them or just a few,
		//we cant use '0' value to assign the location of the elevator  
		printf("\nElevator %d is  'pickup' wanted on %d floor",elev->Elev_ID[j],elev->request_stack[j][i]);
		}
	}
}
}
/////////////////////////////////////////////////////////////////////
void show_elevator_request_queue(Elevator *elev,uint16_t name)
{
assert(elev);
	printf("\n\nRequested floors inside elevator:\n");

	uint8_t j = find_idx(elev->Elev_ID,elev->number_of_elev,name);

	for (uint8_t i=0;i<(elev->elev_request_num);i++)
	{
		if(elev->in_elev_request_stack[j][i]!=0)
		{
		//WAŻNA UWAGA = POMIJAMY PIĘTRO ZERO - JEST ONO OPISANE JAKO 1
		printf("\nElevator %d is strongly  wanted (by someone in) to go to %d floor\n",elev->Elev_ID[j],elev->in_elev_request_stack[j][i]);
		}
	}

}
/////////////////////////////////////////////////////////////////////
bool in_elevator(Elevator *elev,uint16_t name, uint8_t target)
{

//jezeli winda zajechała na piętro zlecenia!

	
uint8_t i=0; //ta zmienna jest dopasowana jak zaleznie jest ID!!!
i = find_idx(elev->Elev_ID,elev->number_of_elev,name);
if(elev->present_lvl[i]==elev->target_lvl[i])
{
	if(target!=0)//target =0 gdy winda stoi w miejscu ??
	{
		elev->in_elev_request_stack[i][elev->elev_request_num]=target;
		++elev->elev_request_num;
//pickup(elev,elev->Elev_ID[i],target);
	}
}

}
/////////////////////////////////////////////////////////////////////
bool someone_in(Elevator *elev,uint16_t name,bool isempty)
{
	uint8_t i=find_idx(elev->Elev_ID,elev->number_of_elev,name);
	if(isempty == true)
	{
	elev->someone_inside[i]= true;
	}
	else 
	//that's for situation if we want to underline that there's no one in elev
	//so we could use pickup function!!
	elev->someone_inside[i]= false;

}
/////////////////////////////////////////////////////////////////////
bool is_someone_in(Elevator *elev,uint16_t name)
{
uint8_t i=find_idx(elev->Elev_ID,elev->number_of_elev,name);

return elev->someone_inside[i];
}
/////////////////////////////////////////////////////////////////////


Elevator winda;
/////////////////////////////////////////////////////////////////////
void step(uint8_t counter)
{
//simulation function
//every time this function is called another

switch(counter)
{
case 0: init(&winda);
		update(&winda,1001,1,1);
break;

case 1: 
{		
//tasks from request_stack - 
uint8_t new_index=0;
new_index = find_idx(winda.Elev_ID,winda.number_of_elev,1001);
//here we test zero-number request
if(winda.present_lvl[new_index]<winda.request_stack[new_index][0])
{
	winda.up_going[new_index] = true;

	for(int i=winda.present_lvl[new_index];i<=winda.request_stack[new_index][0];i++)
	{
		update(&winda,1001,i,winda.request_stack[new_index][0]);
		printf("\nup\n");
	}
	winda.up_going[new_index] = false;
}
if(winda.present_lvl[new_index]>winda.request_stack[new_index][0])
{
	winda.down_going[new_index] = true;

	for(int i=winda.present_lvl[new_index];i<=winda.request_stack[new_index][0];i--)
	{
		update(&winda,1001,i,winda.request_stack[new_index][0]);
		printf("\ndown\n");
	}
	winda.down_going[new_index] = false;
}
}
break;

case 2: 
{
		pickup(&winda,1001,6);
		//pickup(&winda,1001,3);

}

break;

case 3: someone_in(&winda, 1001,true);
break;

case 4: someone_in(&winda, 1001,false);
break;

case 5: 
if(is_someone_in(&winda, 1001))
{
	in_elevator(&winda,1001,12);
	//in_elevator(&winda,1001,2);



}
else
{
//tasks from elev_request_stack - 
uint8_t new_index = find_idx(winda.Elev_ID,winda.number_of_elev,1001);
////here we test zero-number request
if(winda.present_lvl[new_index]<winda.in_elev_request_stack[new_index][0])
{
	winda.up_going[new_index] = true;

	for(int i=winda.present_lvl[new_index];i<=winda.in_elev_request_stack[new_index][0];i++)
	{
		update(&winda,1001,i,winda.in_elev_request_stack[new_index][0]);
		printf("\nup\n");
	}
	winda.up_going[new_index] = false;
}
if(winda.present_lvl[new_index]>winda.in_elev_request_stack[new_index][0])
{
	winda.down_going[new_index] = true;

	for(int i=winda.present_lvl[new_index];i<=winda.in_elev_request_stack[new_index][0];i--)
	{
		update(&winda,1001,i,winda.in_elev_request_stack[new_index][0]);
		printf("\ndown\n");
	}
	winda.down_going[new_index] = false;
}





}
break;

case 6: status(&winda);
break;

case 7: show_pickup_request_queue(&winda);
break;

case 8: show_elevator_request_queue(&winda,1001);
break;

case 9: printf("\nNumber of Elevators in Buliding = %d",winda.number_of_elev);
break;

case 10: update(&winda,1004,1,1);
break;

case 11:
break;
}

}
/////////////////////////////////////////////////////////////////////
int main()
{

step(0); //elevator init on floor 1

printf("done0");

step(3); //someone walks in -> on 1st floor

printf("done3");

step(2); //someone requests elevator on 6th floor

printf("done2");

step(7);

step(6); //show elevator status



step(5); //check if someone in , if theres add his request to stack (12th floor)
		 //else realise pickup requests -> that request from step(2)!
printf("\ndone5\n");

step(1);//realise task from in_elevator_request_stack ->passenger request! 

step(6);//show elevator status


step(9);//show number of elevators in buliding

step(4);//someone gets out of the elevator

show_elevator_request_queue(&winda,1001);
//show pickup requests

step(5); //do task from request_stack

step(6); //show elevator status





}

/* 
//////////////////////////////////////////////////////////////////
program routine:
0)Elevator is on X floor, its target is also X floor (the same).
(up_going = false and down_going = false)

1) using 'is_someone_in()' we check if there is someone inside the elevator
(for example using wegiht/tension meter and then we call function someone_in())

	-[no one inside]:
Elevator can realise request from request_stack which is set by pickup() function
by people on different floors - set up_going variable to true
to make window go up or set down_going variable to true to get down.
(both can be false,when target lvl = present lvl, but never both true).

	-[someone inside]: 
Person in elevator add requests to in_elev_request_stack, and system realise requests setting up_going/down_going
variable. 


2) update() function is called every floor is passed during motion 
of elevator (up_going/down_going)
- when elevator reaches destination target and present lvl are equal,
also variables up/down_going are both set to false.

-if someone get in the elevator call someone_in() function.
get into the point (1)


///////////////////////////////////////////////////////////////////
*/