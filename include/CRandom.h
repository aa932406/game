#ifndef _CRANDOM_H_\n#define _CRANDOM_H_\nclass CRandom { public: static CRandom* GetInstance(); int32_t UniformInt(int32_t min, int32_t max); };\n#endif
