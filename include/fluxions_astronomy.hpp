#ifndef FLUXIONS_ASTRONOMY_HPP
#define FLUXIONS_ASTRONOMY_HPP

#include <fluxions_gte_scalar_math.hpp>
#include <time.h>

namespace Fluxions {
	//////////////////////////////////////////////////////////////////////
	// A S T R O N O M Y   C A L C U L A T I O N S ///////////////////////
	//////////////////////////////////////////////////////////////////////

	namespace Astronomy {
		using Real = double;

		// namespace for Practical Astronomy functions
		namespace PA {
			struct CivilDateTime {
				int day = 0;
				int month = 0;
				int year = 0;
				bool isdst = false;
				int timeZoneOffset = 0;
				int hh = 0;
				int mm = 0;
				int ss = 0;
				Real ss_frac = 0;
				Real geo_latitude = 0;
				Real geo_longitude = 0;

				Real JD();
				Real UT();
				Real LST();
				Real GST();

				time_t GetTime() const;
			};

			typedef double Real;
			Real DDDeg(Real dd);
			Real DDMin(Real dd);
			Real DDSec(Real dd);
			Real DDDH(Real dd);
			Real DHDD(Real dh);
			Real LctGDay(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY);
			Real LctGMonth(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY);
			Real LctGYear(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY);
			Real LctUT(Real LCH, Real LCM, Real LCS, Real DS, Real ZC, Real LD, Real LM, Real LY);
			Real HMSDH(Real H, Real M, Real S);
			Real CDJD(Real day, Real month, Real year);
			Real JDCDay(Real JD);
			Real JDCMonth(Real JD);
			Real JDCYear(Real JD);

			Real TrueAnomaly(Real AM, Real EC);
			Real EccentricAnomaly(Real AM, Real EC);
			Real SunLong(Real DJ, Real UT);
		} // namespace PA

		// T Y P E S /////////////////////////////////////////////////////

		struct Vector {
			Real x, y, z;

			Vector() : x(0.0), y(0.0), z(0.0) {}
			Vector(Real x_, Real y_, Real z_) : x(x_), y(y_), z(z_) {}
			Vector(Real mu, Real nu) {
				Real mu_ = mu * FX_DEGREES_TO_RADIANS;
				Real nu_ = nu * FX_DEGREES_TO_RADIANS;
				x = std::cos(mu_) * std::cos(nu_);
				y = std::sin(mu_) * std::cos(nu_);
				z = std::sin(nu_);
			}

			void toAngles(Real& mu, Real& nu) const {
				mu = atan2(y, x) * FX_RADIANS_TO_DEGREES;
				nu = asin(z) * FX_RADIANS_TO_DEGREES;
			}

			Real length() {
				return std::sqrt(x * x + y * y + z * z);
			}

			Vector unit() {
				Real inv_length = 1.0 / length();
				return Vector(x * inv_length, y * inv_length, z * inv_length);
			}
		};

		struct Matrix {
			Real m11, m12, m13;
			Real m21, m22, m23;
			Real m31, m32, m33;

			Matrix() : m11(1.0), m12(0.0), m13(0.0),
				m21(0.0), m22(1.0), m23(0.0),
				m31(0.0), m32(0.0), m33(1.0) {}
			Matrix(
				Real a11, Real a12, Real a13,
				Real a21, Real a22, Real a23,
				Real a31, Real a32, Real a33)
				: m11(a11), m12(a12), m13(a13),
				m21(a21), m22(a22), m23(a23),
				m31(a31), m32(a32), m33(a33) {}
			Matrix(const Matrix& M) : m11(M.m11), m12(M.m12), m13(M.m13),
				m21(M.m21), m22(M.m22), m23(M.m23),
				m31(M.m31), m32(M.m32), m33(M.m33) {}

			Matrix times(const Matrix& M) const {
				return Matrix(
					m11 * M.m11 + m12 * M.m21 + m13 * M.m31, m11 * M.m12 + m12 * M.m22 + m13 * M.m32, m11 * M.m13 + m12 * M.m23 + m13 * M.m33,
					m21 * M.m11 + m22 * M.m21 + m23 * M.m31, m21 * M.m12 + m22 * M.m22 + m23 * M.m32, m21 * M.m13 + m22 * M.m23 + m23 * M.m33,
					m31 * M.m11 + m32 * M.m21 + m33 * M.m31, m31 * M.m12 + m32 * M.m22 + m33 * M.m32, m31 * M.m13 + m32 * M.m23 + m33 * M.m33);
			}

			Vector times(const Vector& V) const {
				return Vector(
					m11 * V.x + m12 * V.y + m13 * V.z,
					m21 * V.x + m22 * V.y + m23 * V.z,
					m31 * V.x + m32 * V.y + m33 * V.z);
			}

			Matrix transpose() const {
				return Matrix(
					m11, m21, m31,
					m12, m22, m32,
					m13, m23, m33);
			}
		};

		struct HorizonCoord {
			Real A = 0.0;
			Real a = 0.0;

			HorizonCoord() : A(0.0), a(0.0) {}
			HorizonCoord(Real A_, Real a_) : A(A_), a(a_) {}

			Vector toOpenGLVector() const {
				Vector v(90.0 - A, a);
				// return Vector(-ptr.y, ptr.z, -ptr.X);
				return Vector(v.x, v.z, -v.y);
			}
		};

		struct EclipticCoord {
			// ecliptic longitude
			Real lambda = 0.0;
			// ecliptic latitude
			Real beta = 0.0;

			EclipticCoord() {}
			EclipticCoord(Real lambda_, Real beta_) : lambda(lambda_), beta(beta_) {}
		};

		struct HEquatorialCoord {
			Real delta = 0.0;
			Real H = 0.0;

			HEquatorialCoord() : delta(0.0), H(0.0) {}
			HEquatorialCoord(Real delta_, Real H_) : delta(delta_), H(H_) {}
		};

		struct EquatorialCoord {
			Real delta = 0.0;
			Real alpha = 0.0;

			EquatorialCoord() : delta(0.0), alpha(0.0) {}
			EquatorialCoord(Real delta_, Real alpha_) : delta(delta_), alpha(alpha_) {}
			double H(Real LST) {
				Real result = LST - alpha;
				return result < 0.0 ? result + 24.0 : result;
			}
		};

		struct GalacticCoord {
			Real l = 0.0;
			Real b = 0.0;

			GalacticCoord() : l(0.0), b(0.0) {}
			GalacticCoord(Real l_, Real b_) : l(l_), b(b_) {}
		};

		// struct SunPosition
		// Elevation and Azimuth
		struct SunPosition {
			// Elevation is from the horizon plane toward the zenith. -PI/2 <= elevation <= PI/2
			Real elevation;
			// Azimuth is angle from north clockwise to the east. 0 <= azimuth <= 2 PI
			Real azimuth;
		};

		// struct ECEF
		// Earth Centered Earth Fixed coordinates
		struct ECEF {
			// X and y are coordinates in the earths equatorial plane
			// X = 1, y = 0 is the prime meridian
			// Latitude is phi and -PI/2 <= phi <= PI/2 measured from the equatorial plane
			// Longitude is lambda and measured counterclockwise on the X-y axis with 0 at the prime meridian
			// -PI <= lambda <= PI
			Real x, y;
			// z is Earth's axis of rotation with +Z towards the north pole and -Z towards the south pole
			Real z;
		};

		class AstroCalc {
		public:
			AstroCalc();
			AstroCalc(time_t t, Real fractSeconds, Real latitude, Real longitude);

			// Init initializes all variables so that calculations are made with respect to
			// a certain latitude and longitude on the earth.
			// t is UTC in seconds since January 1, 1970
			// fractSeconds is the fractional seconds of the time
			// latitude is -90 south to 90 north (will be clamped)
			// longitude is 0 at the Prime meridian, -180 west to 180 east (will be wrapped)
			void Init(time_t t, Real fractSeconds, Real latitude, Real longitude);
			void SetLocation(Real latitude, Real longitude);
			void SetDateTime(int day, int month, int year, bool isdst, int timeOffset, int hh, int mm, int ss, Real ss_frac);
			void SetDate(int day, int month, int year, bool isdst, int timeOffset);
			void SetTime(int hh, int mm, int ss, Real ss_frac);
			void SetTime(time_t t, Real fractSeconds = 0.0);

			EclipticCoord equatorial_to_ecliptic(const EquatorialCoord& p) const;
			EclipticCoord galactic_to_ecliptic(const GalacticCoord& p) const;
			EclipticCoord hequatorial_to_ecliptic(const HEquatorialCoord& p) const;
			EclipticCoord horizon_to_ecliptic(const HorizonCoord& p) const;
			EquatorialCoord ecliptic_to_equatorial(const EclipticCoord& p) const;
			EquatorialCoord galactic_to_equatorial(const GalacticCoord& p) const;
			EquatorialCoord hequatorial_to_equatorial(const HEquatorialCoord& p) const;
			EquatorialCoord horizon_to_equatorial(const HorizonCoord& p) const;
			GalacticCoord ecliptic_to_galactic(const EclipticCoord& p) const;
			GalacticCoord equatorial_to_galactic(const EquatorialCoord& p) const;
			GalacticCoord hequatorial_to_galactic(const HEquatorialCoord& p) const;
			GalacticCoord horizon_to_galactic(const HorizonCoord& p) const;
			HEquatorialCoord ecliptic_to_hequatorial(const EclipticCoord& p) const;
			HEquatorialCoord equatorial_to_hequatorial(const EquatorialCoord& p) const;
			HEquatorialCoord galactic_to_hequatorial(const GalacticCoord& p) const;
			HEquatorialCoord horizon_to_hequatorial(const HorizonCoord& p) const;
			HorizonCoord ecliptic_to_horizon(const EclipticCoord& p) const;
			HorizonCoord equatorial_to_horizon(const EquatorialCoord& p) const;
			HorizonCoord galactic_to_horizon(const GalacticCoord& p) const;
			HorizonCoord hequatorial_to_horizon(const HEquatorialCoord& p) const;

			Real getGST() const { return GST; }
			Real getLST() const { return LST; }
			Real getLatitude() const { return geo_latitude; }
			Real getLongitude() const { return geo_longitude; }
			Real getGMT() const { return static_cast<double>(gmt_date + gmt_time); }
			Real getJD() const { return julian_datetime; }
			time_t GetTime() const;

			EclipticCoord getSun() const;

			Real kepler(Real M, Real e) const;
			const PA::CivilDateTime GetDateTime() const { return paTime; }

		private:
			PA::CivilDateTime paTime;
			Real JD = 0;
			Real UT = 0;
			Real LST = 0;
			Real GST = 0;

			void computeTimes();
			void computeMatrices();
			void computeTimesAndMatrices();

			time_t gmt = 0;
			struct tm gmt_tm;
			time_t lct = 0;
			struct tm lct_tm;
			long lct_ds = 0;
			long lct_zs = 0;
			Real g_ds = 0;
			Real g_zc = 0;
			time_t gmt_date = 0;
			time_t gmt_time = 0;
			Real gmt_time_fractSeconds = 0;
			// Julian date
			Real julian_date = 0;
			// Julian time in hours
			Real julian_time = 0;
			// Julian date and time
			Real julian_datetime = 0;
			Real geo_latitude = 0;
			Real geo_longitude = 0;
			Real obliquity_of_the_ecliptic = 0;
			// Real gst = 0;
			// Real lst = 0;

			Matrix ecliptic_to_equatorial_matrix; // C'
			Matrix ecliptic_to_galactic_matrix;
			Matrix ecliptic_to_hequatorial_matrix;
			Matrix ecliptic_to_horizon_matrix;
			Matrix equatorial_to_ecliptic_matrix;	// C
			Matrix equatorial_to_galactic_matrix;	// D
			Matrix equatorial_to_hequatorial_matrix; // B
			Matrix equatorial_to_horizon_matrix;
			Matrix galactic_to_ecliptic_matrix;
			Matrix galactic_to_equatorial_matrix; // D'
			Matrix galactic_to_hequatorial_matrix;
			Matrix galactic_to_horizon_matrix;
			Matrix hequatorial_to_ecliptic_matrix;
			Matrix hequatorial_to_equatorial_matrix; // B'
			Matrix hequatorial_to_galactic_matrix;
			Matrix hequatorial_to_horizon_matrix; // A'
			Matrix horizon_to_ecliptic_matrix;
			Matrix horizon_to_equatorial_matrix;
			Matrix horizon_to_galactic_matrix;
			Matrix horizon_to_hequatorial_matrix; // A

			void generic_transformation(Real mu_in, Real nu_in, const Matrix& m, Real& mu_out, Real& nu_out) const;
		};

	} // namespace Astronomy
} // namespace Fluxions

#endif
