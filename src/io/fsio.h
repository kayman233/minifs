#pragma once
#include "../params.h"
#include <stdio.h>
#include <stdlib.h>

// Для обращения к самому файлу с ФС
void fsRead (size_t offset, void* data, size_t size);
void fsWrite (size_t offset, void* data, size_t size);
