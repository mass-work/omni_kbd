// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later

#include "omni_image_loader.h"
#include "generated/omni_logo.qgf.h"
#include "generated/save.qgf.h"
#include "generated/layer_00.qgf.h"
#include "generated/layer_01.qgf.h"
#include "generated/layer_02.qgf.h"
#include "generated/layer_03.qgf.h"
#include "generated/layer_04.qgf.h"
#include "generated/layer_05.qgf.h"
#include "generated/layer_06.qgf.h"
#include "generated/layer_07.qgf.h"
#include "generated/layer_08.qgf.h"
#include "generated/layer_09.qgf.h"
#include "generated/layer_10.qgf.h"
#include "generated/layer_11.qgf.h"
#include "generated/000.qgf.h"
#include "generated/001.qgf.h"
#include "generated/002.qgf.h"
#include "generated/003.qgf.h"
#include "generated/004.qgf.h"
#include "generated/005.qgf.h"
#include "generated/006.qgf.h"
#include "generated/007.qgf.h"
#include "generated/008.qgf.h"
#include "generated/009.qgf.h"
#include "generated/010.qgf.h"
#include "generated/011.qgf.h"
#include "generated/012.qgf.h"
#include "generated/013.qgf.h"
#include "generated/014.qgf.h"
#include "generated/015.qgf.h"
#include "generated/016.qgf.h"
#include "generated/017.qgf.h"
#include "generated/018.qgf.h"
#include "generated/019.qgf.h"
#include "generated/020.qgf.h"
#include "generated/021.qgf.h"
#include "generated/022.qgf.h"
#include "generated/023.qgf.h"
#include "generated/024.qgf.h"
#include "generated/025.qgf.h"
#include "generated/026.qgf.h"
#include "generated/027.qgf.h"
#include "generated/028.qgf.h"
#include "generated/029.qgf.h"
#include "generated/030.qgf.h"
#include "generated/031.qgf.h"
#include "generated/032.qgf.h"
#include "generated/033.qgf.h"
#include "generated/034.qgf.h"
#include "generated/035.qgf.h"
#include "generated/036.qgf.h"
#include "generated/037.qgf.h"
#include "generated/038.qgf.h"
#include "generated/039.qgf.h"
#include "generated/040.qgf.h"
#include "generated/041.qgf.h"
#include "generated/042.qgf.h"
#include "generated/043.qgf.h"
#include "generated/044.qgf.h"
#include "generated/045.qgf.h"
#include "generated/046.qgf.h"
#include "generated/047.qgf.h"
#include "generated/048.qgf.h"
#include "generated/049.qgf.h"
#include "generated/050.qgf.h"
#include "generated/051.qgf.h"
#include "generated/052.qgf.h"
#include "generated/053.qgf.h"
#include "generated/054.qgf.h"
#include "generated/055.qgf.h"
#include "generated/056.qgf.h"
#include "generated/057.qgf.h"
#include "generated/058.qgf.h"
#include "generated/059.qgf.h"
#include "generated/060.qgf.h"
#include "generated/061.qgf.h"
#include "generated/062.qgf.h"
#include "generated/063.qgf.h"
#include "generated/064.qgf.h"
#include "generated/065.qgf.h"
#include "generated/066.qgf.h"
#include "generated/067.qgf.h"
#include "generated/068.qgf.h"
#include "generated/069.qgf.h"
#include "generated/070.qgf.h"
#include "generated/071.qgf.h"
#include "generated/072.qgf.h"
#include "generated/073.qgf.h"
#include "generated/074.qgf.h"
#include "generated/075.qgf.h"
#include "generated/076.qgf.h"
#include "generated/077.qgf.h"
#include "generated/078.qgf.h"
#include "generated/079.qgf.h"
#include "generated/080.qgf.h"
#include "generated/081.qgf.h"
#include "generated/082.qgf.h"
#include "generated/083.qgf.h"
#include "generated/084.qgf.h"
#include "generated/085.qgf.h"
#include "generated/086.qgf.h"

painter_image_handle_t image_logo;
painter_image_handle_t image_save;
painter_image_handle_t layer_00, layer_01, layer_02, layer_03, layer_04, layer_05, layer_06, layer_07, layer_08, layer_09, layer_10, layer_11;
painter_image_handle_t image_000, image_001, image_002, image_003, image_004, image_005, image_006, image_007, image_008, image_009, image_010, image_011, image_012, image_013, image_014, image_015, image_016, image_017, image_018, image_019, image_020, image_021, image_022, image_023, image_024, image_025, image_026, image_027, image_028, image_029, image_030, image_031, image_032, image_033, image_034, image_035, image_036, image_037, image_038, image_039, image_040, image_041, image_042, image_043, image_044, image_045, image_046, image_047, image_048, image_049, image_050, image_051, image_052, image_053, image_054, image_055, image_056, image_057, image_058, image_059, image_060, image_061, image_062, image_063, image_064, image_065, image_066, image_067, image_068, image_069, image_070, image_071, image_072, image_073, image_074, image_075, image_076, image_077, image_078, image_079, image_080, image_081, image_082, image_083, image_084, image_085, image_086, image_087, image_088, image_089, image_090, image_091, image_092, image_093, image_094, image_095, image_096, image_097, image_098, image_099, image_100, image_101, image_102, image_103, image_104, image_105, image_106, image_107, image_108, image_109, image_110, image_111, image_112, image_113, image_114, image_115, image_116, image_117, image_118, image_119, image_120, image_121, image_122, image_123, image_124, image_125, image_126, image_127, image_128, image_129, image_130, image_131, image_132, image_133, image_134, image_135, image_136, image_137, image_138, image_139, image_140, image_141, image_142, image_143, image_144, image_145, image_146, image_147, image_148, image_149, image_150, image_151, image_152, image_153, image_154, image_155, image_156, image_157, image_158, image_159, image_160, image_161, image_162, image_163, image_164, image_165, image_166, image_167, image_168, image_169, image_170, image_171, image_172, image_173, image_174, image_175, image_176, image_177, image_178, image_179, image_180, image_181, image_182, image_183, image_184, image_185, image_186, image_187, image_188, image_189, image_190, image_191, image_192, image_193, image_194, image_195, image_196, image_197, image_198, image_199, image_200, image_201, image_202, image_203, image_204, image_205, image_206, image_207, image_208, image_209, image_210, image_211, image_212, image_213, image_214, image_215, image_216, image_217, image_218, image_219, image_220, image_221, image_222, image_223, image_224, image_225, image_226, image_227, image_228, image_229, image_230, image_231, image_232, image_233, image_234, image_235, image_236, image_237, image_238, image_239, image_240, image_241, image_242, image_243, image_244, image_245, image_246, image_247, image_248, image_249, image_250, image_251, image_252, image_253, image_254;

// 初期化関数の実装
void initialize_images(void) {
    image_logo = qp_load_image_mem(gfx_omni_logo);
    image_save = qp_load_image_mem(gfx_save);
    layer_00 = qp_load_image_mem(gfx_layer_00);
    layer_01 = qp_load_image_mem(gfx_layer_01);
    layer_02 = qp_load_image_mem(gfx_layer_02);
    layer_03 = qp_load_image_mem(gfx_layer_03);
    layer_04 = qp_load_image_mem(gfx_layer_04);
    layer_05 = qp_load_image_mem(gfx_layer_05);
    layer_06 = qp_load_image_mem(gfx_layer_06);
    layer_07 = qp_load_image_mem(gfx_layer_07);
    layer_08 = qp_load_image_mem(gfx_layer_08);
    layer_09 = qp_load_image_mem(gfx_layer_09);
    layer_10 = qp_load_image_mem(gfx_layer_10);
    layer_11 = qp_load_image_mem(gfx_layer_11);
    image_000 = qp_load_image_mem(gfx_000);
    image_001 = qp_load_image_mem(gfx_001);
    image_002 = qp_load_image_mem(gfx_002);
    image_003 = qp_load_image_mem(gfx_003);
    image_004 = qp_load_image_mem(gfx_004);
    image_005 = qp_load_image_mem(gfx_005);
    image_006 = qp_load_image_mem(gfx_006);
    image_007 = qp_load_image_mem(gfx_007);
    image_008 = qp_load_image_mem(gfx_008);
    image_009 = qp_load_image_mem(gfx_009);
    image_010 = qp_load_image_mem(gfx_010);
    image_011 = qp_load_image_mem(gfx_011);
    image_012 = qp_load_image_mem(gfx_012);
    image_013 = qp_load_image_mem(gfx_013);
    image_014 = qp_load_image_mem(gfx_014);
    image_015 = qp_load_image_mem(gfx_015);
    image_016 = qp_load_image_mem(gfx_016);
    image_017 = qp_load_image_mem(gfx_017);
    image_018 = qp_load_image_mem(gfx_018);
    image_019 = qp_load_image_mem(gfx_019);
    image_020 = qp_load_image_mem(gfx_020);
    image_021 = qp_load_image_mem(gfx_021);
    image_022 = qp_load_image_mem(gfx_022);
    image_023 = qp_load_image_mem(gfx_023);
    image_024 = qp_load_image_mem(gfx_024);
    image_025 = qp_load_image_mem(gfx_025);
    image_026 = qp_load_image_mem(gfx_026);
    image_027 = qp_load_image_mem(gfx_027);
    image_028 = qp_load_image_mem(gfx_028);
    image_029 = qp_load_image_mem(gfx_029);
    image_030 = qp_load_image_mem(gfx_030);
    image_031 = qp_load_image_mem(gfx_031);
    image_032 = qp_load_image_mem(gfx_032);
    image_033 = qp_load_image_mem(gfx_033);
    image_034 = qp_load_image_mem(gfx_034);
    image_035 = qp_load_image_mem(gfx_035);
    image_036 = qp_load_image_mem(gfx_036);
    image_037 = qp_load_image_mem(gfx_037);
    image_038 = qp_load_image_mem(gfx_038);
    image_039 = qp_load_image_mem(gfx_039);
    image_040 = qp_load_image_mem(gfx_040);
    image_041 = qp_load_image_mem(gfx_041);
    image_042 = qp_load_image_mem(gfx_042);
    image_043 = qp_load_image_mem(gfx_043);
    image_044 = qp_load_image_mem(gfx_044);
    image_045 = qp_load_image_mem(gfx_045);
    image_046 = qp_load_image_mem(gfx_046);
    image_047 = qp_load_image_mem(gfx_047);
    image_048 = qp_load_image_mem(gfx_048);
    image_049 = qp_load_image_mem(gfx_049);
    image_050 = qp_load_image_mem(gfx_050);
    image_051 = qp_load_image_mem(gfx_051);
    image_052 = qp_load_image_mem(gfx_052);
    image_053 = qp_load_image_mem(gfx_053);
    image_054 = qp_load_image_mem(gfx_054);
    image_055 = qp_load_image_mem(gfx_055);
    image_056 = qp_load_image_mem(gfx_056);
    image_057 = qp_load_image_mem(gfx_057);
    image_058 = qp_load_image_mem(gfx_058);
    image_059 = qp_load_image_mem(gfx_059);
    image_060 = qp_load_image_mem(gfx_060);
    image_061 = qp_load_image_mem(gfx_061);
    image_062 = qp_load_image_mem(gfx_062);
    image_063 = qp_load_image_mem(gfx_063);
    image_064 = qp_load_image_mem(gfx_064);
    image_065 = qp_load_image_mem(gfx_065);
    image_066 = qp_load_image_mem(gfx_066);
    image_067 = qp_load_image_mem(gfx_067);
    image_068 = qp_load_image_mem(gfx_068);
    image_069 = qp_load_image_mem(gfx_069);
    image_070 = qp_load_image_mem(gfx_070);
    image_071 = qp_load_image_mem(gfx_071);
    image_072 = qp_load_image_mem(gfx_072);
    image_073 = qp_load_image_mem(gfx_073);
    image_074 = qp_load_image_mem(gfx_074);
    image_075 = qp_load_image_mem(gfx_075);
    image_076 = qp_load_image_mem(gfx_076);
    image_077 = qp_load_image_mem(gfx_077);
    image_078 = qp_load_image_mem(gfx_078);
    image_079 = qp_load_image_mem(gfx_079);
    image_080 = qp_load_image_mem(gfx_080);
    image_081 = qp_load_image_mem(gfx_081);
    image_082 = qp_load_image_mem(gfx_082);
    image_083 = qp_load_image_mem(gfx_083);
    image_084 = qp_load_image_mem(gfx_084);
    image_085 = qp_load_image_mem(gfx_085);
    image_086 = qp_load_image_mem(gfx_086);
}
