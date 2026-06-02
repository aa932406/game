#ifndef _CRANDOM_H_
#define _CRANDOM_H_
class CRandom { public: static CRandom* GetInstance(); int32_t UniformInt(int32_t min, int32_t max); };
#endif
