// root1d.h - One dimensional root finding
#pragma once
#include <functional>
#include <initializer_list>
#include <limits>
#include <deque>
#include "done1d.h"
#include "step1d.h"

namespace fms {

	template<class X = double>
	class root1d {
	public:
		std::function<X(const X&)> f, df;
		std::deque<X> x;
		std::deque<X> y;
		root1d(const std::function<X(const X&)>& _f)
			: f(_f)
		{ }
		root1d(const std::function<X(const X&)>& _f, const std::function<X(const X&)>& _df)
			: f(_f), df(_df)
		{ }
		root1d(const root1d&) = default;
		root1d& operator=(const root1d&) = default;
		~root1d()
		{ }

		size_t size() const
		{
			return x.size();
		}
		void push(X x0, X y0 = std::numeric_limits<X>::quiet_NaN())
		{
			x.push_front(x0);
			if (_isnan(y0))
				y0 = f(x0);
			y.push_front(y0);
		}
		void swap(size_t i, size_t j)
		{
			ensure (i < size() && j < size());

			std::swap(x[i], x[j]);
			std::swap(y[i], y[j]);
		}

		void init(size_t n, const X* x0)
		{
			x.assign(x0, x0 + n);
			y = x;
			std::transform(y.begin(), y.end(), y.begin(), f);
		}
		void init(std::initializer_list<X> xi)
		{
			x.assign(xi.begin(), xi.end());
			y = x;
			std::transform(y.begin(), y.end(), y.begin(), f);
		}
		void init(std::initializer_list<X> xi, std::initializer_list<X> yi)
		{
			ensure (xu.size() == yi.size());

			x.assign(xi.begin(), xi.end());
			y.assign(yi.begin(), yi.end());
		}
		// move to the next estimate and return it
		template<class S>
		X step(S& s)
		{
			return s(*this);
		}
		template<class D>
		bool done(const D& d) const
		{
			return d(*this);
		}
		bool cycle() const
		{
			return size() >= 4 && x[0] == x[2] && x[1] == x[3];
		}
		template<class S, class D>
		X solve(S& s, const D& d)
		{
			while (!done(d))
				step(s);

			return x[0];
		}

		bool bracketed() const
		{
			return y.size() < 2 ? false : y[0]*y[1] < 0;
		}
		
		X bracket(X dx = 1) // initial step size
		{
			ensure (x.size() >= 1);

			while (!bracketed()) {
				X yp = f(x[0] + dx);
				X ym = f(x[0] - dx);
				X mp = (yp - y[0])/dx;
				X mm = (y[0] - ym)/dx;
				X ddf = (mp - mm)/dx;

				if (ddf < 0) { 
					// concave - usually overshoots
					// use slope of best approximation
					if (fabs(yp) < fabs(ym))
						push(step1d::newton(x[0], y[0], mp));
					else
						push(step1d::newton(x[0], y[0], mm));
				}
				else { 
					// convex - usually undershoots
					X x0, y0;

					// use slope of worst approximation
					X m = fabs(yp) < fabs(ym) ? mm : mp;

					// |m| >= 1
					if (-1 < m && m < 0)
						m = -1;
					else if(0 < m && m < 1)
						m = 1;
					
					x0 = step1d::newton(x[0], y[0], m);
					y0 = f(x0);

					if (y0*y[0] > 0) { // not bracketed
						yp = f(x0 + dx);
						ym = f(x0 - dx);
						
						if (fabs(yp) < fabs(ym))
							push(x0 + dx, yp);
						else
							push(x0 - dx, ym);
					}
					else {
						push(x0, y0);
					}
				}

				if (done(done1d::interval<double>()))
					break;
			}

			return x[0];
		}
	}; // root1d
} // fms