/*
 * Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
 */

#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <nccl_net.h>
#include <nccl_ofi.h>
#include <nccl_ofi_log.h>
#include "mpi.h"
#include <unistd.h>
#include <nccl.h>
#include <dlfcn.h>
#include <stdarg.h>

#define STR2(v)		#v
#define STR(v)		STR2(v)

#define NUM_REQUESTS	(256)
#define SEND_SIZE	(5000)
#define RECV_SIZE	(5200)

void logger(ncclDebugLogLevel level, unsigned long flags, const char *filefunc,
	    int line, const char *fmt, ...)
{
	va_list vargs;

	switch (level) {
		case NCCL_LOG_WARN:
			printf("WARN: Function: %s Line: %d: ", filefunc, line);
			break;
		case NCCL_LOG_INFO:
			printf("INFO: Function: %s Line: %d: ", filefunc, line);
			break;
		case NCCL_LOG_TRACE:
			printf("TRACE: Function: %s Line: %d: ", filefunc, line);
			break;
		default:
			break;
	};

	va_start(vargs, fmt);
	vprintf(fmt, vargs);
	printf("\n");
	va_end(vargs);
}

ncclNet_t *get_extNet(void)
{
	void *netPluginLib = NULL;
	ncclNet_t *extNet = NULL;

	netPluginLib = dlopen("libnccl-net.so", RTLD_NOW | RTLD_LOCAL);
	if (netPluginLib == NULL) {
		NCCL_OFI_WARN("Unable to load libnccl-net.so: %s", dlerror());
		return NULL;
	}

	extNet = (ncclNet_t *)dlsym(netPluginLib, STR(NCCL_PLUGIN_SYMBOL));
	if (extNet == NULL) {
		NCCL_OFI_WARN("NetPlugin, could not find %s symbol",
			      STR(NCCL_PLUGIN_SYMBOL));
	}

	return extNet;
}

#endif // End TEST_COMMON_H_
