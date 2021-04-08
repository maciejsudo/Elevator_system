#ifndef _ELEVATOR_SYSTEM_
#define _ELEVATOR_SYSTEM_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Structure describing the Elevators object: */
typedef struct {
    
	uint16_t Elev_ID[16];// here we collect elevator identification number
	uint8_t present_lvl[16];// the level elevator is currently on - updated while moving through sensors on 
	uint8_t target_lvl[16];// the target level elevator is going to
	uint8_t number_of_elev;//the number of created elevators 
    
    uint8_t request_stack[16][100];//outside elevator pickup() requests
    uint8_t request_num;//number of those above

    uint8_t in_elev_request_stack[16][100];//inside - Elevator requests
    uint8_t elev_request_num;//number of those above

    bool someone_inside[16];//is there someone inside?
        
    bool up_going[16];  //if elev goes up/down set this true, if not set both to false(at each elevator)
    bool down_going[16];

    //bool doors_open[16];//doors are only open when person walks in, 
    //and when target == present! and up_down_going[X][X] both are false
    //after in_elevator() function is set for the last time, 
    //doors are closed ??


    //if elev goes down set this true(at each elevator)

} Elevator;//as many as we want


void status(Elevator *elev);
//bool update(Elevator1 *elev,uint8_t name, uint8_t now_lvl, uint8_t aim_lvl);
void init(Elevator *elev);

uint8_t find_idx(uint16_t *tab, uint8_t n, uint16_t name);

bool update(Elevator *elev,uint16_t name, uint8_t present, uint8_t target);

bool pickup(Elevator *elev,uint16_t name,uint8_t request_floor);

void show_pickup_request_queue(Elevator *elev);

void show_elevator_request_queue(Elevator *elev,uint16_t name);

bool in_elevator(Elevator *elev,uint16_t name, uint8_t target);

bool someone_in(Elevator *elev,uint16_t name,bool isempty);

bool is_someone_in(Elevator *elev,uint16_t name);

#endif //_ELEVATOR_SYSTEM_
