// Copyright (c) 2017 Danny Angelo Carminati Grein
// MIT License, see LICENSE file for details

#pragma once

#include "cr_shared.h"

// Overridable macros
#ifndef CR_LOG
#ifdef CR_DEBUG
#include <stdio.h>
#define CR_LOG(...) fprintf(stdout, __VA_ARGS__)
#else
#define CR_LOG(...)
#endif
#endif

#ifndef CR_ERROR
#ifdef CR_DEBUG
#include <stdio.h>
#define CR_ERROR(...) fprintf(stderr, __VA_ARGS__)
#else
#define CR_ERROR(...)
#endif
#endif

#ifndef CR_TRACE
#ifdef CR_DEBUG
#include <stdio.h>
#define CR_TRACE fprintf(stdout, "CR_TRACE: %s\n", __FUNCTION__);
#else
#define CR_TRACE
#endif
#endif

#ifndef CR_MAIN_FUNC
#define CR_MAIN_FUNC "cr_main"
#endif

#ifndef CR_ASSERT
#include <assert.h>
#define CR_ASSERT(e) assert(e)
#endif

#ifndef CR_REALLOC
#include <stdlib.h>
#define CR_REALLOC(ptr, size) ::realloc(ptr, size)
#endif

#ifndef CR_FREE
#include <stdlib.h>
#define CR_FREE(ptr) ::free(ptr)
#endif

#ifndef CR_MALLOC
#include <stdlib.h>
#define CR_MALLOC(size) ::malloc(size)
#endif

extern "C" bool cr_plugin_open(cr_plugin &ctx, const char *path, cr_mode mode);
extern "C" int cr_plugin_update(cr_plugin &ctx, bool reloadCheck = true);
extern "C" void cr_plugin_close(cr_plugin &ctx);
