#include "Other/PlayerScoreRankGreater.h"

bool PlayerScoreRankGreater::operator()(const PlayerScore* _Left, const PlayerScore* _Right)
{
    return _Left && _Right && _Left->nDamage > _Right->nDamage;
}
