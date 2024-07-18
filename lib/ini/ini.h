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
    INI_ERR = 0x80, // 1000_0000
    INI_ERR_STREAM_NOT_FOUND,
    INI_ERR_INI_NOT_FOUND,
    INI_ERR_SECTION_NOT_FOUND
}iniStat;

/**
 * @brief 定义了ini解析函数的返回值，用于存储ini解析过程中产生的警告和错误
 * 
 */
typedef struct INIPARSESTAT{
    iniStat stat;

    int warn_num;
    int* warn_line;
    char** warn_line_str;

    int error_num;
    int* error_line;
    char** error_line_str;
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
 * @brief 读取stream流，并解析成ini结构体
 * 
 * @param stream 
 * @param ini_ptr 
 * @return iniParseStat 
 */
iniParseStat iniParse(FILE* stream, ini** ini_ptr);

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
iniStat iniAddSection(ini* ini_ptr , section* section_ptr);

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
 * @brief 提取input_str中的子字符串，以空格为分隔符
 * 
 * @param input_str 
 * @param out_num_tokens 
 * @return char** 
 */
char** splitStringBySpace(char* input_str, int* out_num_tokens);

/**
 * @brief 释放分割字符串的数组。
 * 
 * @param splitResult 
 */
void freeSplitResult(char **splitResult);

/**
 * @brief 去除字符串前后的空格
 * 
 * @param str 
 * @return char* 
 */
char* trim(char *str);
#endif