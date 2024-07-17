#ifndef __EINIP_H__
#define __EINIP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 定义了EINIP函数的返回值
 * 
 */
typedef enum INISTAT{
    INI_OK = 0x00, // 0000_0000
    INI_WARN = 0x40, // 0100_0000
    INI_ERR = 0x80, // 1000_0000
    INI_ERR_STREAM_NOT_FOUND,
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
    struct KVP* kvps[];
}section;

/**
 * @brief 存储ini文件内容的结构体 
 * 
 */
typedef struct INI{ 
    int section_num;
    struct SECTION* sections[];
}ini;

/**
 * @brief 读取stream流，并解析成ini结构体
 * 
 * @param stream 
 * @param ini_ptr 
 * @return iniParseStat 
 */
iniParseStat iniParse(FILE* stream, ini* ini_ptr);

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
#endif