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
};

static struct shared_data shared;


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
}


/**
 * Implements the visitor arrival, touring, and leaving sequence.
 */
void visitor(int id)
{
	// visitor_arrives(id);
	// visitor_tours(id);
	// visitor_leaves(id);
}

/**
 * Implements the guide arrival, entering, admitting, and leaving sequence.
 */
void guide(int id)
{
	// guide_arrives(id);
	// guide_enters(id);
	// guide_admits(id);
	// guide_leaves(id);
}
