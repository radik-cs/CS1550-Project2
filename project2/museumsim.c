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
	//     int museum_tickets_remain;



    /*pthread_mutex_t ticket_mutex;
    pthread_mutex_t visitor_mutex;
    pthread_mutex_t guide_mutex;

    pthread_cond_t museum_visitor_arrives;
    pthread_cond_t museum_visitor_enters;
    pthread_cond_t museum_visitor_finish;

    pthread_cond_t museum_guide_admits;
    pthread_cond_t museum_guide_enters;
     pthread_cond_t museum_guide_tours;
    pthread_cond_t museum_guide_finish;

    int museum_tickets_remain;
    int museum_ticket_full;
    int museum_ticket_per_guide;
    int museum_visitor;
    int museum_visitor_waiting_outside;
    int museum_visitor_leaves;
    int museum_guide_waiting_outside;
    int museum_guide_inside; */

    pthread_mutex_t ticket_mutex;
    pthread_mutex_t visitor_mutex;
    pthread_mutex_t guide_mutex;

    pthread_cond_t museum_guide_tours;
    pthread_cond_t museum_guide_finish;
    pthread_cond_t museum_visitor_arr;
    pthread_cond_t museum_g_ad;
    pthread_cond_t museum_g_en;
    pthread_cond_t museum_v_en;
    pthread_cond_t museum_v_fin;
    
    int museum_tickets;
    int museum_numbers_full;
    int museum_cur_visitor;
    int museum_cur_visitor_waiting_outside;
    int museum_cur_visitor_done;
    int museum_cur_guide_waiting_outside;
    int museum_cur_guide_inside;
    int museum_ticket_per_guide;  
 

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
	// shared.museum_tickets_remain = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);

  

   /* pthread_mutex_init(&shared.ticket_mutex, NULL);
    pthread_mutex_init(&shared.visitor_mutex, NULL);
    pthread_mutex_init(&shared.guide_mutex, NULL);

    pthread_cond_init(&shared.museum_visitor_arrives, NULL);
    pthread_cond_init(&shared.museum_visitor_enters, NULL);
    pthread_cond_init(&shared.museum_visitor_finish, NULL);

    pthread_cond_init(&shared.museum_guide_enters, NULL);
    pthread_cond_init(&shared.museum_guide_admits, NULL);
    pthread_cond_init(&shared.museum_guide_tours, NULL);
    pthread_cond_init(&shared.museum_guide_finish, NULL);
   
 
    shared.museum_tickets_remain = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);
    shared.museum_ticket_per_guide = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);
    shared.museum_ticket_full = 0;
    shared.museum_visitor_waiting_outside = 0;
    shared.museum_visitor = 0;
    museum_visitor_finish = 0;
    shared.museum_guide_inside = 0;
    shared.museum_guide_waiting_outside = 0;
    */


    pthread_mutex_init(&shared.ticket_mutex, NULL);
    pthread_mutex_init(&shared.visitor_mutex, NULL);
    pthread_mutex_init(&shared.guide_mutex, NULL);
    pthread_cond_init(&shared.museum_guide_tours, NULL);
    pthread_cond_init(&shared.museum_guide_finish, NULL);
    pthread_cond_init(&shared.museum_visitor_arr, NULL);
    pthread_cond_init(&shared.museum_g_en, NULL);
    pthread_cond_init(&shared.museum_g_ad, NULL);
    pthread_cond_init(&shared.museum_v_fin, NULL);
    pthread_cond_init(&shared.museum_v_en, NULL);
    //
    // shared.museum_tickets = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);
    shared.museum_tickets = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);
    shared.museum_numbers_full = 0;
    shared.museum_ticket_per_guide = MIN(VISITORS_PER_GUIDE * num_guides, num_visitors);
    shared.museum_cur_guide_inside = 0;
    shared.museum_cur_guide_waiting_outside = 0;
    shared.museum_cur_visitor_waiting_outside = 0;
    shared.museum_cur_visitor = 0;
    shared.museum_cur_visitor_done = 0;

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

   /* pthread_mutex_destroy(&shared.ticket_mutex);
    pthread_mutex_destroy(&shared.visitor_mutex);
    pthread_mutex_destroy(&shared.guide_mutex);
    pthread_cond_destroy(&shared.museum_visitor_arrives);
    pthread_cond_destroy(&shared.museum_visitor_enters);
    pthread_cond_destroy(&shared.museum_visitor_finish);
    pthread_cond_destroy(&shared.museum_guide_admits);
    pthread_cond_destroy(&shared.museum_guide_enters);
    pthread_cond_destroy(&shared.museum_guide_tours);
    pthread_cond_destroy(&shared.museum_guide_finish);
   */

    pthread_mutex_destroy(&shared.ticket_mutex);
    pthread_mutex_destroy(&shared.visitor_mutex);
    pthread_mutex_destroy(&shared.guide_mutex);
    pthread_cond_destroy(&shared.museum_guide_tours);
    pthread_cond_destroy(&shared.museum_guide_finish);
    pthread_cond_destroy(&shared.museum_visitor_arr);
    pthread_cond_destroy(&shared.museum_g_ad);
    pthread_cond_destroy(&shared.museum_g_en);
    pthread_cond_destroy(&shared.museum_v_fin);
    pthread_cond_destroy(&shared.museum_v_en);

}




/**
 * Implements the visitor arrival, touring, and leaving sequence.
 */
void visitor(int id)
/*
{
   // pthread_mutex_unlock(&shared.visitor_mutex);


visitor_arrives(id);
visitor_tours(id);
visitor_leaves(id);

    int museum_ticket_finished= 0;
    pthread_mutex_lock(&shared.ticket_mutex);
    {
        if (shared.museum_tickets_remain == 0)
        {
            visitor_leaves(id);
            pthread_cond_broadcast(&shared.museum_guide_finish);
            pthread_mutex_unlock(&shared.ticket_mutex);
            museum_ticket_finished= 1;
        }
        else
        {
            shared.museum_tickets_remain -= 1;
            shared.museum_ticket_full += 1;
            pthread_cond_broadcast(&shared.museum_visitor_arrives);
            pthread_mutex_unlock(&shared.ticket_mutex);
        }
    }
    if (museum_ticket_finished== 1)
    {
        return;
    }

    pthread_mutex_lock(&shared.guide_mutex);
    while (shared.museum_guide_inside == 0)
    {
        pthread_cond_wait(&shared.museum_guide_tours, &shared.guide_mutex);
    }
    pthread_mutex_unlock(&shared.guide_mutex);
    pthread_mutex_lock(&shared.visitor_mutex);
    {
        while (shared.museum_visitor_waiting_outside == 0)
        {
            pthread_cond_wait(&shared.museum_visitor_enters, &shared.visitor_mutex);
        }
        shared.museum_visitor_waiting_outside = 0;
        pthread_cond_signal(&shared.museum_guide_admits);
    }
    pthread_mutex_unlock(&shared.visitor_mutex);

    visitor_tours(id);

    pthread_mutex_lock(&shared.visitor_mutex);
    {
        if (shared.museum_visitor == VISITORS_PER_GUIDE * GUIDES_ALLOWED_INSIDE)
        {
            museum_visitor_finish = 0;
            visitor_leaves(id);
            pthread_cond_signal(&shared.museum_guide_enters);
        }
        while (museum_visitor_finish == 0)
        {
            pthread_cond_wait(&shared.museum_visitor_finish, &shared.visitor_mutex);
        }
        museum_visitor_finish = 0;

        visitor_leaves(id);

        pthread_cond_signal(&shared.museum_guide_enters);
    }
    pthread_mutex_unlock(&shared.visitor_mutex);

}

/////////////////////////////////////////
static __thread int served_so_far = 0;  // number of visitors served by each guide
*/

{
    visitor_arrives(id);
    int museum_tickets_sold = 0;
    pthread_mutex_lock(&shared.ticket_mutex);
    {
        if (shared.museum_tickets == 0)
        {
            visitor_leaves(id);
            pthread_cond_broadcast(&shared.museum_guide_finish);
            pthread_mutex_unlock(&shared.ticket_mutex);
            museum_tickets_sold = 1;
        }
        else
        {
            shared.museum_tickets -= 1;
            shared.museum_numbers_full += 1;
            pthread_cond_broadcast(&shared.museum_visitor_arr);
            pthread_mutex_unlock(&shared.ticket_mutex);
        }
    }
    if (museum_tickets_sold == 1)
    {
        return;
    }

    pthread_mutex_lock(&shared.guide_mutex);
    while (shared.museum_cur_guide_inside == 0)
    {
        pthread_cond_wait(&shared.museum_guide_tours, &shared.guide_mutex);
    }
    pthread_mutex_unlock(&shared.guide_mutex);
    pthread_mutex_lock(&shared.visitor_mutex);
    {
        while (shared.museum_cur_visitor_waiting_outside == 0)
        {
            pthread_cond_wait(&shared.museum_v_en, &shared.visitor_mutex);
        }
        shared.museum_cur_visitor_waiting_outside = 0;
        pthread_cond_signal(&shared.museum_g_ad);
    }
    pthread_mutex_unlock(&shared.visitor_mutex);
    visitor_tours(id);
    pthread_mutex_lock(&shared.visitor_mutex);
    {
        if (shared.museum_cur_visitor == VISITORS_PER_GUIDE * GUIDES_ALLOWED_INSIDE)
        {
            shared.museum_cur_visitor_done = 0;
            visitor_leaves(id);
            pthread_cond_signal(&shared.museum_g_en);
        }
        while (shared.museum_cur_visitor_done == 0)
        {
            pthread_cond_wait(&shared.museum_v_fin, &shared.visitor_mutex);
        }
        shared.museum_cur_visitor_done = 0;
        visitor_leaves(id);
        pthread_cond_signal(&shared.museum_g_en);
    }
    pthread_mutex_unlock(&shared.visitor_mutex);
}

/////////////////////////////////////////////////
/**
 * Implements the guide arrival, entering, admitting, and leaving sequence.
 */
void guide(int id)
/*
{
	// guide_arrives(id);

 pthread_mutex_lock(&shared.guide_mutex);
    {
        guide_arrives(id);
        shared.museum_guide_waiting_outside += 1;
        while (shared.museum_guide_inside == GUIDES_ALLOWED_INSIDE)
        {
            pthread_cond_wait(&shared.museum_guide_finish, &shared.guide_mutex);
        }
        guide_enters(id);
        shared.museum_guide_waiting_outside -= 1;
        shared.museum_guide_inside += 1;
        pthread_cond_broadcast(&shared.museum_guide_tours);
    }
    pthread_mutex_unlock(&shared.guide_mutex);

    int n_visitors = 0;
    int n_museum_tickets_remain = -1;
    pthread_mutex_lock(&shared.guide_mutex);
    {
        n_museum_tickets_remain = MIN(shared.museum_ticket_per_guide, VISITORS_PER_GUIDE);
        shared.museum_ticket_per_guide -= VISITORS_PER_GUIDE;
    }
    pthread_mutex_unlock(&shared.guide_mutex);

    while (n_visitors != n_museum_tickets_remain)
    {
        pthread_mutex_lock(&shared.ticket_mutex);
        {
            while (shared.museum_ticket_full == 0 && shared.museum_tickets_remain != 0)
            {
                pthread_cond_wait(&shared.museum_visitor_arrives, &shared.ticket_mutex);
            }
            shared.museum_ticket_full -= 1;
        }
        pthread_mutex_unlock(&shared.ticket_mutex);
        n_visitors += 1;
        pthread_mutex_lock(&shared.visitor_mutex);
        {
            while (shared.museum_visitor_waiting_outside != 0)
            {
                pthread_cond_wait(&shared.museum_guide_admits, &shared.visitor_mutex);
            }
            shared.museum_visitor_waiting_outside = 1;
            shared.museum_visitor += 1;
            guide_admits(id);
            pthread_cond_signal(&shared.museum_visitor_enters);
        }
        pthread_mutex_unlock(&shared.visitor_mutex);
    }
    while (n_visitors >= 0)
    {
        pthread_mutex_lock(&shared.visitor_mutex);
        {
            while (museum_visitor_finish != 0)
            {
                pthread_cond_wait(&shared.museum_guide_enters, &shared.visitor_mutex);
            }
            museum_visitor_finish = 1;
            shared.museum_visitor -= 1;
            pthread_cond_signal(&shared.museum_visitor_finish);
        }
        pthread_mutex_unlock(&shared.visitor_mutex);
        n_visitors -= 1;
    }

    pthread_mutex_lock(&shared.guide_mutex);
    {
        shared.museum_guide_inside -= 1;
        guide_leaves(id);
        pthread_cond_broadcast(&shared.museum_guide_finish);
    }
    pthread_mutex_unlock(&shared.guide_mutex);

}
*/

{
    pthread_mutex_lock(&shared.guide_mutex);
    {
        guide_arrives(id);
        shared.museum_cur_guide_waiting_outside += 1;
        while (shared.museum_cur_guide_inside == GUIDES_ALLOWED_INSIDE)
        {
            pthread_cond_wait(&shared.museum_guide_finish, &shared.guide_mutex);
        }
        guide_enters(id);
        shared.museum_cur_guide_waiting_outside -= 1;
        shared.museum_cur_guide_inside += 1;
        pthread_cond_broadcast(&shared.museum_guide_tours);
    }
    pthread_mutex_unlock(&shared.guide_mutex);

    int n_visitors = 0;
    int n_museum_tickets = -1;
    pthread_mutex_lock(&shared.guide_mutex);
    {
        n_museum_tickets = MIN(shared.museum_ticket_per_guide, VISITORS_PER_GUIDE);
        shared.museum_ticket_per_guide -= VISITORS_PER_GUIDE;
    }
    pthread_mutex_unlock(&shared.guide_mutex);

    while (n_visitors != n_museum_tickets)
    {
        pthread_mutex_lock(&shared.ticket_mutex);
        {
            while (shared.museum_numbers_full == 0 && shared.museum_tickets != 0)
            {
                pthread_cond_wait(&shared.museum_visitor_arr, &shared.ticket_mutex);
            }
            shared.museum_numbers_full -= 1;
        }
        pthread_mutex_unlock(&shared.ticket_mutex);
        n_visitors += 1;
        pthread_mutex_lock(&shared.visitor_mutex);
        {
            while (shared.museum_cur_visitor_waiting_outside != 0)
            {
                pthread_cond_wait(&shared.museum_g_ad, &shared.visitor_mutex);
            }
            shared.museum_cur_visitor_waiting_outside = 1;
            shared.museum_cur_visitor += 1;
            guide_admits(id);
            pthread_cond_signal(&shared.museum_v_en);
        }
        pthread_mutex_unlock(&shared.visitor_mutex);
    }
    while (n_visitors >= 0)
    {
        pthread_mutex_lock(&shared.visitor_mutex);
        {
            while (shared.museum_cur_visitor_done != 0)
            {
                pthread_cond_wait(&shared.museum_g_en, &shared.visitor_mutex);
            }
            shared.museum_cur_visitor_done = 1;
            shared.museum_cur_visitor -= 1;
            pthread_cond_signal(&shared.museum_v_fin);
        }
        pthread_mutex_unlock(&shared.visitor_mutex);
        n_visitors -= 1;
    }

    pthread_mutex_lock(&shared.guide_mutex);
    {
        shared.museum_cur_guide_inside -= 1;
        guide_leaves(id);
        pthread_cond_broadcast(&shared.museum_guide_finish);
    }
    pthread_mutex_unlock(&shared.guide_mutex);
}






