#include "Other/FestivalRank.h"

#include <cstring>
#include <algorithm>

FestivalRank::FestivalRank()
{
    CleanUp();
}

FestivalRank::FestivalRank(const FestivalRank &a2)
{
    nIndex = a2.nIndex;
    strDB = a2.strDB;
    nCharId = a2.nCharId;
    strName = a2.strName;
    nScore = a2.nScore;
    nTime = a2.nTime;
    bChange = a2.bChange;
}

FestivalRank::~FestivalRank()
{
}

void FestivalRank::CleanUp()
{
    nIndex = 0;
    strDB.clear();
    nCharId = 0;
    strName.clear();
    nScore = 0;
    nTime = 0;
    bChange = 0;
}
