#include "SaveGameManager.h"
#include "platform/CCFileUtils.h"
#include <fstream>
#include <sstream>

USING_NS_CC;

SaveGameManager* SaveGameManager::_instance = nullptr;

SaveGameManager* SaveGameManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = new(std::nothrow) SaveGameManager();
    }
    return _instance;
}

void SaveGameManager::destroyInstance()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

SaveGameManager::SaveGameManager()
{
}

SaveGameManager::~SaveGameManager()
{
}

std::string SaveGameManager::getSaveFilePath(int slot)
{
    std::stringstream ss;
    ss << "save_" << slot << ".dat";
    return FileUtils::getInstance()->getWritablePath() + ss.str();
}

bool SaveGameManager::saveGame(int slot, const GameData::SaveData& saveData)
{
    std::string path = getSaveFilePath(slot);
    
    // 创建输出流
    std::ofstream outFile(path, std::ios::binary);
    if (!outFile.is_open()) {
        CCLOG("Cannot open file for saving: %s", path.c_str());
        return false;
    }
    
    // 写入存档数据
    outFile.write(reinterpret_cast<const char*>(&saveData.chapter), sizeof(saveData.chapter));
    outFile.write(reinterpret_cast<const char*>(&saveData.scene), sizeof(saveData.scene));
    outFile.write(reinterpret_cast<const char*>(&saveData.dialogIndex), sizeof(saveData.dialogIndex));
    
    // 写入flags数量
    size_t flagsSize = saveData.flags.size();
    outFile.write(reinterpret_cast<const char*>(&flagsSize), sizeof(flagsSize));
    
    // 写入flags内容
    for (const auto& flag : saveData.flags) {
        size_t keyLen = flag.first.length();
        outFile.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
        outFile.write(flag.first.c_str(), keyLen);
        outFile.write(reinterpret_cast<const char*>(&flag.second), sizeof(flag.second));
    }
    
    // 写入variables数量
    size_t varsSize = saveData.variables.size();
    outFile.write(reinterpret_cast<const char*>(&varsSize), sizeof(varsSize));
    
    // 写入variables内容
    for (const auto& var : saveData.variables) {
        size_t keyLen = var.first.length();
        outFile.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
        outFile.write(var.first.c_str(), keyLen);
        outFile.write(reinterpret_cast<const char*>(&var.second), sizeof(var.second));
    }
    
    outFile.close();
    
    CCLOG("Game saved to slot %d", slot);
    return true;
}

GameData::SaveData SaveGameManager::loadGame(int slot)
{
    std::string path = getSaveFilePath(slot);
    
    // 检查文件是否存在
    if (!FileUtils::getInstance()->isFileExist(path)) {
        CCLOG("Save file does not exist: %s", path.c_str());
        return GameData::SaveData(); // 返回默认构造的存档数据
    }
    
    std::ifstream inFile(path, std::ios::binary);
    if (!inFile.is_open()) {
        CCLOG("Cannot open file for loading: %s", path.c_str());
        return GameData::SaveData();
    }
    
    GameData::SaveData saveData;
    
    // 读取存档数据
    inFile.read(reinterpret_cast<char*>(&saveData.chapter), sizeof(saveData.chapter));
    inFile.read(reinterpret_cast<char*>(&saveData.scene), sizeof(saveData.scene));
    inFile.read(reinterpret_cast<char*>(&saveData.dialogIndex), sizeof(saveData.dialogIndex));
    
    // 读取flags
    size_t flagsSize;
    inFile.read(reinterpret_cast<char*>(&flagsSize), sizeof(flagsSize));
    
    for (size_t i = 0; i < flagsSize; ++i) {
        size_t keyLen;
        inFile.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
        
        std::string key;
        key.resize(keyLen);
        inFile.read(&key[0], keyLen);
        
        bool value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));
        
        saveData.flags[key] = value;
    }
    
    // 读取variables
    size_t varsSize;
    inFile.read(reinterpret_cast<char*>(&varsSize), sizeof(varsSize));
    
    for (size_t i = 0; i < varsSize; ++i) {
        size_t keyLen;
        inFile.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
        
        std::string key;
        key.resize(keyLen);
        inFile.read(&key[0], keyLen);
        
        int value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));
        
        saveData.variables[key] = value;
    }
    
    inFile.close();
    
    CCLOG("Game loaded from slot %d", slot);
    return saveData;
}

bool SaveGameManager::hasSave(int slot)
{
    std::string path = getSaveFilePath(slot);
    return FileUtils::getInstance()->isFileExist(path);
}

std::vector<bool> SaveGameManager::getAllSaveSlotsStatus()
{
    std::vector<bool> slots(10); // 假设有10个存档槽位
    
    for (int i = 0; i < 10; ++i) {
        slots[i] = hasSave(i);
    }
    
    return slots;
}

bool SaveGameManager::deleteSave(int slot)
{
    std::string path = getSaveFilePath(slot);
    
    if (FileUtils::getInstance()->isFileExist(path)) {
        return FileUtils::getInstance()->removeFile(path);
    }
    
    return true; // 文件不存在也算删除成功
}