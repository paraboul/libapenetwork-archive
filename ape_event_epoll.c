/*
    APE Network Library
    Copyright (C) 2010-2013 Anthony Catel <paraboul@gmail.com>

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
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "common.h"
#include "ape_events.h"
#ifndef __WIN32
  #include <sys/time.h>
  #include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef USE_EPOLL_HANDLER
static int event_epoll_add(struct _fdevent *ev, int fd, int bitadd,
        void *attach)
{
    struct epoll_event kev;

    kev.events = ((bitadd & EVENT_LEVEL ? 0 : EPOLLET)) | EPOLLPRI;

    if (bitadd & EVENT_READ) {
        kev.events |= EPOLLIN;
    }

    if (bitadd & EVENT_WRITE) {
        kev.events |= EPOLLOUT;
    }

    memset(&kev.data, 0, sizeof(kev.data));

    kev.data.ptr = attach;

    if (epoll_ctl(ev->epoll_fd, EPOLL_CTL_ADD, fd, &kev) == -1) {
        return -1;
    }

    return 1;
}

static int event_epoll_del(struct _fdevent *ev, int fd)
{
    struct epoll_event kev;

    kev.events = 0;

    memset(&kev.data, 0, sizeof(kev.data));

    if (epoll_ctl(ev->epoll_fd, EPOLL_CTL_DEL, fd, &kev) == -1) {
        return -1;
    }

    return 1;
}

static int event_epoll_poll(struct _fdevent *ev, int timeout_ms)
{
    int nfds;
    if ((nfds = epoll_wait(ev->epoll_fd, ev->events, *ev->basemem,
                    timeout_ms)) == -1) {
        return -1;
    }

    return nfds;
}

static void *event_epoll_get_fd(struct _fdevent *ev, int i)
{
    /* the value must start by ape_fds */
    return ev->events[i].data.ptr;
}

static void event_epoll_growup(struct _fdevent *ev)
{
    ev->events = realloc(ev->events,
            sizeof(struct epoll_event) * (*ev->basemem));
}

static int event_epoll_revent(struct _fdevent *ev, int i)
{
    int bitret = 0;

    if (ev->events[i].events & EPOLLIN) {
        bitret = EVENT_READ;
    }
    if (ev->events[i].events & EPOLLOUT) {
        bitret |= EVENT_WRITE;
    }

    return bitret;
}


int event_epoll_reload(struct _fdevent *ev)
{
    int nfd;
    if ((nfd = dup(ev->epoll_fd)) != -1) {
        close(nfd);
        close(ev->epoll_fd);
    }
    if ((ev->epoll_fd = epoll_create(1)) == -1) {
        return 0;
    }

    return 1;
}

int event_epoll_init(struct _fdevent *ev)
{
    if ((ev->epoll_fd = epoll_create(1)) == -1) {
        return 0;
    }

    ev->events = malloc(sizeof(struct epoll_event) * (*ev->basemem));

    ev->add     = event_epoll_add;
    ev->del     = event_epoll_del;
    ev->poll    = event_epoll_poll;
    ev->get_current_fd = event_epoll_get_fd;
    ev->growup  = event_epoll_growup;
    ev->revent  = event_epoll_revent;
    ev->reload  = event_epoll_reload;

    printf("epoll() started with %i slots\n", *ev->basemem);

    return 1;
}

#else
int event_epoll_init(struct _fdevent *ev)
{
    return 0;
}
#endif

// vim: ts=4 sts=4 sw=4 et

