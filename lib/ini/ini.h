#ifndef __EINIP_H__
#define __EINIP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief 定义了EINIP函数的返回值
 * 
 */
typedef enum INISTAT{
    INI_OK = 0x00, // 0000_0000
    INI_WARN = 0x40, // 0100_0000
    INI_WARN_KVP_NOT_BELONG_SECTION,
    INI_WARN_SECTION_EXIST_SPACE,
    INI_WARN_KEY_EXIST_SPACE,
    INI_WARN_VALUE_EXIST_SPACE,
    INI_ERR = 0x80, // 1000_0000
    INI_ERR_UNKNOWN_LINE,
    INI_ERR_VALUE_IS_EMPTY,
    INI_ERR_STR_NULL,
    INI_ERR_STREAM_NOT_FOUND,
    INI_ERR_INI_NOT_FOUND,
    INI_ERR_SECTION_NOT_FOUND,
    INI_ERR_KEY_NOT_FOUND,
    INI_ERR_FILE_OPEN
}iniStat;

/**
 * @brief 定义了ini解析函数的返回值，用于存储ini解析过程中产生的警告和错误
 * 
 */
typedef struct INIPARSESTAT{
    iniStat stat;

    int warn_num;
    int* warn_lines;
    iniStat* warn_infos;

    int error_num;
    int* error_lines;
    iniStat* error_infos;
}iniParseStat;

/**
 * @brief key-value对
 * 
 */
typedef struct KVP{
    char* key;
    char* value;
}kvp;

/**
 * @brief ini文件的section
 * 
 */
typedef struct SECTION{
    char* name;
    int kvp_num;
    struct KVP** kvps;
}section;

/**
 * @brief 存储ini文件内容的结构体 
 * 
 */
typedef struct INI{ 
    int section_num;
    struct SECTION** sections;
}ini;

/**
 * @brief 打印ini结构体
 * 
 * @param ini_ptr 
 */
void printfIni(ini* ini_ptr);

/**
 * @brief 解析file_path文件，并返回解析结果。解析时，section合并、键值对覆盖。
 * 
 * @param file_path 
 * @return ini* 
 */
ini* iniParseFile(char* file_path);

/**
 * @brief 解析str字符串，并返回解析结果。解析时，section合并、键值对覆盖。
 * 
 */
ini* iniParseStr(char* str);

/**
 * @brief 判断line_str中是否有需要警告的内容，如果有，则将相关信息写入p_stat结构体
 * 
 * @param p_stat 
 * @param line 
 * @param line_str 
 */
void handleIniWarnAndErr(iniParseStat* p_stat, section* section_ptr, int line, char* lineStr);

/**
 * @brief 读取stream流，并解析成ini结构体
 * 
 * @param stream 
 * @param ini_ptr 
 * @return iniParseStat 
 */
iniParseStat iniParse(FILE* stream, ini** ini_ptr);

/**
 * @brief 释放ini内存
 * 
 * @param ini_ptr 
 * @return iniStat 
 */
iniStat iniFree(ini* ini_ptr);

/**
 * @brief 获取指定section
 * 
 * @param ini_ptr 
 * @param section_name 
 * @return section* 
 */
section* iniGetSection(ini* ini_ptr ,char* section_name);

/**
 * @brief 获取指定key对应的value
 * 
 * @param section_ptr 
 * @param key 
 * @return char* 
 */
char* iniGetValue(section* section_ptr ,char* key);

/**
 * @brief 获取指定key对应的kvp
 * 
 * @param section_ptr 
 * @param key 
 * @return kvp* 
 */
kvp* iniGetKvp(section* section_ptr ,char* key);

/**
 * @brief 将ini_ptr中section_name的section名设置为target_name。如果
 *        出现相同的section_name则只修改最后一个同section_name的section
 *        的name。若段不存在则返回INI_ERR_SECTION_NOT_FOUND
 * 
 * @param ini_ptr 
 * @param section_name 
 * @param target_name 
 * @return iniStat 
 */
iniStat iniSetSection(ini* ini_ptr ,char* section_name, char* target_name);

/**
 * @brief 设置指定key对应的value
 * 
 * @param section_ptr 
 * @param key 
 * @param value 
 * @return iniStat 
 */
iniStat iniSetValue(section* section_ptr ,char* key ,char* value);

/**
 * @brief 将section添加到ini中
 * 
 * @param ini_ptr 
 * @param section_ptr 
 * @return iniStat 
 */
iniStat iniAddSection(ini* ini_ptr , char* section_name);

/**
 * @brief 将key-value对添加到section中
 * 
 * @param section_ptr 
 * @param key 
 * @param value 
 * @return iniStat 
 */
iniStat iniAddKey(section* section_ptr ,char* key ,char* value);

/**
 * @brief 删除ini中的section
 * 
 * @param ini_ptr 
 * @param section_ptr 
 * @return iniStat 
 */
iniStat iniDelSection(ini* ini_ptr ,char* section_name);

/**
 * @brief 删除section中的key-value对
 * 
 * @param section_ptr 
 * @param key 
 * @return iniStat 
 */
iniStat iniDelKey(section* section_ptr ,char* key);

/**
 * @brief 将ini_ptr中的ini数据保存至file_path文件中
 * 
 * @param ini_ptr 
 * @param file_path 
 * @return iniStat 
 */
iniStat iniSaveFile(ini* ini_ptr ,char* file_path);

/**
 * @brief 以字符串形式返回ini_ptr中的ini数据
 * 
 */
char* iniSaveStr(ini* ini_ptr);

/**
 * @brief 去除字符串前后的空格
 * 
 * @param str 
 * @return char* 
 */
char* trim(char *str);
#endif