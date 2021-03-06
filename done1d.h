// done1d.h - termination conditions for 1-d root finding
#pragma once
#include <algorithm>
#include <functional>
#include <limits>

namespace fms {

	template<class X> class root1d;

	template<class X>
	using done1d_f = std::function<bool(const root1d<X>&)>;

	namespace done1d {

		template<class X>
		struct done {
			bool operator()(const root1d<X>& s) { return call(s); }
		private:
			virtual bool call(const root1d<X>&) = 0;
		};

		// for roots near 0
		template<class X>
		inline done1d_f<X> interval(X abs = 16*std::numeric_limits<X>::epsilon(), X rel = 0)
		{
			return [abs,rel](const root1d<X>& s) {
				X min_ = s.x[0]*s.x[1] < 0 ? 0 : (std::min)(fabs(s.x[0]), fabs(s.x[1]));

				return fabs(s.x[0] - s.x[1]) < abs + rel*min_; 
			};
		}

		template<class X>
		inline std::function<bool(const root1d<X>&)> delta(X abs = std::numeric_limits<X>::epsilon(), X rel = 0)
		{
			return [abs,rel](const root1d<X>& s) {
				return fabs(s.x[0] - s.x[1]) < abs + rel*s.x[0]; 
			};
		}

		template<class X>
		inline std::function<bool(const root1d<X>&)> residual(X abs = 0)
		{
			return [abs](const root1d<X>& s) {
				return abs == 0 ? s.y[0] + 1 == 1 : fabs(s.y[0]) < abs;
			};
		}
		
	} // done1d
} // fms