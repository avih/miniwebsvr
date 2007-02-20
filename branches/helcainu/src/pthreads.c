/*  MiniWebsvr - A small webserver
    Copyright (C) 2007  Nickolas Antonie Grigoriadis
    E-Mail: grigi_ at users.sourceforge.net

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <pthread.h>

#include "os_compat.h"

#include "server.h"
#include "logging.h"
#include "config.h"

#include "pthreads.h"

extern server_config config;

int threads_init()
{
	pool = malloc(config.thread_pool_size*sizeof(struct pool_item));
	thread_pool = malloc(config.thread_pool_size*sizeof(pthread_t)+config.thread_pool_adjust*(sizeof(pthread_t)));
	if((pool == NULL)||(thread_pool == NULL))
	{
		DebugMSG("Can't allocate memory for thread pool need");
		return -1;
	}
	
	memset(pool, 0, sizeof(struct pool_item)*config.thread_pool_size);
	memset(thread_pool, 0, sizeof(pthread_t)*config.thread_pool_size + sizeof(pthread_t)*config.thread_pool_adjust);

	size_t i = 0;
		
	for(i = 0; i < config.thread_pool_size; i++)
	{
		pthread_create(&thread_pool[i], NULL, (void*)&worker, (void*)i);
	}


	return 1;

}

void threads_add(struct server_struct* sock)
{
	int wait = 0;
	while(!push_request(sock) && loop) 
	{
		pthread_mutex_lock(&pool_mutex);
		pthread_cond_wait(&thread_free, &pool_mutex);
		pthread_mutex_unlock(&pool_mutex);

		if(config.thread_pool_adjust > 0)
		{
			wait = 1;
			scount++;
			if((scount == THREAD_SPAWN_AT)&&(spawned < THREAD_POOL_ADJUST))
			{
				pthread_mutex_lock(&thread_pool_mutex);
				spawned++;
				DebugMSG("thread_pool adjusted to %d: 1 spawned", config.thread_pool_size + spawned);
				pthread_mutex_unlock(&thread_pool_mutex);
				pthread_create(&spawned_thread, NULL, (void*)&worker, (void*)(-1));
				pthread_detach(spawned_thread);
			}
			if(scount == THREAD_SPAWN_AT)
				scount = 0;
		}

	}
	if((config.thread_pool_adjust > 0)&&(!wait))
	{
		if(dcount == THREAD_KILL_AT)
		{
			dcount = 0;
		}
		if(spawned > 0)
		{
			dcount++;
		}
	}

	pthread_cond_signal(&new_request);
}

void threads_shutdown()
{
	int i;
	for(i = 0; i < config.thread_pool_size; i++)
	{
		pthread_cancel(thread_pool[i]);
	}
	for(i = 0; i < config.thread_pool_size; i++)
	{
		pthread_join(thread_pool[i], NULL);
	}
	free(thread_pool);
	free(pool);
}

void worker(int n)
{
	int index;
	DebugMSG("Worker %d started!", n);
	while(loop) // Global variable of "loop" indicates that server is running, if not set, quit
	{
		pthread_mutex_lock(&pool_mutex);
		pthread_cond_wait(&new_request, &pool_mutex); 
		index = pop_request(); 
		if(index == -1)
		{
			pthread_mutex_unlock(&pool_mutex);
			continue;
		}
		else
		{
			pool[index].working=1;
			pthread_mutex_unlock(&pool_mutex);
		}

		if(index != -1)
			server(pool[index].item);

		pthread_mutex_lock(&pool_mutex);
		free(pool[index].item);
		pool[index].item=NULL;
		pool[index].working=0;
		pthread_mutex_unlock(&pool_mutex);
		
		if(n == -1)
		{
			pthread_mutex_lock(&thread_pool_mutex);
			if(dcount == THREAD_KILL_AT)
			{
				dcount = 0;
				spawned--;
				DebugMSG("thread_pool adjusted to %d: 1 killed", config.thread_pool_size + spawned);
				pthread_mutex_unlock(&thread_pool_mutex);
				pthread_exit(NULL);
			}
			else
				pthread_mutex_unlock(&thread_pool_mutex);
		}
		pthread_cond_signal(&thread_free);
	}

}

int push_request(struct server_struct* request)
{
	int i, added = 0;
	pthread_mutex_lock(&pool_mutex);
	for(i = 0; (i < config.thread_pool_size)&&(!added); i++)
	{
		if(pool[i].item == NULL)
		{
			pool[i].item = request;
			pool[i].working = 0;
			added = 1;
		}
	}
	pthread_mutex_unlock(&pool_mutex);
	if(added)
		pthread_cond_signal(&new_request);
	return added;
}

int pop_request()
{
	int i, iret = -1;
	for(i = 0; (i < config.thread_pool_size)&&(iret==-1); i++)
	{
		if((pool[i].working == 0)&&(pool[i].item != NULL))
			iret=i;
 	}
	return iret;
}

