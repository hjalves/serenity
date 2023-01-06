/*
 * Copyright (c) 2023, Humberto Alves <hjalves@live.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

void dbgln(char const* line);
bool launch(char const* url_or_path);
void create_app();
void exec_app();
void notify(char const* title, char const* text, char const* icon_path);

__END_DECLS
