// xll.root1d.cpp
#include "xllroot.h"
#include "../xllfunction/xllfunction.h"

using namespace xll;
using namespace fms;

typedef traits<XLOPERX>::xfp xfp;
typedef traits<XLOPERX>::xword xword;

static AddInX xai_root1d(
	FunctionX(XLL_HANDLEX, _T("?xll_root1d"), CATEGORY_ _T("ROOT1D"))
	.Handle(_T("Function"), _T("is a handle to a function."))
	.Handle(_T("_Derivative"), _T("is a handle to an optional derivative of Function."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a one dimensional root finder."))
);
HANDLEX WINAPI xll_root1d(HANDLEX f, HANDLEX df)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> pf(f);
		auto F = [pf](double x) { return (*pf)(OPERX(x)).val.num; };

		if (df) {
			handle<function> pdf(df);
			auto dF = [pdf](double x) { return (*pdf)(OPERX(x)).val.num; };
			handle<root1d<>> ph(new root1d<>(F, dF));

			h = ph.get();
		}
		else {
			handle<root1d<>> ph(new root1d<>(F));

			h = ph.get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root1d_x(
	FunctionX(XLL_FPX, _T("?xll_root1d_x"), CATEGORY_ _T("ROOT1D.X"))
	.Handle(_T("Root"), _T("is a handle returned by XLL.ROOT1D"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return root approximations."))
);
xfp* WINAPI xll_root1d_x(HANDLEX h)
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
static AddInX xai_root1d_y(
	FunctionX(XLL_FPX, _T("?xll_root1d_y"), CATEGORY_ _T("ROOT1D.Y"))
	.Handle(_T("Root"), _T("is a handle returned by XLL.ROOT1D"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return residuals."))
);
xfp* WINAPI xll_root1d_y(HANDLEX h)
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

static AddInX xai_root1d_bracket(
	FunctionX(XLL_DOUBLEX, _T("?xll_root1d_bracket"), CATEGORY_ _T("ROOT1D.BRACKET"))
	.Handle(_T("Root"), _T("is a handle returned by XLL.ROOT1D"))
	.Num(_T("Delta"), _T("is the initial incremential step size."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Bracket the root of a monotonic function."))
);
HANDLEX WINAPI xll_root1d_bracket(HANDLEX h, double dx)
{
#pragma XLLEXPORT
	try {
		handle<root1d<>> ph(h);

		if (dx == 0)
			dx = 1;

		ph->bracket(dx);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		h = handlex();
	}

	return h;
}

static AddInX xai_root1d_done(
	FunctionX(XLL_BOOLX, _T("?xll_root1d_done"), CATEGORY_ _T("ROOT1D.DONE"))
	.Handle(_T("Root"), _T("is a handle returned by XLL.ROOT1D"))
	.Handle(_T("Done"), _T("is a handle returned by XLL.ROOT1D.DONE.*"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Test convergence."))
);
BOOL WINAPI xll_root1d_done(HANDLEX h, HANDLEX d)
{
#pragma XLLEXPORT
	BOOL b(false);

	try {
		handle<root1d<>> ph(h);
		handle<fms::done1d_f<double>> pd(d);

		b = ph->done(*pd);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return b;
}

static AddInX xai_root1d_init(
	FunctionX(XLL_HANDLEX, _T("?xll_root1d_init"), CATEGORY_ _T("ROOT1D.INIT"))
	.Handle(_T("Root"), _T("is a handle returned by XLL.ROOT1D"))
	.Array(_T("Init"), _T("is a set of initial values."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Initialize abscissae."))
);
HANDLEX WINAPI xll_root1d_init(HANDLEX h, xfp* px)
{
#pragma XLLEXPORT
	try {
		handle<root1d<>> ph(h);

		ph->init(size(*px), px->array);

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root1d_step(
	FunctionX(XLL_HANDLEX, _T("?xll_root1d_step"), CATEGORY_ _T("ROOT1D.STEP"))
	.Handle(_T("Root"), _T("is a handle returned by XLL.ROOT1D"))
	.Handle(_T("Step"), _T("is a handle returned by XLL.ROOT1D.STEP.*"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Get next approximation."))
);
HANDLEX WINAPI xll_root1d_step(HANDLEX h, HANDLEX s)
{
#pragma XLLEXPORT
	try {
		handle<root1d<>> ph(h);
		handle<fms::step1d_f<double>> ps(s);

		ph->step(*ps);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

