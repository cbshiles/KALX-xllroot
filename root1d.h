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
		std::function<X(const X&)> f;
		std::deque<X> x;
		std::deque<X> y;
		root1d(const std::function<X(const X&)>& _f)
			: f(_f)
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
			ensure (y.size() >= 2);

			return y[0]*y[1] < 0;
		}
		
		X bracket(X m = 1) // minimum absolute slope
		{
			ensure (x.size() >= 2);


			X m_ = (y[0] - y[1])/(x[0] - x[1]);
			m = m_ > m ? m : m_ < -m ? -m : m_; // |m| > 1

			while (!bracketed() && !done(done1d::residual<double>())) {
				push(x[0] - y[0]/m);
			}

			return x[0];
		}
	}; // root1d
} // fms