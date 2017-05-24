/***************************************************************************
*                                                                          *
* Project64-video - A Nintendo 64 gfx plugin.                              *
* http://www.pj64-emu.com/                                                 *
* Copyright (C) 2017 Project64. All rights reserved.                       *
* Copyright (C) 2003-2009  Sergey 'Gonetz' Lipski                          *
* Copyright (C) 2002 Dave2001                                              *
*                                                                          *
* License:                                                                 *
* GNU/GPLv2 http://www.gnu.org/licenses/gpl-2.0.html                       *
* version 2 of the License, or (at your option) any later version.         *
*                                                                          *
****************************************************************************/
#pragma once
#include <Project64-video/Renderer/types.h>
#include <glide.h>
#include "../GlideExtensions.h"

void gfxClipWindow(FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy);
void gfxColorMask(FxBool rgb, FxBool a);
FxU32 gfxTexMinAddress(GrChipID_t tmu);
FxBool gfxSstWinClose(GrContext_t context);
void gfxTextureBufferExt(GrChipID_t tmu, FxU32 startAddress, GrLOD_t lodmin, GrLOD_t lodmax, GrAspectRatio_t aspect, GrTextureFormat_t fmt, FxU32 evenOdd);
FxU32 gfxTexMaxAddress(GrChipID_t tmu);
FxU32 gfxTexTextureMemRequired(FxU32 evenOdd, GrTexInfo *info);
void gfxConstantColorValue(GrColor_t value);
void gfxColorCombine(GrCombineFunction_t function, GrCombineFactor_t factor, GrCombineLocal_t local, GrCombineOther_t other, FxBool invert);
void gfxAlphaCombine(GrCombineFunction_t function, GrCombineFactor_t factor, GrCombineLocal_t local, GrCombineOther_t other, FxBool invert);
void gfxTexCombine(GrChipID_t tmu, GrCombineFunction_t rgb_function, GrCombineFactor_t rgb_factor, GrCombineFunction_t alpha_function, GrCombineFactor_t alpha_factor, FxBool rgb_invert, FxBool alpha_invert);
void gfxAlphaBlendFunction(GrAlphaBlendFnc_t rgb_sf, GrAlphaBlendFnc_t rgb_df, GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df);
void gfxAlphaTestReferenceValue(gfxAlpha_t value);
void gfxAlphaTestFunction(GrCmpFnc_t function);
void gfxFogMode(GrFogMode_t mode);
void gfxFogGenerateLinear(float nearZ, float farZ);
void gfxFogColorValue(GrColor_t fogcolor);

extern uint32_t nbTextureUnits;
extern uint32_t g_scr_res_x, g_scr_res_y, g_res_x, g_res_y;
