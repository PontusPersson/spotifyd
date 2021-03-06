/*
 * spotifyd - A daemon playing music from spotify, in the spirit of MPD.
 * Copyright (C) 2015 Simon Persson
 * 
 * Spotifyd program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Spotifyd program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <libspotify/api.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "queue.h"
#include "spotifyd.h"

sp_track *queue[PLAY_QUEUE_LEN];
unsigned queue_len;
bool queue_random;
unsigned queue_position;

void queue_init()
{
	srand(time(NULL));
	queue_len = 0;
	queue_position = 0;
	queue_random = 0;
	memset(queue, 0, PLAY_QUEUE_LEN * sizeof(sp_track *));
}

bool queue_add_track(sp_track *track)
{
	bool track_added;

	if(track != NULL && queue_len + 1< PLAY_QUEUE_LEN)
	{
		sp_track_add_ref(track);
		queue[queue_len++] = track;
		track_added = 1;
	}
	else
	{
		track_added = 0;
	}

	return track_added;
}

int queue_get_next()
{
	int next_track;
	if(queue_random)
	{
		next_track = rand()%queue_len;
	}
	else
	{
		next_track = (queue_position + 1)%queue_len;
	}
	return next_track;
}

int queue_get_prev()
{
	srand(time(NULL));
	int prev_track;
	if(queue_random)
	{
		prev_track = rand()%queue_len;
	}
	else
	{
		prev_track = (queue_position - 1)%queue_len;
	}
	return prev_track;
}

bool queue_toggle_random()
{
	queue_random = !queue_random;
	return queue_random;
}

sp_track *queue_get(unsigned i)
{
	return queue[i];
}

void queue_set_current(unsigned i)
{
	queue_position = i;
}

sp_track *queue_get_current()
{
	sp_track *ret_val = NULL;
	ret_val = queue[queue_position];
	return ret_val;
}

unsigned queue_get_len()
{
	return queue_len;
}

bool queue_del_track(unsigned trackn)
{
	bool ret_val = 0;

	if(trackn < queue_len && queue[trackn] != NULL)
	{
		sp_track_release(queue[trackn]);
		queue[trackn] = NULL;
		--queue_len;
		memmove(&queue[trackn], &queue[trackn+1], PLAY_QUEUE_LEN - trackn);
		memset(&queue[queue_len], 0, sizeof(sp_track *)*(PLAY_QUEUE_LEN - queue_len));
		ret_val = 1;
	}
	return ret_val;
}
