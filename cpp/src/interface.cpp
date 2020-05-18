#include "_precompile.h"

#include "util.h"
#include "util_py.h"
#include "circle_fitter.h"
#include "cirkel1.h"

int ndiameters[] = { 81, 51, 33, 23, 15, 11, 7 };
CircleFitter cf(ndiameters, sizeof(ndiameters) / sizeof(int));

////////////////////////////////////////////////////////////////////////////////////////////////

PyObject* Py_stacker1(PyObject* self, PyObject* args)
{
	PyArrayObject* aJ;
	PyArrayObject* aR;
	PyArrayObject* aRGB;
	PyArrayObject* aRGB_INC;
	int bN;
	int STACK_SIZE;
	int fail_limit1;
	int fail_limit2;

	if (!PyArg_ParseTuple(args, "O!O!iiiiO!O!", &PyArray_Type, &aJ, &PyArray_Type, &aR, &bN,
		&STACK_SIZE, &fail_limit1, &fail_limit2,
		&PyArray_Type, &aRGB, &PyArray_Type, &aRGB_INC))
	{
		return 0;
	}
	if (2 != aJ->nd || aJ->descr->type_num != PyArray_DOUBLE)
	{
		PyErr_SetString(PyExc_TypeError, "H array should be 2-dimensional and of type float64");
		goto error;
	}
	if (2 != aR->nd || aR->descr->type_num != PyArray_DOUBLE)
	{
		PyErr_SetString(PyExc_TypeError, "S array should be 2-dimensional and of type float64");
		goto error;
	}
	/*
	if ((aJ->dimensions[0] != aR->dimensions[0]) || (aJ->dimensions[1] != aR->dimensions[1]))
	{
		PyErr_SetString(PyExc_TypeError, "arrays must have the same shape");
		goto error;
	}
	*/

	PyArrayObject* new_array;

	{
		const double* jdata = (const double*)aJ->data;
		const double* rdata = (const double*)aR->data;
		const int JY = aJ->dimensions[0];
		const int JX = aJ->dimensions[1];
		const int RY = aR->dimensions[0];
		const int RX = aR->dimensions[1];

		const unsigned char* rgb = (unsigned char*)aRGB->data;
		const unsigned char* rgb_inc = (unsigned char*)aRGB_INC->data;

		npy_intp dimensions[3];
		dimensions[0] = bN;
		dimensions[1] = bN;
		dimensions[2] = 3;
		new_array = (PyArrayObject*)PyArray_SimpleNew(3, dimensions, PyArray_UBYTE);
		unsigned char* canvas = (unsigned char*)new_array->data;
		memset(canvas, 0, bN * bN * 3);

		int fails = 0;
		node3 n(jdata, JY, JX, rdata, RY, RX);
		n._depth = 0;
		n._angle = 0;
		n._x = 0.5;
		n._y = 0.5;

		n.act3(canvas, bN, STACK_SIZE, fail_limit1, fail_limit2, rgb, rgb_inc);
	}

	return (PyObject*)new_array;
error:
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#define ADD_VARARG(name, func) {name,(PyCFunction)&func,METH_VARARGS,nullptr}

std::vector<PyMethodDef> v_method_defs =
{
	ADD_VARARG("cirkel1", Py_stacker1),
	{0,0,0,0}
};

