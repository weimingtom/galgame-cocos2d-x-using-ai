#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include <string>
#include <vector>
#include <map>

/**
 * @brief 游戏数据命名空间
 * 包含游戏中使用的各种数据结构
 */
namespace GameData
{
    /**
     * @brief 对话数据结构
     */
    struct DialogData
    {
        std::string speaker;      // 说话者
        std::string text;         // 对话内容
        std::string background;   // 背景图片名
        std::vector<std::string> characters; // 出现场的角色
        std::vector<std::string> choices;    // 可选选项（如果有）
        
        DialogData() {}
        DialogData(const std::string& sp, const std::string& txt) 
            : speaker(sp), text(txt) {}
    };
    
    /**
     * @brief 角色数据结构
     */
    struct CharacterData
    {
        std::string id;           // 角色ID
        std::string name;         // 角色名称
        std::string defaultSprite;// 默认立绘
        std::map<std::string, std::string> expressions; // 表情映射
        
        CharacterData() {}
        CharacterData(const std::string& i, const std::string& n) 
            : id(i), name(n) {}
    };
    
    /**
     * @brief 存档数据结构
     */
    struct SaveData
    {
        int chapter;              // 章节
        int scene;                // 场景编号
        int dialogIndex;          // 对话索引
        std::map<std::string, bool> flags; // 游戏标志
        std::map<std::string, int> variables; // 游戏变量
        
        SaveData() : chapter(0), scene(0), dialogIndex(0) {}
    };
}

#endif // __GAMEDATA_H__