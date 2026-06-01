#ifndef _CHRTASK_H_
#define _CHRTASK_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;

class ChrTask
{
public:
    ChrTask();
    ~ChrTask();

    void init(Player *player, MemChrTaskVector * tasks);
    bool CheckTaskCondition(int32_t taskCondition);
    void reset();
    int32_t receive(int32_t tid);
    bool ForceSubmit(int32_t tid);
    int32_t submit(int32_t tid, int32_t nTimes);
    int32_t giveUp(int32_t tid);
    void checkTaskCanSubmit(int32_t dungenID, int8_t dungeonType);
    int32_t quickDone(int32_t tid);
    bool receivePreCheck(int32_t tid, CfgTask * cfgTask);
    bool submitPreCheck(int32_t tid, CfgTask * cfgTask);
    void updateTaskUseItem(int32_t id, int32_t Count);
    void updateTaskTypeCount(int32_t taskType);
    void updateTaskPlant(int32_t id);
    void updateTaskMonster(int32_t Mid, int32_t group_id, int32_t level);
    void updateTaskSkill(int32_t skillID);
    void updateTaskCount(int32_t Condition, int32_t param);
    void updateTaskDrop(int32_t tid, int32_t itemid);
    void cmdResetTrunkTask(int32_t trunkTaskId);
    bool isDoingTaskType(int32_t type);
    bool isDoingTask(int32_t nTaskId);
    int8_t GetTaskState(int32_t nTaskId);
    bool setTaskCanSubmit(int32_t tid, bool setCondition);
    void sendTaskList();
    void sendTaskKilledMonster();
    void saveToDB(Answer::NetPacket *packet);
    void SaveDBData(PlayerDBData * dbData);
    int32_t getMonster(int32_t tid);
    void setMonster(int32_t tid);
    void sendTaskReceived(int32_t tid);
    void sendTaskSubmitted(int32_t tid, int32_t exp);
    void sendTaskGiveUped(int32_t tid);
    int32_t GetTypeTask(int8_t Type);
    int8_t GetTaskType(int32_t TaskId);
    void CleanTaskType(int8_t nType);
    TaskRequest GetTaskCfgRequest(int32_t Tid);
    void checkTaskDrop(int32_t nTid, int32_t nCount);
    void dropItemFromGroup(DropItem &retstr, int32_t groupId);
    float GetTaskRewardRate(TaskType type);

private:
        // TODO: 确认类型 m_player
        // TODO: 确认类型 m_taskMonsters
        // TODO: 确认类型 m_tasks
        int32_t m_trunkTaskId;
};

#endif // _CHRTASK_H_
