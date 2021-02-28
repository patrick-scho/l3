#pragma once

#include <stdio.h>
#include <setjmp.h>

extern jmp_buf error_jmp_buf;
void error(const char *msg);
