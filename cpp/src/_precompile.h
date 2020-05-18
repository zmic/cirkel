#include <math.h>
#include <vector>
#include <tuple>

#if DEBUG_BUILD_FOR_RELEASE_PYTHON
#ifdef _DEBUG
#define DEBUG_WAS_DEFINED
#undef _DEBUG
#endif
#endif

#include <python.h>
#include "numpy/arrayobject.h"

#ifdef DEBUG_WAS_DEFINED
#define _DEBUG 1
#undef DEBUG_WAS_DEFINED
#endif


//#include "structmember.h"
