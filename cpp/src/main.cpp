#include "_precompile.h"
#include "util_py.h"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define _STR(x) #x
#define STR(x) _STR(x)
#define MODULE_STRING STR(MODULE_NAME)
#define MODULE_DOC MODULE_STRING

#define __INIT_FUNCTION(x) PyInit_##x
#define INIT_FUNCTION_RETURN_TYPE PyObject*
#define RETURN(x) return x

static struct PyModuleDef moduledef = {
PyModuleDef_HEAD_INIT,
MODULE_STRING,       /* m_name */
MODULE_DOC,          /* m_doc */
-1,                  /* m_size */
NULL, //&v_method_defs.front(),           /* m_methods */
NULL,                /* m_reload */
NULL,                /* m_traverse */
NULL,                /* m_clear */
NULL,                /* m_free */
};

#define MOD_ERROR_VAL NULL
#define MOD_SUCCESS_VAL(val) val
#define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
#define MOD_DEF(ob, name, doc, methods) \
        static struct PyModuleDef moduledef = { \
        PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
        ob = PyModule_Create(&moduledef);

#define _INIT_FUNCTION(x) __INIT_FUNCTION(x)
#define INIT_FUNCTION _INIT_FUNCTION(MODULE_NAME)

extern "C"
#ifdef _MSC_VER
__declspec(dllexport)
#endif
INIT_FUNCTION_RETURN_TYPE INIT_FUNCTION()
{
#ifdef _DEBUG
#if defined(_MSC_VER)
	int tmpDbgFlag;
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
	//_CrtSetBreakAlloc(148);
	//_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE );
	//_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
#endif
#endif
	PyMethodDef null = { 0,0,0,0 };
	v_method_defs.push_back(null);
	moduledef.m_methods = &v_method_defs.front();
	PyObject* module = PyModule_Create(&moduledef);

	RETURN(module);
}
