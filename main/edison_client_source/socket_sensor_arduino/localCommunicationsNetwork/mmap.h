/*
 * Author: Matthias Hahn <matthias.hahn@intel.com>
 * Copyright (C) 2014 Intel Corporation
 * This file is part of mmap IPC sample provided under the MIT license
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MMAP_HPP
#define MMAP_HPP

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <pthread.h>

/* assert(/tmp mounted to tmpfs, i.e. resides in RAM) */
/* just use any file in /tmp */
static const char* mmapFilePath = "/tmp/arduino";

typedef struct{
	
	unsigned long value;
	int type;
	char data[128];
}sock_msg;
void *send_sensor_value(void *);
void *recv_cmd_type(void*);
extern void exitError(const char* errMsg) ;

int init_socket(const char *ip,int port);
struct mmapData {
 double temperature;
 double soundValue;
 double vibration;
 double touch;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
};

#endif
