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

#ifndef PTHREADS_H
#define PTHREADS_H

extern int THREAD_POOL_SIZE;
extern int THREAD_POOL_ADJUST;
extern int loop;

struct pool_item
{
	struct server_struct* item;
	int working;
};

struct pool_item* pool;

pthread_t* thread_pool;
pthread_t spawned_thread;

int scount = 0;
int spawned = 0;

pthread_cond_t new_request = PTHREAD_COND_INITIALIZER;
pthread_cond_t thread_free = PTHREAD_COND_INITIALIZER;

pthread_mutex_t pool_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread_pool_mutex = PTHREAD_MUTEX_INITIALIZER;


int threads_init();
void threads_shutdown();

void threads_loop();

void worker(int n);

int push_request(struct server_struct* request);
int pop_request();


#endif
