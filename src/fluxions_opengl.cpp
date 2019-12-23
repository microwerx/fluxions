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
#include "pch.hpp"
#include <sstream>
#include <map>
#include <string>
#include <fluxions_opengl.hpp>

namespace Fluxions
{
	OpenGLNameTranslator glNameTranslator;

	//////////////////////////////////////////////////////////////////////
	// O p e n G L N a m e T r a n s l a t o r ///////////////////////////
	//////////////////////////////////////////////////////////////////////

	OpenGLNameTranslator::OpenGLNameTranslator() {
		enums["GL_DEPTH_ATTACHMENT"] = GL_DEPTH_ATTACHMENT;
		enums["GL_STENCIL_ATTACHMENT"] = GL_STENCIL_ATTACHMENT;
		enums["GL_COLOR_ATTACHMENT0"] = GL_COLOR_ATTACHMENT0;
		enums["GL_COLOR_ATTACHMENT1"] = GL_COLOR_ATTACHMENT1;
		enums["GL_COLOR_ATTACHMENT2"] = GL_COLOR_ATTACHMENT2;
		enums["GL_COLOR_ATTACHMENT3"] = GL_COLOR_ATTACHMENT3;
		enums["GL_COLOR_ATTACHMENT4"] = GL_COLOR_ATTACHMENT4;
		enums["GL_COLOR_ATTACHMENT5"] = GL_COLOR_ATTACHMENT5;
		enums["GL_COLOR_ATTACHMENT6"] = GL_COLOR_ATTACHMENT6;
		enums["GL_COLOR_ATTACHMENT7"] = GL_COLOR_ATTACHMENT7;
		enums["GL_COLOR_ATTACHMENT8"] = GL_COLOR_ATTACHMENT8;
		enums["GL_COLOR_ATTACHMENT9"] = GL_COLOR_ATTACHMENT9;
		enums["GL_COLOR_ATTACHMENT10"] = GL_COLOR_ATTACHMENT10;
		enums["GL_COLOR_ATTACHMENT11"] = GL_COLOR_ATTACHMENT11;
		enums["GL_COLOR_ATTACHMENT12"] = GL_COLOR_ATTACHMENT12;
		enums["GL_COLOR_ATTACHMENT13"] = GL_COLOR_ATTACHMENT13;
		enums["GL_COLOR_ATTACHMENT14"] = GL_COLOR_ATTACHMENT14;
		enums["GL_COLOR_ATTACHMENT15"] = GL_COLOR_ATTACHMENT15;
		enums["GL_ZERO"] = 0;
		enums["GL_FALSE"] = 0;
		enums["GL_LOGIC_OP"] = 0x0BF1;
		enums["GL_NONE"] = 0;
		enums["GL_TEXTURE_COMPONENTS"] = 0x1003;
		enums["GL_NO_ERROR"] = 0;
		enums["GL_POINTS"] = 0x0000;
		enums["GL_CURRENT_BIT"] = 0x00000001;
		enums["GL_TRUE"] = 1;
		enums["GL_ONE"] = 1;
		enums["GL_CLIENT_PIXEL_STORE_BIT"] = 0x00000001;
		enums["GL_LINES"] = 0x0001;
		enums["GL_LINE_LOOP"] = 0x0002;
		enums["GL_POINT_BIT"] = 0x00000002;
		enums["GL_CLIENT_VERTEX_ARRAY_BIT"] = 0x00000002;
		enums["GL_LINE_STRIP"] = 0x0003;
		enums["GL_LINE_BIT"] = 0x00000004;
		enums["GL_TRIANGLES"] = 0x0004;
		enums["GL_TRIANGLE_STRIP"] = 0x0005;
		enums["GL_TRIANGLE_FAN"] = 0x0006;
		enums["GL_QUADS"] = 0x0007;
		enums["GL_QUAD_STRIP"] = 0x0008;
		enums["GL_POLYGON_BIT"] = 0x00000008;
		enums["GL_POLYGON"] = 0x0009;
		enums["GL_POLYGON_STIPPLE_BIT"] = 0x00000010;
		enums["GL_PIXEL_MODE_BIT"] = 0x00000020;
		enums["GL_LIGHTING_BIT"] = 0x00000040;
		enums["GL_FOG_BIT"] = 0x00000080;
		enums["GL_DEPTH_BUFFER_BIT"] = 0x00000100;
		enums["GL_ACCUM"] = 0x0100;
		enums["GL_LOAD"] = 0x0101;
		enums["GL_RETURN"] = 0x0102;
		enums["GL_MULT"] = 0x0103;
		enums["GL_ADD"] = 0x0104;
		enums["GL_NEVER"] = 0x0200;
		enums["GL_ACCUM_BUFFER_BIT"] = 0x00000200;
		enums["GL_LESS"] = 0x0201;
		enums["GL_EQUAL"] = 0x0202;
		enums["GL_LEQUAL"] = 0x0203;
		enums["GL_GREATER"] = 0x0204;
		enums["GL_NOTEQUAL"] = 0x0205;
		enums["GL_GEQUAL"] = 0x0206;
		enums["GL_ALWAYS"] = 0x0207;
		enums["GL_SRC_COLOR"] = 0x0300;
		enums["GL_ONE_MINUS_SRC_COLOR"] = 0x0301;
		enums["GL_SRC_ALPHA"] = 0x0302;
		enums["GL_ONE_MINUS_SRC_ALPHA"] = 0x0303;
		enums["GL_DST_ALPHA"] = 0x0304;
		enums["GL_ONE_MINUS_DST_ALPHA"] = 0x0305;
		enums["GL_DST_COLOR"] = 0x0306;
		enums["GL_ONE_MINUS_DST_COLOR"] = 0x0307;
		enums["GL_SRC_ALPHA_SATURATE"] = 0x0308;
		enums["GL_STENCIL_BUFFER_BIT"] = 0x00000400;
		enums["GL_FRONT_LEFT"] = 0x0400;
		enums["GL_FRONT_RIGHT"] = 0x0401;
		enums["GL_BACK_LEFT"] = 0x0402;
		enums["GL_BACK_RIGHT"] = 0x0403;
		enums["GL_FRONT"] = 0x0404;
		enums["GL_BACK"] = 0x0405;
		enums["GL_LEFT"] = 0x0406;
		enums["GL_RIGHT"] = 0x0407;
		enums["GL_FRONT_AND_BACK"] = 0x0408;
		enums["GL_AUX0"] = 0x0409;
		enums["GL_AUX1"] = 0x040A;
		enums["GL_AUX2"] = 0x040B;
		enums["GL_AUX3"] = 0x040C;
		enums["GL_INVALID_ENUM"] = 0x0500;
		enums["GL_INVALID_VALUE"] = 0x0501;
		enums["GL_INVALID_OPERATION"] = 0x0502;
		enums["GL_STACK_OVERFLOW"] = 0x0503;
		enums["GL_STACK_UNDERFLOW"] = 0x0504;
		enums["GL_OUT_OF_MEMORY"] = 0x0505;
		enums["GL_2D"] = 0x0600;
		enums["GL_3D"] = 0x0601;
		enums["GL_3D_COLOR"] = 0x0602;
		enums["GL_3D_COLOR_TEXTURE"] = 0x0603;
		enums["GL_4D_COLOR_TEXTURE"] = 0x0604;
		enums["GL_PASS_THROUGH_TOKEN"] = 0x0700;
		enums["GL_POINT_TOKEN"] = 0x0701;
		enums["GL_LINE_TOKEN"] = 0x0702;
		enums["GL_POLYGON_TOKEN"] = 0x0703;
		enums["GL_BITMAP_TOKEN"] = 0x0704;
		enums["GL_DRAW_PIXEL_TOKEN"] = 0x0705;
		enums["GL_COPY_PIXEL_TOKEN"] = 0x0706;
		enums["GL_LINE_RESET_TOKEN"] = 0x0707;
		enums["GL_EXP"] = 0x0800;
		enums["GL_VIEWPORT_BIT"] = 0x00000800;
		enums["GL_EXP2"] = 0x0801;
		enums["GL_CW"] = 0x0900;
		enums["GL_CCW"] = 0x0901;
		enums["GL_COEFF"] = 0x0A00;
		enums["GL_ORDER"] = 0x0A01;
		enums["GL_DOMAIN"] = 0x0A02;
		enums["GL_CURRENT_COLOR"] = 0x0B00;
		enums["GL_CURRENT_INDEX"] = 0x0B01;
		enums["GL_CURRENT_NORMAL"] = 0x0B02;
		enums["GL_CURRENT_TEXTURE_COORDS"] = 0x0B03;
		enums["GL_CURRENT_RASTER_COLOR"] = 0x0B04;
		enums["GL_CURRENT_RASTER_INDEX"] = 0x0B05;
		enums["GL_CURRENT_RASTER_TEXTURE_COORDS"] = 0x0B06;
		enums["GL_CURRENT_RASTER_POSITION"] = 0x0B07;
		enums["GL_CURRENT_RASTER_POSITION_VALID"] = 0x0B08;
		enums["GL_CURRENT_RASTER_DISTANCE"] = 0x0B09;
		enums["GL_POINT_SMOOTH"] = 0x0B10;
		enums["GL_POINT_SIZE"] = 0x0B11;
		enums["GL_POINT_SIZE_RANGE"] = 0x0B12;
		enums["GL_POINT_SIZE_GRANULARITY"] = 0x0B13;
		enums["GL_LINE_SMOOTH"] = 0x0B20;
		enums["GL_LINE_WIDTH"] = 0x0B21;
		enums["GL_LINE_WIDTH_RANGE"] = 0x0B22;
		enums["GL_LINE_WIDTH_GRANULARITY"] = 0x0B23;
		enums["GL_LINE_STIPPLE"] = 0x0B24;
		enums["GL_LINE_STIPPLE_PATTERN"] = 0x0B25;
		enums["GL_LINE_STIPPLE_REPEAT"] = 0x0B26;
		enums["GL_LIST_MODE"] = 0x0B30;
		enums["GL_MAX_LIST_NESTING"] = 0x0B31;
		enums["GL_LIST_BASE"] = 0x0B32;
		enums["GL_LIST_INDEX"] = 0x0B33;
		enums["GL_POLYGON_MODE"] = 0x0B40;
		enums["GL_POLYGON_SMOOTH"] = 0x0B41;
		enums["GL_POLYGON_STIPPLE"] = 0x0B42;
		enums["GL_EDGE_FLAG"] = 0x0B43;
		enums["GL_CULL_FACE"] = 0x0B44;
		enums["GL_CULL_FACE_MODE"] = 0x0B45;
		enums["GL_FRONT_FACE"] = 0x0B46;
		enums["GL_LIGHTING"] = 0x0B50;
		enums["GL_LIGHT_MODEL_LOCAL_VIEWER"] = 0x0B51;
		enums["GL_LIGHT_MODEL_TWO_SIDE"] = 0x0B52;
		enums["GL_LIGHT_MODEL_AMBIENT"] = 0x0B53;
		enums["GL_SHADE_MODEL"] = 0x0B54;
		enums["GL_COLOR_MATERIAL_FACE"] = 0x0B55;
		enums["GL_COLOR_MATERIAL_PARAMETER"] = 0x0B56;
		enums["GL_COLOR_MATERIAL"] = 0x0B57;
		enums["GL_FOG"] = 0x0B60;
		enums["GL_FOG_INDEX"] = 0x0B61;
		enums["GL_FOG_DENSITY"] = 0x0B62;
		enums["GL_FOG_START"] = 0x0B63;
		enums["GL_FOG_END"] = 0x0B64;
		enums["GL_FOG_MODE"] = 0x0B65;
		enums["GL_FOG_COLOR"] = 0x0B66;
		enums["GL_DEPTH_RANGE"] = 0x0B70;
		enums["GL_DEPTH_TEST"] = 0x0B71;
		enums["GL_DEPTH_WRITEMASK"] = 0x0B72;
		enums["GL_DEPTH_CLEAR_VALUE"] = 0x0B73;
		enums["GL_DEPTH_FUNC"] = 0x0B74;
		enums["GL_ACCUM_CLEAR_VALUE"] = 0x0B80;
		enums["GL_STENCIL_TEST"] = 0x0B90;
		enums["GL_STENCIL_CLEAR_VALUE"] = 0x0B91;
		enums["GL_STENCIL_FUNC"] = 0x0B92;
		enums["GL_STENCIL_VALUE_MASK"] = 0x0B93;
		enums["GL_STENCIL_FAIL"] = 0x0B94;
		enums["GL_STENCIL_PASS_DEPTH_FAIL"] = 0x0B95;
		enums["GL_STENCIL_PASS_DEPTH_PASS"] = 0x0B96;
		enums["GL_STENCIL_REF"] = 0x0B97;
		enums["GL_STENCIL_WRITEMASK"] = 0x0B98;
		enums["GL_MATRIX_MODE"] = 0x0BA0;
		enums["GL_NORMALIZE"] = 0x0BA1;
		enums["GL_VIEWPORT"] = 0x0BA2;
		enums["GL_MODELVIEW_STACK_DEPTH"] = 0x0BA3;
		enums["GL_PROJECTION_STACK_DEPTH"] = 0x0BA4;
		enums["GL_TEXTURE_STACK_DEPTH"] = 0x0BA5;
		enums["GL_MODELVIEW_MATRIX"] = 0x0BA6;
		enums["GL_PROJECTION_MATRIX"] = 0x0BA7;
		enums["GL_TEXTURE_MATRIX"] = 0x0BA8;
		enums["GL_ATTRIB_STACK_DEPTH"] = 0x0BB0;
		enums["GL_CLIENT_ATTRIB_STACK_DEPTH"] = 0x0BB1;
		enums["GL_ALPHA_TEST"] = 0x0BC0;
		enums["GL_ALPHA_TEST_FUNC"] = 0x0BC1;
		enums["GL_ALPHA_TEST_REF"] = 0x0BC2;
		enums["GL_DITHER"] = 0x0BD0;
		enums["GL_BLEND_DST"] = 0x0BE0;
		enums["GL_BLEND_SRC"] = 0x0BE1;
		enums["GL_BLEND"] = 0x0BE2;
		enums["GL_LOGIC_OP_MODE"] = 0x0BF0;
		enums["GL_INDEX_LOGIC_OP"] = 0x0BF1;
		enums["GL_COLOR_LOGIC_OP"] = 0x0BF2;
		enums["GL_AUX_BUFFERS"] = 0x0C00;
		enums["GL_DRAW_BUFFER"] = 0x0C01;
		enums["GL_READ_BUFFER"] = 0x0C02;
		enums["GL_SCISSOR_BOX"] = 0x0C10;
		enums["GL_SCISSOR_TEST"] = 0x0C11;
		enums["GL_INDEX_CLEAR_VALUE"] = 0x0C20;
		enums["GL_INDEX_WRITEMASK"] = 0x0C21;
		enums["GL_COLOR_CLEAR_VALUE"] = 0x0C22;
		enums["GL_COLOR_WRITEMASK"] = 0x0C23;
		enums["GL_INDEX_MODE"] = 0x0C30;
		enums["GL_RGBA_MODE"] = 0x0C31;
		enums["GL_DOUBLEBUFFER"] = 0x0C32;
		enums["GL_STEREO"] = 0x0C33;
		enums["GL_RENDER_MODE"] = 0x0C40;
		enums["GL_PERSPECTIVE_CORRECTION_HINT"] = 0x0C50;
		enums["GL_POINT_SMOOTH_HINT"] = 0x0C51;
		enums["GL_LINE_SMOOTH_HINT"] = 0x0C52;
		enums["GL_POLYGON_SMOOTH_HINT"] = 0x0C53;
		enums["GL_FOG_HINT"] = 0x0C54;
		enums["GL_TEXTURE_GEN_S"] = 0x0C60;
		enums["GL_TEXTURE_GEN_T"] = 0x0C61;
		enums["GL_TEXTURE_GEN_R"] = 0x0C62;
		enums["GL_TEXTURE_GEN_Q"] = 0x0C63;
		enums["GL_PIXEL_MAP_I_TO_I"] = 0x0C70;
		enums["GL_PIXEL_MAP_S_TO_S"] = 0x0C71;
		enums["GL_PIXEL_MAP_I_TO_R"] = 0x0C72;
		enums["GL_PIXEL_MAP_I_TO_G"] = 0x0C73;
		enums["GL_PIXEL_MAP_I_TO_B"] = 0x0C74;
		enums["GL_PIXEL_MAP_I_TO_A"] = 0x0C75;
		enums["GL_PIXEL_MAP_R_TO_R"] = 0x0C76;
		enums["GL_PIXEL_MAP_G_TO_G"] = 0x0C77;
		enums["GL_PIXEL_MAP_B_TO_B"] = 0x0C78;
		enums["GL_PIXEL_MAP_A_TO_A"] = 0x0C79;
		enums["GL_PIXEL_MAP_I_TO_I_SIZE"] = 0x0CB0;
		enums["GL_PIXEL_MAP_S_TO_S_SIZE"] = 0x0CB1;
		enums["GL_PIXEL_MAP_I_TO_R_SIZE"] = 0x0CB2;
		enums["GL_PIXEL_MAP_I_TO_G_SIZE"] = 0x0CB3;
		enums["GL_PIXEL_MAP_I_TO_B_SIZE"] = 0x0CB4;
		enums["GL_PIXEL_MAP_I_TO_A_SIZE"] = 0x0CB5;
		enums["GL_PIXEL_MAP_R_TO_R_SIZE"] = 0x0CB6;
		enums["GL_PIXEL_MAP_G_TO_G_SIZE"] = 0x0CB7;
		enums["GL_PIXEL_MAP_B_TO_B_SIZE"] = 0x0CB8;
		enums["GL_PIXEL_MAP_A_TO_A_SIZE"] = 0x0CB9;
		enums["GL_UNPACK_SWAP_BYTES"] = 0x0CF0;
		enums["GL_UNPACK_LSB_FIRST"] = 0x0CF1;
		enums["GL_UNPACK_ROW_LENGTH"] = 0x0CF2;
		enums["GL_UNPACK_SKIP_ROWS"] = 0x0CF3;
		enums["GL_UNPACK_SKIP_PIXELS"] = 0x0CF4;
		enums["GL_UNPACK_ALIGNMENT"] = 0x0CF5;
		enums["GL_PACK_SWAP_BYTES"] = 0x0D00;
		enums["GL_PACK_LSB_FIRST"] = 0x0D01;
		enums["GL_PACK_ROW_LENGTH"] = 0x0D02;
		enums["GL_PACK_SKIP_ROWS"] = 0x0D03;
		enums["GL_PACK_SKIP_PIXELS"] = 0x0D04;
		enums["GL_PACK_ALIGNMENT"] = 0x0D05;
		enums["GL_MAP_COLOR"] = 0x0D10;
		enums["GL_MAP_STENCIL"] = 0x0D11;
		enums["GL_INDEX_SHIFT"] = 0x0D12;
		enums["GL_INDEX_OFFSET"] = 0x0D13;
		enums["GL_RED_SCALE"] = 0x0D14;
		enums["GL_RED_BIAS"] = 0x0D15;
		enums["GL_ZOOM_X"] = 0x0D16;
		enums["GL_ZOOM_Y"] = 0x0D17;
		enums["GL_GREEN_SCALE"] = 0x0D18;
		enums["GL_GREEN_BIAS"] = 0x0D19;
		enums["GL_BLUE_SCALE"] = 0x0D1A;
		enums["GL_BLUE_BIAS"] = 0x0D1B;
		enums["GL_ALPHA_SCALE"] = 0x0D1C;
		enums["GL_ALPHA_BIAS"] = 0x0D1D;
		enums["GL_DEPTH_SCALE"] = 0x0D1E;
		enums["GL_DEPTH_BIAS"] = 0x0D1F;
		enums["GL_MAX_EVAL_ORDER"] = 0x0D30;
		enums["GL_MAX_LIGHTS"] = 0x0D31;
		enums["GL_MAX_CLIP_PLANES"] = 0x0D32;
		enums["GL_MAX_TEXTURE_SIZE"] = 0x0D33;
		enums["GL_MAX_PIXEL_MAP_TABLE"] = 0x0D34;
		enums["GL_MAX_ATTRIB_STACK_DEPTH"] = 0x0D35;
		enums["GL_MAX_MODELVIEW_STACK_DEPTH"] = 0x0D36;
		enums["GL_MAX_NAME_STACK_DEPTH"] = 0x0D37;
		enums["GL_MAX_PROJECTION_STACK_DEPTH"] = 0x0D38;
		enums["GL_MAX_TEXTURE_STACK_DEPTH"] = 0x0D39;
		enums["GL_MAX_VIEWPORT_DIMS"] = 0x0D3A;
		enums["GL_MAX_CLIENT_ATTRIB_STACK_DEPTH"] = 0x0D3B;
		enums["GL_SUBPIXEL_BITS"] = 0x0D50;
		enums["GL_INDEX_BITS"] = 0x0D51;
		enums["GL_RED_BITS"] = 0x0D52;
		enums["GL_GREEN_BITS"] = 0x0D53;
		enums["GL_BLUE_BITS"] = 0x0D54;
		enums["GL_ALPHA_BITS"] = 0x0D55;
		enums["GL_DEPTH_BITS"] = 0x0D56;
		enums["GL_STENCIL_BITS"] = 0x0D57;
		enums["GL_ACCUM_RED_BITS"] = 0x0D58;
		enums["GL_ACCUM_GREEN_BITS"] = 0x0D59;
		enums["GL_ACCUM_BLUE_BITS"] = 0x0D5A;
		enums["GL_ACCUM_ALPHA_BITS"] = 0x0D5B;
		enums["GL_NAME_STACK_DEPTH"] = 0x0D70;
		enums["GL_AUTO_NORMAL"] = 0x0D80;
		enums["GL_MAP1_COLOR_4"] = 0x0D90;
		enums["GL_MAP1_INDEX"] = 0x0D91;
		enums["GL_MAP1_NORMAL"] = 0x0D92;
		enums["GL_MAP1_TEXTURE_COORD_1"] = 0x0D93;
		enums["GL_MAP1_TEXTURE_COORD_2"] = 0x0D94;
		enums["GL_MAP1_TEXTURE_COORD_3"] = 0x0D95;
		enums["GL_MAP1_TEXTURE_COORD_4"] = 0x0D96;
		enums["GL_MAP1_VERTEX_3"] = 0x0D97;
		enums["GL_MAP1_VERTEX_4"] = 0x0D98;
		enums["GL_MAP2_COLOR_4"] = 0x0DB0;
		enums["GL_MAP2_INDEX"] = 0x0DB1;
		enums["GL_MAP2_NORMAL"] = 0x0DB2;
		enums["GL_MAP2_TEXTURE_COORD_1"] = 0x0DB3;
		enums["GL_MAP2_TEXTURE_COORD_2"] = 0x0DB4;
		enums["GL_MAP2_TEXTURE_COORD_3"] = 0x0DB5;
		enums["GL_MAP2_TEXTURE_COORD_4"] = 0x0DB6;
		enums["GL_MAP2_VERTEX_3"] = 0x0DB7;
		enums["GL_MAP2_VERTEX_4"] = 0x0DB8;
		enums["GL_MAP1_GRID_DOMAIN"] = 0x0DD0;
		enums["GL_MAP1_GRID_SEGMENTS"] = 0x0DD1;
		enums["GL_MAP2_GRID_DOMAIN"] = 0x0DD2;
		enums["GL_MAP2_GRID_SEGMENTS"] = 0x0DD3;
		enums["GL_TEXTURE_1D"] = 0x0DE0;
		enums["GL_TEXTURE_2D"] = 0x0DE1;
		enums["GL_FEEDBACK_BUFFER_POINTER"] = 0x0DF0;
		enums["GL_FEEDBACK_BUFFER_SIZE"] = 0x0DF1;
		enums["GL_FEEDBACK_BUFFER_TYPE"] = 0x0DF2;
		enums["GL_SELECTION_BUFFER_POINTER"] = 0x0DF3;
		enums["GL_SELECTION_BUFFER_SIZE"] = 0x0DF4;
		enums["GL_TEXTURE_WIDTH"] = 0x1000;
		enums["GL_TRANSFORM_BIT"] = 0x00001000;
		enums["GL_TEXTURE_HEIGHT"] = 0x1001;
		enums["GL_TEXTURE_INTERNAL_FORMAT"] = 0x1003;
		enums["GL_TEXTURE_BORDER_COLOR"] = 0x1004;
		enums["GL_TEXTURE_BORDER"] = 0x1005;
		enums["GL_DONT_CARE"] = 0x1100;
		enums["GL_FASTEST"] = 0x1101;
		enums["GL_NICEST"] = 0x1102;
		enums["GL_AMBIENT"] = 0x1200;
		enums["GL_DIFFUSE"] = 0x1201;
		enums["GL_SPECULAR"] = 0x1202;
		enums["GL_POSITION"] = 0x1203;
		enums["GL_SPOT_DIRECTION"] = 0x1204;
		enums["GL_SPOT_EXPONENT"] = 0x1205;
		enums["GL_SPOT_CUTOFF"] = 0x1206;
		enums["GL_CONSTANT_ATTENUATION"] = 0x1207;
		enums["GL_LINEAR_ATTENUATION"] = 0x1208;
		enums["GL_QUADRATIC_ATTENUATION"] = 0x1209;
		enums["GL_COMPILE"] = 0x1300;
		enums["GL_COMPILE_AND_EXECUTE"] = 0x1301;
		enums["GL_BYTE"] = 0x1400;
		enums["GL_UNSIGNED_BYTE"] = 0x1401;
		enums["GL_SHORT"] = 0x1402;
		enums["GL_UNSIGNED_SHORT"] = 0x1403;
		enums["GL_INT"] = 0x1404;
		enums["GL_UNSIGNED_INT"] = 0x1405;
		enums["GL_FLOAT"] = 0x1406;
		enums["GL_2_BYTES"] = 0x1407;
		enums["GL_3_BYTES"] = 0x1408;
		enums["GL_4_BYTES"] = 0x1409;
		enums["GL_DOUBLE"] = 0x140A;
		enums["GL_CLEAR"] = 0x1500;
		enums["GL_AND"] = 0x1501;
		enums["GL_AND_REVERSE"] = 0x1502;
		enums["GL_COPY"] = 0x1503;
		enums["GL_AND_INVERTED"] = 0x1504;
		enums["GL_NOOP"] = 0x1505;
		enums["GL_XOR"] = 0x1506;
		enums["GL_OR"] = 0x1507;
		enums["GL_NOR"] = 0x1508;
		enums["GL_EQUIV"] = 0x1509;
		enums["GL_INVERT"] = 0x150A;
		enums["GL_OR_REVERSE"] = 0x150B;
		enums["GL_COPY_INVERTED"] = 0x150C;
		enums["GL_OR_INVERTED"] = 0x150D;
		enums["GL_NAND"] = 0x150E;
		enums["GL_SET"] = 0x150F;
		enums["GL_EMISSION"] = 0x1600;
		enums["GL_SHININESS"] = 0x1601;
		enums["GL_AMBIENT_AND_DIFFUSE"] = 0x1602;
		enums["GL_COLOR_INDEXES"] = 0x1603;
		enums["GL_MODELVIEW"] = 0x1700;
		enums["GL_PROJECTION"] = 0x1701;
		enums["GL_TEXTURE"] = 0x1702;
		enums["GL_COLOR"] = 0x1800;
		enums["GL_DEPTH"] = 0x1801;
		enums["GL_STENCIL"] = 0x1802;
		enums["GL_COLOR_INDEX"] = 0x1900;
		enums["GL_STENCIL_INDEX"] = 0x1901;
		enums["GL_DEPTH_COMPONENT"] = 0x1902;
		enums["GL_RED"] = 0x1903;
		enums["GL_GREEN"] = 0x1904;
		enums["GL_BLUE"] = 0x1905;
		enums["GL_ALPHA"] = 0x1906;
		enums["GL_RGB"] = 0x1907;
		enums["GL_RGBA"] = 0x1908;
		enums["GL_LUMINANCE"] = 0x1909;
		enums["GL_LUMINANCE_ALPHA"] = 0x190A;
		enums["GL_BITMAP"] = 0x1A00;
		enums["GL_POINT"] = 0x1B00;
		enums["GL_LINE"] = 0x1B01;
		enums["GL_FILL"] = 0x1B02;
		enums["GL_RENDER"] = 0x1C00;
		enums["GL_FEEDBACK"] = 0x1C01;
		enums["GL_SELECT"] = 0x1C02;
		enums["GL_FLAT"] = 0x1D00;
		enums["GL_SMOOTH"] = 0x1D01;
		enums["GL_KEEP"] = 0x1E00;
		enums["GL_REPLACE"] = 0x1E01;
		enums["GL_INCR"] = 0x1E02;
		enums["GL_DECR"] = 0x1E03;
		enums["GL_VENDOR"] = 0x1F00;
		enums["GL_RENDERER"] = 0x1F01;
		enums["GL_VERSION"] = 0x1F02;
		enums["GL_EXTENSIONS"] = 0x1F03;
		enums["GL_S"] = 0x2000;
		enums["GL_ENABLE_BIT"] = 0x00002000;
		enums["GL_T"] = 0x2001;
		enums["GL_R"] = 0x2002;
		enums["GL_Q"] = 0x2003;
		enums["GL_MODULATE"] = 0x2100;
		enums["GL_DECAL"] = 0x2101;
		enums["GL_TEXTURE_ENV_MODE"] = 0x2200;
		enums["GL_TEXTURE_ENV_COLOR"] = 0x2201;
		enums["GL_TEXTURE_ENV"] = 0x2300;
		enums["GL_EYE_LINEAR"] = 0x2400;
		enums["GL_OBJECT_LINEAR"] = 0x2401;
		enums["GL_SPHERE_MAP"] = 0x2402;
		enums["GL_TEXTURE_GEN_MODE"] = 0x2500;
		enums["GL_OBJECT_PLANE"] = 0x2501;
		enums["GL_EYE_PLANE"] = 0x2502;
		enums["GL_NEAREST"] = 0x2600;
		enums["GL_LINEAR"] = 0x2601;
		enums["GL_NEAREST_MIPMAP_NEAREST"] = 0x2700;
		enums["GL_LINEAR_MIPMAP_NEAREST"] = 0x2701;
		enums["GL_NEAREST_MIPMAP_LINEAR"] = 0x2702;
		enums["GL_LINEAR_MIPMAP_LINEAR"] = 0x2703;
		enums["GL_TEXTURE_MAG_FILTER"] = 0x2800;
		enums["GL_TEXTURE_MIN_FILTER"] = 0x2801;
		enums["GL_TEXTURE_WRAP_S"] = 0x2802;
		enums["GL_TEXTURE_WRAP_T"] = 0x2803;
		enums["GL_CLAMP"] = 0x2900;
		enums["GL_REPEAT"] = 0x2901;
		enums["GL_POLYGON_OFFSET_UNITS"] = 0x2A00;
		enums["GL_POLYGON_OFFSET_POINT"] = 0x2A01;
		enums["GL_POLYGON_OFFSET_LINE"] = 0x2A02;
		enums["GL_R3_G3_B2"] = 0x2A10;
		enums["GL_V2F"] = 0x2A20;
		enums["GL_V3F"] = 0x2A21;
		enums["GL_C4UB_V2F"] = 0x2A22;
		enums["GL_C4UB_V3F"] = 0x2A23;
		enums["GL_C3F_V3F"] = 0x2A24;
		enums["GL_N3F_V3F"] = 0x2A25;
		enums["GL_C4F_N3F_V3F"] = 0x2A26;
		enums["GL_T2F_V3F"] = 0x2A27;
		enums["GL_T4F_V4F"] = 0x2A28;
		enums["GL_T2F_C4UB_V3F"] = 0x2A29;
		enums["GL_T2F_C3F_V3F"] = 0x2A2A;
		enums["GL_T2F_N3F_V3F"] = 0x2A2B;
		enums["GL_T2F_C4F_N3F_V3F"] = 0x2A2C;
		enums["GL_T4F_C4F_N3F_V4F"] = 0x2A2D;
		enums["GL_CLIP_PLANE0"] = 0x3000;
		enums["GL_CLIP_PLANE1"] = 0x3001;
		enums["GL_CLIP_PLANE2"] = 0x3002;
		enums["GL_CLIP_PLANE3"] = 0x3003;
		enums["GL_CLIP_PLANE4"] = 0x3004;
		enums["GL_CLIP_PLANE5"] = 0x3005;
		enums["GL_LIGHT0"] = 0x4000;
		enums["GL_COLOR_BUFFER_BIT"] = 0x00004000;
		enums["GL_LIGHT1"] = 0x4001;
		enums["GL_LIGHT2"] = 0x4002;
		enums["GL_LIGHT3"] = 0x4003;
		enums["GL_LIGHT4"] = 0x4004;
		enums["GL_LIGHT5"] = 0x4005;
		enums["GL_LIGHT6"] = 0x4006;
		enums["GL_LIGHT7"] = 0x4007;
		enums["GL_HINT_BIT"] = 0x00008000;
		enums["GL_POLYGON_OFFSET_FILL"] = 0x8037;
		enums["GL_POLYGON_OFFSET_FACTOR"] = 0x8038;
		enums["GL_ALPHA4"] = 0x803B;
		enums["GL_ALPHA8"] = 0x803C;
		enums["GL_ALPHA12"] = 0x803D;
		enums["GL_ALPHA16"] = 0x803E;
		enums["GL_LUMINANCE4"] = 0x803F;
		enums["GL_LUMINANCE8"] = 0x8040;
		enums["GL_LUMINANCE12"] = 0x8041;
		enums["GL_LUMINANCE16"] = 0x8042;
		enums["GL_LUMINANCE4_ALPHA4"] = 0x8043;
		enums["GL_LUMINANCE6_ALPHA2"] = 0x8044;
		enums["GL_LUMINANCE8_ALPHA8"] = 0x8045;
		enums["GL_LUMINANCE12_ALPHA4"] = 0x8046;
		enums["GL_LUMINANCE12_ALPHA12"] = 0x8047;
		enums["GL_LUMINANCE16_ALPHA16"] = 0x8048;
		enums["GL_INTENSITY"] = 0x8049;
		enums["GL_INTENSITY4"] = 0x804A;
		enums["GL_INTENSITY8"] = 0x804B;
		enums["GL_INTENSITY12"] = 0x804C;
		enums["GL_INTENSITY16"] = 0x804D;
		enums["GL_RGB4"] = 0x804F;
		enums["GL_RGB5"] = 0x8050;
		enums["GL_RGB8"] = 0x8051;
		enums["GL_RGB10"] = 0x8052;
		enums["GL_RGB12"] = 0x8053;
		enums["GL_RGB16"] = 0x8054;
		enums["GL_RGBA2"] = 0x8055;
		enums["GL_RGBA4"] = 0x8056;
		enums["GL_RGB5_A1"] = 0x8057;
		enums["GL_RGBA8"] = 0x8058;
		enums["GL_RGB10_A2"] = 0x8059;
		enums["GL_RGBA12"] = 0x805A;
		enums["GL_RGBA16"] = 0x805B;
		enums["GL_TEXTURE_RED_SIZE"] = 0x805C;
		enums["GL_TEXTURE_GREEN_SIZE"] = 0x805D;
		enums["GL_TEXTURE_BLUE_SIZE"] = 0x805E;
		enums["GL_TEXTURE_ALPHA_SIZE"] = 0x805F;
		enums["GL_TEXTURE_LUMINANCE_SIZE"] = 0x8060;
		enums["GL_TEXTURE_INTENSITY_SIZE"] = 0x8061;
		enums["GL_PROXY_TEXTURE_1D"] = 0x8063;
		enums["GL_PROXY_TEXTURE_2D"] = 0x8064;
		enums["GL_TEXTURE_PRIORITY"] = 0x8066;
		enums["GL_TEXTURE_RESIDENT"] = 0x8067;
		enums["GL_TEXTURE_BINDING_1D"] = 0x8068;
		enums["GL_TEXTURE_BINDING_2D"] = 0x8069;
		enums["GL_VERTEX_ARRAY"] = 0x8074;
		enums["GL_NORMAL_ARRAY"] = 0x8075;
		enums["GL_COLOR_ARRAY"] = 0x8076;
		enums["GL_INDEX_ARRAY"] = 0x8077;
		enums["GL_TEXTURE_COORD_ARRAY"] = 0x8078;
		enums["GL_EDGE_FLAG_ARRAY"] = 0x8079;
		enums["GL_VERTEX_ARRAY_SIZE"] = 0x807A;
		enums["GL_VERTEX_ARRAY_TYPE"] = 0x807B;
		enums["GL_VERTEX_ARRAY_STRIDE"] = 0x807C;
		enums["GL_NORMAL_ARRAY_TYPE"] = 0x807E;
		enums["GL_NORMAL_ARRAY_STRIDE"] = 0x807F;
		enums["GL_COLOR_ARRAY_SIZE"] = 0x8081;
		enums["GL_COLOR_ARRAY_TYPE"] = 0x8082;
		enums["GL_COLOR_ARRAY_STRIDE"] = 0x8083;
		enums["GL_INDEX_ARRAY_TYPE"] = 0x8085;
		enums["GL_INDEX_ARRAY_STRIDE"] = 0x8086;
		enums["GL_TEXTURE_COORD_ARRAY_SIZE"] = 0x8088;
		enums["GL_TEXTURE_COORD_ARRAY_TYPE"] = 0x8089;
		enums["GL_TEXTURE_COORD_ARRAY_STRIDE"] = 0x808A;
		enums["GL_EDGE_FLAG_ARRAY_STRIDE"] = 0x808C;
		enums["GL_VERTEX_ARRAY_POINTER"] = 0x808E;
		enums["GL_NORMAL_ARRAY_POINTER"] = 0x808F;
		enums["GL_COLOR_ARRAY_POINTER"] = 0x8090;
		enums["GL_INDEX_ARRAY_POINTER"] = 0x8091;
		enums["GL_TEXTURE_COORD_ARRAY_POINTER"] = 0x8092;
		enums["GL_EDGE_FLAG_ARRAY_POINTER"] = 0x8093;
		enums["GL_COLOR_INDEX1_EXT"] = 0x80E2;
		enums["GL_COLOR_INDEX2_EXT"] = 0x80E3;
		enums["GL_COLOR_INDEX4_EXT"] = 0x80E4;
		enums["GL_COLOR_INDEX8_EXT"] = 0x80E5;
		enums["GL_COLOR_INDEX12_EXT"] = 0x80E6;
		enums["GL_COLOR_INDEX16_EXT"] = 0x80E7;
		enums["GL_EVAL_BIT"] = 0x00010000;
		enums["GL_LIST_BIT"] = 0x00020000;
		enums["GL_TEXTURE_BIT"] = 0x00040000;
		enums["GL_SCISSOR_BIT"] = 0x00080000;
		enums["GL_ALL_ATTRIB_BITS"] = 0x000fffff;
		enums["GL_CLIENT_ALL_ATTRIB_BITS"] = 0xffffffff;

		enums["GL_SMOOTH_POINT_SIZE_RANGE"] = 0x0B12;
		enums["GL_SMOOTH_POINT_SIZE_GRANULARITY"] = 0x0B13;
		enums["GL_SMOOTH_LINE_WIDTH_RANGE"] = 0x0B22;
		enums["GL_SMOOTH_LINE_WIDTH_GRANULARITY"] = 0x0B23;
		enums["GL_UNSIGNED_BYTE_3_3_2"] = 0x8032;
		enums["GL_UNSIGNED_SHORT_4_4_4_4"] = 0x8033;
		enums["GL_UNSIGNED_SHORT_5_5_5_1"] = 0x8034;
		enums["GL_UNSIGNED_INT_8_8_8_8"] = 0x8035;
		enums["GL_UNSIGNED_INT_10_10_10_2"] = 0x8036;
		enums["GL_RESCALE_NORMAL"] = 0x803A;
		enums["GL_TEXTURE_BINDING_3D"] = 0x806A;
		enums["GL_PACK_SKIP_IMAGES"] = 0x806B;
		enums["GL_PACK_IMAGE_HEIGHT"] = 0x806C;
		enums["GL_UNPACK_SKIP_IMAGES"] = 0x806D;
		enums["GL_UNPACK_IMAGE_HEIGHT"] = 0x806E;
		enums["GL_TEXTURE_3D"] = 0x806F;
		enums["GL_PROXY_TEXTURE_3D"] = 0x8070;
		enums["GL_TEXTURE_DEPTH"] = 0x8071;
		enums["GL_TEXTURE_WRAP_R"] = 0x8072;
		enums["GL_MAX_3D_TEXTURE_SIZE"] = 0x8073;
		enums["GL_BGR"] = 0x80E0;
		enums["GL_BGRA"] = 0x80E1;
		enums["GL_MAX_ELEMENTS_VERTICES"] = 0x80E8;
		enums["GL_MAX_ELEMENTS_INDICES"] = 0x80E9;
		enums["GL_CLAMP_TO_EDGE"] = 0x812F;
		enums["GL_TEXTURE_MIN_LOD"] = 0x813A;
		enums["GL_TEXTURE_MAX_LOD"] = 0x813B;
		enums["GL_TEXTURE_BASE_LEVEL"] = 0x813C;
		enums["GL_TEXTURE_MAX_LEVEL"] = 0x813D;
		enums["GL_LIGHT_MODEL_COLOR_CONTROL"] = 0x81F8;
		enums["GL_SINGLE_COLOR"] = 0x81F9;
		enums["GL_SEPARATE_SPECULAR_COLOR"] = 0x81FA;
		enums["GL_UNSIGNED_BYTE_2_3_3_REV"] = 0x8362;
		enums["GL_UNSIGNED_SHORT_5_6_5"] = 0x8363;
		enums["GL_UNSIGNED_SHORT_5_6_5_REV"] = 0x8364;
		enums["GL_UNSIGNED_SHORT_4_4_4_4_REV"] = 0x8365;
		enums["GL_UNSIGNED_SHORT_1_5_5_5_REV"] = 0x8366;
		enums["GL_UNSIGNED_INT_8_8_8_8_REV"] = 0x8367;
		enums["GL_ALIASED_POINT_SIZE_RANGE"] = 0x846D;
		enums["GL_ALIASED_LINE_WIDTH_RANGE"] = 0x846E;

		enums["GL_MULTISAMPLE"] = 0x809D;
		enums["GL_SAMPLE_ALPHA_TO_COVERAGE"] = 0x809E;
		enums["GL_SAMPLE_ALPHA_TO_ONE"] = 0x809F;
		enums["GL_SAMPLE_COVERAGE"] = 0x80A0;
		enums["GL_SAMPLE_BUFFERS"] = 0x80A8;
		enums["GL_SAMPLES"] = 0x80A9;
		enums["GL_SAMPLE_COVERAGE_VALUE"] = 0x80AA;
		enums["GL_SAMPLE_COVERAGE_INVERT"] = 0x80AB;
		enums["GL_CLAMP_TO_BORDER"] = 0x812D;
		enums["GL_TEXTURE0"] = 0x84C0;
		enums["GL_TEXTURE1"] = 0x84C1;
		enums["GL_TEXTURE2"] = 0x84C2;
		enums["GL_TEXTURE3"] = 0x84C3;
		enums["GL_TEXTURE4"] = 0x84C4;
		enums["GL_TEXTURE5"] = 0x84C5;
		enums["GL_TEXTURE6"] = 0x84C6;
		enums["GL_TEXTURE7"] = 0x84C7;
		enums["GL_TEXTURE8"] = 0x84C8;
		enums["GL_TEXTURE9"] = 0x84C9;
		enums["GL_TEXTURE10"] = 0x84CA;
		enums["GL_TEXTURE11"] = 0x84CB;
		enums["GL_TEXTURE12"] = 0x84CC;
		enums["GL_TEXTURE13"] = 0x84CD;
		enums["GL_TEXTURE14"] = 0x84CE;
		enums["GL_TEXTURE15"] = 0x84CF;
		enums["GL_TEXTURE16"] = 0x84D0;
		enums["GL_TEXTURE17"] = 0x84D1;
		enums["GL_TEXTURE18"] = 0x84D2;
		enums["GL_TEXTURE19"] = 0x84D3;
		enums["GL_TEXTURE20"] = 0x84D4;
		enums["GL_TEXTURE21"] = 0x84D5;
		enums["GL_TEXTURE22"] = 0x84D6;
		enums["GL_TEXTURE23"] = 0x84D7;
		enums["GL_TEXTURE24"] = 0x84D8;
		enums["GL_TEXTURE25"] = 0x84D9;
		enums["GL_TEXTURE26"] = 0x84DA;
		enums["GL_TEXTURE27"] = 0x84DB;
		enums["GL_TEXTURE28"] = 0x84DC;
		enums["GL_TEXTURE29"] = 0x84DD;
		enums["GL_TEXTURE30"] = 0x84DE;
		enums["GL_TEXTURE31"] = 0x84DF;
		enums["GL_ACTIVE_TEXTURE"] = 0x84E0;
		enums["GL_CLIENT_ACTIVE_TEXTURE"] = 0x84E1;
		enums["GL_MAX_TEXTURE_UNITS"] = 0x84E2;
		enums["GL_TRANSPOSE_MODELVIEW_MATRIX"] = 0x84E3;
		enums["GL_TRANSPOSE_PROJECTION_MATRIX"] = 0x84E4;
		enums["GL_TRANSPOSE_TEXTURE_MATRIX"] = 0x84E5;
		enums["GL_TRANSPOSE_COLOR_MATRIX"] = 0x84E6;
		enums["GL_SUBTRACT"] = 0x84E7;
		enums["GL_COMPRESSED_ALPHA"] = 0x84E9;
		enums["GL_COMPRESSED_LUMINANCE"] = 0x84EA;
		enums["GL_COMPRESSED_LUMINANCE_ALPHA"] = 0x84EB;
		enums["GL_COMPRESSED_INTENSITY"] = 0x84EC;
		enums["GL_COMPRESSED_RGB"] = 0x84ED;
		enums["GL_COMPRESSED_RGBA"] = 0x84EE;
		enums["GL_TEXTURE_COMPRESSION_HINT"] = 0x84EF;
		enums["GL_NORMAL_MAP"] = 0x8511;
		enums["GL_REFLECTION_MAP"] = 0x8512;
		enums["GL_TEXTURE_CUBE_MAP"] = 0x8513;
		enums["GL_TEXTURE_BINDING_CUBE_MAP"] = 0x8514;
		enums["GL_TEXTURE_CUBE_MAP_POSITIVE_X"] = 0x8515;
		enums["GL_TEXTURE_CUBE_MAP_NEGATIVE_X"] = 0x8516;
		enums["GL_TEXTURE_CUBE_MAP_POSITIVE_Y"] = 0x8517;
		enums["GL_TEXTURE_CUBE_MAP_NEGATIVE_Y"] = 0x8518;
		enums["GL_TEXTURE_CUBE_MAP_POSITIVE_Z"] = 0x8519;
		enums["GL_TEXTURE_CUBE_MAP_NEGATIVE_Z"] = 0x851A;
		enums["GL_PROXY_TEXTURE_CUBE_MAP"] = 0x851B;
		enums["GL_MAX_CUBE_MAP_TEXTURE_SIZE"] = 0x851C;
		enums["GL_COMBINE"] = 0x8570;
		enums["GL_COMBINE_RGB"] = 0x8571;
		enums["GL_COMBINE_ALPHA"] = 0x8572;
		enums["GL_RGB_SCALE"] = 0x8573;
		enums["GL_ADD_SIGNED"] = 0x8574;
		enums["GL_INTERPOLATE"] = 0x8575;
		enums["GL_CONSTANT"] = 0x8576;
		enums["GL_PRIMARY_COLOR"] = 0x8577;
		enums["GL_PREVIOUS"] = 0x8578;
		enums["GL_SOURCE0_RGB"] = 0x8580;
		enums["GL_SOURCE1_RGB"] = 0x8581;
		enums["GL_SOURCE2_RGB"] = 0x8582;
		enums["GL_SOURCE0_ALPHA"] = 0x8588;
		enums["GL_SOURCE1_ALPHA"] = 0x8589;
		enums["GL_SOURCE2_ALPHA"] = 0x858A;
		enums["GL_OPERAND0_RGB"] = 0x8590;
		enums["GL_OPERAND1_RGB"] = 0x8591;
		enums["GL_OPERAND2_RGB"] = 0x8592;
		enums["GL_OPERAND0_ALPHA"] = 0x8598;
		enums["GL_OPERAND1_ALPHA"] = 0x8599;
		enums["GL_OPERAND2_ALPHA"] = 0x859A;
		enums["GL_TEXTURE_COMPRESSED_IMAGE_SIZE"] = 0x86A0;
		enums["GL_TEXTURE_COMPRESSED"] = 0x86A1;
		enums["GL_NUM_COMPRESSED_TEXTURE_FORMATS"] = 0x86A2;
		enums["GL_COMPRESSED_TEXTURE_FORMATS"] = 0x86A3;
		enums["GL_DOT3_RGB"] = 0x86AE;
		enums["GL_DOT3_RGBA"] = 0x86AF;
		enums["GL_MULTISAMPLE_BIT"] = 0x20000000;

		enums["GL_BLEND_DST_RGB"] = 0x80C8;
		enums["GL_BLEND_SRC_RGB"] = 0x80C9;
		enums["GL_BLEND_DST_ALPHA"] = 0x80CA;
		enums["GL_BLEND_SRC_ALPHA"] = 0x80CB;
		enums["GL_POINT_SIZE_MIN"] = 0x8126;
		enums["GL_POINT_SIZE_MAX"] = 0x8127;
		enums["GL_POINT_FADE_THRESHOLD_SIZE"] = 0x8128;
		enums["GL_POINT_DISTANCE_ATTENUATION"] = 0x8129;
		enums["GL_GENERATE_MIPMAP"] = 0x8191;
		enums["GL_GENERATE_MIPMAP_HINT"] = 0x8192;
		enums["GL_DEPTH_COMPONENT16"] = 0x81A5;
		enums["GL_DEPTH_COMPONENT24"] = 0x81A6;
		enums["GL_DEPTH_COMPONENT32"] = 0x81A7;
		enums["GL_DEPTH_COMPONENT32F"] = GL_DEPTH_COMPONENT32F;
		enums["GL_MIRRORED_REPEAT"] = 0x8370;
		enums["GL_FOG_COORDINATE_SOURCE"] = 0x8450;
		enums["GL_FOG_COORDINATE"] = 0x8451;
		enums["GL_FRAGMENT_DEPTH"] = 0x8452;
		enums["GL_CURRENT_FOG_COORDINATE"] = 0x8453;
		enums["GL_FOG_COORDINATE_ARRAY_TYPE"] = 0x8454;
		enums["GL_FOG_COORDINATE_ARRAY_STRIDE"] = 0x8455;
		enums["GL_FOG_COORDINATE_ARRAY_POINTER"] = 0x8456;
		enums["GL_FOG_COORDINATE_ARRAY"] = 0x8457;
		enums["GL_COLOR_SUM"] = 0x8458;
		enums["GL_CURRENT_SECONDARY_COLOR"] = 0x8459;
		enums["GL_SECONDARY_COLOR_ARRAY_SIZE"] = 0x845A;
		enums["GL_SECONDARY_COLOR_ARRAY_TYPE"] = 0x845B;
		enums["GL_SECONDARY_COLOR_ARRAY_STRIDE"] = 0x845C;
		enums["GL_SECONDARY_COLOR_ARRAY_POINTER"] = 0x845D;
		enums["GL_SECONDARY_COLOR_ARRAY"] = 0x845E;
		enums["GL_MAX_TEXTURE_LOD_BIAS"] = 0x84FD;
		enums["GL_TEXTURE_FILTER_CONTROL"] = 0x8500;
		enums["GL_TEXTURE_LOD_BIAS"] = 0x8501;
		enums["GL_INCR_WRAP"] = 0x8507;
		enums["GL_DECR_WRAP"] = 0x8508;
		enums["GL_TEXTURE_DEPTH_SIZE"] = 0x884A;
		enums["GL_DEPTH_TEXTURE_MODE"] = 0x884B;
		enums["GL_TEXTURE_COMPARE_MODE"] = 0x884C;
		enums["GL_TEXTURE_COMPARE_FUNC"] = 0x884D;
		enums["GL_COMPARE_R_TO_TEXTURE"] = 0x884E;

		enums["GL_CURRENT_FOG_COORD"] = GL_CURRENT_FOG_COORDINATE;
		enums["GL_FOG_COORD"] = GL_FOG_COORDINATE;
		enums["GL_FOG_COORD_ARRAY"] = GL_FOG_COORDINATE_ARRAY;
		enums["GL_FOG_COORD_ARRAY_BUFFER_BINDING"] = GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING;
		enums["GL_FOG_COORD_ARRAY_POINTER"] = GL_FOG_COORDINATE_ARRAY_POINTER;
		enums["GL_FOG_COORD_ARRAY_STRIDE"] = GL_FOG_COORDINATE_ARRAY_STRIDE;
		enums["GL_FOG_COORD_ARRAY_TYPE"] = GL_FOG_COORDINATE_ARRAY_TYPE;
		enums["GL_FOG_COORD_SRC"] = GL_FOG_COORDINATE_SOURCE;
		enums["GL_SRC0_ALPHA"] = GL_SOURCE0_ALPHA;
		enums["GL_SRC0_RGB"] = GL_SOURCE0_RGB;
		enums["GL_SRC1_ALPHA"] = GL_SOURCE1_ALPHA;
		enums["GL_SRC1_RGB"] = GL_SOURCE1_RGB;
		enums["GL_SRC2_ALPHA"] = GL_SOURCE2_ALPHA;
		enums["GL_SRC2_RGB"] = GL_SOURCE2_RGB;
		enums["GL_BUFFER_SIZE"] = 0x8764;
		enums["GL_BUFFER_USAGE"] = 0x8765;
		enums["GL_QUERY_COUNTER_BITS"] = 0x8864;
		enums["GL_CURRENT_QUERY"] = 0x8865;
		enums["GL_QUERY_RESULT"] = 0x8866;
		enums["GL_QUERY_RESULT_AVAILABLE"] = 0x8867;
		enums["GL_ARRAY_BUFFER"] = 0x8892;
		enums["GL_ELEMENT_ARRAY_BUFFER"] = 0x8893;
		enums["GL_ARRAY_BUFFER_BINDING"] = 0x8894;
		enums["GL_ELEMENT_ARRAY_BUFFER_BINDING"] = 0x8895;
		enums["GL_VERTEX_ARRAY_BUFFER_BINDING"] = 0x8896;
		enums["GL_NORMAL_ARRAY_BUFFER_BINDING"] = 0x8897;
		enums["GL_COLOR_ARRAY_BUFFER_BINDING"] = 0x8898;
		enums["GL_INDEX_ARRAY_BUFFER_BINDING"] = 0x8899;
		enums["GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING"] = 0x889A;
		enums["GL_EDGE_FLAG_ARRAY_BUFFER_BINDING"] = 0x889B;
		enums["GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING"] = 0x889C;
		enums["GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING"] = 0x889D;
		enums["GL_WEIGHT_ARRAY_BUFFER_BINDING"] = 0x889E;
		enums["GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING"] = 0x889F;
		enums["GL_READ_ONLY"] = 0x88B8;
		enums["GL_WRITE_ONLY"] = 0x88B9;
		enums["GL_READ_WRITE"] = 0x88BA;
		enums["GL_BUFFER_ACCESS"] = 0x88BB;
		enums["GL_BUFFER_MAPPED"] = 0x88BC;
		enums["GL_BUFFER_MAP_POINTER"] = 0x88BD;
		enums["GL_STREAM_DRAW"] = 0x88E0;
		enums["GL_STREAM_READ"] = 0x88E1;
		enums["GL_STREAM_COPY"] = 0x88E2;
		enums["GL_STATIC_DRAW"] = 0x88E4;
		enums["GL_STATIC_READ"] = 0x88E5;
		enums["GL_STATIC_COPY"] = 0x88E6;
		enums["GL_DYNAMIC_DRAW"] = 0x88E8;
		enums["GL_DYNAMIC_READ"] = 0x88E9;
		enums["GL_DYNAMIC_COPY"] = 0x88EA;
		enums["GL_SAMPLES_PASSED"] = 0x8914;

		enums["GL_BLEND_EQUATION_RGB"] = GL_BLEND_EQUATION;
		enums["GL_VERTEX_ATTRIB_ARRAY_ENABLED"] = 0x8622;
		enums["GL_VERTEX_ATTRIB_ARRAY_SIZE"] = 0x8623;
		enums["GL_VERTEX_ATTRIB_ARRAY_STRIDE"] = 0x8624;
		enums["GL_VERTEX_ATTRIB_ARRAY_TYPE"] = 0x8625;
		enums["GL_CURRENT_VERTEX_ATTRIB"] = 0x8626;
		enums["GL_VERTEX_PROGRAM_POINT_SIZE"] = 0x8642;
		enums["GL_VERTEX_PROGRAM_TWO_SIDE"] = 0x8643;
		enums["GL_VERTEX_ATTRIB_ARRAY_POINTER"] = 0x8645;
		enums["GL_STENCIL_BACK_FUNC"] = 0x8800;
		enums["GL_STENCIL_BACK_FAIL"] = 0x8801;
		enums["GL_STENCIL_BACK_PASS_DEPTH_FAIL"] = 0x8802;
		enums["GL_STENCIL_BACK_PASS_DEPTH_PASS"] = 0x8803;
		enums["GL_MAX_DRAW_BUFFERS"] = 0x8824;
		enums["GL_DRAW_BUFFER0"] = 0x8825;
		enums["GL_DRAW_BUFFER1"] = 0x8826;
		enums["GL_DRAW_BUFFER2"] = 0x8827;
		enums["GL_DRAW_BUFFER3"] = 0x8828;
		enums["GL_DRAW_BUFFER4"] = 0x8829;
		enums["GL_DRAW_BUFFER5"] = 0x882A;
		enums["GL_DRAW_BUFFER6"] = 0x882B;
		enums["GL_DRAW_BUFFER7"] = 0x882C;
		enums["GL_DRAW_BUFFER8"] = 0x882D;
		enums["GL_DRAW_BUFFER9"] = 0x882E;
		enums["GL_DRAW_BUFFER10"] = 0x882F;
		enums["GL_DRAW_BUFFER11"] = 0x8830;
		enums["GL_DRAW_BUFFER12"] = 0x8831;
		enums["GL_DRAW_BUFFER13"] = 0x8832;
		enums["GL_DRAW_BUFFER14"] = 0x8833;
		enums["GL_DRAW_BUFFER15"] = 0x8834;
		enums["GL_BLEND_EQUATION_ALPHA"] = 0x883D;
		enums["GL_POINT_SPRITE"] = 0x8861;
		enums["GL_COORD_REPLACE"] = 0x8862;
		enums["GL_MAX_VERTEX_ATTRIBS"] = 0x8869;
		enums["GL_VERTEX_ATTRIB_ARRAY_NORMALIZED"] = 0x886A;
		enums["GL_MAX_TEXTURE_COORDS"] = 0x8871;
		enums["GL_MAX_TEXTURE_IMAGE_UNITS"] = 0x8872;
		enums["GL_FRAGMENT_SHADER"] = 0x8B30;
		enums["GL_VERTEX_SHADER"] = 0x8B31;
		enums["GL_MAX_FRAGMENT_UNIFORM_COMPONENTS"] = 0x8B49;
		enums["GL_MAX_VERTEX_UNIFORM_COMPONENTS"] = 0x8B4A;
		enums["GL_MAX_VARYING_FLOATS"] = 0x8B4B;
		enums["GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS"] = 0x8B4C;
		enums["GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS"] = 0x8B4D;
		enums["GL_SHADER_TYPE"] = 0x8B4F;
		enums["GL_FLOAT_VEC2"] = 0x8B50;
		enums["GL_FLOAT_VEC3"] = 0x8B51;
		enums["GL_FLOAT_VEC4"] = 0x8B52;
		enums["GL_INT_VEC2"] = 0x8B53;
		enums["GL_INT_VEC3"] = 0x8B54;
		enums["GL_INT_VEC4"] = 0x8B55;
		enums["GL_BOOL"] = 0x8B56;
		enums["GL_BOOL_VEC2"] = 0x8B57;
		enums["GL_BOOL_VEC3"] = 0x8B58;
		enums["GL_BOOL_VEC4"] = 0x8B59;
		enums["GL_FLOAT_MAT2"] = 0x8B5A;
		enums["GL_FLOAT_MAT3"] = 0x8B5B;
		enums["GL_FLOAT_MAT4"] = 0x8B5C;
		enums["GL_SAMPLER_1D"] = 0x8B5D;
		enums["GL_SAMPLER_2D"] = 0x8B5E;
		enums["GL_SAMPLER_3D"] = 0x8B5F;
		enums["GL_SAMPLER_CUBE"] = 0x8B60;
		enums["GL_SAMPLER_1D_SHADOW"] = 0x8B61;
		enums["GL_SAMPLER_2D_SHADOW"] = 0x8B62;
		enums["GL_DELETE_STATUS"] = 0x8B80;
		enums["GL_COMPILE_STATUS"] = 0x8B81;
		enums["GL_LINK_STATUS"] = 0x8B82;
		enums["GL_VALIDATE_STATUS"] = 0x8B83;
		enums["GL_INFO_LOG_LENGTH"] = 0x8B84;
		enums["GL_ATTACHED_SHADERS"] = 0x8B85;
		enums["GL_ACTIVE_UNIFORMS"] = 0x8B86;
		enums["GL_ACTIVE_UNIFORM_MAX_LENGTH"] = 0x8B87;
		enums["GL_SHADER_SOURCE_LENGTH"] = 0x8B88;
		enums["GL_ACTIVE_ATTRIBUTES"] = 0x8B89;
		enums["GL_ACTIVE_ATTRIBUTE_MAX_LENGTH"] = 0x8B8A;
		enums["GL_FRAGMENT_SHADER_DERIVATIVE_HINT"] = 0x8B8B;
		enums["GL_SHADING_LANGUAGE_VERSION"] = 0x8B8C;
		enums["GL_CURRENT_PROGRAM"] = 0x8B8D;
		enums["GL_POINT_SPRITE_COORD_ORIGIN"] = 0x8CA0;
		enums["GL_LOWER_LEFT"] = 0x8CA1;
		enums["GL_UPPER_LEFT"] = 0x8CA2;
		enums["GL_STENCIL_BACK_REF"] = 0x8CA3;
		enums["GL_STENCIL_BACK_VALUE_MASK"] = 0x8CA4;
		enums["GL_STENCIL_BACK_WRITEMASK"] = 0x8CA5;

		enums["GL_CURRENT_RASTER_SECONDARY_COLOR"] = 0x845F;
		enums["GL_PIXEL_PACK_BUFFER"] = 0x88EB;
		enums["GL_PIXEL_UNPACK_BUFFER"] = 0x88EC;
		enums["GL_PIXEL_PACK_BUFFER_BINDING"] = 0x88ED;
		enums["GL_PIXEL_UNPACK_BUFFER_BINDING"] = 0x88EF;
		enums["GL_FLOAT_MAT2x3"] = 0x8B65;
		enums["GL_FLOAT_MAT2x4"] = 0x8B66;
		enums["GL_FLOAT_MAT3x2"] = 0x8B67;
		enums["GL_FLOAT_MAT3x4"] = 0x8B68;
		enums["GL_FLOAT_MAT4x2"] = 0x8B69;
		enums["GL_FLOAT_MAT4x3"] = 0x8B6A;
		enums["GL_SRGB"] = 0x8C40;
		enums["GL_SRGB8"] = 0x8C41;
		enums["GL_SRGB_ALPHA"] = 0x8C42;
		enums["GL_SRGB8_ALPHA8"] = 0x8C43;
		enums["GL_SLUMINANCE_ALPHA"] = 0x8C44;
		enums["GL_SLUMINANCE8_ALPHA8"] = 0x8C45;
		enums["GL_SLUMINANCE"] = 0x8C46;
		enums["GL_SLUMINANCE8"] = 0x8C47;
		enums["GL_COMPRESSED_SRGB"] = 0x8C48;
		enums["GL_COMPRESSED_SRGB_ALPHA"] = 0x8C49;
		enums["GL_COMPRESSED_SLUMINANCE"] = 0x8C4A;
		enums["GL_COMPRESSED_SLUMINANCE_ALPHA"] = 0x8C4B;

		enums["GL_CLIP_DISTANCE0"] = GL_CLIP_PLANE0;
		enums["GL_CLIP_DISTANCE1"] = GL_CLIP_PLANE1;
		enums["GL_CLIP_DISTANCE2"] = GL_CLIP_PLANE2;
		enums["GL_CLIP_DISTANCE3"] = GL_CLIP_PLANE3;
		enums["GL_CLIP_DISTANCE4"] = GL_CLIP_PLANE4;
		enums["GL_CLIP_DISTANCE5"] = GL_CLIP_PLANE5;
		enums["GL_COMPARE_REF_TO_TEXTURE"] = GL_COMPARE_R_TO_TEXTURE_ARB;
		enums["GL_MAX_CLIP_DISTANCES"] = GL_MAX_CLIP_PLANES;
		enums["GL_MAX_VARYING_COMPONENTS"] = GL_MAX_VARYING_FLOATS;
		enums["GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT"] = 0x0001;
		enums["GL_MAJOR_VERSION"] = 0x821B;
		enums["GL_MINOR_VERSION"] = 0x821C;
		enums["GL_NUM_EXTENSIONS"] = 0x821D;
		enums["GL_CONTEXT_FLAGS"] = 0x821E;
		enums["GL_DEPTH_BUFFER"] = 0x8223;
		enums["GL_STENCIL_BUFFER"] = 0x8224;
		enums["GL_RGBA32F"] = 0x8814;
		enums["GL_RGB32F"] = 0x8815;
		enums["GL_RGBA16F"] = 0x881A;
		enums["GL_RGB16F"] = 0x881B;
		enums["GL_VERTEX_ATTRIB_ARRAY_INTEGER"] = 0x88FD;
		enums["GL_MAX_ARRAY_TEXTURE_LAYERS"] = 0x88FF;
		enums["GL_MIN_PROGRAM_TEXEL_OFFSET"] = 0x8904;
		enums["GL_MAX_PROGRAM_TEXEL_OFFSET"] = 0x8905;
		enums["GL_CLAMP_VERTEX_COLOR"] = 0x891A;
		enums["GL_CLAMP_FRAGMENT_COLOR"] = 0x891B;
		enums["GL_CLAMP_READ_COLOR"] = 0x891C;
		enums["GL_FIXED_ONLY"] = 0x891D;
		enums["GL_TEXTURE_RED_TYPE"] = 0x8C10;
		enums["GL_TEXTURE_GREEN_TYPE"] = 0x8C11;
		enums["GL_TEXTURE_BLUE_TYPE"] = 0x8C12;
		enums["GL_TEXTURE_ALPHA_TYPE"] = 0x8C13;
		enums["GL_TEXTURE_LUMINANCE_TYPE"] = 0x8C14;
		enums["GL_TEXTURE_INTENSITY_TYPE"] = 0x8C15;
		enums["GL_TEXTURE_DEPTH_TYPE"] = 0x8C16;
		enums["GL_TEXTURE_1D_ARRAY"] = 0x8C18;
		enums["GL_PROXY_TEXTURE_1D_ARRAY"] = 0x8C19;
		enums["GL_TEXTURE_2D_ARRAY"] = 0x8C1A;
		enums["GL_PROXY_TEXTURE_2D_ARRAY"] = 0x8C1B;
		enums["GL_TEXTURE_BINDING_1D_ARRAY"] = 0x8C1C;
		enums["GL_TEXTURE_BINDING_2D_ARRAY"] = 0x8C1D;
		enums["GL_R11F_G11F_B10F"] = 0x8C3A;
		enums["GL_UNSIGNED_INT_10F_11F_11F_REV"] = 0x8C3B;
		enums["GL_RGB9_E5"] = 0x8C3D;
		enums["GL_UNSIGNED_INT_5_9_9_9_REV"] = 0x8C3E;
		enums["GL_TEXTURE_SHARED_SIZE"] = 0x8C3F;
		enums["GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH"] = 0x8C76;
		enums["GL_TRANSFORM_FEEDBACK_BUFFER_MODE"] = 0x8C7F;
		enums["GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS"] = 0x8C80;
		enums["GL_TRANSFORM_FEEDBACK_VARYINGS"] = 0x8C83;
		enums["GL_TRANSFORM_FEEDBACK_BUFFER_START"] = 0x8C84;
		enums["GL_TRANSFORM_FEEDBACK_BUFFER_SIZE"] = 0x8C85;
		enums["GL_PRIMITIVES_GENERATED"] = 0x8C87;
		enums["GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN"] = 0x8C88;
		enums["GL_RASTERIZER_DISCARD"] = 0x8C89;
		enums["GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS"] = 0x8C8A;
		enums["GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS"] = 0x8C8B;
		enums["GL_INTERLEAVED_ATTRIBS"] = 0x8C8C;
		enums["GL_SEPARATE_ATTRIBS"] = 0x8C8D;
		enums["GL_TRANSFORM_FEEDBACK_BUFFER"] = 0x8C8E;
		enums["GL_TRANSFORM_FEEDBACK_BUFFER_BINDING"] = 0x8C8F;
		enums["GL_RGBA32UI"] = 0x8D70;
		enums["GL_RGB32UI"] = 0x8D71;
		enums["GL_RGBA16UI"] = 0x8D76;
		enums["GL_RGB16UI"] = 0x8D77;
		enums["GL_RGBA8UI"] = 0x8D7C;
		enums["GL_RGB8UI"] = 0x8D7D;
		enums["GL_RGBA32I"] = 0x8D82;
		enums["GL_RGB32I"] = 0x8D83;
		enums["GL_RGBA16I"] = 0x8D88;
		enums["GL_RGB16I"] = 0x8D89;
		enums["GL_RGBA8I"] = 0x8D8E;
		enums["GL_RGB8I"] = 0x8D8F;
		enums["GL_RED_INTEGER"] = 0x8D94;
		enums["GL_GREEN_INTEGER"] = 0x8D95;
		enums["GL_BLUE_INTEGER"] = 0x8D96;
		enums["GL_ALPHA_INTEGER"] = 0x8D97;
		enums["GL_RGB_INTEGER"] = 0x8D98;
		enums["GL_RGBA_INTEGER"] = 0x8D99;
		enums["GL_BGR_INTEGER"] = 0x8D9A;
		enums["GL_BGRA_INTEGER"] = 0x8D9B;
		enums["GL_SAMPLER_1D_ARRAY"] = 0x8DC0;
		enums["GL_SAMPLER_2D_ARRAY"] = 0x8DC1;
		enums["GL_SAMPLER_1D_ARRAY_SHADOW"] = 0x8DC3;
		enums["GL_SAMPLER_2D_ARRAY_SHADOW"] = 0x8DC4;
		enums["GL_SAMPLER_CUBE_SHADOW"] = 0x8DC5;
		enums["GL_UNSIGNED_INT_VEC2"] = 0x8DC6;
		enums["GL_UNSIGNED_INT_VEC3"] = 0x8DC7;
		enums["GL_UNSIGNED_INT_VEC4"] = 0x8DC8;
		enums["GL_INT_SAMPLER_1D"] = 0x8DC9;
		enums["GL_INT_SAMPLER_2D"] = 0x8DCA;
		enums["GL_INT_SAMPLER_3D"] = 0x8DCB;
		enums["GL_INT_SAMPLER_CUBE"] = 0x8DCC;
		enums["GL_INT_SAMPLER_1D_ARRAY"] = 0x8DCE;
		enums["GL_INT_SAMPLER_2D_ARRAY"] = 0x8DCF;
		enums["GL_UNSIGNED_INT_SAMPLER_1D"] = 0x8DD1;
		enums["GL_UNSIGNED_INT_SAMPLER_2D"] = 0x8DD2;
		enums["GL_UNSIGNED_INT_SAMPLER_3D"] = 0x8DD3;
		enums["GL_UNSIGNED_INT_SAMPLER_CUBE"] = 0x8DD4;
		enums["GL_UNSIGNED_INT_SAMPLER_1D_ARRAY"] = 0x8DD6;
		enums["GL_UNSIGNED_INT_SAMPLER_2D_ARRAY"] = 0x8DD7;
		enums["GL_QUERY_WAIT"] = 0x8E13;
		enums["GL_QUERY_NO_WAIT"] = 0x8E14;
		enums["GL_QUERY_BY_REGION_WAIT"] = 0x8E15;
		enums["GL_QUERY_BY_REGION_NO_WAIT"] = 0x8E16;

		enums["GL_TEXTURE_RECTANGLE"] = 0x84F5;
		enums["GL_TEXTURE_BINDING_RECTANGLE"] = 0x84F6;
		enums["GL_PROXY_TEXTURE_RECTANGLE"] = 0x84F7;
		enums["GL_MAX_RECTANGLE_TEXTURE_SIZE"] = 0x84F8;
		enums["GL_SAMPLER_2D_RECT"] = 0x8B63;
		enums["GL_SAMPLER_2D_RECT_SHADOW"] = 0x8B64;
		enums["GL_TEXTURE_BUFFER"] = 0x8C2A;
		enums["GL_MAX_TEXTURE_BUFFER_SIZE"] = 0x8C2B;
		enums["GL_TEXTURE_BINDING_BUFFER"] = 0x8C2C;
		enums["GL_TEXTURE_BUFFER_DATA_STORE_BINDING"] = 0x8C2D;
		enums["GL_TEXTURE_BUFFER_FORMAT"] = 0x8C2E;
		enums["GL_SAMPLER_BUFFER"] = 0x8DC2;
		enums["GL_INT_SAMPLER_2D_RECT"] = 0x8DCD;
		enums["GL_INT_SAMPLER_BUFFER"] = 0x8DD0;
		enums["GL_UNSIGNED_INT_SAMPLER_2D_RECT"] = 0x8DD5;
		enums["GL_UNSIGNED_INT_SAMPLER_BUFFER"] = 0x8DD8;
		enums["GL_RED_SNORM"] = 0x8F90;
		enums["GL_RG_SNORM"] = 0x8F91;
		enums["GL_RGB_SNORM"] = 0x8F92;
		enums["GL_RGBA_SNORM"] = 0x8F93;
		enums["GL_R8_SNORM"] = 0x8F94;
		enums["GL_RG8_SNORM"] = 0x8F95;
		enums["GL_RGB8_SNORM"] = 0x8F96;
		enums["GL_RGBA8_SNORM"] = 0x8F97;
		enums["GL_R16_SNORM"] = 0x8F98;
		enums["GL_RG16_SNORM"] = 0x8F99;
		enums["GL_RGB16_SNORM"] = 0x8F9A;
		enums["GL_RGBA16_SNORM"] = 0x8F9B;
		enums["GL_SIGNED_NORMALIZED"] = 0x8F9C;
		enums["GL_PRIMITIVE_RESTART"] = 0x8F9D;
		enums["GL_PRIMITIVE_RESTART_INDEX"] = 0x8F9E;
		enums["GL_BUFFER_ACCESS_FLAGS"] = 0x911F;
		enums["GL_BUFFER_MAP_LENGTH"] = 0x9120;
		enums["GL_BUFFER_MAP_OFFSET"] = 0x9121;

		enums["GL_CONTEXT_CORE_PROFILE_BIT"] = 0x00000001;
		enums["GL_CONTEXT_COMPATIBILITY_PROFILE_BIT"] = 0x00000002;
		enums["GL_LINES_ADJACENCY"] = 0x000A;
		enums["GL_LINE_STRIP_ADJACENCY"] = 0x000B;
		enums["GL_TRIANGLES_ADJACENCY"] = 0x000C;
		enums["GL_TRIANGLE_STRIP_ADJACENCY"] = 0x000D;
		enums["GL_PROGRAM_POINT_SIZE"] = 0x8642;
		enums["GL_GEOMETRY_VERTICES_OUT"] = 0x8916;
		enums["GL_GEOMETRY_INPUT_TYPE"] = 0x8917;
		enums["GL_GEOMETRY_OUTPUT_TYPE"] = 0x8918;
		enums["GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS"] = 0x8C29;
		enums["GL_FRAMEBUFFER"] = GL_FRAMEBUFFER;
		enums["GL_RENDERBUFFER"] = GL_RENDERBUFFER;
		enums["GL_FRAMEBUFFER_ATTACHMENT_LAYERED"] = 0x8DA7;
		enums["GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"] = 0x8DA8;
		enums["GL_GEOMETRY_SHADER"] = 0x8DD9;
		enums["GL_MAX_GEOMETRY_UNIFORM_COMPONENTS"] = 0x8DDF;
		enums["GL_MAX_GEOMETRY_OUTPUT_VERTICES"] = 0x8DE0;
		enums["GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS"] = 0x8DE1;
		enums["GL_MAX_VERTEX_OUTPUT_COMPONENTS"] = 0x9122;
		enums["GL_MAX_GEOMETRY_INPUT_COMPONENTS"] = 0x9123;
		enums["GL_MAX_GEOMETRY_OUTPUT_COMPONENTS"] = 0x9124;
		enums["GL_MAX_FRAGMENT_INPUT_COMPONENTS"] = 0x9125;
		enums["GL_CONTEXT_PROFILE_MASK"] = 0x9126;

		enums["GL_VERTEX_ATTRIB_ARRAY_DIVISOR"] = 0x88FE;
		enums["GL_RGB10_A2UI"] = 0x906F;

		enums["GL_SAMPLE_SHADING"] = 0x8C36;
		enums["GL_MIN_SAMPLE_SHADING_VALUE"] = 0x8C37;
		enums["GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET"] = 0x8E5E;
		enums["GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET"] = 0x8E5F;
		enums["GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS"] = 0x8F9F;
		enums["GL_TEXTURE_CUBE_MAP_ARRAY"] = 0x9009;
		enums["GL_TEXTURE_BINDING_CUBE_MAP_ARRAY"] = 0x900A;
		enums["GL_PROXY_TEXTURE_CUBE_MAP_ARRAY"] = 0x900B;
		enums["GL_SAMPLER_CUBE_MAP_ARRAY"] = 0x900C;
		enums["GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW"] = 0x900D;
		enums["GL_INT_SAMPLER_CUBE_MAP_ARRAY"] = 0x900E;
		enums["GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY"] = 0x900F;

		enums["GL_COMPRESSED_RGBA_BPTC_UNORM"] = 0x8E8C;
		enums["GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM"] = 0x8E8D;
		enums["GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT"] = 0x8E8E;
		enums["GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT"] = 0x8E8F;
		enums["GL_COPY_READ_BUFFER_BINDING"] = 0x8F36;
		enums["GL_COPY_WRITE_BUFFER_BINDING"] = 0x8F37;

		enums["GL_NUM_SHADING_LANGUAGE_VERSIONS"] = 0x82E9;
		enums["GL_VERTEX_ATTRIB_ARRAY_LONG"] = 0x874E;

		enums["GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED"] = 0x8221;
		enums["GL_MAX_VERTEX_ATTRIB_STRIDE"] = 0x82E5;
		enums["GL_TEXTURE_BUFFER_BINDING"] = 0x8C2A;

		for (auto& e : enums) {
			enum_strings[e.second] = e.first;
		}
	}

	int OpenGLNameTranslator::getEnum(const std::string& name) const noexcept {
		std::map<std::string, int>::const_iterator it = enums.find(name);
		if (it == enums.end())
			it = enums.find(std::string("GL_") + name);
		if (it == enums.end())
			return 0;
		return it->second;
	}

	const char* OpenGLNameTranslator::getString(int id) const noexcept {
		std::map<int, std::string>::const_iterator it = enum_strings.find(id);
		if (it != enum_strings.end())
			return it->second.c_str();
		return empty_string.c_str();
	}

	//////////////////////////////////////////////////////////////////////
	// Q u i c k G L E r r o r C h e c k e r /////////////////////////////
	//////////////////////////////////////////////////////////////////////

	QuickGLErrorChecker::QuickGLErrorChecker() {
		e = glGetError();
	}

	bool QuickGLErrorChecker::isError() {
		e = glGetError();
		if (e != GL_NO_ERROR) {
			Hf::Log.error("%s(): OpenGL Error %s", __FUNCTION__, glewGetErrorString(e));
			return true;
		}
		return false;
	}

	void QuickGLErrorChecker::reset() {
		e = glGetError();
	}

	//////////////////////////////////////////////////////////////////////
	// F l u x i o n s G L D e b u g F u n c /////////////////////////////
	//////////////////////////////////////////////////////////////////////

	void EnableGLDebugFunc() {
		if (GLEW_ARB_debug_output) {
			glDebugMessageCallback((GLDEBUGPROC)Fluxions::FluxionsGLDebugFunc, NULL);
			glEnable(GL_DEBUG_OUTPUT);
		}
		else {
			HFLOGWARN("No glDebugMessageCallback");
		}
	}

	void APIENTRY FluxionsGLDebugFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
		if (id == 131185)
			return;

		std::ostringstream ostr;
		ostr << "(" << g_CurrentDebugMessage << ") ";
		//ostr << "frame: " << gt_frameCount;
		//ostr << " error: " << gt_errorCount++;
		ostr << " id: " << id;

		ostr << " severity: ";
		switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			ostr << "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			ostr << "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			ostr << "HIGH";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			ostr << "NOTIFICATION";
			break;
		default:
			ostr << severity;
			break;
		}

		ostr << " source: ";
		switch (source) {
		case GL_DEBUG_SOURCE_API:
			ostr << "API";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			ostr << "APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			ostr << "OTHER";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			ostr << "SHADER COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			ostr << "THIRD PARTY";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			ostr << "WINDOW SYSTEM";
			break;
		default:
			ostr << type;
			break;
		}

		ostr << " type: ";
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			ostr << "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			ostr << "DEPRECATED BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			ostr << "UNDEFINED BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			ostr << "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			ostr << "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			ostr << "MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			ostr << "PUSH GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			ostr << "POP GROUP";
			break;
		case GL_DEBUG_TYPE_OTHER:
			ostr << "OTHER";
			break;
		default:
			ostr << type;
			break;
		}

		unsigned up = ((unsigned long long)(userParam) & 0xFFFFFFFF);
		HFLOGINFO("(%i, %#010X) %s", length, up, ostr.str().c_str());
		std::string m = message;
		std::istringstream istr(m);
		std::string line;
		while (getline(istr, line)) {
			Hf::Log.info("> %s", line.c_str());
		}
	}


} // namespace Fluxions
