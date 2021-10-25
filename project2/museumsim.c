#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "museumsim.h"

//
// In all of the definitions below, some code has been provided as an example
// to get you started, but you do not have to use it. You may change anything
// in this file except the function signatures.
//


struct shared_data {
	// Add any relevant synchronization constructs and shared state here.
	// For example:
	//     pthread_mutex_t ticket_mutex;
	//     int tickets;

	pthread_mutex_t visitor_guide_mutex;
	pthread_cond_t guide_inside_may_enter_cond; // indicate guide_inside or guide_may_enter changed
	pthread_cond_t can_inside_cond; // indicate can_inside changes

	int tickets_remain;
	int visitors_waiting; // # of visitors waiting outside of museum
	int can_inside; // # of visitor can go inside
	int guide_inside; // # of guides inside of museum 
	int visitor_leaves; // # of visitors entering and leaving museum
	int guide_may_enter; // boolean flag indicating whether new arriving guide can enter museum
    int visitor_inside;
} shared;

// static struct shared_data shared;

/**
 * Set up the shared variables for your implementation.
 * 
 * `museum_init` will be called before any threads of the simulation
 * are spawned.
 */
void museum_init(int num_guides, int num_visitors)
{
	// pthread_mutex_init(&shared.ticket_mutex, NULL);
	//
	// shared.tickets = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);

    pthread_mutex_init(&shared.visitor_guide_mutex, NULL);
    pthread_cond_init(&shared.guide_inside_may_enter_cond, NULL);
    pthread_cond_init(&shared.can_inside_cond, NULL);
    shared.tickets_remain = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);
    shared.guide_inside = 0;
    shared.can_inside = 0;
    shared.visitors_waiting = 0;
    shared.visitor_inside = 0;
    shared.visitor_leaves = 0;
    shared.guide_may_enter = 1;

}


/**
 * Tear down the shared variables for your implementation.
 * 
 * `museum_destroy` will be called after all threads of the simulation
 * are done executing.
 */
void museum_destroy()
{
	// pthread_mutex_destroy(&shared.ticket_mutex);

    pthread_mutex_destroy(&shared.visitor_guide_mutex);
    pthread_cond_destroy(&shared.guide_inside_may_enter_cond);
    pthread_cond_destroy(&shared.can_inside_cond);
}




/**
 * Implements the visitor arrival, touring, and leaving sequence.
 */
void visitor(int id)
{
	
	visitor_tours(id);
	visitor_leaves(id);

    visitor_arrives(id);

    pthread_mutex_lock(&shared.visitor_guide_mutex);
    {
        if (shared.tickets_remain == 0) // If no tickets remain, leave directly
        {
            visitor_leaves(id);
            pthread_mutex_unlock(&shared.visitor_guide_mutex);
            return;
        }
        // get ticket and wait
        shared.visitors_waiting++;
        shared.tickets_remain--;
    }

	pthread_mutex_unlock(&shared.visitor_guide_mutex);

    pthread_mutex_lock(&shared.visitor_guide_mutex);
    {
        // wait for admitance from guides
        while (shared.visitor_inside == shared.can_inside)
        {
            pthread_cond_wait(&shared.can_inside_cond, &shared.visitor_guide_mutex);
        }
        shared.visitor_inside++;
        visitor_tours(id);
    }

    pthread_mutex_unlock(&shared.visitor_guide_mutex);

    pthread_mutex_lock(&shared.visitor_guide_mutex);
    {
        visitor_leaves(id);
        shared.visitor_leaves++;
    }

    pthread_mutex_unlock(&shared.visitor_guide_mutex);

}

static __thread int served_so_far = 0;  // number of visitors served by each guide

/**
 * Implements the guide arrival, entering, admitting, and leaving sequence.
 */
void guide(int id)
{
	// guide_arrives(id);
	guide_enters(id);
	guide_admits(id);
    guide_leaves(id);
    
	guide_arrives(id);

 pthread_mutex_lock(&shared.visitor_guide_mutex);


        while (shared.guide_inside == GUIDES_ALLOWED_INSIDE || !shared.guide_may_enter)
        {
            pthread_cond_wait(&shared.guide_inside_may_enter_cond, &shared.visitor_guide_mutex);
        }
        guide_enters(id);
        shared.guide_inside++;
        // if number of guides inside reaches GUIDES_ALLOWED_INSIDE, new guide can only enter
        // after this group of GUIDES_ALLOWED_INSIDE guides leave
        if (shared.guide_inside == GUIDES_ALLOWED_INSIDE)
        {
            shared.guide_may_enter = 0;
        }
		pthread_mutex_unlock(&shared.visitor_guide_mutex);
		
 while (1)
    {
        pthread_mutex_lock(&shared.visitor_guide_mutex);
        // this guide already served VISITORS_PER_GUIDE visitors
        if (served_so_far >= VISITORS_PER_GUIDE)
        {
            pthread_mutex_unlock(&shared.visitor_guide_mutex);
            break;
        }
        if (shared.visitors_waiting)
        {
            served_so_far++;
            shared.can_inside++;
            shared.visitors_waiting--;
            guide_admits(id);
            pthread_cond_signal(&shared.can_inside_cond);
            pthread_mutex_unlock(&shared.visitor_guide_mutex);
        }
        else if (!shared.tickets_remain) // visitors_waiting == 0 and tickets_remain == 0
        {
            pthread_mutex_unlock(&shared.visitor_guide_mutex);
            break;
        }
        else // visitors_waiting == 0 and tickets_remain != 0
        {
            pthread_mutex_unlock(&shared.visitor_guide_mutex);
        }
    }

	while (1)
    {
        pthread_mutex_lock(&shared.visitor_guide_mutex);
        if (shared.visitor_leaves == shared.can_inside) // all visitor leaves
        {
            guide_leaves(id);
            shared.guide_inside--;
            if (shared.guide_inside == 0) // all guides leave, new guide can enter now
            {
                shared.guide_may_enter = 1;
            }
            pthread_cond_signal(&shared.guide_inside_may_enter_cond);
            pthread_mutex_unlock(&shared.visitor_guide_mutex);
            break;
        }
        pthread_mutex_unlock(&shared.visitor_guide_mutex);
    }
}



