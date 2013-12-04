// xll.root1d.cpp
#include "../xll8/xll/xll.h"
#include "../xllfunction/xllfunction.h"
#include "root1d.h"

using namespace xll;
using namespace fms;

typedef traits<XLOPERX>::xfp xfp;
typedef traits<XLOPERX>::xword xword;

static AddInX xai_root1d_state(
	FunctionX(XLL_HANDLEX, _T("?xll_root1d_state"), _T("XLL.ROOT1D"))
	.Handle(_T("Function"), _T("is a handle to a function returned by XLL.BIND."))
	.Array(_T("Init"), _T("is an array of initial values to use."))
	.Uncalced()
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return a handle to a one dimensional root finder."))
);
HANDLEX WINAPI xll_root1d_state(HANDLEX f, const xfp* px)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> pf(f);
		auto F = [pf](double x) { return (*pf)(OPERX(x)).val.num; };
		handle<root1d<>> ph(new root1d<>(F));
		ph->init(size(*px), px->array);

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root1d_state_x(
	FunctionX(XLL_FPX, _T("?xll_root1d_state_x"), _T("XLL.ROOT1D.X"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by XLL.ROOT1D"))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return root approximations."))
);
xfp* WINAPI xll_root1d_state_x(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		handle<root1d<>> ph(h);

		x.resize(static_cast<xword>(ph->x.size()), 1);
		std::copy(ph->x.begin(), ph->x.end(), x.begin());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return x.get();
}
static AddInX xai_root1d_state_y(
	FunctionX(XLL_FPX, _T("?xll_root1d_state_y"), _T("XLL.ROOT1D.Y"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by XLL.ROOT1D"))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return residuals."))
);
xfp* WINAPI xll_root1d_state_y(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX y;

	try {
		handle<root1d<>> ph(h);

		y.resize(static_cast<xword>(ph->x.size()), 1);
		std::copy(ph->y.begin(), ph->y.end(), y.begin());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y.get();
}

static AddInX xai_root1d_done(
	FunctionX(XLL_BOOLX, _T("?xll_root1d_done"), _T("XLL.ROOT1D.DONE"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by XLL.ROOT1D"))
	.Arg(XLL_HANDLEX, _T("Done"), _T("is a handle returned by XLL.ROOT1D.DONE.*"))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Test convergence."))
);
BOOL WINAPI xll_root1d_done(HANDLEX h, HANDLEX d)
{
#pragma XLLEXPORT
	BOOL b(false);

	try {
		handle<root1d<>> ph(h);
		handle<std::function<bool(const root1d<>&)>> pd(d);

		b = ph->done(*pd);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return b;
}

static AddInX xai_root1d_state_bracket(
	FunctionX(XLL_DOUBLEX, _T("?xll_root1d_state_bracket"), _T("XLL.ROOT1D.BRACKET"))
	.Arg(XLL_HANDLEX, _T("state"), _T("is a handle returned by XLL.ROOT1D"))
	.Arg(XLL_DOUBLEX, _T("_Slope"), _T("is the optional minumum slope to use. Default is 1."))
	.Uncalced()
	.Category(_T("XLL"))
	.FunctionHelp(_T("Bracket the root of a monotonic function with argument _1."))
);
HANDLEX WINAPI xll_root1d_state_bracket(HANDLEX h, double m)
{
#pragma XLLEXPORT
	try {
		handle<root1d<>> ph(h);

		if (m == 0)
			m = 1;

		ph->bracket(m);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		h = handlex();
	}

	return h;
}
