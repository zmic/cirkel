#include "_precompile.h"

#include "util.h"
#include "util_py.h"
#include "circle_fitter.h"
#include "circle_fitter2.h"
#include "cirkel1.h"
#include "cirkel2.h"

int ndiameters[] = { 61, 51, 33, 23, 15, 11, 7 };
CircleFitter cf(ndiameters, sizeof(ndiameters) / sizeof(int));
CircleFitter2 CF2(100);

////////////////////////////////////////////////////////////////////////////////////////////////

// shenanigans because the designers of numpy made... poor decisions
int whose_idea_was_this()
{
	import_array();
	return 0;
}

PyObject* Py_circlefitter1_set_diameter(PyObject* self, PyObject* args)
{
	PyArrayObject* aJ;

	if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &aJ))
	{
		return 0;
	}
	if (1 != aJ->nd || aJ->descr->type_num != PyArray_LONG)
	{
		PyErr_SetString(PyExc_TypeError, "Diameter array should be 1-dimensional and of type int");
		goto error;
	}

	{
		int* jdata = (int*)aJ->data;
		const int JN = aJ->dimensions[0];
		cf.create_masks(jdata, JN);
	}
	Py_RETURN_NONE;
error:
	return 0;
}


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
	int fanout_multiplier;
	double f0;
	double f1;

	if (!PyArg_ParseTuple(args, "O!O!iiiiiddO!O!", &PyArray_Type, &aJ, &PyArray_Type, &aR, &bN,
		&STACK_SIZE, &fail_limit1, &fail_limit2, &fanout_multiplier, &f0, &f1, 
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

		n.act3(canvas, bN, STACK_SIZE, fail_limit1, fail_limit2, fanout_multiplier, f0, f1, rgb, rgb_inc);
	}

	return (PyObject*)new_array;
error:
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

PyObject* Py_stacker4(PyObject* self, PyObject* args)
{
	PyArrayObject* aS;  // start nodes
	PyArrayObject* aJ;  // instructions
	PyArrayObject* aR;  // angles
	PyArrayObject* aD;  // try diameters
	PyArrayObject* aB;  // branch angles
	PyArrayObject* aRGB;
	PyArrayObject* aRGB_INC;
	int bN;
	int STACK_SIZE;
	int param1;
	int param2;
	int red_b;
	int green_b;
	int blue_b;


	if (!PyArg_ParseTuple(args, "O!O!O!O!O!iiiiO!O!iii",
		&PyArray_Type, &aS,
		&PyArray_Type, &aJ,
		&PyArray_Type, &aR,
		&PyArray_Type, &aD,
		&PyArray_Type, &aB,
		&bN,
		&STACK_SIZE, &param1, &param2,
		&PyArray_Type, &aRGB, &PyArray_Type, &aRGB_INC,
		&red_b, &green_b, &blue_b))
	{
		return 0;
	}
	if (2 != aS->nd || aS->descr->type_num != PyArray_DOUBLE)
	{
		PyErr_SetString(PyExc_TypeError, "Start array should be 2-dimensional and of type float64");
		goto error;
	}
	if (2 != aJ->nd || aJ->descr->type_num != PyArray_LONG)
	{
		PyErr_SetString(PyExc_TypeError, "Instruction array should be 2-dimensional and of type int32");
		goto error;
	}
	if (2 != aR->nd || aR->descr->type_num != PyArray_DOUBLE)
	{
		PyErr_SetString(PyExc_TypeError, "Angles array should be 2-dimensional and of type float64");
		goto error;
	}
	if (2 != aD->nd || aD->descr->type_num != PyArray_LONG)
	{
		PyErr_SetString(PyExc_TypeError, "Diameters array should be 2-dimensional and of type int32");
		goto error;
	}
	if (2 != aB->nd || aB->descr->type_num != PyArray_DOUBLE)
	{
		PyErr_SetString(PyExc_TypeError, "Branch array should be 2-dimensional and of type float64");
		goto error;
	}


	PyArrayObject* new_array;

	{
		const double* sdata = (const double*)aS->data;
		const int* jdata = (const int*)aJ->data;
		const double* rdata = (const double*)aR->data;
		const int* ddata = (const int*)aD->data;
		const double* bdata = (const double*)aB->data;
		const int SY = aS->dimensions[0];
		const int SX = aS->dimensions[1];
		const int JY = aJ->dimensions[0];
		const int JX = aJ->dimensions[1];
		const int RY = aR->dimensions[0];
		const int RX = aR->dimensions[1];
		const int DY = aD->dimensions[0];
		const int DX = aD->dimensions[1];
		const int BY = aB->dimensions[0];
		const int BX = aB->dimensions[1];

		const unsigned char* rgb = (unsigned char*)aRGB->data;
		const unsigned char* rgb_inc = (unsigned char*)aRGB_INC->data;

		npy_intp dimensions[3];
		dimensions[0] = bN;
		dimensions[1] = bN;
		dimensions[2] = 3;
		new_array = (PyArrayObject*)PyArray_SimpleNew(3, dimensions, PyArray_UBYTE);
		unsigned char* canvas = (unsigned char*)new_array->data;
		unsigned char* mask = new unsigned char[bN * bN];
		memset(mask, 0, bN * bN);

		unsigned char* canvas2 = canvas;
		unsigned char* canvas2_end = canvas2 + 3 * bN * bN;
		for (; canvas2 != canvas2_end;)
		{
			*canvas2++ = red_b;
			*canvas2++ = green_b;
			*canvas2++ = blue_b;
		}

		circular_bufferB<int> j(jdata, -1, JY, JX);
		circular_bufferB<double> r(rdata, -1e300, RY, RX);
		circular_bufferB<int> d(ddata, -1, DY, DX);
		circular_bufferB<double> b(bdata, -1e300, BY, BX);

		std::vector<node4*> all;
		for (int i = 0; i < SY; i++)
		{
			node4* n = new node4(0, sdata[0], sdata[1], sdata[2], 0, j, r, d, b, SY);
			n->_red = rgb[0];
			n->_green = rgb[1];
			n->_blue = rgb[2];
			all.push_back(n);
			sdata += 3;
		}
		for (int s = 1; s;)
		{
			s = 0;
			for (auto e : all)
			{
				s += e->act3(mask, canvas, bN, STACK_SIZE - 1, param1, param2, rgb + 3, rgb_inc);
			}
		}
		for (auto e : all)
		{
			delete e;
		}
		delete[] mask;
	}

	return (PyObject*)new_array;
error:
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

#define ADD_VARARG(name, func) {name,(PyCFunction)&func,METH_VARARGS,nullptr}

std::vector<PyMethodDef> v_method_defs =
{
	ADD_VARARG("circlefitter1_set_diameter", Py_circlefitter1_set_diameter),
	ADD_VARARG("cirkel1", Py_stacker1),
	ADD_VARARG("cirkel2", Py_stacker4),
	{0,0,0,0}
};

