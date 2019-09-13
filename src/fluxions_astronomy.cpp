// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com

#pragma warning(disable:26451)

#include "pch.hpp"
#include <math.h>
#include <assert.h>
#include <memory.h>
#include <cmath>
#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_astronomy.hpp>

namespace Fluxions
{

	namespace Astronomy
	{
		// A S T R O N O M I C A L   C O N S T A N T S ///////////////////

		constexpr Real MJD_offset = -2400000.5;
		constexpr Real J1970_0_0 = 2440587.5;
		constexpr Real J2000_0_0 = 2451543.5;
		constexpr Real J2000_1_5 = 2451545.0;
		constexpr Real J2010_0_0 = 2455196.5;

		// U T I L I T Y   F U N C T I O N S ////////////////////////////////

		// Wraps the variable X in the half open domain [a, b)
		Real wrap(Real x, Real a, Real b)
		{
			assert(a < b);
			Real out = fmod(x - a, b - a) + a;
			if (out < a)
				out += b - a;
			return out;
		}

		// same as wrap(X, a, b), but with a = 0.0
		Real wrap(Real x, Real b)
		{
			assert(b != 0.0);
			Real out = fmod(x, b);
			if (out < 0.0)
				out += b;
			return out;
		}

		// Converts decimal degrees, minutes, seconds to decimal degrees
		Real ddmmss_to_degrees(Real dd, Real mm, Real ss)
		{
			return dd + mm / 60.0 + ss / 3600.0;
		}

		Real clamp(Real x, Real a, Real b)
		{
			if (x < a)
				return a;
			if (x > b)
				return b;
			return x;
		}

		Real vb_round(Real x, int numDecimalPlaces)
		{
			Real z = pow(10.0, numDecimalPlaces);
			Real y = trunc(round(x * z) / z);
			return y;
		}

		Real vb_fix(Real x)
		{
			return trunc(x);
		}

		namespace PA
		{
			Real CivilDateTime::GST()
			{
				Real julian = JD();
				Real s = julian - 2451545.0;
				Real t = s / 36525.0;
				Real t0 = wrap(6.697374558 + t * (2400.051336 + (0.000025862 * t)), 24.0);
				Real ut = 1.002737909 * UT();
				return wrap(t0 + ut, 24.0);
			}

			Real CivilDateTime::LST()
			{
				return wrap(geo_longitude / 15.0 + GST(), 24.0);
			}

			Real CivilDateTime::JD()
			{
				Real Y, M;
				Real A, B, C, D;

				if (month < 3)
				{
					Y = year - 1;
					M = month + 12;
				}
				else
				{
					Y = year;
					M = month;
				}

				if (year > 1582)
				{
					A = trunc(Y / 100.0);
					B = 2.0 - A + trunc(A / 4.0);
				}
				else
				{
					if (year == 1582 && month > 10)
					{
						A = trunc(Y / 100.0);
						B = 2.0 - A + trunc(A / 4.0);
					}
					else
					{
						if (year == 1582 && month == 10 && day >= 15)
						{
							A = trunc(Y / 100.0);
							B = 2.0 - A + trunc(A / 4.0);
						}
						else
						{
							B = 0.0;
						}
					}
				}

				if (Y < 0)
				{
					C = trunc((365.25 * Y) - 0.75);
				}
				else
				{
					C = trunc(365.25 * Y);
				}

				D = trunc(30.6001 * (M + 1));
				return B + C + D + day + 1720994.5;
			}

			Real CivilDateTime::UT()
			{
				Real DS = isdst ? 1.0 : 0.0;
				Real A = HMSDH(hh, mm, ss + ss_frac);
				Real B = A - DS - timeZoneOffset;
				Real C = day + (B / 24.0);
				Real D = CDJD(C, month, year);
				Real E = JDCDay(D);
				Real E1 = trunc(E);
				return 24.0 * (E - E1);
			}

			time_t CivilDateTime::GetTime() const
			{
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

			Real DDDeg(Real dd)
			{
				Real a = fabs(dd);
				Real b = a * 3600.0;
				Real c = vb_round(b - 60.0 * trunc(b / 60.0), 2);
				// FIXME: d is not used, but why?
				// Real d;
				Real e;

				if (c == 60.0)
				{
					// d = 0;
					e = b + 60.0;
				}
				else
				{
					// d = c;
					e = b;
				}

				if (dd < 0.0)
				{
					return -trunc(e / 3600.0);
				}
				else
				{
					return trunc(e / 3600.0);
				}
			}

			Real DDMin(Real dd)
			{
				Real a = fabs(dd);
				Real b = a * 3600.0;
				Real c = vb_round(b - 60 * trunc(b / 60.0), 2);
				// Real d;
				Real e;
				if (c == 60.0)
				{
					// d = 0;
					e = b + 60;
				}
				else
				{
					// d = c;
					e = b;
				}

				return fmod(trunc(e / 60.0), 60.0);
			}

			Real DDSec(Real dd)
			{
				Real a = fabs(dd);
				Real b = a * 3600.0;
				Real c = vb_round(b - 60.0 * trunc(b / 60.0), 2);
				Real d;
				if (c == 60.0)
				{
					d = 0.0;
				}
				else
				{
					d = c;
				}

				return d;
			}

			Real DDDH(Real dd)
			{
				return dd / 15.0;
			}

			Real DHDD(Real dh)
			{
				return dh * 15.0;
			}

			Real LctGDay(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY)
			{
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				Real E = JDCDay(D);
				return trunc(E);
			}

			Real LctGMonth(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY)
			{
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				return JDCMonth(D);
			}

			Real LctGYear(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY)
			{
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				return JDCYear(D);
			}

			Real HMSDH(Real H, Real M, Real S)
			{
				Real A = fabs(S) / 60.0;
				Real B = (fabs(M) + A) / 60.0;
				Real C = fabs(H) + B;

				if (H < 0 || M < 0 || S < 0)
				{
					return -C;
				}
				else
				{
					return C;
				}
			}

			Real CDJD(Real day, Real month, Real year)
			{
				Real Y, M;
				Real A, B, C, D;

				if (month < 3)
				{
					Y = year - 1;
					M = month + 12;
				}
				else
				{
					Y = year;
					M = month;
				}

				if (year > 1582)
				{
					A = trunc(Y / 100.0);
					B = 2.0 - A + trunc(A / 4.0);
				}
				else
				{
					if (year == 1582 && month > 10)
					{
						A = trunc(Y / 100.0);
						B = 2.0 - A + trunc(A / 4.0);
					}
					else
					{
						if (year == 1582 && month == 10 && day >= 15)
						{
							A = trunc(Y / 100.0);
							B = 2.0 - A + trunc(A / 4.0);
						}
						else
						{
							B = 0.0;
						}
					}
				}

				if (Y < 0)
				{
					C = trunc((365.25 * Y) - 0.75);
				}
				else
				{
					C = trunc(365.25 * Y);
				}

				D = trunc(30.6001 * (M + 1));
				return B + C + D + day + 1720994.5;
			}

			Real JDCDay(Real JD)
			{
				Real I = trunc(JD + 0.5);
				Real F = JD + 0.5 - I;
				Real A = trunc((I - 1867216.25) / 36524.25);
				Real B = I;
				if (I > 2299160)
				{
					B = I + 1 + A - trunc(A / 4.0);
				}
				Real C = B + 1524;
				Real D = trunc((C - 122.1) / 365.25);
				Real E = trunc(365.25 * D);
				Real G = trunc((C - E) / 30.6001);
				return C - E + F - trunc(30.6001 * G);
			}

			Real JDCMonth(Real JD)
			{
				Real I = trunc(JD + 0.5);
				//FIXME: Real F = JD + 0.5 - I;
				Real A = trunc((I - 1867216.25) / 36524.25);
				Real B = I;
				if (I > 2299160)
				{
					B = I + 1 + A - trunc(A / 4.0);
				}
				Real C = B + 1524;
				Real D = trunc((C - 122.1) / 365.25);
				Real E = trunc(365.25 * D);
				Real G = trunc((C - E) / 30.6001);

				if (G < 13.5)
				{
					return G - 1.0;
				}
				else
				{
					return G - 13.0;
				}
			}

			Real JDCYear(Real JD)
			{
				Real I = trunc(JD + 0.5);
				//FIXME: Real F = JD + 0.5 - I;
				Real A = trunc((I - 1867216.25) / 36524.25);
				Real B = I;
				if (I > 2299160)
				{
					B = I + 1 + A - trunc(A / 4.0);
				}
				Real C = B + 1524;
				Real D = trunc((C - 122.1) / 365.25);
				Real E = trunc(365.25 * D);
				Real G = trunc((C - E) / 30.6001);

				Real H = G - 13.0;
				if (G < 13.5)
				{
					H = G - 1.0;
				}

				if (H > 2.5)
				{
					return D - 4716;
				}
				else
				{
					return D - 4715;
				}
			}

			Real LctUT(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY)
			{
				Real A = HMSDH(LCH, LCM, LCS);
				Real B = A - DS - ZC;
				Real C = LD + (B / 24.0);
				Real D = CDJD(C, LM, LY);
				Real E = JDCDay(D);
				Real E1 = trunc(E);
				return 24.0 * (E - E1);
			}

			Real TrueAnomaly(Real AM, Real EC)
			{
				Real M = AM - FX_TWOPI * trunc(AM / FX_TWOPI);
				Real AE = M;

				Real D = AE - (EC * sin(AE)) - M;
				while (fabs(D) > 0.000001)
				{
					D = D / (1.0 - (EC * cos(AE)));
					AE = AE - D;
					D = AE - (EC * sin(AE)) - M;
				}
				Real A = sqrt((1.0 + EC) / (1.0 - EC)) * tan(AE / 2.0);
				Real AT = 2.0 * atan(A);
				return AT;
			}

			Real EccentricAnomaly(Real AM, Real EC)
			{
				Real M = AM - FX_TWOPI * trunc(AM / FX_TWOPI);
				Real AE = M;
				Real D = AE - (EC * sin(AE)) - M;
				while (fabs(D) > 0.000001)
				{
					D = D / (1.0 - (EC * cos(AE)));
					AE = AE - D;
					D = AE - (EC * sin(AE)) - M;
				}
				return AE;
			}

			Real SunLong(Real DJ, Real UT)
			{
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
		} // namespace PA

		AstroCalc::AstroCalc()
		{
			memset(&gmt_tm, 0, sizeof(struct tm));
			memset(&lct_tm, 0, sizeof(struct tm));
		}

		AstroCalc::AstroCalc(time_t t, Real fractSeconds, Real latitude, Real longitude)
		{
			Init(t, fractSeconds, latitude, longitude);
		}

		void AstroCalc::Init(time_t t, Real fractSeconds, Real latitude, Real longitude)
		{
			gmt = t;
			gmt_time_fractSeconds = fractSeconds;

			geo_latitude = clamp(latitude, -90.0, 90.0);
			geo_longitude = wrap(longitude, -180.0, 180.0);
			paTime.geo_latitude = geo_latitude;
			paTime.geo_longitude = geo_longitude;

			// compute constants related to time
			computeTimesAndMatrices();
		}

		void AstroCalc::SetLocation(Real latitude, Real longitude)
		{
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
		void AstroCalc::SetDate(int day, int month, int year, bool isdst, int timeZoneOffset)
		{
			paTime.day = day;
			paTime.month = month;
			paTime.year = year;
			paTime.isdst = isdst;
			paTime.timeZoneOffset = timeZoneOffset;

			computeTimesAndMatrices();
		}

		void AstroCalc::SetTime(int hh, int mm, int ss, Real ss_frac)
		{
			paTime.hh = hh;
			paTime.mm = mm;
			paTime.ss = ss;
			paTime.ss_frac = ss_frac;

			computeTimesAndMatrices();
		}

		void AstroCalc::SetDateTime(int day, int month, int year, bool isdst, int timeZoneOffset, int hh, int mm, int ss, Real ss_frac)
		{
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

		time_t AstroCalc::GetTime() const
		{
			return paTime.GetTime();
		}

		void AstroCalc::SetTime(time_t t, Real fractSeconds)
		{
			t += (time_t)trunc(fractSeconds);
			fractSeconds -= trunc(fractSeconds);
			if (fractSeconds < 0.0f)
			{
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
			if (gmt > lct)
			{
				if (gmt_tm.tm_hour < lct_tm.tm_hour)
				{
					timeZoneOffset -= 24;
				}
			}
			else
			{
				if (gmt_tm.tm_hour > lct_tm.tm_hour)
				{
					timeZoneOffset += 24;
				}
			}

			// init_time(t, fractSeconds);
			// init_matrices();

			SetDateTime(lct_tm.tm_mday, lct_tm.tm_mon + 1, lct_tm.tm_year + 1900, lct_tm.tm_isdst ? 1 : 0, timeZoneOffset, lct_tm.tm_hour, lct_tm.tm_min, lct_tm.tm_sec, fractSeconds);
			computeTimesAndMatrices();
		}

		void AstroCalc::computeTimes()
		{
			UT = paTime.UT();
			JD = paTime.JD();
			LST = paTime.LST();
			GST = paTime.GST();
		}

		void AstroCalc::computeMatrices()
		{
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

		void AstroCalc::computeTimesAndMatrices()
		{
			computeTimes();
			computeMatrices();
		}


		void AstroCalc::generic_transformation(Real mu_in, Real nu_in, const Matrix& m, Real& mu_out, Real& nu_out) const
		{
			Vector v(mu_in, nu_in);
			Vector w = m.times(v);
			w.toAngles(mu_out, nu_out);
		}

		GalacticCoord AstroCalc::ecliptic_to_galactic(const EclipticCoord& p) const
		{
			GalacticCoord out;
			generic_transformation(p.lambda, p.beta, ecliptic_to_galactic_matrix, out.l, out.b);
			return out;
		}

		GalacticCoord AstroCalc::hequatorial_to_galactic(const HEquatorialCoord& p) const
		{
			GalacticCoord out;
			generic_transformation(p.H, p.delta, hequatorial_to_galactic_matrix, out.l, out.b);
			return out;
		}

		HEquatorialCoord AstroCalc::galactic_to_hequatorial(const GalacticCoord& p) const
		{
			HEquatorialCoord out;
			generic_transformation(p.l, p.b, galactic_to_hequatorial_matrix, out.H, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::galactic_to_ecliptic(const GalacticCoord& p) const
		{
			EclipticCoord out;
			generic_transformation(p.l, p.b, galactic_to_ecliptic_matrix, out.lambda, out.beta);
			return out;
		}

		HorizonCoord AstroCalc::hequatorial_to_horizon(const HEquatorialCoord& p) const
		{
			HorizonCoord out;
			Vector v(p.H, p.delta);
			Vector w = hequatorial_to_horizon_matrix.times(v);
			w.toAngles(out.A, out.a);
			//out.A = wrap(out.A, 360.0);
			//out.a = wrap(out.a, 360.0);
			return out;
		}

		EclipticCoord AstroCalc::hequatorial_to_ecliptic(const HEquatorialCoord& p) const
		{
			EclipticCoord out;
			generic_transformation(p.H, p.delta, hequatorial_to_ecliptic_matrix, out.lambda, out.beta);
			return out;
		}

		HEquatorialCoord AstroCalc::ecliptic_to_hequatorial(const EclipticCoord& p) const
		{
			HEquatorialCoord out;
			generic_transformation(p.lambda, p.beta, ecliptic_to_hequatorial_matrix, out.H, out.delta);
			return out;
		}

		HEquatorialCoord AstroCalc::horizon_to_hequatorial(const HorizonCoord& p) const
		{
			HEquatorialCoord out;
			Vector v(p.A, p.a);
			Vector w = horizon_to_hequatorial_matrix.times(v);
			w.toAngles(out.H, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::horizon_to_ecliptic(const HorizonCoord& p) const
		{
			EclipticCoord out;
			generic_transformation(p.A, p.a, horizon_to_galactic_matrix, out.lambda, out.beta);
			return out;
		}

		GalacticCoord AstroCalc::horizon_to_galactic(const HorizonCoord& p) const
		{
			GalacticCoord out;
			generic_transformation(p.A, p.a, horizon_to_galactic_matrix, out.l, out.b);
			return out;
		}

		HorizonCoord AstroCalc::ecliptic_to_horizon(const EclipticCoord& p) const
		{
			HorizonCoord out;
			generic_transformation(p.lambda, p.beta, ecliptic_to_horizon_matrix, out.A, out.a);
			out.A = wrap(out.A, 360.0);
			out.a = wrap(out.a, -180.0, 180.0);
			return out;
		}

		HorizonCoord AstroCalc::galactic_to_horizon(const GalacticCoord& p) const
		{
			HorizonCoord out;
			generic_transformation(p.l, p.b, ecliptic_to_horizon_matrix, out.A, out.a);
			out.A = wrap(out.A, 360.0);
			out.a = wrap(out.a, -180.0, 180.0);
			return out;
		}

		GalacticCoord AstroCalc::equatorial_to_galactic(const EquatorialCoord& p) const
		{
			GalacticCoord out;
			generic_transformation(p.alpha, p.delta, equatorial_to_galactic_matrix, out.l, out.b);
			return out;
		}

		EquatorialCoord AstroCalc::galactic_to_equatorial(const GalacticCoord& p) const
		{
			EquatorialCoord out;
			generic_transformation(p.l, p.b, galactic_to_equatorial_matrix, out.alpha, out.delta);
			return out;
		}

		HorizonCoord AstroCalc::equatorial_to_horizon(const EquatorialCoord& p) const
		{
			HorizonCoord out;
			Vector v(p.alpha, p.delta);
			Vector w = equatorial_to_horizon_matrix.times(v);
			w.toAngles(out.A, out.a);
			out.A = wrap(out.A, 360.0);
			out.a = wrap(out.a, 360.0);
			return out;
		}

		EquatorialCoord AstroCalc::horizon_to_equatorial(const HorizonCoord& p) const
		{
			EquatorialCoord out;
			Vector v(p.A, p.a);
			Vector w = horizon_to_equatorial_matrix.times(v);
			w.toAngles(out.alpha, out.delta);
			return out;
		}

		HEquatorialCoord AstroCalc::equatorial_to_hequatorial(const EquatorialCoord& p) const
		{
			HEquatorialCoord out;
			Vector v(p.alpha, p.delta);
			Vector w = equatorial_to_hequatorial_matrix.times(v);
			w.toAngles(out.H, out.delta);
			return out;
		}

		EquatorialCoord AstroCalc::hequatorial_to_equatorial(const HEquatorialCoord& p) const
		{
			EquatorialCoord out;
			Vector v(p.H, p.delta);
			Vector w = hequatorial_to_equatorial_matrix.times(v);
			w.toAngles(out.alpha, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::equatorial_to_ecliptic(const EquatorialCoord& p) const
		{
			EclipticCoord out;
			Vector v(p.alpha, p.delta);
			Vector w = equatorial_to_ecliptic_matrix.times(v);
			w.toAngles(out.lambda, out.beta);
			return out;
		}

		EquatorialCoord AstroCalc::ecliptic_to_equatorial(const EclipticCoord& p) const
		{
			EquatorialCoord out;
			Vector v(p.lambda, p.beta);
			Vector w = ecliptic_to_equatorial_matrix.times(v);
			w.toAngles(out.alpha, out.delta);
			return out;
		}

		EclipticCoord AstroCalc::getSun() const
		{
			Real DJ = JD - 2415020.0;
			Real sunLong = PA::SunLong(DJ, UT);

			EclipticCoord sun(sunLong, 0.0);

			return sun;
		}

		Real AstroCalc::kepler(Real M, Real e) const
		{
			Real E = M;
			Real epsilon = 10e-6;
			Real delta = E - e * sin(E) - M;
			do
			{
				E = E - delta / (1.0 - e * cos(E));
				delta = E - e * sin(E) - M;
			} while (fabs(delta) >= epsilon);
			return E;
		}
	} // namespace Astronomy
} // namespace Fluxions