#ifndef __EINIP_H__
#define __EINIP_H__
/**
 * @file ini.h
 * @author  buttfa (1662332017@qq.com)
 * @brief ini.h是EIniP库的头文件，定义了EIniP的函数、结构体和枚举类型
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief 该枚举类型定义了大部分EIniP函数的返回值，以及
 *        解析过程中的警告和错误
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
    INI_ERR_KEY_IS_EMPTY,
    INI_ERR_VALUE_IS_EMPTY,
    INI_ERR_STR_NULL,
    INI_ERR_STREAM_NOT_FOUND,
    INI_ERR_INI_NOT_FOUND,
    INI_ERR_SECTION_NOT_FOUND,
    INI_ERR_KEY_NOT_FOUND,
    INI_ERR_FILE_OPEN
}iniStat;

/**
 * @brief 定义了EIniP解析函数的返回值，用于存储ini解析过程中产生的警告和错误
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
 * @param ini_ptr 所需要打印的ini结构体
 */
void printfIni(ini* ini_ptr);

/**
 * @brief 解析file_path文件，并返回解析结果。解析时，section合并、键值对覆盖。
 * 
 * @param file_path 需要解析的文件路径
 * @return ini* 解析得到的ini结构体指针，返回NULL则表示解析失败
 */
ini* iniParseFile(char* file_path);

/**
 * @brief 解析str字符串，并返回解析结果。解析时，section合并、键值对覆盖。
 * 
 * @param str 需要解析的字符串
 * @return ini* 解析得到的ini结构体指针，返回NULL则表示解析失败
 */
ini* iniParseStr(char* str);

/**
 * @brief 判断line_str中是否有需要警告或报错的内容，如果有，则将相关信息写入p_stat结构体
 * 
 * @param p_stat 需要添加警告或报错信息的iniParseStat结构体指针
 * @param section_ptr lineStr所属的section的指针
 * @param line lineStr出现的行数
 * @param lineStr 需要检查的字符串
 */
void handleIniWarnAndErr(iniParseStat* p_stat, section* section_ptr, int line, char* lineStr);

/**
 * @brief 读取stream流，并解析成ini结构体
 * 
 * @param stream 需要读取的stream
 * @param ini_ptr 需要存入的ini结构体二级指针
 * @return iniParseStat 该结构体详细说明了警告和错误情况
 */
iniParseStat iniParse(FILE* stream, ini** ini_ptr);

/**
 * @brief 释放ini内存
 * 
 * @param ini_ptr 需要释放的ini
 * @return iniStat 返回INI_OK表示成功释放，
 *                 返回INI_ERR_INI_NOT_FOUND表示该ini_ptr为空指针
 */
iniStat iniFree(ini* ini_ptr);

/**
 * @brief 获取ini_ptr中最后一个同section_name的section
 * 
 * @param ini_ptr 需要操作的ini指针
 * @param section_name 目标section的名称
 * @return section* 如果存在则返回该section指针，否则返回NULL
 */
section* iniGetSection(ini* ini_ptr ,char* section_name);

/**
 * @brief 获取section_ptr中最后一个同key的键值对的value
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 目标键值对的key名
 * @return char* 如果存在则返回该value指针，否则返回NULL
 */
char* iniGetValue(section* section_ptr ,char* key);

/**
 * @brief 获取section_ptr中最后一个同key的键值对
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 目标键值对的key名
 * @return kvp* 如果存在则返回该kvp指针，否则返回NULL
 */
kvp* iniGetKvp(section* section_ptr ,char* key);

/**
 * @brief 将ini_ptr中最后一个section_name的section名设置为target_name。
 * 
 * @param ini_ptr 需要操作的ini指针
 * @param section_name 需要修改的section的名称
 * @param target_name 修改后的section名称
 * @return iniStat 返回INI_OK表示成功修改，
 *                 返回INI_ERR_INI_NOT_FOUND表示该ini_ptr为空指针，
 *                 返回INI_ERR_STR_NULL表示section_name或target_name为空指针
 *                 返回INI_ERR_SECTION_NOT_FOUND表示该section_name对应的section不存在
 */
iniStat iniSetSection(ini* ini_ptr ,char* section_name, char* target_name);

/**
 * @brief 将section_ptr中最后一个同key名的键值对的值设置为value。
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 目标key名
 * @param value 写入的value
 * @return iniStat 返回INI_OK表示成功修改，
 *                 返回INI_ERR_SECTION_NOT_FOUND表示该section_ptr为空指针，
 *                 返回INI_ERR_STR_NULL表示key或value为空指针
 *                 返回INI_ERR_KEY_NOT_FOUND表示该key对应的键值对不存在
 */
iniStat iniSetValue(section* section_ptr ,char* key ,char* value);

/**
 * @brief 创建名为section_name的section，并添加
 *        到ini的sections末尾
 * 
 * @param ini_ptr 需要操作的ini指针
 * @param section_name 需要创建的section名称
 * @return iniStat 返回INI_OK表示成功添加，
 *                 返回INI_ERR_INI_NOT_FOUND表示该ini_ptr为空指针，
 *                 返回INI_ERR_STR_NULL表示section_name为空指针
 */
iniStat iniAddSection(ini* ini_ptr , char* section_name);

/**
 * @brief 创建key-value对，并添加到section的kvps末尾
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 构成key-value对的key
 * @param value 构成key-value对的value
 * @return iniStat 返回INI_OK表示成功添加，
 *                 返回INI_ERR_SECTION_NOT_FOUND表示该section_ptr为空指针，
 *                 返回INI_ERR_STR_NULL表示key或value为空指针
 */
iniStat iniAddKvp(section* section_ptr ,char* key ,char* value);

/**
 * @brief 删除ini_ptr中最后一个同section_name的section
 * 
 * @param ini_ptr 需要操作的ini指针
 * @param section_name 需要删除的section名称
 * @return iniStat 返回INI_OK表示成功删除，
 *                 返回INI_ERR_INI_NOT_FOUND表示该ini_ptr为空指针，
 *                 返回INI_ERR_SECTION_NOT_FOUND表示该section_name对应的section不存在，或者该section_name为空指针
 */
iniStat iniDelSection(ini* ini_ptr ,char* section_name);

/**
 * @brief 删除section_ptr中最后一个同key名的键值对
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 需要删除的键值对的key名
 * @return iniStat 返回INI_OK表示成功删除，
 *                 返回INI_ERR_SECTION_NOT_FOUND表示该section_ptr为空指针，
 *                 返回INI_ERR_KEY_NOT_FOUND表示该key对应的键值对不存在，或者该key为空指针
 */
iniStat iniDelKvp(section* section_ptr ,char* key);

/**
 * @brief 将ini_ptr中的ini数据保存至file_path文件中
 * 
 * @param ini_ptr 需要保存的ini指针
 * @param file_path 保存的文件路径
 * @return iniStat 返回INI_OK表示成功保存，
 *                 返回INI_ERR_FILE_OPEN表示该file_path为空指针或者无法打开该文件，
 */
iniStat iniSaveFile(ini* ini_ptr ,char* file_path);

/**
 * @brief 将ini_ptr中的ini数据保存至字符串中并返回
 * 
 * @param ini_ptr 需要保存的ini指针
 * @return char* 保存成功返回的字符串指针，否则返回NULL
 */
char* iniSaveStr(ini* ini_ptr);
#endif