#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define DLLEXPORT __declspec(dllexport)
#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)
#include <Windows.h>
#include <iostream>
#include "hook.h"