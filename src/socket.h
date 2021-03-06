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
#pragma once
#include "spotifyd.h"

void sock_send_str(int sockfd, const char * const str);
int sock_readline(int sockfd, char *buf);
void *sock_connection_handler(void *sockfd_ptr);
void sock_send_track_with_trackn(int sockfd, sp_track *track, int trackn);
void sock_send_track(int sockfd, sp_track *track);
void *sock_accept_connections_ip(void *not_used);
void *sock_accept_connections_un(void *not_used);
int sock_create_un();
int sock_create_ip();
void sock_close();
