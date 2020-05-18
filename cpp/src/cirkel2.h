extern CircleFitter2 CF2;

struct node4
{

	int _depth;
	double _x;
	double _y;
	double _angle;
	int _lastd;

	circular_bufferB<int> _j;
	circular_bufferB<double> _r;
	circular_bufferB<int> _d;
	circular_bufferB<double> _b;

	std::vector<node4*> _kids;
	unsigned char _red;
	unsigned char _green;
	unsigned char _blue;
	int _count;
	int _active;
	int _fails;
	bool _kidsfirst;


	node4(
		int depth,
		double x,
		double y,
		double angle,
		int lastd,
		circular_bufferB<int>& j,
		circular_bufferB<double>& r,
		circular_bufferB<int>& d,
		circular_bufferB<double>& b,
		int count
	)
		:
		_depth(depth),
		_x(x),
		_y(y),
		_angle(angle),
		_lastd(lastd),
		_j(j),
		_r(r),
		_d(d),
		_b(b),
		_count(count),
		_active(true),
		_fails(0),
		_kidsfirst(false)
	{}



	int act3(
		unsigned char* mask,
		unsigned char* canvas,
		int N,
		const int STACK_SIZE,
		const int start_d,
		const int max_split,
		const unsigned char* rgb,
		const unsigned char* rgb_inc
	)
	{
		int s = 0;
		//for (;;)
		{
			if (_kidsfirst && _kids.size())
			{
				for (auto e : _kids)
				{
					{
						s += e->act3(mask, canvas, N, STACK_SIZE, start_d, max_split, rgb + 3, rgb_inc + 3);
					}
				}
				if (s == 0)
				{
					for (auto e : _kids)
					{
						delete e;
					}
					_kids.clear();
				}
			}

			int instruction = *_j++;
			switch (instruction)
			{
			case 1:
			{
				//double offset_angle = *_r++;
				if (_count > 1000000 || _depth == STACK_SIZE || !_active)
				{
					;
				}
				else
				{
					int count2 = 0;
					for (const double* pa = _b._p; pa < _b._end;)
					{
						double delta_angle = *pa++;
						if (delta_angle == _b._delimiter)
						{
							break;
						}
						node4* p2 = new node4(
							_depth + 1,
							_x,
							_y,
							_angle + delta_angle,
							_lastd,
							_j,
							_r,
							_d,
							_b,
							_count);

						p2->_j.next_row();
						p2->_r.next_row();
						p2->_d.next_row();
						p2->_b.next_row();
						p2->_red = rgb[0];
						p2->_green = rgb[1];
						p2->_blue = rgb[2];
						p2->_fails = 0;
						_kids.push_back(p2);
						++count2;
					}
					for (auto e : _kids)
					{
						e->_count *= count2;
					}
					_active = false;
					break;
				}
			}
			case 0:
			{
				int d = CF2.fit_neighbour(
					_d._p,
					_d._end,
					mask,
					canvas,
					N, N,
					_x, _y,
					_lastd,
					_angle,
					_red, _green, _blue);
				_red += rgb_inc[0];
				_green += rgb_inc[1];
				_blue += rgb_inc[2];
				if (d)
				{
					++s;
					_lastd = d;
					_active = true;
				}
				_angle += *_r++;
				_d++;
				break;
			}
			}


			if (!_kidsfirst && _kids.size())
			{
				int sk = 0;
				for (auto e : _kids)
				{
					{
						sk += e->act3(mask, canvas, N, STACK_SIZE, start_d, max_split, rgb + 3, rgb_inc + 3);
					}
				}
				if (sk == 0)
				{
					for (auto e : _kids)
					{
						delete e;
					}
					_kids.clear();
				}
				s += sk;
			}
		}
		return s;
	}
};

