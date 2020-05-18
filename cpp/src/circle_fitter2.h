class CircleFitter2
{
	std::vector< std::tuple<std::vector<unsigned char>, std::vector<unsigned char> > > _masks;
public:
	CircleFitter2(int max_diameter) : _masks(max_diameter + 1)
	{
		for (int d = 0; d <= max_diameter; ++d)
		{
			std::vector<unsigned char>& v = std::get<0>(_masks[d]);
			std::vector<unsigned char>& v2 = std::get<1>(_masks[d]);
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
						v[y * d + x] = 255 * (r + 0.5 - di);
					}
					if (di <= r - 1.5)
					{
						v2[y * d + x] = 255;
					}
					else if (di <= r - .5)
					{
						v2[y * d + x] = 255 * (r - 0.5 - di);
					}
				}
			}
		}
	}

	int fit_neighbour(
		const int* try_diameters,
		const int* const try_diameters_end,
		unsigned char* mask,
		unsigned char* canvas,
		const int X, const int Y,
		double& _x, double& _y,
		const int dparent,
		const double angle,
		unsigned char red, unsigned char g, unsigned char b)
	{
		const double ca = cos(angle) / X;
		const double sa = sin(angle) / Y;

		for (; try_diameters != try_diameters_end;)
		{
			const int d = *try_diameters++;
			if (d < 0)
			{
				return 0;
			}
			auto& e = _masks[d];
			const int r = d / 2;
			double stick_distance = d < dparent ? double(d - 20) / 40 : double(dparent - 20) / 40;
			double x2 = _x + (dparent * 0.5 + r + stick_distance) * ca;
			double y2 = _y + (dparent * 0.5 + r + stick_distance) * sa;
			int x = X * x2;
			int y = Y * y2;

			if (x + r >= X || x - r < 0 || y + r >= Y || y - r < 0)
			{
				continue;
			}
			x -= r;
			y -= r;
			bool success = true;
			{
				const unsigned char* m = &std::get<0>(e)[0];
				const unsigned char* a2 = mask + (y * X + x);
				const unsigned char* at = a2 + d;
				const unsigned char* ae = at + (d - 1) * X;
				for (;;)
				{
					if (*m++ && *a2)
					{
						success = false;
						break;
					}
					a2++;
					if (a2 == at)
					{
						if (at == ae)
						{
							break;
						}
						at += X;
						a2 = at - d;
					}
				}
			}
			if (success)
			{
				const unsigned char* m = &std::get<1>(e)[0];
				unsigned char* a2 = canvas + 3 * (y * X + x);
				unsigned char* at = a2 + 3 * d;
				unsigned char* am = mask + (y * Y + x);

				bool success = true;
				for (int i = 0; i < d * d; i++)
				{
					const int v = *m++;
					if (v)
					{
						if (v == 0xff)
						{
							*am++ = 1;
							*a2++ = red;
							*a2++ = g;
							*a2++ = b;
						}
						else
						{
							const int v2 = 0xff - v;
							*am++ = 1;
							*a2++ = ((*a2 * v2) + (red * v)) >> 8;
							*a2++ = ((*a2 * v2) + (g * v)) >> 8;
							*a2++ = ((*a2 * v2) + (b * v)) >> 8;
						}
					}
					else
					{
						am++;
						a2 += 3;
					}
					if (a2 == at)
					{
						am = am - d + X;
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

