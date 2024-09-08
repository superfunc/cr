// Copyright (c) 2017 Danny Angelo Carminati Grein
// MIT License, see LICENSE file for details

#pragma once

#include "cr_shared.h"

#include <stdbool.h>

extern "C" bool cr_plugin_open(cr_plugin *ctx, const char *path, cr_mode mode);
extern "C" int cr_plugin_update(cr_plugin *ctx, bool reloadCheck);
extern "C" void cr_plugin_close(cr_plugin *ctx);
