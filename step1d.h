// step1d.h - add next approximation to 1-d root finding
#pragma once
#include <functional>

namespace fms {

	template<class X> class root1d;

	template<class X>
	using step1d_f = std::function<X(root1d<X>&)>;

	namespace step1d {

		template<class X = double>
		inline X bisect(X x0, X x1)
		{
			return (x0 + x1)/2;
		}
		// step methods
		template<class X = double>
		inline X bisect(root1d<X>& s)
		{
			ensure (s.bracketed());
		
			s.push(bisect(s.x[0], s.x[1]));

			if (!s.bracketed()) {
				s.swap(1,2);
			}

			return s.x[0];
		}

		template<class X = double>
		inline X secant(X x0, X x1, X y0, X y1)
		{
			ensure (y1 != y0);

			return (x0*y1 - x1*y0)/(y1 - y0);
		}
		template<class X = double>
		inline X secant(root1d<X>& s)
		{
			ensure (s.x.size() >= 2);

			s.push(secant(s.x[0], s.x[1], s.y[0], s.y[1]));

			return s.x[0];
		}

		template<class X = double>
		inline X inverse_quadratic(X x0, X x1, X x2, X y0, X y1, X y2)
		{
			X dy0 = y1 - y2;
			X dy1 = y2 - y0;
			X dy2 = y0 - y1;

			ensure (dy0 != 0);
			ensure (dy1 != 0);
			ensure (dy2 != 0);

			return x0*y1*y2/(dy1*dy2) + x1*y2*y0/(dy2*dy0) + x2*y0*y1/(dy0*dy1);
		}
		template<class X = double>
		inline X inverse_quadratic(root1d<X>& s)
		{
			ensure (s.x.size() >= 3);

			s.push(inverse_quadratic(s.x[0], s.x[1], s.x[2], s.y[0], s.y[1], s.y[2]));

			return s.x[0];
		}

		template<class X = double>
		inline X newton(X x0, X y0, X m)
		{
			return x0 - y0/m;
		}
		template<class X = double>
		inline X newton(root1d<X>& s, X m)
		{
			ensure (s.x.size() >= 1);

			s.push(newton(s.x[0], s.y[0], m));

			return x[0];
		}
		template<class X = double>
		inline X newton(root1d<X>& s)
		{
			ensure (s.x.size() >= 1);

			s.push(newton(s.x[0], s.y[0], s.df(s.x[0])));

			return s.x[0];
		}

	} // step1d
} // fms