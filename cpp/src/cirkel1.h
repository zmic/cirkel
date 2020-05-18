extern CircleFitter cf;

struct node
{
	int _depth;
	double _angle;
	double _x;
	double _y;
	circular_bufferA<double> _j;
	circular_bufferA<double> _r;

	node(const double* const jbegin, int jY, int jX,
		const double* const rbegin, int rY, int rX) :
		_j(jbegin, jY, jX),
		_r(rbegin, rY, rX)
	{}


	node(node* p) :
		_depth(p->_depth + 1),
		_angle(p->_angle),
		_x(p->_x),
		_y(p->_y),
		_j(p->_j),
		_r(p->_r)
	{}


	int act3(unsigned char* canvas,
		int N,
		const int STACK_SIZE,
		const int fail_limit1,
		const int fail_limit2,
		const unsigned char* rgb,
		const unsigned char* rgb_inc
	)
	{
		unsigned char r = rgb[0];
		unsigned char g = rgb[1];
		unsigned char b = rgb[2];
		unsigned char r_inc = rgb_inc[0];
		unsigned char g_inc = rgb_inc[1];
		unsigned char b_inc = rgb_inc[2];

		int total = 0;

		if (_depth != 0)
		{
			SquareJumper sq(_x, _y, _angle);
			for (int fails = 0; fails < fail_limit1; ++fails)
			{
				if (r < 2)
				{
					r = 2;
				}
				sq.next(*_j, *_r);
				_r++;
				_j++;
				int i = int(sq._y * N);
				int j = int(sq._x * N);

				if (cf.fit(canvas, N, N, i, j, r, g, b))
				{
					fails = 0;
					total += 1;
					r += r_inc;
					g += g_inc;
					b += b_inc;
				}
			}
			_x = sq._x;
			_y = sq._x;
		}

		if (_depth < STACK_SIZE)
		{
			for (int fails = 0; fails < fail_limit2;)
			{
				node* child = new node(this);
				child->_j.next_row();
				child->_r.next_row();
				int result = child->act3(
					canvas,
					N,
					STACK_SIZE,
					fail_limit1,
					fail_limit2,
					rgb + 3,
					rgb_inc + 3);
				if (result)
				{
					fails = 0;
				}
				else
				{
					fails += 1;
				}
				_angle += *_r;
				delete child;
			}
		}
		return total;
	}

};

struct node3 : public node
{
	std::vector<node3*> _kids;
	int _lastd;
	unsigned char _red;
	unsigned char _g;
	unsigned char _b;
	bool _dead;


	node3(const double* const jbegin, int jY, int jX,
		const double* const rbegin, int rY, int rX) : node(jbegin, jY, jX, rbegin, rY, rX), _dead(false)
	{}

	node3(node3* p) : node(p)
	{

	}

	int act3(unsigned char* canvas,
		int N,
		const int STACK_SIZE,
		const int start_d,
		const int fail_limit2,
		const unsigned char* rgb,
		const unsigned char* rgb_inc
	)
	{

		_lastd = start_d;
		for (;;)
		{
			if (!act3_(canvas, N, STACK_SIZE, start_d, fail_limit2, rgb, rgb_inc))
			{
				break;
			}
		}
		return 0;
	}

	int act3_(unsigned char* canvas,
		int N,
		const int STACK_SIZE,
		const int fail_limit1,
		const int fail_limit2,
		const unsigned char* rgb,
		const unsigned char* rgb_inc
	)
	{
		for (;;)
		{
			if (_kids.size())
			{
				int s = 0;
				for (auto e : _kids)
				{
					//if (!e->_dead)
					{
						s += e->act3_(canvas, N, STACK_SIZE, fail_limit1, fail_limit2, rgb + 3, rgb_inc + 3);
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
				return s;
			}
			else
			{
				double i = *_j++;
				if (_depth == 0 || (i > .15 && i < .20 && _depth < STACK_SIZE))
				{
					int nbr_rot = int(2 + i * 120);
					for (int j = 0; j < nbr_rot; ++j)
					{
						node3* p2 = new node3(this);
						p2->_lastd = _lastd;
						p2->_angle += j * 2 * d_pi / nbr_rot;
						p2->_j.next_row();
						p2->_r.next_row();
						p2->_red = rgb[0];
						p2->_g = rgb[1];
						p2->_b = rgb[2];
						_kids.push_back(p2);
					}
					continue;
				}
				else if (i < .3)
				{
					_angle += *_r++;
				}
				if (_red < 2)
				{
					_red = 2;
				}
				_lastd = cf.fit_neighbour(
					*_j++,
					canvas,
					N, N,
					_x, _y,
					_lastd,
					_angle,
					_red, _g, _b);
				_red += rgb_inc[0];
				_g += rgb_inc[1];
				_b += rgb_inc[2];

				return _lastd;
			}
		}
	}
};
