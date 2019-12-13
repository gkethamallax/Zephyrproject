static const q31_t in_q31[256] = {
    0x6AF3242E, 0x51C5B68D, 0x47C651B4, 0x40D04483, 
    0x676B45A0, 0x05D81E89, 0x48E28ADE, 0x60C13C3E, 
    0x6D2A7744, 0x5A1E316D, 0x2AD87331, 0x7A3CB15A, 
    0x1A307E74, 0x1E250B23, 0x0003241E, 0x6DB48393, 
    0x1BB67C9B, 0x694F3CA6, 0x3025CBFA, 0x730689FD, 
    0x0A609C0D, 0x4C3E2841, 0x1847CB4C, 0x448A9CF7, 
    0x21C75902, 0x47518BA7, 0x12EF7905, 0x4331DF98, 
    0x74705AE1, 0x5C698D06, 0x42F5460B, 0x0D0BF4AD, 
    0x70937459, 0x7187CDCE, 0x686EB8E0, 0x4C9FE088, 
    0x7A15724D, 0x5333F02A, 0x6C2BC001, 0x74F55BD7, 
    0x79D6D1ED, 0x1E836E34, 0x066444BE, 0x2377B4E3, 
    0x03959184, 0x64A450E9, 0x30CAA923, 0x5831F5D6, 
    0x0FA3075E, 0x661731B3, 0x1C643A79, 0x058E19BC, 
    0x55C10484, 0x5C990354, 0x66B4132F, 0x622316C0, 
    0x01EE1088, 0x0C2F0A4F, 0x35B2FC8C, 0x588066A5, 
    0x542DC071, 0x7EA08025, 0x7FFFFFFF, 0x3C4472AE, 
    0x235B824B, 0x1A49CEA3, 0x0A8B18CA, 0x689E13C2, 
    0x28BBCF95, 0x04FA1ABE, 0x510956F1, 0x2E05DD41, 
    0x76CC3C3F, 0x159B9110, 0x098A6118, 0x44A25A18, 
    0x5C801DE9, 0x70617D77, 0x4F632AB3, 0x7EC44AD8, 
    0x0FA99BF7, 0x49765678, 0x77D422A3, 0x4776FA80, 
    0x21165C49, 0x3BF22E25, 0x0914E607, 0x2C510ECC, 
    0x3A72B72A, 0x4059529E, 0x44C012C6, 0x35B8D30F, 
    0x3033F985, 0x1DCF011B, 0x6FD6176D, 0x68147C93, 
    0x2F339223, 0x4019DD3C, 0x4508DDEC, 0x17933715, 
    0x6F110350, 0x35E3CD3E, 0x4C323F5F, 0x2DDD6BB6, 
    0x71591D6D, 0x5E20F78D, 0x402E1C7F, 0x565A4310, 
    0x6C0B62F0, 0x72B974EE, 0x0DFF0611, 0x72255808, 
    0x2FF2CA2C, 0x35FDAF51, 0x640FB786, 0x258F08D9, 
    0x110B4CF5, 0x0944C661, 0x64A86796, 0x283F80D1, 
    0x52D3C1A3, 0x1A233443, 0x0108C9A9, 0x62DCD642, 
    0x3B9B5B29, 0x0F5DF403, 0x38643B30, 0x152B30F4, 
    0x13A03AEF, 0x06159A1B, 0x6A7B1A86, 0x62B2D529, 
    0x15324BB5, 0x42944B2A, 0x4C30D584, 0x3C1A2988, 
    0x33C32CA0, 0x1FC40D8F, 0x04FE37AB, 0x61C8AC1F, 
    0x3F9D83AC, 0x7B7EDE1A, 0x1036A823, 0x2A1A3C0A, 
    0x231C5AD6, 0x5AAE7EAC, 0x3AE56CA1, 0x2EA76D2E, 
    0x5A93B5F4, 0x7968FF55, 0x41CB5F21, 0x0CFBC518, 
    0x7C63730A, 0x0546A5BF, 0x7C2F91BF, 0x11A3CD5D, 
    0x53B02A83, 0x51344B9E, 0x5149C94D, 0x1BDDE4C7, 
    0x27A5A449, 0x30750558, 0x7D78C649, 0x68DA0EBD, 
    0x5B381336, 0x2A7450E3, 0x3E907D00, 0x1036B931, 
    0x17F90383, 0x7AAB7681, 0x59C52FCB, 0x7F540EB5, 
    0x4E384860, 0x10CE5A8E, 0x5DCBDFA9, 0x417E0564, 
    0x27B016AB, 0x68F5C0F9, 0x3F1F85E7, 0x3C9B92B0, 
    0x151277E8, 0x15044C3C, 0x62AD90D0, 0x1789B11B, 
    0x1AF86764, 0x5A7E1AC8, 0x60C40773, 0x621B089C, 
    0x06A4C148, 0x4D56E367, 0x12E3677B, 0x68E030DC, 
    0x04951184, 0x090E903C, 0x58A486AE, 0x691C685D, 
    0x55B5A4C0, 0x73DD16CA, 0x4D056497, 0x281D0F87, 
    0x67C97DD3, 0x43CDDEE1, 0x37D8438E, 0x4BF26E37, 
    0x6E9B0724, 0x63BE9F4D, 0x0E589176, 0x1865146B, 
    0x627C04AA, 0x2AEFC6EE, 0x3B5D56BF, 0x77F05CC1, 
    0x03814DF0, 0x0422D2F0, 0x55875239, 0x4B645E96, 
    0x09FECE9A, 0x0AF9BCF9, 0x3FDB5ADB, 0x3A5F4CD9, 
    0x5E6D1BC8, 0x3B4B2D90, 0x72802AF5, 0x16515883, 
    0x73F6B734, 0x4965CD46, 0x6CA221C6, 0x54690754, 
    0x0CAC32EF, 0x773CF758, 0x535D3215, 0x0AACE5F6, 
    0x56546E7B, 0x72DB2FB2, 0x17EA55EC, 0x771F3C3C, 
    0x341F586A, 0x3DB1C294, 0x4C64C1FA, 0x4DD8D1E6, 
    0x708AFB12, 0x6503EF7F, 0x16E4C179, 0x6C2BB11A, 
    0x475D6685, 0x16AF1570, 0x2541751D, 0x75EA8474, 
    0x7DCFD85C, 0x5A5E1A52, 0x5775BAE4, 0x1147E074, 
    0x62F90A7C, 0x7D97349A, 0x6D745791, 0x0FDA13CC 
    };

static const uint32_t ref_f32[256] = {
    0x3f55e648, 0x3f238b6d, 0x3f0f8ca3, 0x3f01a089, 
    0x3f4ed68b, 0x3d3b03d1, 0x3f11c516, 0x3f418278, 
    0x3f5a54ef, 0x3f343c63, 0x3eab61cd, 0x3f747963, 
    0x3e5183f4, 0x3e712859, 0x38c90783, 0x3f5b6907, 
    0x3e5db3e5, 0x3f529e79, 0x3ec09730, 0x3f660d14, 
    0x3da609c1, 0x3f187c51, 0x3e423e5a, 0x3f09153a, 
    0x3e871d64, 0x3f0ea317, 0x3e177bc8, 0x3f0663bf, 
    0x3f68e0b6, 0x3f38d31a, 0x3f05ea8c, 0x3dd0bf4b, 
    0x3f6126e9, 0x3f630f9c, 0x3f50dd72, 0x3f193fc1, 
    0x3f742ae5, 0x3f2667e0, 0x3f585780, 0x3f69eab8, 
    0x3f73ada4, 0x3e741b72, 0x3d4c8898, 0x3e8dded4, 
    0x3ce56461, 0x3f4948a2, 0x3ec32aa5, 0x3f3063ec, 
    0x3dfa3076, 0x3f4c2e63, 0x3e6321d4, 0x3d31c338, 
    0x3f2b8209, 0x3f393207, 0x3f4d6826, 0x3f44462e, 
    0x3c770844, 0x3dc2f0a5, 0x3ed6cbf2, 0x3f3100cd, 
    0x3f285b81, 0x3f7d4100, 0x3f800000, 0x3ef111cb, 
    0x3e8d6e09, 0x3e524e75, 0x3da8b18d, 0x3f513c28, 
    0x3ea2ef3e, 0x3d1f4358, 0x3f2212ae, 0x3eb81775, 
    0x3f6d9878, 0x3e2cdc89, 0x3d98a611, 0x3f0944b4, 
    0x3f39003c, 0x3f60c2fb, 0x3f1ec655, 0x3f7d8896, 
    0x3dfa99bf, 0x3f12ecad, 0x3f6fa845, 0x3f0eedf5, 
    0x3e845971, 0x3eefc8b9, 0x3d914e60, 0x3eb1443b, 
    0x3ee9cadd, 0x3f00b2a5, 0x3f098026, 0x3ed6e34c, 
    0x3ec0cfe6, 0x3e6e7809, 0x3f5fac2f, 0x3f5028f9, 
    0x3ebcce49, 0x3f0033ba, 0x3f0a11bc, 0x3e3c99b9, 
    0x3f5e2207, 0x3ed78f35, 0x3f18647f, 0x3eb775af, 
    0x3f62b23b, 0x3f3c41ef, 0x3f005c39, 0x3f2cb486, 
    0x3f5816c6, 0x3f6572ea, 0x3ddff061, 0x3f644ab0, 
    0x3ebfcb29, 0x3ed7f6bd, 0x3f481f6f, 0x3e963c23, 
    0x3e085a68, 0x3d944c66, 0x3f4950cf, 0x3ea0fe03, 
    0x3f25a783, 0x3e5119a2, 0x3c0464d5, 0x3f45b9ad, 
    0x3eee6d6d, 0x3df5df40, 0x3ee190ed, 0x3e295988, 
    0x3e1d01d7, 0x3d42b343, 0x3f54f635, 0x3f4565aa, 
    0x3e29925e, 0x3f052896, 0x3f1861ab, 0x3ef068a6, 
    0x3ecf0cb2, 0x3e7e206c, 0x3d1fc6f5, 0x3f439158, 
    0x3efe760f, 0x3f76fdbc, 0x3e01b541, 0x3ea868f0, 
    0x3e8c716b, 0x3f355cfd, 0x3eeb95b3, 0x3eba9db5, 
    0x3f35276c, 0x3f72d1ff, 0x3f0396be, 0x3dcfbc51, 
    0x3f78c6e6, 0x3d28d4b8, 0x3f785f23, 0x3e0d1e6b, 
    0x3f276055, 0x3f226897, 0x3f229393, 0x3e5eef26, 
    0x3e9e9691, 0x3ec1d415, 0x3f7af18d, 0x3f51b41d, 
    0x3f367026, 0x3ea9d144, 0x3efa41f4, 0x3e01b5ca, 
    0x3e3fc81c, 0x3f7556ed, 0x3f338a60, 0x3f7ea81d, 
    0x3f1c7091, 0x3e0672d4, 0x3f3b97bf, 0x3f02fc0b, 
    0x3e9ec05b, 0x3f51eb82, 0x3efc7e18, 0x3ef26e4b, 
    0x3e2893bf, 0x3e282262, 0x3f455b22, 0x3e3c4d89, 
    0x3e57c33b, 0x3f34fc36, 0x3f41880f, 0x3f443611, 
    0x3d549829, 0x3f1aadc7, 0x3e171b3c, 0x3f51c062, 
    0x3d12a231, 0x3d90e904, 0x3f31490d, 0x3f5238d1, 
    0x3f2b6b4a, 0x3f67ba2e, 0x3f1a0ac9, 0x3ea0743e, 
    0x3f4f92fc, 0x3f079bbe, 0x3edf610e, 0x3f17e4dc, 
    0x3f5d360e, 0x3f477d3f, 0x3de58917, 0x3e4328a3, 
    0x3f44f809, 0x3eabbf1c, 0x3eed755b, 0x3f6fe0ba, 
    0x3ce0537c, 0x3d045a5e, 0x3f2b0ea4, 0x3f16c8bd, 
    0x3d9fecea, 0x3daf9bd0, 0x3eff6d6b, 0x3ee97d33, 
    0x3f3cda38, 0x3eed2cb6, 0x3f650056, 0x3e328ac4, 
    0x3f67ed6e, 0x3f12cb9b, 0x3f594444, 0x3f28d20f, 
    0x3dcac32f, 0x3f6e79ef, 0x3f26ba64, 0x3daace5f, 
    0x3f2ca8dd, 0x3f65b65f, 0x3e3f52af, 0x3f6e3e78, 
    0x3ed07d62, 0x3ef6c70a, 0x3f18c984, 0x3f1bb1a4, 
    0x3f6115f6, 0x3f4a07df, 0x3e37260c, 0x3f585762, 
    0x3f0ebacd, 0x3e3578ab, 0x3e9505d4, 0x3f6bd509, 
    0x3f7b9fb1, 0x3f34bc35, 0x3f2eeb76, 0x3e0a3f04, 
    0x3f45f215, 0x3f7b2e69, 0x3f5ae8af, 0x3dfda13d 
    };

static const q15_t ref_q15[256] = {
    0x6AF3, 0x51C6, 0x47C6, 0x40D0, 0x676B, 0x05D8, 0x48E3, 0x60C1, 
    0x6D2A, 0x5A1E, 0x2AD8, 0x7A3D, 0x1A30, 0x1E25, 0x0003, 0x6DB5, 
    0x1BB6, 0x694F, 0x3026, 0x7307, 0x0A61, 0x4C3E, 0x1848, 0x448B, 
    0x21C7, 0x4752, 0x12EF, 0x4332, 0x7470, 0x5C6A, 0x42F5, 0x0D0C, 
    0x7093, 0x7188, 0x686F, 0x4CA0, 0x7A15, 0x5334, 0x6C2C, 0x74F5, 
    0x79D7, 0x1E83, 0x0664, 0x2378, 0x0396, 0x64A4, 0x30CB, 0x5832, 
    0x0FA3, 0x6617, 0x1C64, 0x058E, 0x55C1, 0x5C99, 0x66B4, 0x6223, 
    0x01EE, 0x0C2F, 0x35B3, 0x5880, 0x542E, 0x7EA1, 0x7FFF, 0x3C44, 
    0x235C, 0x1A4A, 0x0A8B, 0x689E, 0x28BC, 0x04FA, 0x5109, 0x2E06, 
    0x76CC, 0x159C, 0x098A, 0x44A2, 0x5C80, 0x7061, 0x4F63, 0x7EC4, 
    0x0FAA, 0x4976, 0x77D4, 0x4777, 0x2116, 0x3BF2, 0x0915, 0x2C51, 
    0x3A73, 0x4059, 0x44C0, 0x35B9, 0x3034, 0x1DCF, 0x6FD6, 0x6814, 
    0x2F34, 0x401A, 0x4509, 0x1793, 0x6F11, 0x35E4, 0x4C32, 0x2DDD, 
    0x7159, 0x5E21, 0x402E, 0x565A, 0x6C0B, 0x72B9, 0x0DFF, 0x7225, 
    0x2FF3, 0x35FE, 0x6410, 0x258F, 0x110B, 0x0945, 0x64A8, 0x2840, 
    0x52D4, 0x1A23, 0x0109, 0x62DD, 0x3B9B, 0x0F5E, 0x3864, 0x152B, 
    0x13A0, 0x0616, 0x6A7B, 0x62B3, 0x1532, 0x4294, 0x4C31, 0x3C1A, 
    0x33C3, 0x1FC4, 0x04FE, 0x61C9, 0x3F9E, 0x7B7F, 0x1037, 0x2A1A, 
    0x231C, 0x5AAE, 0x3AE5, 0x2EA7, 0x5A94, 0x7969, 0x41CB, 0x0CFC, 
    0x7C63, 0x0547, 0x7C30, 0x11A4, 0x53B0, 0x5134, 0x514A, 0x1BDE, 
    0x27A6, 0x3075, 0x7D79, 0x68DA, 0x5B38, 0x2A74, 0x3E90, 0x1037, 
    0x17F9, 0x7AAB, 0x59C5, 0x7F54, 0x4E38, 0x10CE, 0x5DCC, 0x417E, 
    0x27B0, 0x68F6, 0x3F20, 0x3C9C, 0x1512, 0x1504, 0x62AE, 0x178A, 
    0x1AF8, 0x5A7E, 0x60C4, 0x621B, 0x06A5, 0x4D57, 0x12E3, 0x68E0, 
    0x0495, 0x090F, 0x58A5, 0x691C, 0x55B6, 0x73DD, 0x4D05, 0x281D, 
    0x67C9, 0x43CE, 0x37D8, 0x4BF2, 0x6E9B, 0x63BF, 0x0E59, 0x1865, 
    0x627C, 0x2AF0, 0x3B5D, 0x77F0, 0x0381, 0x0423, 0x5587, 0x4B64, 
    0x09FF, 0x0AFA, 0x3FDB, 0x3A5F, 0x5E6D, 0x3B4B, 0x7280, 0x1651, 
    0x73F7, 0x4966, 0x6CA2, 0x5469, 0x0CAC, 0x773D, 0x535D, 0x0AAD, 
    0x5654, 0x72DB, 0x17EA, 0x771F, 0x341F, 0x3DB2, 0x4C65, 0x4DD9, 
    0x708B, 0x6504, 0x16E5, 0x6C2C, 0x475D, 0x16AF, 0x2541, 0x75EB, 
    0x7DD0, 0x5A5E, 0x5776, 0x1148, 0x62F9, 0x7D97, 0x6D74, 0x0FDA 
    };

static const q7_t ref_q7[256] = {
    0x6B, 0x52, 0x48, 0x41, 0x67, 0x06, 0x49, 0x61, 
    0x6D, 0x5A, 0x2B, 0x7A, 0x1A, 0x1E, 0x00, 0x6E, 
    0x1C, 0x69, 0x30, 0x73, 0x0A, 0x4C, 0x18, 0x45, 
    0x22, 0x47, 0x13, 0x43, 0x74, 0x5C, 0x43, 0x0D, 
    0x71, 0x72, 0x68, 0x4D, 0x7A, 0x53, 0x6C, 0x75, 
    0x7A, 0x1F, 0x06, 0x23, 0x04, 0x65, 0x31, 0x58, 
    0x10, 0x66, 0x1C, 0x06, 0x56, 0x5D, 0x67, 0x62, 
    0x02, 0x0C, 0x36, 0x59, 0x54, 0x7F, 0x7F, 0x3C, 
    0x23, 0x1A, 0x0B, 0x69, 0x29, 0x05, 0x51, 0x2E, 
    0x77, 0x16, 0x0A, 0x45, 0x5D, 0x70, 0x4F, 0x7F, 
    0x10, 0x49, 0x78, 0x47, 0x21, 0x3C, 0x09, 0x2C, 
    0x3A, 0x40, 0x45, 0x36, 0x30, 0x1E, 0x70, 0x68, 
    0x2F, 0x40, 0x45, 0x18, 0x6F, 0x36, 0x4C, 0x2E, 
    0x71, 0x5E, 0x40, 0x56, 0x6C, 0x73, 0x0E, 0x72, 
    0x30, 0x36, 0x64, 0x26, 0x11, 0x09, 0x65, 0x28, 
    0x53, 0x1A, 0x01, 0x63, 0x3C, 0x0F, 0x38, 0x15, 
    0x14, 0x06, 0x6A, 0x63, 0x15, 0x43, 0x4C, 0x3C, 
    0x34, 0x20, 0x05, 0x62, 0x40, 0x7B, 0x10, 0x2A, 
    0x23, 0x5B, 0x3B, 0x2F, 0x5B, 0x79, 0x42, 0x0D, 
    0x7C, 0x05, 0x7C, 0x12, 0x54, 0x51, 0x51, 0x1C, 
    0x28, 0x30, 0x7D, 0x69, 0x5B, 0x2A, 0x3F, 0x10, 
    0x18, 0x7B, 0x5A, 0x7F, 0x4E, 0x11, 0x5E, 0x41, 
    0x28, 0x69, 0x3F, 0x3D, 0x15, 0x15, 0x63, 0x18, 
    0x1B, 0x5A, 0x61, 0x62, 0x07, 0x4D, 0x13, 0x69, 
    0x05, 0x09, 0x59, 0x69, 0x56, 0x74, 0x4D, 0x28, 
    0x68, 0x44, 0x38, 0x4C, 0x6F, 0x64, 0x0E, 0x18, 
    0x62, 0x2B, 0x3B, 0x78, 0x04, 0x04, 0x56, 0x4B, 
    0x0A, 0x0B, 0x40, 0x3A, 0x5E, 0x3B, 0x73, 0x16, 
    0x74, 0x49, 0x6D, 0x54, 0x0D, 0x77, 0x53, 0x0B, 
    0x56, 0x73, 0x18, 0x77, 0x34, 0x3E, 0x4C, 0x4E, 
    0x71, 0x65, 0x17, 0x6C, 0x47, 0x17, 0x25, 0x76, 
    0x7E, 0x5A, 0x57, 0x11, 0x63, 0x7E, 0x6D, 0x10 
    };

