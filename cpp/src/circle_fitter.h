class CircleFitter
{
	std::vector< std::tuple<int, std::vector<unsigned char>, std::vector<unsigned char> > > _masks;
public:
	CircleFitter(int* diameters, int ndiameters)
	{
		create_masks(diameters, ndiameters);
	}
	void create_masks(int* diameters, int ndiameters)
	{
		_masks.clear();
		_masks.resize(ndiameters);
		for (int ri = 0; ri < ndiameters; ++ri)
		{
			const int d = diameters[ri];
			std::get<0>(_masks[ri]) = d;
			std::vector<unsigned char>& v = std::get<1>(_masks[ri]);
			std::vector<unsigned char>& v2 = std::get<2>(_masks[ri]);
			v.resize(d * d);
			v2.resize(d * d);
			const double r = (d - 1) / 2.0;
			for (int y = 0; y < d; y++)
			{
				const double fy = y - r;
				for (int x = 0; x < d; x++)
				{
					const double fx = x - r;
					const double di = sqrt(fy * fy + fx * fx);
					if (di <= r - 0.5)
					{
						v[y * d + x] = 255;
					}
					else if (di <= r + 0.5)
					{
						v[y * d + x] = unsigned char(255 * (r + 0.5 - di));
					}
					if (di <= r - 1.5)
					{
						v2[y * d + x] = 255;
					}
					else if (di <= r - .5)
					{
						v2[y * d + x] = unsigned char(255 * (r - 0.5 - di));
					}
				}
			}
		}
	}
	int fit(unsigned char* a, const int X, const int Y, int x, int y)
	{
		if (*(a + y * Y + x))
		{
			return 0;
		}

		for (auto& e : _masks)
		{
			const int d = std::get<0>(e);
			const int r = d / 2;
			if (x + r >= X || x - r < 0 || y + r >= Y || y - r < 0)
			{
				continue;
			}
			x -= r;
			y -= r;
			const unsigned char* m = &std::get<1>(e)[0];
			const unsigned char* a2 = a + y * Y + x;
			const unsigned char* at = a2 + d;
			bool success = true;
			for (int i = 0; i < d * d; i++)
			{
				if (int(*m++) + *a2++ > 255)
					//if (*m++ && *a2++ )
				{
					success = false;
					break;
				}
				if (a2 == at)
				{
					at += X;
					a2 = at - d;
				}
			}
			if (success)
			{
				const unsigned char* m = &std::get<2>(e)[0];
				unsigned char* a2 = a + y * Y + x;
				unsigned char* at = a2 + d;
				bool success = true;
				for (int i = 0; i < d * d; i++)
				{
					*a2++ += *m++;
					if (a2 == at)
					{
						at += X;
						a2 = at - d;
					}
				}
				return d;
			}
			x += r;
			y += r;

		}
		return 0;
	}

	int fit(unsigned char* a, const int X, const int Y, int x, int y, unsigned char red, unsigned char green, unsigned char blue)
	{
		if (*(a + 3 * (y * Y + x)))
		{
			return 0;
		}

		for (auto& e : _masks)
		{
			const int d = std::get<0>(e);
			const int r = d / 2;
			if (x + r >= X || x - r < 0 || y + r >= Y || y - r < 0)
			{
				continue;
			}
			x -= r;
			y -= r;
			const unsigned char* m = &std::get<1>(e)[0];
			const unsigned char* a2 = a + 3 * (y * Y + x);
			const unsigned char* at = a2 + 3 * d;
			bool success = true;
			for (int i = 0; i < d * d; i++)
			{
				if (*m++ && *a2)
				{
					success = false;
					break;
				}
				a2 += 3;
				if (a2 == at)
				{
					at += 3 * X;
					a2 = at - 3 * d;
				}
			}
			if (success)
			{
				const unsigned char* m = &std::get<2>(e)[0];
				unsigned char* a2 = a + 3 * (y * Y + x);
				unsigned char* at = a2 + 3 * d;
				bool success = true;
				for (int i = 0; i < d * d; i++)
				{
					const int v = *m++;
					if (v)
					{
						*a2++ = (red * v + 1) >> 8;
						*a2++ = (green * v + 1) >> 8;
						*a2++ = (blue * v + 1) >> 8;
					}
					else
					{
						a2 += 3;
					}
					if (a2 == at)
					{
						at += 3 * X;
						a2 = at - 3 * d;
					}
				}
				return d;
			}
			x += r;
			y += r;

		}
		return 0;
	}

	int fit_single(const int mask_index, unsigned char* a, const int X, const int Y, int x, int y, unsigned char r2, unsigned char g, unsigned char b)
	{
		auto& e = _masks[mask_index];
		{
			const int d = std::get<0>(e);
			const int r = d / 2;
			if (x + r >= X || x - r < 0 || y + r >= Y || y - r < 0)
			{
				return 0;
			}
			x -= r;
			y -= r;
			const unsigned char* m = &std::get<1>(e)[0];
			const unsigned char* a2 = a + 3 * (y * Y + x);
			const unsigned char* at = a2 + 3 * d;
			bool success = true;
			for (int i = 0; i < d * d; i++)
			{
				if (*m++ && *a2)
				{
					success = false;
					break;
				}
				a2 += 3;
				if (a2 == at)
				{
					at += 3 * X;
					a2 = at - 3 * d;
				}
			}
			if (success)
			{
				const unsigned char* m = &std::get<2>(e)[0];
				unsigned char* a2 = a + 3 * (y * Y + x);
				unsigned char* at = a2 + 3 * d;
				bool success = true;
				for (int i = 0; i < d * d; i++)
				{
					const int v = *m++;
					if (v)
					{
						*a2++ = (r2 * v) >> 8;
						*a2++ = (g * v) >> 8;
						*a2++ = (b * v) >> 8;
					}
					else
					{
						a2 += 3;
					}
					if (a2 == at)
					{
						at += 3 * X;
						a2 = at - 3 * d;
					}
				}
				return d;
			}
			x += r;
			y += r;

		}
		return 0;
	}


	int fit_neighbour(
		float first,
		unsigned char* a,
		const int X, const int Y,
		double& _x, double& _y,
		int dparent,
		const float angle,
		unsigned char r2, unsigned char g, unsigned char b)
	{
		const double ca = cos(angle) / X;
		const double sa = sin(angle) / Y;

		int start_ei = first * .7 * _masks.size();
		for (int ei = start_ei; ei < _masks.size(); ++ei)
		//for (auto& e : _masks)
		{
			auto& e = _masks[ei];
			const int d = std::get<0>(e);
			const int r = d / 2;
			double x2 = _x + (dparent * 0.5 + r + 1) * ca;
			double y2 = _y + (dparent * 0.5 + r + 1) * sa;
			int x = X * x2;
			int y = Y * y2;

			if (x + r >= X || x - r < 0 || y + r >= Y || y - r < 0)
			{
				continue;
			}
			x -= r;
			y -= r;
			const unsigned char* m = &std::get<1>(e)[0];
			const unsigned char* a2 = a + 3 * (y * Y + x);
			const unsigned char* at = a2 + 3 * d;
			bool success = true;
			for (int i = 0; i < d * d; i++)
			{
				if (*m++ && *a2)
				{
					success = false;
					break;
				}
				a2 += 3;
				if (a2 == at)
				{
					at += 3 * X;
					a2 = at - 3 * d;
				}
			}
			if (success)
			{
				const unsigned char* m = &std::get<2>(e)[0];
				unsigned char* a2 = a + 3 * (y * Y + x);
				unsigned char* at = a2 + 3 * d;
				bool success = true;
				for (int i = 0; i < d * d; i++)
				{
					const int v = *m++;
					if (v)
					{
						*a2++ = (r2 * v) >> 8;
						*a2++ = (g * v) >> 8;
						*a2++ = (b * v) >> 8;
					}
					else
					{
						a2 += 3;
					}
					if (a2 == at)
					{
						at += 3 * X;
						a2 = at - 3 * d;
					}
				}
				_x = x2;
				_y = y2;
				return d;
			}
		}
		return 0;
	}

};

