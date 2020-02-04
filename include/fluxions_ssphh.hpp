#ifndef FLUXIONS_SSPHH_HPP
#define FLUXIONS_SSPHH_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_config.hpp>
#include <fluxions_sphl.hpp>
#include <fluxions_sphl_sampler.hpp>

namespace Fluxions {
	//namespace SSPHH
	//{
	//	class Node
	//	{
	//	public:
	//		Node() {}
	//		virtual ~Node() {}
	//	}; // class Node

	//	template <typename T>
	//	T Legendre(unsigned int n, T x)
	//	{
	//		if (x < -1.0 || x > 1.0)
	//			return 0.0;
	//		switch (n)
	//		{
	//		case 0:
	//			return 1;
	//		case 1:
	//			return x;
	//		case 2:
	//			return 1.0 / 2.0 * (3.0 * pow(x, 2) - 1.0);
	//		case 3:
	//			return 1.0 / 2.0 * (5.0 * pow(x, 3) - 3.0 * x);
	//		case 4:
	//			return 1.0 / 8.0 * (35.0 * pow(x, 4) - 30.0 * pow(x, 2) + 3);
	//		case 5:
	//			return 1.0 / 8.0 * (63.0 * pow(x, 5) - 70.0 * pow(x, 3) + 15.0 * x);
	//		case 6:
	//			return 1.0 / 16.0 * (231.0 * pow(x, 6) - 315.0 * pow(x, 4) + 105.0 * pow(x, 2) - 5.0);
	//		case 7:
	//			return 1.0 / 16.0 * (429.0 * pow(x, 7) - 693.0 * pow(x, 5) + 315.0 * pow(x, 3) - 35.0 * x);
	//		case 8:
	//			return 1.0 / 128.0 * (6435.0 * pow(x, 8) - 12012.0 * pow(x, 6) + 6930.0 * pow(x, 4) - 1260.0 * pow(x, 2) + 35.0);
	//		case 9:
	//			return 1.0 / 128.0 * (12155.0 * pow(x, 9) - 25740.0 * pow(x, 7) + 18018.0 * pow(x, 5) - 4620.0 * pow(x, 3) + 315.0 * x);
	//		case 10:
	//			return 1.0 / 256.0 * (46189.0 * pow(x, 10) - 109395.0 * pow(x, 8) + 90090.0 * pow(x, 6) - 30030.0 * pow(x, 4) + 3465.0 * pow(x, 2) - 63.0);
	//		default:
	//			return 0.0;
	//		}
	//	}

	//	static const int MaxSphls = 16;
	//} // namespace SSPHH

} // namespace Fluxions

#endif
