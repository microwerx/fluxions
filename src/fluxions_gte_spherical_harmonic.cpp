// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
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
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_spherical_harmonic.hpp>
//#include <fluxions_fileio.hpp>
//#include <kasl_json.hpp>

namespace Fluxions
{
	template class TSphericalHarmonic<float, float>;
	template class TSphericalHarmonic<float, double>;
	//template class TSphericalHarmonic<Color3f>;
	//template class TSphericalHarmonic<Color4f>;
	//template class TSphericalHarmonic<Vector3f>;
	//template class TSphericalHarmonic<Vector4f>;

	double _legendre_polynomial(int l, int m, double x)
	{
		// evaluate an Associated Legendre Polynomial P(l,ptr,X) at X
		double pmm = 1.0;
		if (m > 0) {
			double somx2 = sqrt((1.0 - x) * (1.0 + x));
			double fact = 1.0;
			for (int i = 1; i <= m; i++) {
				pmm *= (-fact) * somx2;
				fact += 2.0;
			}
		}
		if (l == m)
			return pmm;
		double pmmp1 = x * (2.0 * m + 1.0) * pmm;
		if (l == m + 1)
			return pmmp1;
		double pll = 0.0;
		for (int ll = m + 2; ll <= l; ++ll) {
			pll = ((2.0 * ll - 1.0) * x * pmmp1 - (ll + m - 1.0) * pmm) / (ll - m);
			pmm = pmmp1;
			pmmp1 = pll;
		}
		return pll;
	}

	double legendre_polynomial(int l, int m, double x) noexcept
	{
		if (l < 0 || abs(m) > l || x < -1.0 || x > 1.0)
			return 0.0;

		if (l == 0) {
			return 1.0;
		}
		else if (l == 1) {
			return x;
		}
		else if (l == 2) {
			double t1 = pow(x, 2.0);
			return (8.0 * t1 + 4.0 * (t1 - 1.0)) / 8.0;
		}
		else if (l == 3) {
			return (48.0 * pow(x, 3.0) + 72.0 * x * (pow(x, 2.0) - 1.0)) / 48.0;
		}
		else if (l == 4) {
			double t1 = pow(x, 2.0);
			double t2 = t1 - 1.0;
			return (144.0 * pow(t2, 2.0) + 384.0 * pow(x, 4.0) + 1152.0 * t1 * t2) / 384.0;
		}
		else if (l == 5) {
			double t1 = pow(x, 2.0) - 1.0;
			return (7200.0 * x * pow(t1, 2.0) + 3840.0 * pow(x, 5.0) + 19200.0 * pow(x, 3.0) * t1) / 3840.0;
		}
		else if (l == 6) {
			double t1 = pow(x, 2.0);
			double t2 = t1 - 1.0;
			return (14400.0 * pow(t2, 3.0) + 259200.0 * t1 * pow(t2, 2.0) + 46080.0 * pow(x, 6.0) + 345600.0 * pow(x, 4.0) * t2) / 46080.0;
		}
		else if (l == 7) {
			double t1 = pow(x, 2.0) - 1.0;
			return (1411200.0 * x * pow(t1, 3.0) + 8467200.0 * pow(x, 3.0) * pow(t1, 2.0) + 645120.0 * pow(x, 7.0) + 6773760.0 * pow(x, 5.0) * t1) / 645120.0;
		}
		else if (l == 8) {
			double t1 = pow(x, 2.0);
			double t2 = t1 - 1.0;
			return (2822400.0 * pow(t2, 4.0) + 90316800.0 * t1 * pow(t2, 3.0) + 270950400.0 * pow(x, 4.0) * pow(t2, 2.0) + 10321920.0 * pow(x, 8.0) + 144506880.0 * pow(x, 6.0) * t2) / 10321920.0;
		}
		else if (l == 9) {
			double t1 = pow(x, 2.0) - 1.0;
			return (457228800.0 * x * pow(t1, 4.0) + 4877107200.0 * pow(x, 3.0) * pow(t1, 3.0) + 8778792960.0 * pow(x, 5.0) * pow(t1, 2.0) + 185794560.0 * pow(x, 9.0) + 3344302080.0 * pow(x, 7.0) * t1) / 185794560.0;
		}
		else if (l == 10) {
			double t1 = pow(x, 2.0);
			double t2 = t1 - 1.0;
			return (914457600.0 * pow(t2, 5.0) + 45722880000.0 * t1 * pow(t2, 4.0) + 243855360000.0 * pow(x, 4.0) * pow(t2, 3.0) + 292626432000.0 * pow(x, 6.0) * pow(t2, 2.0) + 3715891200.0 * pow(x, 10.0) + 83607552000.0 * pow(x, 8.0) * t2) / 3715891200.0;
		}
		return 0.0;
	}

	double spherical_harmonic(int l, int m, double theta, double phi) noexcept
	{
		// K = sqrt(2.0) * sqrt((2 * l + 1) / (4 * FX_PI) * factorial(l - abs(ptr)) / factorial(l + abs(ptr)))
		static const double K[] = {
			3.9894228040143300E-01,
			4.8860251190292000E-01,
			6.9098829894267100E-01,
			4.8860251190292000E-01,
			1.8209140509868000E-01,
			3.6418281019736000E-01,
			8.9206205807638600E-01,
			3.6418281019736000E-01,
			1.8209140509868000E-01,
			3.9336239328442900E-02,
			9.6353714754685200E-02,
			3.0469719964297700E-01,
			1.0555020614111900E+00,
			3.0469719964297700E-01,
			9.6353714754685200E-02,
			3.9336239328442900E-02,
			5.9603403376112000E-03,
			1.6858388283618400E-02,
			6.3078313050504000E-02,
			2.6761861742291600E-01,
			1.1968268412043000E+00,
			2.6761861742291600E-01,
			6.3078313050504000E-02,
			1.6858388283618400E-02,
			5.9603403376112000E-03,
			6.9458418713245500E-04,
			2.1964680580751800E-03,
			9.3188247511476300E-03,
			4.5652731285460200E-02,
			2.4157154730437200E-01,
			1.3231418571003100E+00,
			2.4157154730437200E-01,
			4.5652731285460200E-02,
			9.3188247511476300E-03,
			2.1964680580751800E-03,
			6.9458418713245500E-04,
			6.5722376641838800E-05,
			2.2766899107568600E-04,
			1.0678622237645000E-03,
			5.8489222826344400E-03,
			3.5093533695806600E-02,
			2.2195099524523100E-01,
			1.4384068479379000E+00,
			2.2195099524523100E-01,
			3.5093533695806600E-02,
			5.8489222826344400E-03,
			1.0678622237645000E-03,
			2.2766899107568600E-04,
			6.5722376641838800E-05,
			5.2330094536914700E-06,
			1.9580128477462500E-05,
			9.9839457185235300E-05,
			5.9903674311141200E-04,
			3.9735602250741300E-03,
			2.8097313806030600E-02,
			2.0647224590289700E-01,
			1.5450968080927600E+00,
			2.0647224590289700E-01,
			2.8097313806030600E-02,
			3.9735602250741300E-03,
			5.9903674311141200E-04,
			9.9839457185235300E-05,
			1.9580128477462500E-05,
			5.2330094536914700E-06,
			3.5960417862376100E-07,
			1.4384167144950500E-06,
			7.8785328162140500E-06,
			5.1058728265780300E-05,
			3.6818972564450700E-04,
			2.8519853513317000E-03,
			2.3169638523677900E-02,
			1.9385110382005300E-01,
			1.6448811606198900E+00,
			1.9385110382005300E-01,
			2.3169638523677900E-02,
			2.8519853513317000E-03,
			3.6818972564450700E-04,
			5.1058728265780300E-05,
			7.8785328162140500E-06,
			1.4384167144950500E-06,
			3.5960417862376100E-07,
			2.1732833668965400E-08,
			9.2204604370348500E-08,
			5.3764061256674500E-07,
			3.7248834287122300E-06,
			2.8852822971932900E-05,
			2.4140003633280300E-04,
			2.1319873940141700E-03,
			1.9539987227523200E-02,
			1.8330132807744700E-01,
			1.7389490845755100E+00,
			1.8330132807744700E-01,
			1.9539987227523200E-02,
			2.1319873940141700E-03,
			2.4140003633280300E-04,
			2.8852822971932900E-05,
			3.7248834287122300E-06,
			5.3764061256674500E-07,
			9.2204604370348500E-08,
			2.1732833668965400E-08,
			1.1720804032140900E-09,
			5.2417029133641500E-09,
			3.2312026838545200E-08,
			2.3744393492865400E-07,
			1.9580128477462500E-06,
			1.7512999313514300E-05,
			1.6614289947501100E-04,
			1.6447307921068500E-03,
			1.6773028807119500E-02,
			1.7431042854448500E-01,
			1.8281831978578600E+00,
			1.7431042854448500E-01,
			1.6773028807119500E-02,
			1.6447307921068500E-03,
			1.6614289947501100E-04,
			1.7512999313514300E-05,
			1.9580128477462500E-06,
			2.3744393492865400E-07,
			3.2312026838545200E-08,
			5.2417029133641500E-09,
			1.1720804032140900E-09 };

		// m0K = sqrt((2 * l + 1) / (4 * FX_PI))
		double m0K[] = {
			2.8209479177387800E-01,
			4.8860251190292000E-01,
			6.3078313050504000E-01,
			7.4635266518023100E-01,
			8.4628437532163400E-01,
			9.3560257962738900E-01,
			1.0171072362820500E+00,
			1.0925484305920800E+00,
			1.1631066229203200E+00,
			1.2296226898414800E+00,
			1.2927207364566000E+00 };

		int lm = l * (l + 1) + m;
		double sc;
		double x = cos(theta);
		if (m == 0)
			return m0K[l] * _legendre_polynomial(l, m, x);
		if (m < 0) {
			m = -m;
			sc = sin(m * phi);
		}
		else {
			sc = cos(m * phi);
		}

		return K[lm] * sc * _legendre_polynomial(l, m, x);
	}

	const float T1 = 0.2820947917738782f;
	const float T2 = 1.0f;
	const float T3 = .3454941494713354f;
	const float T4 = 0.3454941494713354f;
	const float T5 = 0.48860251190292f;
	const float T6 = 1.0f;
	const float T7 = 1.0f;
	const float T8 = 1.0f;
	const float T9 = 2.0f;
	const float T10 = .3862742020231897f;
	const float T11 = 0.3862742020231897f;
	const float T12 = 0.7725484040463795f;
	const float T13 = 0.7725484040463795f;
	const float T14 = 0.6307831305050402f;
	const float T15 = -3.0f;
	const float T16 = 1.5f;
	const float T17 = 1.0f;
	const float T18 = 1.0f;
	const float T19 = 1.0f;
	const float T20 = 1.0f;
	const float T21 = -2.5f;
	const float T22 = 1.25f;
	const float T23 = 1.0f;
	const float T24 = 2.0f;
	const float T25 = 3.0f;
	const float T26 = -0.4172238236327842f;
	const float T27 = 0.4172238236327842f;
	const float T28 = 1.021985476433283f;
	const float T29 = 1.021985476433283f;
	const float T30 = 1.292720736456603f;
	const float T31 = 1.292720736456603f;
	const float T32 = 0.7463526651802309f;
	const float T33 = -6.0f;
	const float T34 = 2.5f;
	const float T35 = 7.5f;
	const float T36 = 1.0f;
	const float T37 = 1.0f;
	const float T38 = 1.0f;
	const float T39 = 1.0f;
	const float T40 = -4.5f;
	const float T41 = 1.75f;
	const float T42 = 5.25f;
	const float T43 = 1.0f;
	const float T44 = 2.0f;
	const float T45 = -2.333333333333333f;
	const float T46 = 1.166666666666667f;
	const float T47 = 1.0f;
	const float T48 = 3.0f;
	const float T49 = 4.0f;
	const float T50 = .4425326924449827f;
	const float T51 = 0.4425326924449827f;
	const float T52 = 1.251671470898353f;
	const float T53 = 1.251671470898353f;
	const float T54 = 2.007139630671868f;
	const float T55 = 2.007139630671868f;
	const float T56 = 1.89234939151512f;
	const float T57 = 1.89234939151512f;
	const float T58 = 0.8462843753216346f;
	const float T59 = -10.0f;
	const float T60 = 4.375f;
	const float T61 = 17.5f;
	const float T62 = 22.5f;
	const float T63 = 1.0f;
	const float T64 = 1.0f;
	const float T65 = 1.0f;
	const float T66 = 1.0f;
	const float T67 = -7.0f;
	const float T68 = 2.625f;
	const float T69 = 10.5f;
	const float T70 = 14.0f;
	const float T71 = 1.0f;
	const float T72 = 2.0f;
	const float T73 = -4.0f;
	const float T74 = 1.5f;
	const float T75 = 4.5f;
	const float T76 = 1.0f;
	const float T77 = 3.0f;
	const float T78 = -2.25f;
	const float T79 = 1.125f;
	const float T80 = 1.0f;
	const float T81 = 4.0f;
	const float T82 = 5.0f;
	const float T83 = .4641322034408583f;
	const float T84 = 0.4641322034408583f;
	const float T85 = 1.467714898305751f;
	const float T86 = 1.467714898305751f;
	const float T87 = 2.767549753174722f;
	const float T88 = 2.767549753174722f;
	const float T89 = 3.389542366521799f;
	const float T90 = 3.389542366521799f;
	const float T91 = 2.562253188609721f;
	const float T92 = 2.562253188609721f;
	const float T93 = 0.9356025796273889f;
	const float T94 = -15.0f;
	const float T95 = 7.875f;
	const float T96 = 39.375f;
	const float T97 = 70.0f;
	const float T98 = 52.5f;
	const float T99 = 1.0f;
	const float T100 = 1.0f;
	const float T101 = 1.0f;
	const float T102 = 1.0f;
	const float T103 = -10.0f;
	const float T104 = 4.125f;
	const float T105 = 20.625f;
	const float T106 = 37.5f;
	const float T107 = 30.0f;
	const float T108 = 1.0f;
	const float T109 = 2.0f;
	const float T110 = -6.0f;
	const float T111 = 2.0625f;
	const float T112 = 8.25f;
	const float T113 = 11.25f;
	const float T114 = 1.0f;
	const float T115 = 3.0f;
	const float T116 = -3.75f;
	const float T117 = 1.375f;
	const float T118 = 4.125f;
	const float T119 = 1.0f;
	const float T120 = 4.0f;
	const float T121 = -2.2f;
	const float T122 = 1.1f;
	const float T123 = 1.0f;
	const float T124 = 5.0f;
	const float T125 = 6.0f;
	const float T126 = .4830841135800663f;
	const float T127 = 0.4830841135800663f;
	const float T128 = 1.673452458100098f;
	const float T129 = 1.673452458100098f;
	const float T130 = 3.567812628539981f;
	const float T131 = 3.567812628539981f;
	const float T132 = 5.211123886941726f;
	const float T133 = 5.211123886941726f;
	const float T134 = 5.211123886941726f;
	const float T135 = 5.211123886941726f;
	const float T136 = 3.295804130409127f;
	const float T137 = 3.295804130409127f;
	const float T138 = 1.017107236282055f;
	const float T139 = -21.0f;
	const float T140 = 14.4375f;
	const float T141 = 86.625f;
	const float T142 = 196.875f;
	const float T143 = 210.0f;
	const float T144 = 105.0f;
	const float T145 = 1.0f;
	const float T146 = 1.0f;
	const float T147 = 1.0f;
	const float T148 = 1.0f;
	const float T149 = -13.5f;
	const float T150 = 6.703125f;
	const float T151 = 40.21875f;
	const float T152 = 92.8125f;
	const float T153 = 103.125f;
	const float T154 = 56.25f;
	const float T155 = 1.0f;
	const float T156 = 2.0f;
	const float T157 = -8.333333333333334f;
	const float T158 = 2.979166666666667f;
	const float T159 = 14.89583333333333f;
	const float T160 = 27.5f;
	const float T161 = 22.91666666666667f;
	const float T162 = 1.0f;
	const float T163 = 3.0f;
	const float T164 = -5.5f;
	const float T165 = 1.7875f;
	const float T166 = 7.15f;
	const float T167 = 9.9f;
	const float T168 = 1.0f;
	const float T169 = 4.0f;
	const float T170 = -3.6f;
	const float T171 = 1.3f;
	const float T172 = 3.9f;
	const float T173 = 1.0f;
	const float T174 = 5.0f;
	const float T175 = -2.166666666666667f;
	const float T176 = 1.083333333333333f;
	const float T177 = 1.0f;
	const float T178 = 6.0f;
	const float T179 = 7.0f;
	const float T180 = .5000395635705508f;
	const float T181 = 0.5000395635705508f;
	const float T182 = 1.870976726712969f;
	const float T183 = 1.870976726712969f;
	const float T184 = 4.403144694917254f;
	const float T185 = 4.403144694917254f;
	const float T186 = 7.338574491528757f;
	const float T187 = 7.338574491528757f;
	const float T188 = 8.850653848899654f;
	const float T189 = 8.850653848899654f;
	const float T190 = 7.510028825390116f;
	const float T191 = 7.510028825390116f;
	const float T192 = 4.08794190573313f;
	const float T193 = 4.08794190573313f;
	const float T194 = 1.092548430592079f;
	const float T195 = -28.0f;
	const float T196 = 26.8125f;
	const float T197 = 187.6875f;
	const float T198 = 519.75f;
	const float T199 = 721.875f;
	const float T200 = 525.0f;
	const float T201 = 189.0f;
	const float T202 = 1.0f;
	const float T203 = 1.0f;
	const float T204 = 1.0f;
	const float T205 = 1.0f;
	const float T206 = -17.5f;
	const float T207 = 11.171875f;
	const float T208 = 78.203125f;
	const float T209 = 218.96875f;
	const float T210 = 312.8125f;
	const float T211 = 240.625f;
	const float T212 = 96.25f;
	const float T213 = 1.0f;
	const float T214 = 2.0f;
	const float T215 = -11.0f;
	const float T216 = 4.46875f;
	const float T217 = 26.8125f;
	const float T218 = 62.5625f;
	const float T219 = 71.5f;
	const float T220 = 41.25f;
	const float T221 = 1.0f;
	const float T222 = 3.0f;
	const float T223 = -7.5f;
	const float T224 = 2.4375f;
	const float T225 = 12.1875f;
	const float T226 = 22.75f;
	const float T227 = 19.5f;
	const float T228 = 1.0f;
	const float T229 = 4.0f;
	const float T230 = -5.2f;
	const float T231 = 1.625f;
	const float T232 = 6.5f;
	const float T233 = 9.1f;
	const float T234 = 1.0f;
	const float T235 = 5.0f;
	const float T236 = -3.5f;
	const float T237 = 1.25f;
	const float T238 = 3.75f;
	const float T239 = 1.0f;
	const float T240 = 6.0f;
	const float T241 = -2.142857142857143f;
	const float T242 = 1.071428571428571f;
	const float T243 = 1.0f;
	const float T244 = 7.0f;
	const float T245 = 8.0f;
	const float T246 = .5154289843972844f;
	const float T247 = 0.5154289843972844f;
	const float T248 = 2.061715937589137f;
	const float T249 = 2.061715937589137f;
	const float T250 = 5.269825522198925f;
	const float T251 = 5.269825522198925f;
	const float T252 = 9.757820781492292f;
	const float T253 = 9.757820781492292f;
	const float T254 = 13.53166275556099f;
	const float T255 = 13.53166275556099f;
	const float T256 = 13.97544119967543f;
	const float T257 = 13.97544119967543f;
	const float T258 = 10.32154736617226f;
	const float T259 = 10.32154736617226f;
	const float T260 = 4.934643481859657f;
	const float T261 = 4.934643481859657f;
	const float T262 = 1.16310662292032f;
	const float T263 = -36.0f;
	const float T264 = 50.2734375f;
	const float T265 = 402.1875f;
	const float T266 = 1313.8125f;
	const float T267 = 2252.25f;
	const float T268 = 2165.625f;
	const float T269 = 1155.0f;
	const float T270 = 315.0f;
	const float T271 = 1.0f;
	const float T272 = 1.0f;
	const float T273 = 1.0f;
	const float T274 = 1.0f;
	const float T275 = -22.0f;
	const float T276 = 18.9921875f;
	const float T277 = 151.9375f;
	const float T278 = 500.5f;
	const float T279 = 875.875f;
	const float T280 = 875.875f;
	const float T281 = 500.5f;
	const float T282 = 154.0f;
	const float T283 = 1.0f;
	const float T284 = 2.0f;
	const float T285 = -14.0f;
	const float T286 = 6.90625f;
	const float T287 = 48.34375f;
	const float T288 = 136.5f;
	const float T289 = 199.0625f;
	const float T290 = 159.25f;
	const float T291 = 68.25f;
	const float T292 = 1.0f;
	const float T293 = 3.0f;
	const float T294 = -9.75f;
	const float T295 = 3.453125f;
	const float T296 = 20.71875f;
	const float T297 = 48.75f;
	const float T298 = 56.875f;
	const float T299 = 34.125f;
	const float T300 = 1.0f;
	const float T301 = 4.0f;
	const float T302 = -7.0f;
	const float T303 = 2.125f;
	const float T304 = 10.625f;
	const float T305 = 20.0f;
	const float T306 = 17.5f;
	const float T307 = 1.0f;
	const float T308 = 5.0f;
	const float T309 = -5.0f;
	const float T310 = 1.517857142857143f;
	const float T311 = 6.071428571428571f;
	const float T312 = 8.571428571428571f;
	const float T313 = 1.0f;
	const float T314 = 6.0f;
	const float T315 = -3.428571428571428f;
	const float T316 = 1.214285714285714f;
	const float T317 = 3.642857142857143f;
	const float T318 = 1.0f;
	const float T319 = 7.0f;
	const float T320 = -2.125f;
	const float T321 = 1.0625f;
	const float T322 = 1.0f;
	const float T323 = 8.0f;
	const float T324 = 9.0f;
	const float T325 = .5295529414924497f;
	const float T326 = 0.5295529414924497f;
	const float T327 = 2.246702855559565f;
	const float T328 = 2.246702855559565f;
	const float T329 = 6.1649017754256f;
	const float T330 = 6.1649017754256f;
	const float T331 = 12.4575769484935f;
	const float T332 = 12.4575769484935f;
	const float T333 = 19.29919522224509f;
	const float T334 = 19.29919522224509f;
	const float T335 = 23.06695026675698f;
	const float T336 = 23.06695026675698f;
	const float T337 = 20.89454242784359f;
	const float T338 = 20.89454242784359f;
	const float T339 = 13.67868889815137f;
	const float T340 = 13.67868889815137f;
	const float T341 = 5.832612543782816f;
	const float T342 = 5.832612543782816f;
	const float T343 = 1.229622689841484f;
	const float T344 = -45.0f;
	const float T345 = 94.9609375f;
	const float T346 = 854.6484375f;
	const float T347 = 3217.5f;
	const float T348 = 6569.0625f;
	const float T349 = 7882.875f;
	const float T350 = 5630.625f;
	const float T351 = 2310.0f;
	const float T352 = 495.0f;
	const float T353 = 1.0f;
	const float T354 = 1.0f;
	const float T355 = 1.0f;
	const float T356 = 1.0f;
	const float T357 = -27.0f;
	const float T358 = 32.8046875f;
	const float T359 = 295.2421875f;
	const float T360 = 1118.8125f;
	const float T361 = 2320.5f;
	const float T362 = 2866.5f;
	const float T363 = 2149.875f;
	const float T364 = 955.5f;
	const float T365 = 234.0f;
	const float T366 = 1.0f;
	const float T367 = 2.0f;
	const float T368 = -17.33333333333333f;
	const float T369 = 10.93489583333333f;
	const float T370 = 87.47916666666667f;
	const float T371 = 290.0625f;
	const float T372 = 515.6666666666666f;
	const float T373 = 530.8333333333334f;
	const float T374 = 318.5f;
	const float T375 = 106.1666666666667f;
	const float T376 = 1.0f;
	const float T377 = 3.0f;
	const float T378 = -12.25f;
	const float T379 = 5.046875f;
	const float T380 = 35.328125f;
	const float T381 = 100.40625f;
	const float T382 = 148.75f;
	const float T383 = 122.5f;
	const float T384 = 55.125f;
	const float T385 = 1.0f;
	const float T386 = 4.0f;
	const float T387 = -9.0f;
	const float T388 = 2.883928571428572f;
	const float T389 = 17.30357142857143f;
	const float T390 = 40.98214285714285f;
	const float T391 = 48.57142857142857f;
	const float T392 = 30.0f;
	const float T393 = 1.0f;
	const float T394 = 5.0f;
	const float T395 = -6.666666666666667f;
	const float T396 = 1.922619047619048f;
	const float T397 = 9.613095238095237f;
	const float T398 = 18.21428571428572f;
	const float T399 = 16.19047619047619f;
	const float T400 = 1.0f;
	const float T401 = 6.0f;
	const float T402 = -4.857142857142857f;
	const float T403 = 1.441964285714286f;
	const float T404 = 5.767857142857143f;
	const float T405 = 8.196428571428571f;
	const float T406 = 1.0f;
	const float T407 = 7.0f;
	const float T408 = -3.375f;
	const float T409 = 1.1875f;
	const float T410 = 3.5625f;
	const float T411 = 1.0f;
	const float T412 = 8.0f;
	const float T413 = -2.111111111111111f;
	const float T414 = 1.055555555555556f;
	const float T415 = 1.0f;
	const float T416 = 9.0f;
	const float T417 = 10.0f;
	const float T418 = .5426302919442215f;
	const float T419 = 0.5426302919442215f;
	const float T420 = 2.426716438875672f;
	const float T421 = 2.426716438875672f;
	const float T422 = 7.085977009124305f;
	const float T423 = 7.085977009124305f;
	const float T424 = 15.42846933461992f;
	const float T425 = 15.42846933461992f;
	const float T426 = 26.19367417398156f;
	const float T427 = 26.19367417398156f;
	const float T428 = 35.14250162010206f;
	const float T429 = 35.14250162010206f;
	const float T430 = 37.04344926522661f;
	const float T431 = 37.04344926522661f;
	const float T432 = 29.93562762740751f;
	const float T433 = 29.93562762740751f;
	const float T434 = 17.61257877966902f;
	const float T435 = 17.61257877966902f;
	const float T436 = 6.779084733043598f;
	const float T437 = 6.779084733043598f;
	const float T438 = 1.292720736456603f;
	const float T439 = -55.0f;
	const float T440 = 180.42578125f;
	const float T441 = 1804.2578125f;
	const float T442 = 7691.8359375f;
	const float T443 = 18232.5f;
	const float T444 = 26276.25f;
	const float T445 = 23648.625f;
	const float T446 = 13138.125f;
	const float T447 = 4290.0f;
	const float T448 = 742.5f;
	const float T449 = 1.0f;
	// const float T450 = 0.0f;
	// const float T451 = 0.0f;
	// const float T452 = 0.0f;
	// const float T453 = 0.0f;

	const bool debug_sph = true;

	template <typename T>
	T calc_degree_0(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 0;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		return T1 * a[0];
	}

	template <typename T>
	T calc_degree_1(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 1;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(T2 * phi);
		T t2 = sin(theta);
		return T3 * t1 * a[3] * t2 - T4 * t1 * a[1] * t2 + T5 * a[2] * cos(theta);
	}

	template <typename T>
	T calc_degree_2(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 2;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T6 - T7 * t1;
		T t3 = cos(T8 * phi);
		T t4 = sin(theta);
		T t5 = cos(T9 * phi);
		T t6 = pow(t4, 2);
		return T10 * t5 * a[8] * t6 + T11 * t5 * a[4] * t6 + T12 * t3 * a[7] * t1 * t4 - T13 * t3 * a[5] * t1 * t4 + T14 * a[6] * (T15 * t2 + T16 * t2 * t2 + T17);
	}

	template <typename T>
	T calc_degree_3(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 3;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T18 - T19 * t1;
		T t3 = pow(t2, 2);
		T t4 = cos(T20 * phi);
		T t5 = T21 * t2 + T22 * t3 + T23;
		T t6 = sin(theta);
		T t7 = cos(T24 * phi);
		T t8 = pow(t6, 2);
		T t9 = cos(T25 * phi);
		T t10 = pow(t6, 3);
		return T26 * t9 * a[9] * t10 + T27 * t9 * a[15] * t10 + T28 * t7 * a[14] * t1 * t8 + T29 * t7 * a[10] * t1 * t8 +
			T30 * t4 * a[13] * t5 * t6 - T31 * t4 * a[11] * t5 * t6 +
			T32 * a[12] * (T33 * t2 - T34 * pow(t2, 3) + T35 * t3 + T36);
	}

	template <typename T>
	T calc_degree_4(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 4;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T37 - T38 * t1;
		T t3 = pow(t2, 2);
		T t4 = pow(t2, 3);
		T t5 = cos(T39 * phi);
		T t6 = T40 * t2 - T41 * t4 + T42 * t3 + T43;
		T t7 = sin(theta);
		T t8 = cos(T44 * phi);
		T t9 = T45 * t2 + T46 * t3 + T47;
		T t10 = pow(t7, 2);
		T t11 = cos(T48 * phi);
		T t12 = pow(t7, 3);
		T t13 = cos(T49 * phi);
		T t14 = pow(t7, 4);
		return T50 * t13 * a[24] * t14 + T51 * t13 * a[16] * t14 + T52 * t11 * a[23] * t1 * t12 -
			T53 * t11 * a[17] * t1 * t12 + T54 * t8 * a[22] * t9 * t10 + T55 * t8 * a[18] * t9 * t10 +
			T56 * t5 * a[21] * t6 * t7 - T57 * t5 * a[19] * t6 * t7 +
			T58 * a[20] * (T59 * t2 + T60 * pow(t2, 4) - T61 * t4 + T62 * t3 + T63);
	}

	template <typename T>
	T calc_degree_5(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 5;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T64 - T65 * t1;
		T t3 = pow(t2, 2);
		T t4 = pow(t2, 3);
		T t5 = pow(t2, 4);
		T t6 = cos(T66 * phi);
		T t7 = T67 * t2 + T68 * t5 - T69 * t4 + T70 * t3 + T71;
		T t8 = sin(theta);
		T t9 = cos(T72 * phi);
		T t10 = T73 * t2 - T74 * t4 + T75 * t3 + T76;
		T t11 = pow(t8, 2);
		T t12 = cos(T77 * phi);
		T t13 = T78 * t2 + T79 * t3 + T80;
		T t14 = pow(t8, 3);
		T t15 = cos(T81 * phi);
		T t16 = pow(t8, 4);
		T t17 = cos(T82 * phi);
		T t18 = pow(t8, 5);
		return T83 * t17 * a[35] * t18 - T84 * t17 * a[25] * t18 + T85 * t15 * a[34] * t1 * t16 +
			T86 * t15 * a[26] * t1 * t16 + T87 * t12 * a[33] * t13 * t14 - T88 * t12 * a[27] * t13 * t14 +
			T89 * t9 * a[32] * t10 * t11 + T90 * t9 * a[28] * t10 * t11 + T91 * t6 * a[31] * t7 * t8 -
			T92 * t6 * a[29] * t7 * t8 +
			T93 * a[30] * (T94 * t2 - T95 * pow(t2, 5) + T96 * t5 - T97 * t4 + T98 * t3 + T99);
	}

	template <typename T>
	T calc_degree_6(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 6;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T100 - T101 * t1;
		T t3 = pow(t2, 2);
		T t4 = pow(t2, 3);
		T t5 = pow(t2, 4);
		T t6 = pow(t2, 5);
		T t7 = cos(T102 * phi);
		T t8 = T103 * t2 - T104 * t6 + T105 * t5 - T106 * t4 + T107 * t3 + T108;
		T t9 = sin(theta);
		T t10 = cos(T109 * phi);
		T t11 = T110 * t2 + T111 * t5 - T112 * t4 + T113 * t3 + T114;
		T t12 = pow(t9, 2);
		T t13 = cos(T115 * phi);
		T t14 = T116 * t2 - T117 * t4 + T118 * t3 + T119;
		T t15 = pow(t9, 3);
		T t16 = cos(T120 * phi);
		T t17 = T121 * t2 + T122 * t3 + T123;
		T t18 = pow(t9, 4);
		T t19 = cos(T124 * phi);
		T t20 = pow(t9, 5);
		T t21 = cos(T125 * phi);
		T t22 = pow(t9, 6);
		return T126 * t21 * a[48] * t22 + T127 * t21 * a[36] * t22 + T128 * t19 * a[47] * t1 * t20 -
			T129 * t19 * a[37] * t1 * t20 + T130 * t16 * a[46] * t17 * t18 + T131 * t16 * a[38] * t17 * t18 +
			T132 * t13 * a[45] * t14 * t15 - T133 * t13 * a[39] * t14 * t15 + T134 * t10 * a[44] * t11 * t12 +
			T135 * t10 * a[40] * t11 * t12 + T136 * t7 * a[43] * t8 * t9 - T137 * t7 * a[41] * t8 * t9 +
			T138 * a[42] * (T139 * t2 + T140 * pow(t2, 6) - T141 * t6 + T142 * t5 - T143 * t4 + T144 * t3 + T145);
	}

	template <typename T>
	T calc_degree_7(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 7;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T146 - T147 * t1;
		T t3 = pow(t2, 2);
		T t4 = pow(t2, 3);
		T t5 = pow(t2, 4);
		T t6 = pow(t2, 5);
		T t7 = pow(t2, 6);
		T t8 = cos(T148 * phi);
		T t9 = T149 * t2 + T150 * t7 - T151 * t6 + T152 * t5 - T153 * t4 + T154 * t3 + T155;
		T t10 = sin(theta);
		T t11 = cos(T156 * phi);
		T t12 = T157 * t2 - T158 * t6 + T159 * t5 - T160 * t4 + T161 * t3 + T162;
		T t13 = pow(t10, 2);
		T t14 = cos(T163 * phi);
		T t15 = T164 * t2 + T165 * t5 - T166 * t4 + T167 * t3 + T168;
		T t16 = pow(t10, 3);
		T t17 = cos(T169 * phi);
		T t18 = T170 * t2 - T171 * t4 + T172 * t3 + T173;
		T t19 = pow(t10, 4);
		T t20 = cos(T174 * phi);
		T t21 = T175 * t2 + T176 * t3 + T177;
		T t22 = pow(t10, 5);
		T t23 = cos(T178 * phi);
		T t24 = pow(t10, 6);
		T t25 = cos(T179 * phi);
		T t26 = pow(t10, 7);
		return T180 * t25 * a[63] * t26 - T181 * t25 * a[49] * t26 + T182 * t23 * a[62] * t1 * t24 +
			T183 * t23 * a[50] * t1 * t24 + T184 * t20 * a[61] * t21 * t22 - T185 * t20 * a[51] * t21 * t22 +
			T186 * t17 * a[60] * t18 * t19 + T187 * t17 * a[52] * t18 * t19 + T188 * t14 * a[59] * t15 * t16 -
			T189 * t14 * a[53] * t15 * t16 + T190 * t11 * a[58] * t12 * t13 + T191 * t11 * a[54] * t12 * t13 +
			T192 * t8 * a[57] * t9 * t10 - T193 * t8 * a[55] * t9 * t10 + T194 * a[56] * (T195 * t2 - T196 * pow(t2, 7) + T197 * t7 - T198 * t6 + T199 * t5 - T200 * t4 + T201 * t3 + T202);
	}

	template <typename T>
	T calc_degree_8(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 8;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T203 - T204 * t1;
		T t3 = pow(t2, 2);
		T t4 = pow(t2, 3);
		T t5 = pow(t2, 4);
		T t6 = pow(t2, 5);
		T t7 = pow(t2, 6);
		T t8 = pow(t2, 7);
		T t9 = cos(T205 * phi);
		T t10 = T206 * t2 - T207 * t8 + T208 * t7 - T209 * t6 + T210 * t5 - T211 * t4 + T212 * t3 + T213;
		T t11 = sin(theta);
		T t12 = cos(T214 * phi);
		T t13 = T215 * t2 + T216 * t7 - T217 * t6 + T218 * t5 - T219 * t4 + T220 * t3 + T221;
		T t14 = pow(t11, 2);
		T t15 = cos(T222 * phi);
		T t16 = T223 * t2 - T224 * t6 + T225 * t5 - T226 * t4 + T227 * t3 + T228;
		T t17 = pow(t11, 3);
		T t18 = cos(T229 * phi);
		T t19 = T230 * t2 + T231 * t5 - T232 * t4 + T233 * t3 + T234;
		T t20 = pow(t11, 4);
		T t21 = cos(T235 * phi);
		T t22 = T236 * t2 - T237 * t4 + T238 * t3 + T239;
		T t23 = pow(t11, 5);
		T t24 = cos(T240 * phi);
		T t25 = T241 * t2 + T242 * t3 + T243;
		T t26 = pow(t11, 6);
		T t27 = cos(T244 * phi);
		T t28 = pow(t11, 7);
		T t29 = cos(T245 * phi);
		T t30 = pow(t11, 8);
		return T246 * t29 * a[80] * t30 + T247 * t29 * a[64] * t30 + T248 * t27 * a[79] * t1 * t28 - T249 * t27 * a[65] * t1 * t28 + T250 * t24 * a[78] * t25 * t26 + T251 * t24 * a[66] * t25 * t26 + T252 * t21 * a[77] * t22 * t23 -
			T253 * t21 * a[67] * t22 * t23 + T254 * t18 * a[76] * t19 * t20 + T255 * t18 * a[68] * t19 * t20 +
			T256 * t15 * a[75] * t16 * t17 - T257 * t15 * a[69] * t16 * t17 + T258 * t12 * a[74] * t13 * t14 +
			T259 * t12 * a[70] * t13 * t14 + T260 * t9 * a[73] * t10 * t11 - T261 * t9 * a[71] * t10 * t11 + T262 * a[72] * (T263 * t2 + T264 * pow(t2, 8) - T265 * t8 + T266 * t7 - T267 * t6 + T268 * t5 - T269 * t4 + T270 * t3 + T271);
	}

	template <typename T>
	T calc_degree_9(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 9;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T272 - T273 * t1;
		T t3 = pow(t2, 2);
		T t4 = pow(t2, 3);
		T t5 = pow(t2, 4);
		T t6 = pow(t2, 5);
		T t7 = pow(t2, 6);
		T t8 = pow(t2, 7);
		T t9 = pow(t2, 8);
		T t10 = cos(T274 * phi);
		T t11 = T275 * t2 + T276 * t9 - T277 * t8 + T278 * t7 - T279 * t6 + T280 * t5 - T281 * t4 + T282 * t3 + T283;
		T t12 = sin(theta);
		T t13 = cos(T284 * phi);
		T t14 = T285 * t2 - T286 * t8 + T287 * t7 - T288 * t6 + T289 * t5 - T290 * t4 + T291 * t3 + T292;
		T t15 = pow(t12, 2);
		T t16 = cos(T293 * phi);
		T t17 = T294 * t2 + T295 * t7 - T296 * t6 + T297 * t5 - T298 * t4 + T299 * t3 + T300;
		T t18 = pow(t12, 3);
		T t19 = cos(T301 * phi);
		T t20 = T302 * t2 - T303 * t6 + T304 * t5 - T305 * t4 + T306 * t3 + T307;
		T t21 = pow(t12, 4);
		T t22 = cos(T308 * phi);
		T t23 = T309 * t2 + T310 * t5 - T311 * t4 + T312 * t3 + T313;
		T t24 = pow(t12, 5);
		T t25 = cos(T314 * phi);
		T t26 = T315 * t2 - T316 * t4 + T317 * t3 + T318;
		T t27 = pow(t12, 6);
		T t28 = cos(T319 * phi);
		T t29 = T320 * t2 + T321 * t3 + T322;
		T t30 = pow(t12, 7);
		T t31 = cos(T323 * phi);
		T t32 = pow(t12, 8);
		T t33 = cos(T324 * phi);
		T t34 = pow(t12, 9);
		return T325 * t33 * a[99] * t34 - T326 * t33 * a[81] * t34 + T327 * t31 * a[98] * t1 * t32 + T328 * t31 * a[82] * t1 * t32 + T329 * t28 * a[97] * t29 * t30 - T330 * t28 * a[83] * t29 * t30 + T331 * t25 * a[96] * t26 * t27 +
			T332 * t25 * a[84] * t26 * t27 + T333 * t22 * a[95] * t23 * t24 - T334 * t22 * a[85] * t23 * t24 +
			T335 * t19 * a[94] * t20 * t21 + T336 * t19 * a[86] * t20 * t21 + T337 * t16 * a[93] * t17 * t18 -
			T338 * t16 * a[87] * t17 * t18 + T339 * t13 * a[92] * t14 * t15 + T340 * t13 * a[88] * t14 * t15 +
			T341 * t10 * a[91] * t11 * t12 - T342 * t10 * a[89] * t11 * t12 + T343 * a[90] * (T344 * t2 - T345 * pow(t2, 9) + T346 * t9 - T347 * t8 + T348 * t7 - T349 * t6 + T350 * t5 - T351 * t4 + T352 * t3 + T353);
	}

	template <typename T>
	T calc_degree_10(const T *a, T theta, T phi)
	{
		if (debug_sph) // && randomSampler(0.0f, 1.0f) > 0.5f)
		{
			const int l = 10;
			double accum = 0.0;
			for (int m = -l; m <= l; m++) {
				const int lm = l * (l + 1) + m;
				accum += a[lm] * spherical_harmonic(l, m, theta, phi);
			}
			return (T)accum;
		}

		T t1 = cos(theta);
		T t2 = T354 - T355 * t1;
		T t3 = pow(t2, 2);
		T t4 = pow(t2, 3);
		T t5 = pow(t2, 4);
		T t6 = pow(t2, 5);
		T t7 = pow(t2, 6);
		T t8 = pow(t2, 7);
		T t9 = pow(t2, 8);
		T t10 = pow(t2, 9);
		T t11 = cos(T356 * phi);
		T t12 = T357 * t2 - T358 * t10 + T359 * t9 - T360 * t8 + T361 * t7 - T362 * t6 + T363 * t5 - T364 * t4 +
			T365 * t3 + T366;
		T t13 = sin(theta);
		T t14 = cos(T367 * phi);
		T t15 = T368 * t2 + T369 * t9 - T370 * t8 + T371 * t7 - T372 * t6 + T373 * t5 - T374 * t4 + T375 * t3 + T376;
		T t16 = pow(t13, 2);
		T t17 = cos(T377 * phi);
		T t18 = T378 * t2 - T379 * t8 + T380 * t7 - T381 * t6 + T382 * t5 - T383 * t4 + T384 * t3 + T385;
		T t19 = pow(t13, 3);
		T t20 = cos(T386 * phi);
		T t21 = T387 * t2 + T388 * t7 - T389 * t6 + T390 * t5 - T391 * t4 + T392 * t3 + T393;
		T t22 = pow(t13, 4);
		T t23 = cos(T394 * phi);
		T t24 = T395 * t2 - T396 * t6 + T397 * t5 - T398 * t4 + T399 * t3 + T400;
		T t25 = pow(t13, 5);
		T t26 = cos(T401 * phi);
		T t27 = T402 * t2 + T403 * t5 - T404 * t4 + T405 * t3 + T406;
		T t28 = pow(t13, 6);
		T t29 = cos(T407 * phi);
		T t30 = T408 * t2 - T409 * t4 + T410 * t3 + T411;
		T t31 = pow(t13, 7);
		T t32 = cos(T412 * phi);
		T t33 = T413 * t2 + T414 * t3 + T415;
		T t34 = pow(t13, 8);
		T t35 = cos(T416 * phi);
		T t36 = pow(t13, 9);
		T t37 = cos(T417 * phi);
		T t38 = pow(t13, 10);
		return T418 * t37 * a[120] * t38 + T419 * t37 * a[100] * t38 + T420 * t35 * a[119] * t1 * t36 -
			T421 * t35 * a[101] * t1 * t36 + T422 * t32 * a[118] * t33 * t34 + T423 * t32 * a[102] * t33 * t34 +
			T424 * t29 * a[117] * t30 * t31 - T425 * t29 * a[103] * t30 * t31 + T426 * t26 * a[116] * t27 * t28 +
			T427 * t26 * a[104] * t27 * t28 + T428 * t23 * a[115] * t24 * t25 - T429 * t23 * a[105] * t24 * t25 +
			T430 * t20 * a[114] * t21 * t22 + T431 * t20 * a[106] * t21 * t22 + T432 * t17 * a[113] * t18 * t19 -
			T433 * t17 * a[107] * t18 * t19 + T434 * t14 * a[112] * t15 * t16 + T435 * t14 * a[108] * t15 * t16 +
			T436 * t11 * a[111] * t12 * t13 - T437 * t11 * a[109] * t12 * t13 + T438 * a[110] * (T439 * t2 + T440 * pow(t2, 10) - T441 * t10 + T442 * t9 - T443 * t8 + T444 * t7 - T445 * t6 + T446 * t5 - T447 * t4 + T448 * t3 + T449);
	}

	template <typename T>
	T calc_spherical_harmonic(int degree, int band, const T theta, const T phi)
	{
		// if (degree == 0) return calc_degree_0();
		return T(0);
	}

	template <typename T>
	T calc_spherical_harmonic(int max_degree, const T *a, const T theta, const T phi)
	{
		T accum = 0;
		if (max_degree >= 0) {
			accum += calc_degree_0<T>(a, theta, phi);
			if (max_degree >= 1) {
				accum += calc_degree_1<T>(a, theta, phi);
				if (max_degree >= 2) {
					accum += calc_degree_2<T>(a, theta, phi);
					if (max_degree >= 3) {
						accum += calc_degree_3<T>(a, theta, phi);
						if (max_degree >= 4) {
							accum += calc_degree_4<T>(a, theta, phi);
							if (max_degree >= 5) {
								accum += calc_degree_5<T>(a, theta, phi);
								if (max_degree >= 6) {
									accum += calc_degree_6<T>(a, theta, phi);
									if (max_degree >= 7) {
										accum += calc_degree_7<T>(a, theta, phi);
										if (max_degree >= 8) {
											accum += calc_degree_8<T>(a, theta, phi);
											if (max_degree >= 9) {
												accum += calc_degree_9<T>(a, theta, phi);
												if (max_degree >= 10) {
													accum += calc_degree_10<T>(a, theta, phi);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return accum;
	}

	template float calc_degree_0<float>(const float *a, float theta, float phi);
	template float calc_degree_1<float>(const float *a, float theta, float phi);
	template float calc_degree_2<float>(const float *a, float theta, float phi);
	template float calc_degree_3<float>(const float *a, float theta, float phi);
	template float calc_degree_4<float>(const float *a, float theta, float phi);
	template float calc_degree_5<float>(const float *a, float theta, float phi);
	template float calc_degree_6<float>(const float *a, float theta, float phi);
	template float calc_degree_7<float>(const float *a, float theta, float phi);
	template float calc_degree_8<float>(const float *a, float theta, float phi);
	template float calc_degree_9<float>(const float *a, float theta, float phi);
	template float calc_degree_10<float>(const float *a, float theta, float phi);

	template double calc_degree_0<double>(const double *a, double theta, double phi);
	template double calc_degree_1<double>(const double *a, double theta, double phi);
	template double calc_degree_2<double>(const double *a, double theta, double phi);
	template double calc_degree_3<double>(const double *a, double theta, double phi);
	template double calc_degree_4<double>(const double *a, double theta, double phi);
	template double calc_degree_5<double>(const double *a, double theta, double phi);
	template double calc_degree_6<double>(const double *a, double theta, double phi);
	template double calc_degree_7<double>(const double *a, double theta, double phi);
	template double calc_degree_8<double>(const double *a, double theta, double phi);
	template double calc_degree_9<double>(const double *a, double theta, double phi);
	template double calc_degree_10<double>(const double *a, double theta, double phi);

	template double calc_spherical_harmonic<double>(int, const double *, double, double);
	template float calc_spherical_harmonic<float>(int, const float *, float, float);

	// TODO: Move SaveJSON to a higher library
	void Sph4f::SaveJSON(const std::string& path, const std::string& name, const Vector3f& position)
	{

	}
	//void Sph4f::SaveJSON(const std::string &path, const std::string &name, const Vector3f &position)
	//{
	//	FilePathInfo fpi(path);
	//	Df::JSONPtr json = Df::JSON::MakeObject({
	//		{"numChannels", Df::JSON::MakeNumber(3)},
	//		{"maxDegree", Df::JSON::MakeNumber((int)msph[0].GetMaxDegree())},
	//		{"coefs", Df::JSON::MakeArray()},
	//		{"meta", Df::JSON::MakeObject()}
	//		});

	//	auto coefs = json->getMember("coefs");
	//	for (unsigned lm = 0; lm < msph->getMaxCoefficients(); lm++) {
	//		float r = msph[0].getCoefficient(lm);
	//		float g = msph[1].getCoefficient(lm);
	//		float b = msph[2].getCoefficient(lm);
	//		coefs->PushBack(Df::JSON::MakeArray({ r, g, b }));
	//	}

	//	auto meta = json->getMember("meta");
	//	meta->PushBack(Df::JSON::MakeObject({
	//		{ "name", Df::JSON::MakeString(name) },
	//		{ "position", Df::JSON::MakeArray({
	//			position.x,
	//			position.y,
	//			position.z
	//			})
	//		}
	//		}));

	//	std::ofstream fout(fpi.path);
	//	fout << json->Serialize();
	//	fout.close();
	//}

	bool Sph4f::fromVectorFormat(int maxDegrees, const std::vector<std::vector<float>> & v)
	{
		// do a sanity check on the incoming data
		unsigned lmCount = maxDegree * (maxDegree + 1) + maxDegree + 1;
		if (lmCount != v.size()) return false;

		// then attempt to resize to fit the incoming data
		resize(maxDegrees);

		// then ensure that we only copy the acceptable amount of data
		maxDegrees = std::min(maxDegrees, maxDegree);
		lmCount = maxDegree * (maxDegree + 1) + maxDegree + 1;

		for (unsigned lm = 0; lm < lmCount; lm++) {
			auto & v_element = v[lm];
			unsigned c = (unsigned)v_element.size();
			if (c >= 1) msph[0].setCoefficient(lm, v_element[0]);
			else msph[0].setCoefficient(lm, 0.0f);
			if (c >= 2) msph[1].setCoefficient(lm, v_element[1]);
			else msph[0].setCoefficient(lm, 0.0f);
			if (c >= 3) msph[2].setCoefficient(lm, v_element[2]);
			else msph[0].setCoefficient(lm, 0.0f);
			if (c >= 4) msph[3].setCoefficient(lm, v_element[3]);
			else msph[0].setCoefficient(lm, 0.0f);
		}
		return true;
	}

	bool Sph4f::toVectorFormat(int numChannels, std::vector<std::vector<float>> & v)
	{
		if (numChannels < 1 || numChannels > 4) return false;
		v.resize(size());
		int lmCount = (int)msph->getMaxCoefficients();
		for (unsigned lm = 0; lm < (unsigned)lmCount; lm++) {
			auto & v_element = v[lm];
			v_element.resize(numChannels);
			for (int i = 0; i < numChannels; i++) {
				v_element[i] = msph[i].getCoefficient(lm);
			}
		}
		return true;
	}
} // namespace Fluxions
