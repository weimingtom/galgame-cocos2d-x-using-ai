#ifndef __SAVEGAME_MANAGER_H__
#define __SAVEGAME_MANAGER_H__

#include "cocos2d.h"
#include "GameData.h"
#include <string>
#include <vector>

/**
 * @brief 存档管理器类
 * 处理游戏的保存和读取功能
 */
class SaveGameManager : public cocos2d::Ref
{
public:
    static SaveGameManager* getInstance();
    static void destroyInstance();
    
    /**
     * @brief 保存游戏到指定槽位
     * @param slot 存档槽位 (0-9)
     * @param saveData 存档数据
     * @return 是否成功
     */
    bool saveGame(int slot, const GameData::SaveData& saveData);
    
    /**
     * @brief 从指定槽位读取游戏
     * @param slot 存档槽位 (0-9)
     * @return 存档数据，失败则返回默认值
     */
    GameData::SaveData loadGame(int slot);
    
    /**
     * @brief 检查指定槽位是否已有存档
     * @param slot 存档槽位 (0-9)
     * @return 是否有存档
     */
    bool hasSave(int slot);
    
    /**
     * @brief 获取所有存档槽位的状态
     * @return 各槽位是否有存档的布尔数组
     */
    std::vector<bool> getAllSaveSlotsStatus();
    
    /**
     * @brief 删除指定槽位的存档
     * @param slot 存档槽位 (0-9)
     * @return 是否成功
     */
    bool deleteSave(int slot);

private:
    static SaveGameManager* _instance;
    
    std::string getSaveFilePath(int slot); // 获取存档文件路径
    
    SaveGameManager();
    virtual ~SaveGameManager();
};

#endif // __SAVEGAME_MANAGER_H__