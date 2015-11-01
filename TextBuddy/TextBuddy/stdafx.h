// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

// #include <cassert>
// #include <cstdlib>
// #include <iostream>
// #include <stdio.h>

// Switch off assert in test explorer (doesn't work in UI.exe)
#define NDEBUG
#include <assert.h>

#include <iomanip>
#include <fstream>
#include <sstream>

#include <algorithm>
#include <string>
#include <set>
#include <vector>

#include "TbLogger.h"
#include "Task.h"
#include "Command.h"
#include "Utilities.h"