#include "fluxions_pch.hpp"
#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_astronomy.hpp>
#include <cassert>
#pragma warning(disable : 26451)

namespace Fluxions {
	namespace Astronomy {
		// A S T R O N O M I C A L   C O N S T A N T S ///////////////////

		constexpr Real MJD_offset = -2400000.5;
		constexpr Real J1970_0_0 = 2440587.5;
		constexpr Real J2000_0_0 = 2451543.5;
		constexpr Real J2000_1_5 = 2451545.0;
		constexpr Real J2010_0_0 = 2455196.5;

		// U T I L I T Y   F U N C T I O N S ////////////////////////////////

		// Wraps the variable X in the half open domain [a, b)
		Real wrap(Real x, Real a, Real b) {
			assert(a < b);
			Real out = fmod(x - a, b - a) + a;
			if (out < a)
				out += b - a;
			return out;
		}

		// same as wrap(X, a, b), but with a = 0.0
		Real wrap(Real x, Real b) {
			assert(b != 0.0);
			Real out = fmod(x, b);
			if (out < 0.0)
				out += b;
			return out;
		}

		// Converts decimal degrees, minutes, seconds to decimal degrees
		Real ddmmss_to_degrees(Real dd, Real mm, Real ss) {
			return dd + mm / 60.0 + ss / 3600.0;
		}

		Real clamp(Real x, Real a, Real b) {
			if (x < a)
				return a;
			if (x > b)
				return b;
			return x;
		}

		Real vb_round(Real x, int numDecimalPlaces) {
			Real z = pow(10.0, numDecimalPlaces);
			Real y = trunc(round(x * z) / z);
			return y;
		}

		Real vb_fix(Real x) {
			return trunc(x);
		}

		inline Real Fix(Real x) {
			return std::trunc(x);
		}

		inline Real Int(Real x) {
			return std::floor(x);
		}

		inline Real Sin(Real x) {
			return std::sin(x);
		}

		inline Real Cos(Real x) {
			return std::cos(x);
		}

		inline Real Tan(Real x) {
			return std::tan(x);
		}

		inline Real Atan2(Real x, Real y) {
			return std::atan2(y, x);
		}

		inline Real Unwind(Real x) {
			return x - 6.283185308 * Int(x / 6.283185308);
		}

		namespace PA {
			Real CivilDateTime::GST() {
				Real julian = JD();
				Real s = julian - 2451545.0;
				Real t = s / 36525.0;
				Real t0 = wrap(6.697374558 + t * (2400.051336 + (0.000025862 * t)), 24.0);
				Real ut = 1.002737909 * UT();
				return wrap(t0 + ut, 24.0);
			}

			Real CivilDateTime::LST() {
				return wrap(geo_longitude / 15.0 + GST(), 24.0);
			}

			Real CivilDateTime::JD() {
				Real Y, M;
				Real A, B, C, D;

				if (month < 3) {
					Y = year - 1;
					M = month + 12;
				}
				else {
					Y = year;
					M = month;
				}

				if (year > 1582) {
					A = trunc(Y / 100.0);
					B = 2.0 - A + trunc(A / 4.0);
				}
				else {
					if (year == 1582 && month > 10) {
						A = trunc(Y / 100.0);
						B = 2.0 - A + trunc(A / 4.0);
					}
					else {
						if (year == 1582 && month == 10 && day >= 15) {
							A = trunc(Y / 100.0);
							B = 2.0 - A + trunc(A / 4.0);
						}
						else {
							B = 0.0;
						}
					}
				}

				if (Y < 0) {
					C = trunc((365.25 * Y) - 0.75);
				}
				else {
					C = trunc(365.25 * Y);
				}

				D = trunc(30.6001 * (M + 1));
				return B + C + D + day + 1720994.5;
			}

			Real CivilDateTime::UT() {
				Real DS = isdst ? 1.0 : 0.0;
				Real A = HMSDH(hh, mm, ss + ss_frac);
				Real B = A - DS - timeZoneOffset;
				Real C = day + (B / 24.0);
				Real D = CDJD(C, month, year);
				Real E = JDCDay(D);
				Real E1 = trunc(E);
				return 24.0 * (E - E1);
			}

			time_t CivilDateTime::getTime() const {
				tm localtime;
				localtime.tm_hour = hh;
				localtime.tm_min = mm;
				localtime.tm_sec = ss;
				localtime.tm_year = year - 1900;
				localtime.tm_mon = month;
				localtime.tm_mday = day;
				localtime.tm_isdst = isdst ? 1 : 0;
				localtime.tm_wday = 0;
				localtime.tm_yday = 0;
				return mktime(&localtime);
			}

			GreenwichDateTime CivilDateTime::getGreenwich() const {
				GreenwichDateTime gdt;
				Real ds = isdst ? 1 : 0;
				gdt.day = LctGDay(hh, mm, ss, ds, timeZoneOffset, day, month, year);
				gdt.month = LctGMonth(hh, mm, ss, ds, timeZoneOffset, day, month, year);
				gdt.year = LctGYear(hh, mm, ss, ds, timeZoneOffset, day, month, year);
				gdt.UT = LctUT(hh, mm, ss, ds, timeZoneOffset, day, month, year);
				return gdt;
			}

			Real CivilDateTime::getDayOfYear() const {
				static constexpr Real days[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
				Real a = 0;
				for (int i = 0; i < month; i++) {
					a += days[i];
				}
				a += (hh * 3600 + mm * 60 + ss) / 86400.0f;
				return a;
			}

			Real CivilDateTime::getMonthOfYear() const {
				return getDayOfYear() / 12.0f;
			}

			Real DDDeg(Real dd) {
				Real a = fabs(dd);
				Real b = a * 3600.0;
				Real c = vb_round(b - 60.0 * trunc(b / 60.0), 2);
				// FIXME: d is not used, but why?
				// Real d;
				Real e;

				if (c == 60.0) {
					// d = 0;
					e = b + 60.0;
				}
				else {
					// d = c;
					e = b;
				}

				if (dd < 0.0) {
					return -trunc(e / 3600.0);
				}
				else {
					return trunc(e / 3600.0);
				}
			}

			Real DDMin(Real dd) {
				Real a = fabs(dd);
				Real b = a * 3600.0;
				Real c = vb_round(b - 60 * trunc(b / 60.0), 2);
				// Real d;
				Real e;
				if (c == 60.0) {
					// d = 0;
					e = b + 60;
				}
				else {
					// d = c;
					e = b;
				}

				return fmod(trunc(e / 60.0), 60.0);
			}

			Real DDSec(Real dd) {
				Real a = fabs(dd);
				Real b = a * 3600.0;
				Real c = vb_round(b - 60.0 * trunc(b / 60.0), 2);
				Real d;
				if (c == 60.0) {
					d = 0.0;
				}
				else {
					d = c;
				}

				return d;
			}

			Real DDDH(Real dd) {
				return dd / 15.0;
			}

			Real DHDD(Real dh) {
				return dh * 15.0;
			}

			// Function DMSDD(D As Double, M As Double, S As Double) As Double
			//     A = Abs(S) / 60
			//     B = (Abs(M) + A) / 60
			//     C = Abs(D) + B

			//     If ((D < 0) Or (M < 0) Or (S < 0)) Then
			//         DMSDD = -C
			//     Else
			//         DMSDD = C
			//     End If    
			// End Function
			Real DMSDD(Real D, Real M, Real S) {
				Real A = std::abs(S) / 60.0;
				Real B = (std::abs(M) + A) / 60.0;
				Real C = std::abs(D) + B;
				if ((D < 0) || (M < 0) || (S < 0))
					return -C;
				return C;
			}

			Real LctGDay(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY) {
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				Real E = JDCDay(D);
				return Fix(E);
			}

			Real LctGMonth(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY) {
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				return JDCMonth(D);
			}

			Real LctGYear(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY) {
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				return JDCYear(D);
			}

			Real HMSDH(Real H, Real M, Real S) {
				Real A = fabs(S) / 60.0;
				Real B = (fabs(M) + A) / 60.0;
				Real C = fabs(H) + B;

				if (H < 0 || M < 0 || S < 0) {
					return -C;
				}
				else {
					return C;
				}
			}

			Real CDJD(Real day, Real month, Real year) {
				Real Y, M;
				Real A, B, C, D;

				if (month < 3) {
					Y = year - 1;
					M = month + 12;
				}
				else {
					Y = year;
					M = month;
				}

				if (year > 1582) {
					A = trunc(Y / 100.0);
					B = 2.0 - A + trunc(A / 4.0);
				}
				else {
					if (year == 1582 && month > 10) {
						A = trunc(Y / 100.0);
						B = 2.0 - A + trunc(A / 4.0);
					}
					else {
						if (year == 1582 && month == 10 && day >= 15) {
							A = trunc(Y / 100.0);
							B = 2.0 - A + trunc(A / 4.0);
						}
						else {
							B = 0.0;
						}
					}
				}

				if (Y < 0) {
					C = trunc((365.25 * Y) - 0.75);
				}
				else {
					C = trunc(365.25 * Y);
				}

				D = trunc(30.6001 * (M + 1));
				return B + C + D + day + 1720994.5;
			}

			Real CDJD(GreenwichDateTime gdt) {
				return CDJD(gdt.day, gdt.month, gdt.year);
			}

			Real JDCDay(Real JD) {
				Real I = trunc(JD + 0.5);
				Real F = JD + 0.5 - I;
				Real A = trunc((I - 1867216.25) / 36524.25);
				Real B = I;
				if (I > 2299160) {
					B = I + 1 + A - trunc(A / 4.0);
				}
				Real C = B + 1524;
				Real D = trunc((C - 122.1) / 365.25);
				Real E = trunc(365.25 * D);
				Real G = trunc((C - E) / 30.6001);
				return C - E + F - trunc(30.6001 * G);
			}

			Real JDCMonth(Real JD) {
				Real I = trunc(JD + 0.5);
				//FIXME: Real F = JD + 0.5 - I;
				Real A = trunc((I - 1867216.25) / 36524.25);
				Real B = I;
				if (I > 2299160) {
					B = I + 1 + A - trunc(A / 4.0);
				}
				Real C = B + 1524;
				Real D = trunc((C - 122.1) / 365.25);
				Real E = trunc(365.25 * D);
				Real G = trunc((C - E) / 30.6001);

				if (G < 13.5) {
					return G - 1.0;
				}
				else {
					return G - 13.0;
				}
			}

			Real JDCYear(Real JD) {
				Real I = trunc(JD + 0.5);
				//FIXME: Real F = JD + 0.5 - I;
				Real A = trunc((I - 1867216.25) / 36524.25);
				Real B = I;
				if (I > 2299160) {
					B = I + 1 + A - trunc(A / 4.0);
				}
				Real C = B + 1524;
				Real D = trunc((C - 122.1) / 365.25);
				Real E = trunc(365.25 * D);
				Real G = trunc((C - E) / 30.6001);

				Real H = G - 13.0;
				if (G < 13.5) {
					H = G - 1.0;
				}

				if (H > 2.5) {
					return D - 4716;
				}
				else {
					return D - 4715;
				}
			}

			Real LctUT(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY) {
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				Real E = JDCDay(D);
				Real E1 = Fix(E);
				return 24.0 * (E - E1);
			}

			// Function NutatObl(GD As Double, GM As Double, GY As Double) As Double
			//         DJ = CDJD(GD, GM, GY) - 2415020#: T = DJ / 36525#: T2 = T * T
			//         A = 100.0021358 * T: B = 360# * (A - Int(A))
			//         L1 = 279.6967 + 0.000303 * T2 + B: l2 = 2# * Radians(L1)
			//         A = 1336.855231 * T: B = 360# * (A - Int(A))
			//         D1 = 270.4342 - 0.001133 * T2 + B: D2 = 2# * Radians(D1)
			//         A = 99.99736056 * T: B = 360# * (A - Int(A))
			//         M1 = 358.4758 - 0.00015 * T2 + B: M1 = Radians(M1)
			//         A = 1325.552359 * T: B = 360# * (A - Int(A))
			//         M2 = 296.1046 + 0.009192 * T2 + B: M2 = Radians(M2)
			//         A = 5.372616667 * T: B = 360# * (A - Int(A))
			//         N1 = 259.1833 + 0.002078 * T2 - B: N1 = Radians(N1)
			//         N2 = 2 * N1

			//         DDO = (9.21 + 0.00091 * T) * Cos(N1)
			//         DDO = DDO + (0.5522 - 0.00029 * T) * Cos(l2) - 0.0904 * Cos(N2)
			//         DDO = DDO + 0.0884 * Cos(D2) + 0.0216 * Cos(l2 + M1)
			//         DDO = DDO + 0.0183 * Cos(D2 - N1) + 0.0113 * Cos(D2 + M2)
			//         DDO = DDO - 0.0093 * Cos(l2 - M1) - 0.0066 * Cos(l2 - N1)

			//         NutatObl = DDO / 3600#        
			// End Function
			Real NutatObl(Real GD, Real GM, Real GY) {
				Real DJ = CDJD(GD, GM, GY) - 2415020.0; Real T = DJ / 36525.0; Real T2 = T * T;
				Real A = 100.0021358 * T; Real B = 360.0 * (A - int(A));
				Real L1 = 279.6967 + 0.000303 * T2 + B; Real l2 = 2.0 * Radians(L1);
				A = 1336.855231 * T; B = 360.0 * (A - int(A));
				Real D1 = 270.4342 - 0.001133 * T2 + B; Real D2 = 2.0 * Radians(D1);
				A = 99.99736056 * T; B = 360.0 * (A - int(A));
				Real M1 = 358.4758 - 0.00015 * T2 + B; M1 = Radians(M1);
				A = 1325.552359 * T; B = 360.0 * (A - int(A));
				Real M2 = 296.1046 + 0.009192 * T2 + B; M2 = Radians(M2);
				A = 5.372616667 * T; B = 360.0 * (A - int(A));
				Real N1 = 259.1833 + 0.002078 * T2 - B; N1 = Radians(N1);
				Real N2 = 2 * N1;

				Real DDO = (9.21 + 0.00091 * T) * std::cos(N1);
				DDO = DDO + (0.5522 - 0.00029 * T) * std::cos(l2) - 0.0904 * std::cos(N2);
				DDO = DDO + 0.0884 * std::cos(D2) + 0.0216 * std::cos(l2 + M1);
				DDO = DDO + 0.0183 * std::cos(D2 - N1) + 0.0113 * std::cos(D2 + M2);
				DDO = DDO - 0.0093 * std::cos(l2 - M1) - 0.0066 * std::cos(l2 - N1);

				return DDO / 3600.0;
			}

			// Function NutatLong(GD As Double, GM As Double, GY As Double) As Double
			//         DJ = CDJD(GD, GM, GY) - 2415020#: T = DJ / 36525#: T2 = T * T
			//         A = 100.0021358 * T: B = 360# * (A - Int(A))
			//         L1 = 279.6967 + 0.000303 * T2 + B: l2 = 2# * Radians(L1)
			//         A = 1336.855231 * T: B = 360# * (A - Int(A))
			//         D1 = 270.4342 - 0.001133 * T2 + B: D2 = 2# * Radians(D1)
			//         A = 99.99736056 * T: B = 360# * (A - Int(A))
			//         M1 = 358.4758 - 0.00015 * T2 + B: M1 = Radians(M1)
			//         A = 1325.552359 * T: B = 360# * (A - Int(A))
			//         M2 = 296.1046 + 0.009192 * T2 + B: M2 = Radians(M2)
			//         A = 5.372616667 * T: B = 360# * (A - Int(A))
			//         N1 = 259.1833 + 0.002078 * T2 - B: N1 = Radians(N1)
			//         N2 = 2 * N1

			//         DP = (-17.2327 - 0.01737 * T) * Sin(N1)
			//         DP = DP + (-1.2729 - 0.00013 * T) * Sin(l2) + 0.2088 * Sin(N2)
			//         DP = DP - 0.2037 * Sin(D2) + (0.1261 - 0.00031 * T) * Sin(M1)
			//         DP = DP + 0.0675 * Sin(M2) - (0.0497 - 0.00012 * T) * Sin(l2 + M1)
			//         DP = DP - 0.0342 * Sin(D2 - N1) - 0.0261 * Sin(D2 + M2)
			//         DP = DP + 0.0214 * Sin(l2 - M1) - 0.0149 * Sin(l2 - D2 + M2)
			//         DP = DP + 0.0124 * Sin(l2 - N1) + 0.0114 * Sin(D2 - M2)

			//         NutatLong = DP / 3600#
			// End Function
			Real NutatLong(GreenwichDateTime gdt) {
				Real DJ = CDJD(gdt) - 2415020.0; Real T = DJ / 36525.0; Real T2 = T * T;
				Real A = 100.0021358 * T; Real B = 360.0 * (A - Int(A));
				Real L1 = 279.6967 + 0.000303 * T2 + B; Real L2 = 2.0 * Radians(L1);
				A = 1336.855231 * T; B = 360.0 * (A - Int(A));
				Real D1 = 270.4342 - 0.001133 * T2 + B; Real D2 = 2.0 * Radians(D1);
				A = 99.99736056 * T; B = 360.0 * (A - Int(A));
				Real M1 = 358.4758 - 0.00015 * T2 + B; M1 = Radians(M1);
				A = 1325.552359 * T; B = 360.0 * (A - Int(A));
				Real M2 = 296.1046 + 0.009192 * T2 + B; M2 = Radians(M2);
				A = 5.372616667 * T; B = 360.0 * (A - Int(A));
				Real N1 = 259.1833 + 0.002078 * T2 - B; N1 = Radians(N1);
				Real N2 = 2 * N1;

				Real DP = (-17.2327 - 0.01737 * T) * Sin(N1);
				DP = DP + (-1.2729 - 0.00013 * T) * Sin(L2) + 0.2088 * Sin(N2);
				DP = DP - 0.2037 * Sin(D2) + (0.1261 - 0.00031 * T) * Sin(M1);
				DP = DP + 0.0675 * Sin(M2) - (0.0497 - 0.00012 * T) * Sin(L2 + M1);
				DP = DP - 0.0342 * Sin(D2 - N1) - 0.0261 * Sin(D2 + M2);
				DP = DP + 0.0214 * Sin(L2 - M1) - 0.0149 * Sin(L2 - D2 + M2);
				DP = DP + 0.0124 * Sin(L2 - N1) + 0.0114 * Sin(D2 - M2);

				return DP / 3600.0;
			}

			// Function Obliq(GD As Double, GM As Double, GY As Double) As Double
			//     A = CDJD(GD, GM, GY)
			//     B = A - 2415020#
			//     C = (B / 36525#) - 1#
			//     D = C * (46.815 + C * (0.0006 - (C * 0.00181)))
			//     E = D / 3600#
			//     Obliq = 23.43929167 - E + NutatObl(GD, GM, GY)
			// End Function
			Real Obliq(Real GD, Real GM, Real GY) {
				Real A = CDJD(GD, GM, GY);
				Real B = A - 2415020.0;
				Real C = (B / 36525.0) - 1.0;
				Real D = C * (46.815 + C * (0.00006 - (C * 0.00181)));
				Real E = D / 3600.0f;
				return 23.43929167 - E + NutatObl(GD, GM, GY);
			}

			Real ECRA(Real ELD, Real ELM, Real ELS, Real BD, Real BM, Real BS, Real GD, Real GM, Real GY) {
				Real A = Radians(DMSDD(ELD, ELM, ELS));
				Real B = Radians(DMSDD(BD, BM, BS));
				Real C = Radians(Obliq(GD, GM, GY));
				Real D = std::sin(A) * std::cos(C) - std::tan(B) * std::sin(C);
				Real E = std::cos(A);
				Real F = Degrees(Atan2(E, D));
				return F - 360.0f * Int(F / 360.0);
			}

			Real ECdec(Real ELD, Real ELM, Real ELS, Real BD, Real BM, Real BS, Real GD, Real GM, Real GY) {
				Real A = Radians(DMSDD(ELD, ELM, ELS));
				Real B = Radians(DMSDD(BD, BM, BS));
				Real C = Radians(Obliq(GD, GM, GY));
				Real D = std::sin(B) * std::cos(C) + std::cos(B) * std::sin(C) * std::sin(A);
				return Degrees(std::asin(D));
			}

			Real TrueAnomaly(Real AM, Real EC) {
				Real M = AM - FX_TWOPI * trunc(AM / FX_TWOPI);
				Real AE = M;

				Real D = AE - (EC * sin(AE)) - M;
				while (fabs(D) > 0.000001) {
					D = D / (1.0 - (EC * cos(AE)));
					AE = AE - D;
					D = AE - (EC * sin(AE)) - M;
				}
				Real A = sqrt((1.0 + EC) / (1.0 - EC)) * tan(AE / 2.0);
				Real AT = 2.0 * atan(A);
				return AT;
			}

			Real EccentricAnomaly(Real AM, Real EC) {
				Real M = AM - FX_TWOPI * trunc(AM / FX_TWOPI);
				Real AE = M;
				Real D = AE - (EC * sin(AE)) - M;
				while (fabs(D) > 0.000001) {
					D = D / (1.0 - (EC * cos(AE)));
					AE = AE - D;
					D = AE - (EC * sin(AE)) - M;
				}
				return AE;
			}

			Real SunLong(Real DJ, Real UT) {
				Real T = DJ / 36525.0 + UT / 876600.0; // centuries
				Real T2 = T * T;

				Real A, B;
				A = 100.0021359 * T;
				B = 360.0 * (A - trunc(A));
				Real L = 279.69668 + 0.0003025 * T2 + B;
				A = 99.99736042 * T;
				B = 360.0 * (A - trunc(A));
				Real M1 = 358.47583 - (0.00015 + 0.0000033 * T) * T2 + B;
				Real EC = 0.01675104 - 0.0000418 * T - 0.000000126 * T2;

				Real AM = M1 * FX_DEGREES_TO_RADIANS;
				Real AT = TrueAnomaly(AM, EC);
				//FIXME: Real AE = EccentricAnomaly(AM, EC);

				A = 62.55209472 * T;
				B = 360.0 * (A - trunc(A));
				Real A1 = (153.23 + B) * FX_DEGREES_TO_RADIANS;
				A = 125.1041894 * T;
				B = 360.0 + (A - trunc(A));
				Real B1 = (216.57 + B) * FX_DEGREES_TO_RADIANS;
				A = 91.56766028 * T;
				B = 360.0 * (A - trunc(A));
				Real C1 = (312.69 + B) * FX_DEGREES_TO_RADIANS;
				A = 2136.853095 * T;
				B = 360.0 * (A - trunc(A));
				Real D1 = (350.74 - 0.00144 * T2 + B) * FX_DEGREES_TO_RADIANS;
				Real E1 = (231.19 + 20.2 * T) * FX_DEGREES_TO_RADIANS;
				A = 183.1353208 * T;
				B = 360.0 * (A - trunc(A));
				Real H1 = (353.4 + B) * FX_DEGREES_TO_RADIANS;

				Real D2 = 0.00134 * cos(A1) + 0.00154 * cos(B1) + 0.002 * cos(C1);
				D2 += 0.00179 * sin(D1) + 0.00178 * sin(E1);
				Real D3 = 0.00000543 * sin(A1) + 0.00001575 * sin(B1);
				D3 += 0.00001627 * sin(C1) + 0.00003076 * cos(D1);
				D3 += 0.00000927 * sin(H1);

				Real SR = AT + (L - M1 + D2) * FX_DEGREES_TO_RADIANS;
				SR = SR - FX_TWOPI * trunc(SR / FX_TWOPI);
				return SR * FX_RADIANS_TO_DEGREES;
			}

			// Function MoonLong(LH As Double, LM As Double, LS As Double, DS As Double, ZC As Double, DY As Double, MN As Double, YR As Double) As Double
			//         UT = LctUT(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GD = LctGDay(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GM = LctGMonth(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GY = LctGYear(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         T = ((CDJD(GD, GM, GY) - 2415020#) / 36525#) + (UT / 876600#)
			//         T2 = T * T

			//         M1 = 27.32158213: M2 = 365.2596407: M3 = 27.55455094
			//         M4 = 29.53058868: M5 = 27.21222039: M6 = 6798.363307
			//         Q = CDJD(GD, GM, GY) - 2415020# + (UT / 24#)
			//         M1 = Q / M1: M2 = Q / M2: M3 = Q / M3
			//         M4 = Q / M4: M5 = Q / M5: M6 = Q / M6
			//         M1 = 360 * (M1 - Int(M1)): M2 = 360 * (M2 - Int(M2))
			//         M3 = 360 * (M3 - Int(M3)): M4 = 360 * (M4 - Int(M4))
			//         M5 = 360 * (M5 - Int(M5)): M6 = 360 * (M6 - Int(M6))

			//         ML = 270.434164 + M1 - (0.001133 - 0.0000019 * T) * T2
			//         MS = 358.475833 + M2 - (0.00015 + 0.0000033 * T) * T2
			//         MD = 296.104608 + M3 + (0.009192 + 0.0000144 * T) * T2
			//         ME1 = 350.737486 + M4 - (0.001436 - 0.0000019 * T) * T2
			//         MF = 11.250889 + M5 - (0.003211 + 0.0000003 * T) * T2
			//         NA = 259.183275 - M6 + (0.002078 + 0.0000022 * T) * T2
			//         A = Radians(51.2 + 20.2 * T): S1 = Sin(A): S2 = Sin(Radians(NA))
			//         B = 346.56 + (132.87 - 0.0091731 * T) * T
			//         S3 = 0.003964 * Sin(Radians(B))
			//         C = Radians(NA + 275.05 - 2.3 * T): S4 = Sin(C)
			//         ML = ML + 0.000233 * S1 + S3 + 0.001964 * S2
			//         MS = MS - 0.001778 * S1
			//         MD = MD + 0.000817 * S1 + S3 + 0.002541 * S2
			//         MF = MF + S3 - 0.024691 * S2 - 0.004328 * S4
			//         ME1 = ME1 + 0.002011 * S1 + S3 + 0.001964 * S2
			//         E = 1# - (0.002495 + 0.00000752 * T) * T: E2 = E * E
			//         ML = Radians(ML): MS = Radians(MS): NA = Radians(NA)
			//         ME1 = Radians(ME1): MF = Radians(MF): MD = Radians(MD)

			//         L = 6.28875 * Sin(MD) + 1.274018 * Sin(2 * ME1 - MD)
			//         L = L + 0.658309 * Sin(2 * ME1) + 0.213616 * Sin(2 * MD)
			//         L = L - E * 0.185596 * Sin(MS) - 0.114336 * Sin(2 * MF)
			//         L = L + 0.058793 * Sin(2 * (ME1 - MD))
			//         L = L + 0.057212 * E * Sin(2 * ME1 - MS - MD) + 0.05332 * Sin(2 * ME1 + MD)
			//         L = L + 0.045874 * E * Sin(2 * ME1 - MS) + 0.041024 * E * Sin(MD - MS)
			//         L = L - 0.034718 * Sin(ME1) - E * 0.030465 * Sin(MS + MD)
			//         L = L + 0.015326 * Sin(2 * (ME1 - MF)) - 0.012528 * Sin(2 * MF + MD)
			//         L = L - 0.01098 * Sin(2 * MF - MD) + 0.010674 * Sin(4 * ME1 - MD)
			//         L = L + 0.010034 * Sin(3 * MD) + 0.008548 * Sin(4 * ME1 - 2 * MD)
			//         L = L - E * 0.00791 * Sin(MS - MD + 2 * ME1) - E * 0.006783 * Sin(2 * ME1 + MS)
			//         L = L + 0.005162 * Sin(MD - ME1) + E * 0.005 * Sin(MS + ME1)
			//         L = L + 0.003862 * Sin(4 * ME1) + E * 0.004049 * Sin(MD - MS + 2 * ME1)
			//         L = L + 0.003996 * Sin(2 * (MD + ME1)) + 0.003665 * Sin(2 * ME1 - 3 * MD)
			//         L = L + E * 0.002695 * Sin(2 * MD - MS) + 0.002602 * Sin(MD - 2 * (MF + ME1))
			//         L = L + E * 0.002396 * Sin(2 * (ME1 - MD) - MS) - 0.002349 * Sin(MD + ME1)
			//         L = L + E2 * 0.002249 * Sin(2 * (ME1 - MS)) - E * 0.002125 * Sin(2 * MD + MS)
			//         L = L - E2 * 0.002079 * Sin(2 * MS) + E2 * 0.002059 * Sin(2 * (ME1 - MS) - MD)
			//         L = L - 0.001773 * Sin(MD + 2 * (ME1 - MF)) - 0.001595 * Sin(2 * (MF + ME1))
			//         L = L + E * 0.00122 * Sin(4 * ME1 - MS - MD) - 0.00111 * Sin(2 * (MD + MF))
			//         L = L + 0.000892 * Sin(MD - 3 * ME1) - E * 0.000811 * Sin(MS + MD + 2 * ME1)
			//         L = L + E * 0.000761 * Sin(4 * ME1 - MS - 2 * MD)
			//         L = L + E2 * 0.000704 * Sin(MD - 2 * (MS + ME1))
			//         L = L + E * 0.000693 * Sin(MS - 2 * (MD - ME1))
			//         L = L + E * 0.000598 * Sin(2 * (ME1 - MF) - MS)
			//         L = L + 0.00055 * Sin(MD + 4 * ME1) + 0.000538 * Sin(4 * MD)
			//         L = L + E * 0.000521 * Sin(4 * ME1 - MS) + 0.000486 * Sin(2 * MD - ME1)
			//         L = L + E2 * 0.000717 * Sin(MD - 2 * MS)
			//         MM = Unwind(ML + Radians(L))

			//         MoonLong = Degrees(MM)
			// End Function
			Real MoonLong(GreenwichDateTime gdt) {
				//UT = LctUT(LH, LM, LS, DS, ZC, DY, MN, YR);
				//GD = LctGDay(LH, LM, LS, DS, ZC, DY, MN, YR);
				//GM = LctGMonth(LH, LM, LS, DS, ZC, DY, MN, YR);
				//GY = LctGYear(LH, LM, LS, DS, ZC, DY, MN, YR);
				Real T = ((CDJD(gdt) - 2415020.0) / 36525.0) + (gdt.UT / 876600.0);
				Real T2 = T * T;

				Real M1 = 27.32158213; Real M2 = 365.2596407; Real M3 = 27.55455094;
				Real M4 = 29.53058868; Real M5 = 27.21222039; Real M6 = 6798.363307;
				Real Q = CDJD(gdt) - 2415020.0 + (gdt.UT / 24.0);
				M1 = Q / M1; M2 = Q / M2; M3 = Q / M3;
				M4 = Q / M4; M5 = Q / M5; M6 = Q / M6;
				M1 = 360 * (M1 - Int(M1)); M2 = 360 * (M2 - Int(M2));
				M3 = 360 * (M3 - Int(M3)); M4 = 360 * (M4 - Int(M4));
				M5 = 360 * (M5 - Int(M5)); M6 = 360 * (M6 - Int(M6));

				Real ML = 270.434164 + M1 - (0.001133 - 0.0000019 * T) * T2;
				Real MS = 358.475833 + M2 - (0.00015 + 0.0000033 * T) * T2;
				Real MD = 296.104608 + M3 + (0.009192 + 0.0000144 * T) * T2;
				Real ME1 = 350.737486 + M4 - (0.001436 - 0.0000019 * T) * T2;
				Real MF = 11.250889 + M5 - (0.003211 + 0.0000003 * T) * T2;
				Real NA = 259.183275 - M6 + (0.002078 + 0.0000022 * T) * T2;
				Real A = Radians(51.2 + 20.2 * T); Real S1 = Sin(A); Real S2 = Sin(Radians(NA));
				Real B = 346.56 + (132.87 - 0.0091731 * T) * T;
				Real S3 = 0.003964 * Sin(Radians(B));
				Real C = Radians(NA + 275.05 - 2.3 * T); Real S4 = Sin(C);
				ML = ML + 0.000233 * S1 + S3 + 0.001964 * S2;
				MS = MS - 0.001778 * S1;
				MD = MD + 0.000817 * S1 + S3 + 0.002541 * S2;
				MF = MF + S3 - 0.024691 * S2 - 0.004328 * S4;
				ME1 = ME1 + 0.002011 * S1 + S3 + 0.001964 * S2;
				Real E = 1.0 - (0.002495 + 0.00000752 * T) * T; Real E2 = E * E;
				ML = Radians(ML); MS = Radians(MS); NA = Radians(NA);
				ME1 = Radians(ME1); MF = Radians(MF); MD = Radians(MD);

				Real L = 6.28875 * Sin(MD) + 1.274018 * Sin(2 * ME1 - MD);
				L = L + 0.658309 * Sin(2 * ME1) + 0.213616 * Sin(2 * MD);
				L = L - E * 0.185596 * Sin(MS) - 0.114336 * Sin(2 * MF);
				L = L + 0.058793 * Sin(2 * (ME1 - MD));
				L = L + 0.057212 * E * Sin(2 * ME1 - MS - MD) + 0.05332 * Sin(2 * ME1 + MD);
				L = L + 0.045874 * E * Sin(2 * ME1 - MS) + 0.041024 * E * Sin(MD - MS);
				L = L - 0.034718 * Sin(ME1) - E * 0.030465 * Sin(MS + MD);
				L = L + 0.015326 * Sin(2 * (ME1 - MF)) - 0.012528 * Sin(2 * MF + MD);
				L = L - 0.01098 * Sin(2 * MF - MD) + 0.010674 * Sin(4 * ME1 - MD);
				L = L + 0.010034 * Sin(3 * MD) + 0.008548 * Sin(4 * ME1 - 2 * MD);
				L = L - E * 0.00791 * Sin(MS - MD + 2 * ME1) - E * 0.006783 * Sin(2 * ME1 + MS);
				L = L + 0.005162 * Sin(MD - ME1) + E * 0.005 * Sin(MS + ME1);
				L = L + 0.003862 * Sin(4 * ME1) + E * 0.004049 * Sin(MD - MS + 2 * ME1);
				L = L + 0.003996 * Sin(2 * (MD + ME1)) + 0.003665 * Sin(2 * ME1 - 3 * MD);
				L = L + E * 0.002695 * Sin(2 * MD - MS) + 0.002602 * Sin(MD - 2 * (MF + ME1));
				L = L + E * 0.002396 * Sin(2 * (ME1 - MD) - MS) - 0.002349 * Sin(MD + ME1);
				L = L + E2 * 0.002249 * Sin(2 * (ME1 - MS)) - E * 0.002125 * Sin(2 * MD + MS);
				L = L - E2 * 0.002079 * Sin(2 * MS) + E2 * 0.002059 * Sin(2 * (ME1 - MS) - MD);
				L = L - 0.001773 * Sin(MD + 2 * (ME1 - MF)) - 0.001595 * Sin(2 * (MF + ME1));
				L = L + E * 0.00122 * Sin(4 * ME1 - MS - MD) - 0.00111 * Sin(2 * (MD + MF));
				L = L + 0.000892 * Sin(MD - 3 * ME1) - E * 0.000811 * Sin(MS + MD + 2 * ME1);
				L = L + E * 0.000761 * Sin(4 * ME1 - MS - 2 * MD);
				L = L + E2 * 0.000704 * Sin(MD - 2 * (MS + ME1));
				L = L + E * 0.000693 * Sin(MS - 2 * (MD - ME1));
				L = L + E * 0.000598 * Sin(2 * (ME1 - MF) - MS);
				L = L + 0.00055 * Sin(MD + 4 * ME1) + 0.000538 * Sin(4 * MD);
				L = L + E * 0.000521 * Sin(4 * ME1 - MS) + 0.000486 * Sin(2 * MD - ME1);
				L = L + E2 * 0.000717 * Sin(MD - 2 * MS);
				Real MM = Unwind(ML + Radians(L));

				return Degrees(MM);
			}

			// Function MoonLat(LH As Double, LM As Double, LS As Double, DS As Double, ZC As Double, DY As Double, MN As Double, YR As Double) As Double

			//         UT = LctUT(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GD = LctGDay(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GM = LctGMonth(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GY = LctGYear(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         T = ((CDJD(GD, GM, GY) - 2415020#) / 36525#) + (UT / 876600#)
			//         T2 = T * T

			//         M1 = 27.32158213: M2 = 365.2596407: M3 = 27.55455094
			//         M4 = 29.53058868: M5 = 27.21222039: M6 = 6798.363307
			//         Q = CDJD(GD, GM, GY) - 2415020# + (UT / 24#)
			//         M1 = Q / M1: M2 = Q / M2: M3 = Q / M3
			//         M4 = Q / M4: M5 = Q / M5: M6 = Q / M6
			//         M1 = 360 * (M1 - Int(M1)): M2 = 360 * (M2 - Int(M2))
			//         M3 = 360 * (M3 - Int(M3)): M4 = 360 * (M4 - Int(M4))
			//         M5 = 360 * (M5 - Int(M5)): M6 = 360 * (M6 - Int(M6))

			//         ML = 270.434164 + M1 - (0.001133 - 0.0000019 * T) * T2
			//         MS = 358.475833 + M2 - (0.00015 + 0.0000033 * T) * T2
			//         MD = 296.104608 + M3 + (0.009192 + 0.0000144 * T) * T2
			//         ME1 = 350.737486 + M4 - (0.001436 - 0.0000019 * T) * T2
			//         MF = 11.250889 + M5 - (0.003211 + 0.0000003 * T) * T2
			//         NA = 259.183275 - M6 + (0.002078 + 0.0000022 * T) * T2
			//         A = Radians(51.2 + 20.2 * T): S1 = Sin(A): S2 = Sin(Radians(NA))
			//         B = 346.56 + (132.87 - 0.0091731 * T) * T
			//         S3 = 0.003964 * Sin(Radians(B))
			//         C = Radians(NA + 275.05 - 2.3 * T): S4 = Sin(C)
			//         ML = ML + 0.000233 * S1 + S3 + 0.001964 * S2
			//         MS = MS - 0.001778 * S1
			//         MD = MD + 0.000817 * S1 + S3 + 0.002541 * S2
			//         MF = MF + S3 - 0.024691 * S2 - 0.004328 * S4
			//         ME1 = ME1 + 0.002011 * S1 + S3 + 0.001964 * S2
			//         E = 1# - (0.002495 + 0.00000752 * T) * T: E2 = E * E
			//         ML = Radians(ML): MS = Radians(MS): NA = Radians(NA)
			//         ME1 = Radians(ME1): MF = Radians(MF): MD = Radians(MD)

			//         G = 5.128189 * Sin(MF) + 0.280606 * Sin(MD + MF)
			//         G = G + 0.277693 * Sin(MD - MF) + 0.173238 * Sin(2 * ME1 - MF)
			//         G = G + 0.055413 * Sin(2 * ME1 + MF - MD) + 0.046272 * Sin(2 * ME1 - MF - MD)
			//         G = G + 0.032573 * Sin(2 * ME1 + MF) + 0.017198 * Sin(2 * MD + MF)
			//         G = G + 0.009267 * Sin(2 * ME1 + MD - MF) + 0.008823 * Sin(2 * MD - MF)
			//         G = G + E * 0.008247 * Sin(2 * ME1 - MS - MF) + 0.004323 * Sin(2 * (ME1 - MD) - MF)
			//         G = G + 0.0042 * Sin(2 * ME1 + MF + MD) + E * 0.003372 * Sin(MF - MS - 2 * ME1)
			//         G = G + E * 0.002472 * Sin(2 * ME1 + MF - MS - MD)
			//         G = G + E * 0.002222 * Sin(2 * ME1 + MF - MS)
			//         G = G + E * 0.002072 * Sin(2 * ME1 - MF - MS - MD)
			//         G = G + E * 0.001877 * Sin(MF - MS + MD) + 0.001828 * Sin(4 * ME1 - MF - MD)
			//         G = G - E * 0.001803 * Sin(MF + MS) - 0.00175 * Sin(3 * MF)
			//         G = G + E * 0.00157 * Sin(MD - MS - MF) - 0.001487 * Sin(MF + ME1)
			//         G = G - E * 0.001481 * Sin(MF + MS + MD) + E * 0.001417 * Sin(MF - MS - MD)
			//         G = G + E * 0.00135 * Sin(MF - MS) + 0.00133 * Sin(MF - ME1)
			//         G = G + 0.001106 * Sin(MF + 3 * MD) + 0.00102 * Sin(4 * ME1 - MF)
			//         G = G + 0.000833 * Sin(MF + 4 * ME1 - MD) + 0.000781 * Sin(MD - 3 * MF)
			//         G = G + 0.00067 * Sin(MF + 4 * ME1 - 2 * MD) + 0.000606 * Sin(2 * ME1 - 3 * MF)
			//         G = G + 0.000597 * Sin(2 * (ME1 + MD) - MF)
			//         G = G + E * 0.000492 * Sin(2 * ME1 + MD - MS - MF) + 0.00045 * Sin(2 * (MD - ME1) - MF)
			//         G = G + 0.000439 * Sin(3 * MD - MF) + 0.000423 * Sin(MF + 2 * (ME1 + MD))
			//         G = G + 0.000422 * Sin(2 * ME1 - MF - 3 * MD) - E * 0.000367 * Sin(MS + MF + 2 * ME1 - MD)
			//         G = G - E * 0.000353 * Sin(MS + MF + 2 * ME1) + 0.000331 * Sin(MF + 4 * ME1)
			//         G = G + E * 0.000317 * Sin(2 * ME1 + MF - MS + MD)
			//         G = G + E2 * 0.000306 * Sin(2 * (ME1 - MS) - MF) - 0.000283 * Sin(MD + 3 * MF)
			//         W1 = 0.0004664 * Cos(NA): W2 = 0.0000754 * Cos(C)
			//         BM = Radians(G) * (1# - W1 - W2)

			//         MoonLat = Degrees(BM)        
			// End Function
			Real MoonLat(GreenwichDateTime gdt) {
				// Function MoonLat(LH As Double, LM As Double, LS As Double, DS As Double, ZC As Double, DY As Double, MN As Double, YR As Double) As Double

				Real T = ((CDJD(gdt) - 2415020.0) / 36525.0) + (gdt.UT / 876600.0);
				Real T2 = T * T;

				Real M1 = 27.32158213; Real M2 = 365.2596407; Real M3 = 27.55455094;
				Real M4 = 29.53058868; Real M5 = 27.21222039; Real M6 = 6798.363307;
				Real Q = CDJD(gdt) - 2415020.0 + (gdt.UT / 24.0);
				M1 = Q / M1; M2 = Q / M2; M3 = Q / M3;
				M4 = Q / M4; M5 = Q / M5; M6 = Q / M6;
				M1 = 360 * (M1 - Int(M1)); M2 = 360 * (M2 - Int(M2));
				M3 = 360 * (M3 - Int(M3)); M4 = 360 * (M4 - Int(M4));
				M5 = 360 * (M5 - Int(M5)); M6 = 360 * (M6 - Int(M6));

				Real ML = 270.434164 + M1 - (0.001133 - 0.0000019 * T) * T2;
				Real MS = 358.475833 + M2 - (0.00015 + 0.0000033 * T) * T2;
				Real MD = 296.104608 + M3 + (0.009192 + 0.0000144 * T) * T2;
				Real ME1 = 350.737486 + M4 - (0.001436 - 0.0000019 * T) * T2;
				Real MF = 11.250889 + M5 - (0.003211 + 0.0000003 * T) * T2;
				Real NA = 259.183275 - M6 + (0.002078 + 0.0000022 * T) * T2;
				Real A = Radians(51.2 + 20.2 * T); Real S1 = Sin(A); Real S2 = Sin(Radians(NA));
				Real B = 346.56 + (132.87 - 0.0091731 * T) * T;
				Real S3 = 0.003964 * Sin(Radians(B));
				Real C = Radians(NA + 275.05 - 2.3 * T); Real S4 = Sin(C);
				ML = ML + 0.000233 * S1 + S3 + 0.001964 * S2;
				MS = MS - 0.001778 * S1;
				MD = MD + 0.000817 * S1 + S3 + 0.002541 * S2;
				MF = MF + S3 - 0.024691 * S2 - 0.004328 * S4;
				ME1 = ME1 + 0.002011 * S1 + S3 + 0.001964 * S2;
				Real E = 1.0 - (0.002495 + 0.00000752 * T) * T; Real E2 = E * E;
				ML = Radians(ML); MS = Radians(MS); NA = Radians(NA);
				ME1 = Radians(ME1); MF = Radians(MF); MD = Radians(MD);

				Real G = 5.128189 * Sin(MF) + 0.280606 * Sin(MD + MF);
				G = G + 0.277693 * Sin(MD - MF) + 0.173238 * Sin(2 * ME1 - MF);
				G = G + 0.055413 * Sin(2 * ME1 + MF - MD) + 0.046272 * Sin(2 * ME1 - MF - MD);
				G = G + 0.032573 * Sin(2 * ME1 + MF) + 0.017198 * Sin(2 * MD + MF);
				G = G + 0.009267 * Sin(2 * ME1 + MD - MF) + 0.008823 * Sin(2 * MD - MF);
				G = G + E * 0.008247 * Sin(2 * ME1 - MS - MF) + 0.004323 * Sin(2 * (ME1 - MD) - MF);
				G = G + 0.0042 * Sin(2 * ME1 + MF + MD) + E * 0.003372 * Sin(MF - MS - 2 * ME1);
				G = G + E * 0.002472 * Sin(2 * ME1 + MF - MS - MD);
				G = G + E * 0.002222 * Sin(2 * ME1 + MF - MS);
				G = G + E * 0.002072 * Sin(2 * ME1 - MF - MS - MD);
				G = G + E * 0.001877 * Sin(MF - MS + MD) + 0.001828 * Sin(4 * ME1 - MF - MD);
				G = G - E * 0.001803 * Sin(MF + MS) - 0.00175 * Sin(3 * MF);
				G = G + E * 0.00157 * Sin(MD - MS - MF) - 0.001487 * Sin(MF + ME1);
				G = G - E * 0.001481 * Sin(MF + MS + MD) + E * 0.001417 * Sin(MF - MS - MD);
				G = G + E * 0.00135 * Sin(MF - MS) + 0.00133 * Sin(MF - ME1);
				G = G + 0.001106 * Sin(MF + 3 * MD) + 0.00102 * Sin(4 * ME1 - MF);
				G = G + 0.000833 * Sin(MF + 4 * ME1 - MD) + 0.000781 * Sin(MD - 3 * MF);
				G = G + 0.00067 * Sin(MF + 4 * ME1 - 2 * MD) + 0.000606 * Sin(2 * ME1 - 3 * MF);
				G = G + 0.000597 * Sin(2 * (ME1 + MD) - MF);
				G = G + E * 0.000492 * Sin(2 * ME1 + MD - MS - MF) + 0.00045 * Sin(2 * (MD - ME1) - MF);
				G = G + 0.000439 * Sin(3 * MD - MF) + 0.000423 * Sin(MF + 2 * (ME1 + MD));
				G = G + 0.000422 * Sin(2 * ME1 - MF - 3 * MD) - E * 0.000367 * Sin(MS + MF + 2 * ME1 - MD);
				G = G - E * 0.000353 * Sin(MS + MF + 2 * ME1) + 0.000331 * Sin(MF + 4 * ME1);
				G = G + E * 0.000317 * Sin(2 * ME1 + MF - MS + MD);
				G = G + E2 * 0.000306 * Sin(2 * (ME1 - MS) - MF) - 0.000283 * Sin(MD + 3 * MF);
				Real W1 = 0.0004664 * Cos(NA); Real W2 = 0.0000754 * Cos(C);
				Real BM = Radians(G) * (1.0 - W1 - W2);

				return Degrees(BM);
			}

			// Function MoonHP(LH As Double, LM As Double, LS As Double, DS As Double, ZC As Double, DY As Double, MN As Double, YR As Double) As Double

			//         UT = LctUT(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GD = LctGDay(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GM = LctGMonth(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         GY = LctGYear(LH, LM, LS, DS, ZC, DY, MN, YR)
			//         T = ((CDJD(GD, GM, GY) - 2415020#) / 36525#) + (UT / 876600#)
			//         T2 = T * T

			//         M1 = 27.32158213: M2 = 365.2596407: M3 = 27.55455094
			//         M4 = 29.53058868: M5 = 27.21222039: M6 = 6798.363307
			//         Q = CDJD(GD, GM, GY) - 2415020# + (UT / 24#)
			//         M1 = Q / M1: M2 = Q / M2: M3 = Q / M3
			//         M4 = Q / M4: M5 = Q / M5: M6 = Q / M6
			//         M1 = 360 * (M1 - Int(M1)): M2 = 360 * (M2 - Int(M2))
			//         M3 = 360 * (M3 - Int(M3)): M4 = 360 * (M4 - Int(M4))
			//         M5 = 360 * (M5 - Int(M5)): M6 = 360 * (M6 - Int(M6))

			//         ML = 270.434164 + M1 - (0.001133 - 0.0000019 * T) * T2
			//         MS = 358.475833 + M2 - (0.00015 + 0.0000033 * T) * T2
			//         MD = 296.104608 + M3 + (0.009192 + 0.0000144 * T) * T2
			//         ME1 = 350.737486 + M4 - (0.001436 - 0.0000019 * T) * T2
			//         MF = 11.250889 + M5 - (0.003211 + 0.0000003 * T) * T2
			//         NA = 259.183275 - M6 + (0.002078 + 0.0000022 * T) * T2
			//         A = Radians(51.2 + 20.2 * T): S1 = Sin(A): S2 = Sin(Radians(NA))
			//         B = 346.56 + (132.87 - 0.0091731 * T) * T
			//         S3 = 0.003964 * Sin(Radians(B))
			//         C = Radians(NA + 275.05 - 2.3 * T): S4 = Sin(C)
			//         ML = ML + 0.000233 * S1 + S3 + 0.001964 * S2
			//         MS = MS - 0.001778 * S1
			//         MD = MD + 0.000817 * S1 + S3 + 0.002541 * S2
			//         MF = MF + S3 - 0.024691 * S2 - 0.004328 * S4
			//         ME1 = ME1 + 0.002011 * S1 + S3 + 0.001964 * S2
			//         E = 1# - (0.002495 + 0.00000752 * T) * T: E2 = E * E
			//         ML = Radians(ML): MS = Radians(MS): NA = Radians(NA)
			//         ME1 = Radians(ME1): MF = Radians(MF): MD = Radians(MD)

			//         PM = 0.950724 + 0.051818 * Cos(MD) + 0.009531 * Cos(2 * ME1 - MD)
			//         PM = PM + 0.007843 * Cos(2 * ME1) + 0.002824 * Cos(2 * MD)
			//         PM = PM + 0.000857 * Cos(2 * ME1 + MD) + E * 0.000533 * Cos(2 * ME1 - MS)
			//         PM = PM + E * 0.000401 * Cos(2 * ME1 - MD - MS)
			//         PM = PM + E * 0.00032 * Cos(MD - MS) - 0.000271 * Cos(ME1)
			//         PM = PM - E * 0.000264 * Cos(MS + MD) - 0.000198 * Cos(2 * MF - MD)
			//         PM = PM + 0.000173 * Cos(3 * MD) + 0.000167 * Cos(4 * ME1 - MD)
			//         PM = PM - E * 0.000111 * Cos(MS) + 0.000103 * Cos(4 * ME1 - 2 * MD)
			//         PM = PM - 0.000084 * Cos(2 * MD - 2 * ME1) - E * 0.000083 * Cos(2 * ME1 + MS)
			//         PM = PM + 0.000079 * Cos(2 * ME1 + 2 * MD) + 0.000072 * Cos(4 * ME1)
			//         PM = PM + E * 0.000064 * Cos(2 * ME1 - MS + MD) - E * 0.000063 * Cos(2 * ME1 + MS - MD)
			//         PM = PM + E * 0.000041 * Cos(MS + ME1) + E * 0.000035 * Cos(2 * MD - MS)
			//         PM = PM - 0.000033 * Cos(3 * MD - 2 * ME1) - 0.00003 * Cos(MD + ME1)
			//         PM = PM - 0.000029 * Cos(2 * (MF - ME1)) - E * 0.000029 * Cos(2 * MD + MS)
			//         PM = PM + E2 * 0.000026 * Cos(2 * (ME1 - MS)) - 0.000023 * Cos(2 * (MF - ME1) + MD)
			//         PM = PM + E * 0.000019 * Cos(4 * ME1 - MS - MD)

			//         MoonHP = PM

			// End Function
			Real MoonHP(GreenwichDateTime gdt) {
				Real T = ((CDJD(gdt) - 2415020.0) / 36525.0) + (gdt.UT / 876600.0);
				Real T2 = T * T;

				Real M1 = 27.32158213; Real M2 = 365.2596407; Real M3 = 27.55455094;
				Real M4 = 29.53058868; Real M5 = 27.21222039; Real M6 = 6798.363307;
				Real Q = CDJD(gdt) - 2415020.0 + (gdt.UT / 24.0);
				M1 = Q / M1; M2 = Q / M2; M3 = Q / M3;
				M4 = Q / M4; M5 = Q / M5; M6 = Q / M6;
				M1 = 360 * (M1 - Int(M1)); M2 = 360 * (M2 - Int(M2));
				M3 = 360 * (M3 - Int(M3)); M4 = 360 * (M4 - Int(M4));
				M5 = 360 * (M5 - Int(M5)); M6 = 360 * (M6 - Int(M6));

				Real ML = 270.434164 + M1 - (0.001133 - 0.0000019 * T) * T2;
				Real MS = 358.475833 + M2 - (0.00015 + 0.0000033 * T) * T2;
				Real MD = 296.104608 + M3 + (0.009192 + 0.0000144 * T) * T2;
				Real ME1 = 350.737486 + M4 - (0.001436 - 0.0000019 * T) * T2;
				Real MF = 11.250889 + M5 - (0.003211 + 0.0000003 * T) * T2;
				Real NA = 259.183275 - M6 + (0.002078 + 0.0000022 * T) * T2;
				Real A = Radians(51.2 + 20.2 * T); Real S1 = Sin(A); Real S2 = Sin(Radians(NA));
				Real B = 346.56 + (132.87 - 0.0091731 * T) * T;
				Real S3 = 0.003964 * Sin(Radians(B));
				Real C = Radians(NA + 275.05 - 2.3 * T); Real S4 = Sin(C);
				ML = ML + 0.000233 * S1 + S3 + 0.001964 * S2;
				MS = MS - 0.001778 * S1;
				MD = MD + 0.000817 * S1 + S3 + 0.002541 * S2;
				MF = MF + S3 - 0.024691 * S2 - 0.004328 * S4;
				ME1 = ME1 + 0.002011 * S1 + S3 + 0.001964 * S2;
				Real E = 1.0 - (0.002495 + 0.00000752 * T) * T; Real E2 = E * E;
				ML = Radians(ML); MS = Radians(MS); NA = Radians(NA);
				ME1 = Radians(ME1); MF = Radians(MF); MD = Radians(MD);

				Real PM = 0.950724 + 0.051818 * Cos(MD) + 0.009531 * Cos(2 * ME1 - MD);
				PM = PM + 0.007843 * Cos(2 * ME1) + 0.002824 * Cos(2 * MD);
				PM = PM + 0.000857 * Cos(2 * ME1 + MD) + E * 0.000533 * Cos(2 * ME1 - MS);
				PM = PM + E * 0.000401 * Cos(2 * ME1 - MD - MS);
				PM = PM + E * 0.00032 * Cos(MD - MS) - 0.000271 * Cos(ME1);
				PM = PM - E * 0.000264 * Cos(MS + MD) - 0.000198 * Cos(2 * MF - MD);
				PM = PM + 0.000173 * Cos(3 * MD) + 0.000167 * Cos(4 * ME1 - MD);
				PM = PM - E * 0.000111 * Cos(MS) + 0.000103 * Cos(4 * ME1 - 2 * MD);
				PM = PM - 0.000084 * Cos(2 * MD - 2 * ME1) - E * 0.000083 * Cos(2 * ME1 + MS);
				PM = PM + 0.000079 * Cos(2 * ME1 + 2 * MD) + 0.000072 * Cos(4 * ME1);
				PM = PM + E * 0.000064 * Cos(2 * ME1 - MS + MD) - E * 0.000063 * Cos(2 * ME1 + MS - MD);
				PM = PM + E * 0.000041 * Cos(MS + ME1) + E * 0.000035 * Cos(2 * MD - MS);
				PM = PM - 0.000033 * Cos(3 * MD - 2 * ME1) - 0.00003 * Cos(MD + ME1);
				PM = PM - 0.000029 * Cos(2 * (MF - ME1)) - E * 0.000029 * Cos(2 * MD + MS);
				PM = PM + E2 * 0.000026 * Cos(2 * (ME1 - MS)) - 0.000023 * Cos(2 * (MF - ME1) + MD);
				PM = PM + E * 0.000019 * Cos(4 * ME1 - MS - MD);

				return PM;
			}
		} // namespace PA

		AstroCalc::AstroCalc() {
			memset(&gmt_tm, 0, sizeof(struct tm));
			memset(&lct_tm, 0, sizeof(struct tm));
		}

		AstroCalc::AstroCalc(time_t t, Real fractSeconds, Real latitude, Real longitude) {
			Init(t, fractSeconds, latitude, longitude);
		}

		void AstroCalc::Init(time_t t, Real fractSeconds, Real latitude, Real longitude) {
			gmt = t;
			gmt_time_fractSeconds = fractSeconds;

			geo_latitude = clamp(latitude, -90.0, 90.0);
			geo_longitude = wrap(longitude, -180.0, 180.0);
			paTime.geo_latitude = geo_latitude;
			paTime.geo_longitude = geo_longitude;

			// compute constants related to time
			computeTimesAndMatrices();
		}

		void AstroCalc::SetLocation(Real latitude, Real longitude) {
			geo_latitude = clamp(latitude, -90.0, 90.0);
			geo_longitude = wrap(longitude, -180.0, 180.0);
			paTime.geo_latitude = geo_latitude;
			paTime.geo_longitude = geo_longitude;

			computeTimesAndMatrices();
		}

		// day = 1 - 31
		// month = 1 - 12
		// year = 2017 for example
		// isdst = true or false if daylight savings time
		// timezone offset = hours from GMT
		void AstroCalc::SetDate(int day, int month, int year, bool isdst, int timeZoneOffset) {
			paTime.day = day;
			paTime.month = month;
			paTime.year = year;
			paTime.isdst = isdst;
			paTime.timeZoneOffset = timeZoneOffset;

			computeTimesAndMatrices();
		}

		void AstroCalc::SetTime(int hh, int mm, int ss, Real ss_frac) {
			paTime.hh = hh;
			paTime.mm = mm;
			paTime.ss = ss;
			paTime.ss_frac = ss_frac;

			computeTimesAndMatrices();
		}

		void AstroCalc::SetDateTime(int day, int month, int year, bool isdst, int timeZoneOffset, int hh, int mm, int ss, Real ss_frac) {
			paTime.day = day;
			paTime.month = month;
			paTime.year = year;
			paTime.isdst = isdst;
			paTime.timeZoneOffset = timeZoneOffset;
			paTime.hh = hh;
			paTime.mm = mm;
			paTime.ss = ss;
			paTime.ss_frac = ss_frac;

			computeTimesAndMatrices();
		}

		time_t AstroCalc::GetTime() const {
			return paTime.getTime();
		}

		void AstroCalc::SetTime(time_t t, Real fractSeconds) {
			t += (time_t)trunc(fractSeconds);
			fractSeconds -= trunc(fractSeconds);
			if (fractSeconds < 0.0f) {
				t--;
				fractSeconds += 1;
			}

			// date without time of day
			gmt_date = (t / 86400) * 86400;
			// time in seconds in the day
			gmt_time = t % 86400;
			gmt_time_fractSeconds = fractSeconds;
			julian_date = gmt_date / 86400.0 + J1970_0_0;
			julian_time = (gmt_time + fractSeconds) / 3600.0;
			julian_datetime = julian_date + julian_time / 24.0;

			// Get local and gmt time from the system based on the input time.
			struct tm* ptm = nullptr;
			gmt = t;
#ifdef __STDC_SECURE_LIB__
			ptm = &gmt_tm;
			gmtime_s(ptm, &gmt);
			gmt = mktime(ptm);
#else
			ptm = gmtime(&gmt);
			memset(&gmt_tm, 0, sizeof(tm));
			memcpy(&gmt_tm, ptm, sizeof(tm));
#endif

			lct = t;
#ifdef __STDC_SECURE_LIB__
			ptm = &lct_tm;
			localtime_s(ptm, &lct);
#else
			ptm = localtime(&lct);
			memset(&lct_tm, 0, sizeof(tm));
			memcpy(&lct_tm, ptm, sizeof(tm));
#endif

			int timeZoneOffset = lct_tm.tm_hour - gmt_tm.tm_hour;
			if (gmt > lct) {
				if (gmt_tm.tm_hour < lct_tm.tm_hour) {
					timeZoneOffset -= 24;
				}
		}
			else {
				if (gmt_tm.tm_hour > lct_tm.tm_hour) {
					timeZoneOffset += 24;
				}
			}

			// init_time(t, fractSeconds);
			// init_matrices();

			SetDateTime(lct_tm.tm_mday, lct_tm.tm_mon + 1, lct_tm.tm_year + 1900, lct_tm.tm_isdst ? 1 : 0, timeZoneOffset, lct_tm.tm_hour, lct_tm.tm_min, lct_tm.tm_sec, fractSeconds);
			computeTimesAndMatrices();
	}

		void AstroCalc::computeTimes() {
			UT = paTime.UT();
			JD = paTime.JD();
			LST = paTime.LST();
			GST = paTime.GST();
		}

		void AstroCalc::computeMatrices() {
			// compute obliquity of the ecliptic

			// centuries
			Real T = (JD - J2000_1_5) / 36525.0;
			// degrees
			Real DE = T * (46.815 + T * (0.0006 - (T * 0.00181))) / 3600.0; // = (46.815 * T + 0.0006 * T*T - 0.00181*T*T*T) / 3600.0;
			// degrees
			obliquity_of_the_ecliptic = 23.439292 - DE;

			Real phi = paTime.geo_latitude * FX_DEGREES_TO_RADIANS;
			Real ST = LST * 15.0 * FX_DEGREES_TO_RADIANS;
			Real e = obliquity_of_the_ecliptic * FX_DEGREES_TO_RADIANS;

			horizon_to_hequatorial_matrix = Matrix(
				-sin(phi), 0.0, cos(phi),
				0.0, -1.0, 0.0,
				cos(phi), 0.0, sin(phi));
			hequatorial_to_horizon_matrix = Matrix(
				-sin(phi), 0.0, cos(phi),
				0.0, -1.0, 0.0,
				cos(phi), 0.0, sin(phi));
			equatorial_to_hequatorial_matrix = Matrix(
				cos(ST), sin(ST), 0.0,
				sin(ST), -cos(ST), 0.0,
				0.0, 0.0, 1.0);
			hequatorial_to_equatorial_matrix = Matrix(
				cos(ST), sin(ST), 0.0,
				sin(ST), -cos(ST), 0.0,
				0.0, 0.0, 1.0);
			equatorial_to_ecliptic_matrix = Matrix(
				1.0, 0.0, 0.0,
				0.0, cos(e), sin(e),
				0.0, -sin(e), cos(e));
			ecliptic_to_equatorial_matrix = Matrix(
				1.0, 0.0, 0.0,
				0.0, cos(e), -sin(e),
				0.0, sin(e), cos(e));
			equatorial_to_galactic_matrix = Matrix(
				-0.0669887, -0.8727558, -0.4835389,
				0.4927285, -0.4503470, 0.7445846,
				-0.8676008, -0.1883746, 0.4601998);
			galactic_to_equatorial_matrix = Matrix(
				-0.0669888, 0.4927285, -0.8676008,
				-0.8727557, -0.4503469, -0.1883746,
				-0.4835389, 0.7445846, 0.4601998);

			equatorial_to_horizon_matrix = hequatorial_to_horizon_matrix.times(equatorial_to_hequatorial_matrix);
			horizon_to_equatorial_matrix = hequatorial_to_equatorial_matrix.times(horizon_to_hequatorial_matrix);
			horizon_to_ecliptic_matrix = equatorial_to_ecliptic_matrix.times(horizon_to_equatorial_matrix);
			ecliptic_to_horizon_matrix = equatorial_to_horizon_matrix.times(ecliptic_to_equatorial_matrix);
			horizon_to_galactic_matrix = equatorial_to_galactic_matrix.times(horizon_to_equatorial_matrix);
			galactic_to_horizon_matrix = equatorial_to_horizon_matrix.times(galactic_to_equatorial_matrix);
			hequatorial_to_ecliptic_matrix = equatorial_to_ecliptic_matrix.times(hequatorial_to_equatorial_matrix);
			ecliptic_to_hequatorial_matrix = equatorial_to_hequatorial_matrix.times(ecliptic_to_equatorial_matrix);
			ecliptic_to_galactic_matrix = equatorial_to_galactic_matrix.times(ecliptic_to_equatorial_matrix);
			hequatorial_to_galactic_matrix = equatorial_to_galactic_matrix.times(hequatorial_to_equatorial_matrix);
			galactic_to_hequatorial_matrix = equatorial_to_hequatorial_matrix.times(galactic_to_equatorial_matrix);
			galactic_to_ecliptic_matrix = equatorial_to_ecliptic_matrix.times(galactic_to_equatorial_matrix);
		}

		void AstroCalc::computeTimesAndMatrices() {
			computeTimes();
			computeMatrices();
		}

		void AstroCalc::generic_transformation(Real mu_in, Real nu_in, const Matrix& m, Real& mu_out, Real& nu_out) const {
			Vector v(mu_in, nu_in);
			Vector w = m.times(v);
			w.toAngles(mu_out, nu_out);
		}

		GalacticCoord AstroCalc::ecliptic_to_galactic(const EclipticCoord& p) const {
			GalacticCoord out;
			generic_transformation(p.lambda, p.beta, ecliptic_to_galactic_matrix, out.l, out.b);
			return out;
		}

		GalacticCoord AstroCalc::hequatorial_to_galactic(const HEquatorialCoord& p) const {
			GalacticCoord out;
			generic_transformation(p.H, p.delta, hequatorial_to_galactic_matrix, out.l, out.b);
			return out;
		}

		HEquatorialCoord AstroCalc::galactic_to_hequatorial(const GalacticCoord& p) const {
			HEquatorialCoord out;
			generic_transformation(p.l, p.b, galactic_to_hequatorial_matrix, out.H, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::galactic_to_ecliptic(const GalacticCoord& p) const {
			EclipticCoord out;
			generic_transformation(p.l, p.b, galactic_to_ecliptic_matrix, out.lambda, out.beta);
			return out;
		}

		HorizonCoord AstroCalc::hequatorial_to_horizon(const HEquatorialCoord& p) const {
			HorizonCoord out;
			Vector v(p.H, p.delta);
			Vector w = hequatorial_to_horizon_matrix.times(v);
			w.toAngles(out.A, out.a);
			//out.A = wrap(out.A, 360.0);
			//out.a = wrap(out.a, 360.0);
			return out;
		}

		EclipticCoord AstroCalc::hequatorial_to_ecliptic(const HEquatorialCoord& p) const {
			EclipticCoord out;
			generic_transformation(p.H, p.delta, hequatorial_to_ecliptic_matrix, out.lambda, out.beta);
			return out;
		}

		HEquatorialCoord AstroCalc::ecliptic_to_hequatorial(const EclipticCoord& p) const {
			HEquatorialCoord out;
			generic_transformation(p.lambda, p.beta, ecliptic_to_hequatorial_matrix, out.H, out.delta);
			return out;
		}

		HEquatorialCoord AstroCalc::horizon_to_hequatorial(const HorizonCoord& p) const {
			HEquatorialCoord out;
			Vector v(p.A, p.a);
			Vector w = horizon_to_hequatorial_matrix.times(v);
			w.toAngles(out.H, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::horizon_to_ecliptic(const HorizonCoord& p) const {
			EclipticCoord out;
			generic_transformation(p.A, p.a, horizon_to_galactic_matrix, out.lambda, out.beta);
			return out;
		}

		GalacticCoord AstroCalc::horizon_to_galactic(const HorizonCoord& p) const {
			GalacticCoord out;
			generic_transformation(p.A, p.a, horizon_to_galactic_matrix, out.l, out.b);
			return out;
		}

		HorizonCoord AstroCalc::ecliptic_to_horizon(const EclipticCoord& p) const {
			HorizonCoord out;
			generic_transformation(p.lambda, p.beta, ecliptic_to_horizon_matrix, out.A, out.a);
			out.A = wrap(out.A, 360.0);
			out.a = wrap(out.a, -180.0, 180.0);
			return out;
		}

		HorizonCoord AstroCalc::galactic_to_horizon(const GalacticCoord& p) const {
			HorizonCoord out;
			generic_transformation(p.l, p.b, ecliptic_to_horizon_matrix, out.A, out.a);
			out.A = wrap(out.A, 360.0);
			out.a = wrap(out.a, -180.0, 180.0);
			return out;
		}

		GalacticCoord AstroCalc::equatorial_to_galactic(const EquatorialCoord& p) const {
			GalacticCoord out;
			generic_transformation(p.alpha, p.delta, equatorial_to_galactic_matrix, out.l, out.b);
			return out;
		}

		EquatorialCoord AstroCalc::galactic_to_equatorial(const GalacticCoord& p) const {
			EquatorialCoord out;
			generic_transformation(p.l, p.b, galactic_to_equatorial_matrix, out.alpha, out.delta);
			return out;
		}

		HorizonCoord AstroCalc::equatorial_to_horizon(const EquatorialCoord& p) const {
			HorizonCoord out;
			Vector v(p.alpha, p.delta);
			Vector w = equatorial_to_horizon_matrix.times(v);
			w.toAngles(out.A, out.a);
			out.A = wrap(out.A, 360.0);
			out.a = wrap(out.a, 360.0);
			return out;
		}

		EquatorialCoord AstroCalc::horizon_to_equatorial(const HorizonCoord& p) const {
			EquatorialCoord out;
			Vector v(p.A, p.a);
			Vector w = horizon_to_equatorial_matrix.times(v);
			w.toAngles(out.alpha, out.delta);
			return out;
		}

		HEquatorialCoord AstroCalc::equatorial_to_hequatorial(const EquatorialCoord& p) const {
			HEquatorialCoord out;
			Vector v(p.alpha, p.delta);
			Vector w = equatorial_to_hequatorial_matrix.times(v);
			w.toAngles(out.H, out.delta);
			return out;
		}

		EquatorialCoord AstroCalc::hequatorial_to_equatorial(const HEquatorialCoord& p) const {
			EquatorialCoord out;
			Vector v(p.H, p.delta);
			Vector w = hequatorial_to_equatorial_matrix.times(v);
			w.toAngles(out.alpha, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::equatorial_to_ecliptic(const EquatorialCoord& p) const {
			EclipticCoord out;
			Vector v(p.alpha, p.delta);
			Vector w = equatorial_to_ecliptic_matrix.times(v);
			w.toAngles(out.lambda, out.beta);
			return out;
		}

		EquatorialCoord AstroCalc::ecliptic_to_equatorial(const EclipticCoord& p) const {
			EquatorialCoord out;
			Vector v(p.lambda, p.beta);
			Vector w = ecliptic_to_equatorial_matrix.times(v);
			w.toAngles(out.alpha, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::getSun() const {
			Real DJ = JD - 2415020.0;
			Real sunLong = PA::SunLong(DJ, UT);

			EclipticCoord sun(sunLong, 0.0);

			return sun;
		}

		EquatorialCoord AstroCalc::getMoon() const {
			PA::GreenwichDateTime gdt = paTime.getGreenwich();
			Real moonEclLong = PA::MoonLong(gdt);
			Real moonEclLat = PA::MoonLat(gdt);
			Real moonNutationLong = PA::NutatLong(gdt);
			Real moonCorrectedLong = moonEclLong + moonNutationLong;
			Real moonHorzParallax = PA::MoonHP(gdt);
			Real earthMoonDistance = 6378.14 / std::sin(FX_DEGREES_TO_RADIANS * moonHorzParallax);
			Real RA = PA::DDDH(PA::ECRA(moonCorrectedLong, 0, 0, moonEclLat, 0, 0, gdt.day, gdt.month, gdt.year));
			Real dec = PA::ECdec(moonCorrectedLong, 0, 0, moonEclLat, 0, 0, gdt.day, gdt.month, gdt.year);
			return { RA, dec };
		}

		Real AstroCalc::kepler(Real M, Real e) const {
			Real E = M;
			Real epsilon = 10e-6;
			Real delta = E - e * sin(E) - M;
			do {
				E = E - delta / (1.0 - e * cos(E));
				delta = E - e * sin(E) - M;
			} while (fabs(delta) >= epsilon);
			return E;
		}
} // namespace Astronomy
} // namespace Fluxions
