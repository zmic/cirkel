const float f_pi = float(3.14159265358979323846);
const double d_pi = double(3.14159265358979323846);
const double sqj_epsilon = 0.0000001;

struct SquareJumper
{
	double _x, _y, _r;
	SquareJumper() : _x(.5), _y(.5), _r(0)
	{
	}
	SquareJumper(double x, double y, double r) : _x(x), _y(y), _r(r)
	{
	}
	void next(double j, const double r)
	{
		_r += r;
		const double dx = j * cos(_r);
		const double dy = j * sin(_r);
		if (j < 0)
		{
			j = -j;
		}
		double tx, ty;
		if (dy > sqj_epsilon)
		{
			ty = (.999999 - _y) / dy;
		}
		else if (dy < -sqj_epsilon)
		{
			ty = -_y / dy;
		}
		else
		{
			ty = 1;
		}
		if (dx > sqj_epsilon)
		{
			tx = (.999999 - _x) / dx;
		}
		else if (dx < -sqj_epsilon)
		{
			tx = -_x / dx;
		}
		else
		{
			tx = 1;
		}
		double t = tx < ty ? tx : ty;
		if (t > 1)
		{
			_x += dx;
			_y += dy;
		}
		else
		{
			_x += dx * t * j;
			_y += dy * t * j;
		}
	}
};




template<class T>
struct circular_bufferA
{
	circular_bufferA(const T* const begin, int Y, int X) : _begin_begin(begin), _begin(begin), _end(begin + X), _p(begin), _Y(Y), _X(X)
	{}
	void next_row()
	{
		__int64 i = (_begin - _begin_begin) / _X;
		i += 1;
		if (i == _Y)
		{
			i = 0;
		}
		_begin = _begin_begin + i * _X;
		_end = _begin + _X;
		_p = _begin;
	}
	void operator++()
	{
		if (++_p == _end)
		{
			_p = _begin;
		}
	}
	const T* operator++(int)
	{
		const T* v = _p;
		if (++_p == _end)
		{
			_p = _begin;
		}
		return v;
	}
	double operator*()
	{
		return *_p;
	}
	const T* const _begin_begin;
	int _Y;
	int _X;
	const T* _begin;
	const T* _end;
	const T* _p;

};
