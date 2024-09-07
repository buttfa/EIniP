/**
 * @file ini.c
 * @author  buttfa (1662332017@qq.com)
 * @brief ini.c是EIniP库的实现源文件
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

/**
 * @brief 去除字符串前后的空格
 *        注：该函数是在原字符串上操作，会修改原字符串，且返回值是基于原字符串的索引地址
 * 
 * @param str 操作的字符串
 * @return char* 返回去除空格后的字符串指针
 */
static char* trim(char *str) {
    if (str == NULL || str == (char*)0x01)
        return NULL;

    char *end;
    
    // 去除前导空格
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0)  // 字符串为空的情况
        return str;
    
    // 去除后导空格
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // 将结尾的'\0'放回正确的位置
    end[1] = '\0';
    
    return str;
}

/**
 * @brief 打印ini结构体
 * 
 * @param ini_ptr 所需要打印的ini结构体
 */
void printfIni(ini* ini_ptr) {
    // 检查ini_ptr是否为空指针
    if (ini_ptr == NULL)
        return;

    printf("section_num: %d\n\n", ini_ptr->section_num);
    for (int i = 0; i < ini_ptr->section_num; i++) {
        printf("section_name: %s, kvp_num: %d\n", ini_ptr->sections[i]->name, ini_ptr->sections[i]->kvp_num);
        for (int j = 0; j < ini_ptr->sections[i]->kvp_num; j++) {
            printf("key: %s, value: %s\n", ini_ptr->sections[i]->kvps[j]->key, ini_ptr->sections[i]->kvps[j]->value);
        } 
        printf("\n");
    }
}

/**
 * @brief 解析file_path文件，并返回解析结果。解析时，section合并、键值对覆盖。
 * 
 * @param file_path 需要解析的文件路径
 * @return ini* 解析得到的ini结构体指针，返回NULL则表示解析失败
 */
ini* iniParseFile(char* file_path) {
    // 判断file_path是否为空指针
    if (file_path == NULL) 
        return NULL;

    // 创建FILE流
    FILE* stream = fopen(file_path, "r");
    if (stream == NULL) 
        return NULL;
    
    // 调用iniParse函数
    ini* ini_ptr = NULL;
    iniParseStat* p_stat = iniParse(stream, &ini_ptr);
    iniStat stat = p_stat->stat;
    iniDestroyStat((iniParseStat**)&p_stat);
    fclose(stream);
    
    // 返回ini_ptr
    if (stat & INI_ERR)
        return NULL; 
    return ini_ptr;
}

/**
 * @brief 解析str字符串，并返回解析结果。解析时，section合并、键值对覆盖。
 * 
 * @param str 需要解析的字符串
 * @return ini* 解析得到的ini结构体指针，返回NULL则表示解析失败
 */
ini* iniParseStr(char* str) {
    // 判断str是否为空指针
    if (str == NULL) 
        return NULL;

    // 创建FILE流
    FILE* stream = fmemopen(str, strlen(str), "r");
    if (stream == NULL) 
        return NULL;

    // 调用iniParse函数
    ini* ini_ptr = NULL;
    iniParseStat* p_stat = iniParse(stream, &ini_ptr);
    iniStat stat = p_stat->stat;
    iniDestroyStat((iniParseStat**)&p_stat);
    fclose(stream);
    
    // 返回ini_ptr
    if (stat & INI_ERR)
        return NULL; 
    return ini_ptr;
}

/**
 * @brief 向p_stat中添加警告信息
 * 
 * @param p_stat 需要添加警告信息的iniParseStat结构体指针
 * @param line 出现警告的行数
 * @param stat 警告的状态码
 */
static void addIniWarning(iniParseStat* p_stat, int line, iniStat stat) {
    // 写入stat
    p_stat->stat = (iniStat)((int)p_stat->stat | (int)INI_WARN);

    // 添加发生警告的行数
    p_stat->warn_num++;
    p_stat->warn_lines = (int*)realloc(p_stat->warn_lines, sizeof(int) * p_stat->warn_num);
    p_stat->warn_lines[p_stat->warn_num - 1] = line;

    // 添加警告信息
    p_stat->warn_infos = (iniStat*)realloc(p_stat->warn_infos, sizeof(iniStat) * p_stat->warn_num);
    p_stat->warn_infos[p_stat->warn_num - 1] = stat;
}

/**
 * @brief 向p_stat中添加错误信息
 * 
 * @param p_stat 需要添加错误信息的iniParseStat结构体指针
 * @param line 出现错误的行数
 * @param stat 错误的状态码
 */
static void addIniError(iniParseStat* p_stat, int line, iniStat stat) {
    // 写入stat
    p_stat->stat = (iniStat)((int)p_stat->stat | (int)INI_ERR);

    // 添加发生错误的行数
    p_stat->error_num++;
    p_stat->error_lines = (int*)realloc(p_stat->error_lines, sizeof(int) * p_stat->error_num);
    p_stat->error_lines[p_stat->error_num - 1] = line;

    // 添加错误信息
    p_stat->error_infos = (iniStat*)realloc(p_stat->error_infos, sizeof(iniStat) * p_stat->error_num);
    p_stat->error_infos[p_stat->error_num - 1] = stat;
}

/**
 * @brief 判断line_str中是否有需要警告或报错的内容，如果有，则将相关信息写入p_stat结构体
 * 
 * @param p_stat 需要添加警告或报错信息的iniParseStat结构体指针
 * @param section_ptr lineStr所属的section的指针
 * @param line lineStr出现的行数
 * @param lineStr 需要检查的字符串
 */
void handleIniWarnAndErr(iniParseStat* p_stat, section* section_ptr, int line, char* lineStr) {
    // 检测lineStr是否为空字符
    if (lineStr == NULL || strlen(lineStr) == 0)
        return;
    
    // 拷贝数据
    char* line_str = strdup(lineStr);

    // 如果line_str没有等号，则产生INI_ERR_UNKNOWN_LINE错误
    if (strchr(line_str, '=') == NULL)
        addIniError(p_stat, line, INI_ERR_UNKNOWN_LINE);
    
    // 如果line_str有等号，但等号右边没有值，则产生INI_ERR_VALUE_IS_EMPTY警告
    if (strchr(line_str, '=') != NULL && strlen(trim(strchr(line_str, '=')+1))==0) 
        addIniError(p_stat, line, INI_ERR_VALUE_IS_EMPTY);

    // 如果line_str有等号，但等号左边没有键，则产生INI_ERR_KEY_IS_EMPTY错误
    if (strchr(line_str, '=') != NULL && strlen(trim(strtok(line_str, "=")))==0) 
        addIniError(p_stat, line, INI_ERR_KEY_IS_EMPTY);

    // 获取键和值
    free(line_str);
    line_str = strdup(lineStr);
    char* value = trim(strchr(line_str, '=')+1);
    char* key = trim(strtok(line_str, "="));
    // 如果line_str的键值对的键中存在空格，则产生INI_WARN_KEY_EXIST_SPACE警告
    if (key != NULL && strchr(key, ' ') != NULL) 
        addIniWarning(p_stat, line, INI_WARN_KEY_EXIST_SPACE);

    // 如果line_str的键值对的值中存在空格，则产生INI_WARN_VALUE_EXIST_SPACE警告
    if (value != NULL && strchr(value, ' ') != NULL) 
        addIniWarning(p_stat, line, INI_WARN_VALUE_EXIST_SPACE);

    // 当section_ptr为空指针，则产生INI_WARN_KVP_NOT_BELONG_SECTION警告
    if (key != NULL && value != NULL && section_ptr == NULL)
        addIniWarning(p_stat, line, INI_WARN_KVP_NOT_BELONG_SECTION);

    free(line_str);
}

/**
 * @brief 读取stream流，并解析成ini结构体
 * 
 * @param stream 需要读取的stream
 * @param ini_ptr 需要存入的ini结构体二级指针
 * @return iniParseStat* 该结构体详细说明了警告和错误情况
 */
iniParseStat* iniParse(FILE* stream, ini** ini_ptr) {
    // 创建iniParseStat结构体
    iniParseStat* p_stat = (iniParseStat*)malloc(sizeof(iniParseStat));
    memset(p_stat, 0, sizeof(iniParseStat));

    // 检查stream是否为空
    if (stream == NULL) {
        p_stat->stat = INI_ERR_STREAM_NOT_FOUND;
        return p_stat;
    }

    // 如果ini_ptr为空指针，则分配内存
    if (*ini_ptr == NULL) {
        *ini_ptr = (ini*)malloc(sizeof(ini));
    }
    memset(*ini_ptr, 0, sizeof(ini));

    char buf[1024];              // 缓存区
    section *section_ptr = NULL; // section指针
    kvp* kvp_ptr = NULL;         // kvp指针
    int row = 0;                 // 行数
    while (fgets(buf, sizeof(buf), stream) != NULL) {
        // 去除行首和行尾的空格及行尾的回车符
        char* tmp = trim(buf);
        row++;
        // 跳过注释行和空行
        if (tmp[0] == '#' || tmp[0] == ';' || strlen(tmp)==0) {
            continue;
        }

        // 判断是否是section
        if (tmp[0] == '[' && tmp[strlen(tmp) - 1] == ']') {
            // 提取section名称
            tmp[strlen(tmp) - 1] = '\0';
            
            // 如果section已存在，则使section_ptr指向对应的section
            if ((section_ptr=iniGetSection(*ini_ptr, tmp+1)) != NULL)
                continue;

            
            // 否则，将新section添加到ini中
            iniAddSection(*ini_ptr, tmp+1);
            section_ptr = iniGetSection(*ini_ptr, tmp+1);
            // 如果新seciont_name中存在空格，则产生INI_WARN_SECTION_EXIST_SPACE警告
            if (strchr(tmp+1, ' ') != NULL)
                addIniWarning(p_stat, row, INI_WARN_SECTION_EXIST_SPACE);

            continue;
        }

        int index = 0, key_index = 0, value_index = 0;
        // 判断是否是key-value，如果不满足条件，则跳过
        handleIniWarnAndErr(p_stat, section_ptr, row, tmp);
        if ((int)p_stat->stat & (int)INI_ERR) 
            continue;

        // 找到=字符
        char* equal_pos = strchr(tmp, '=');
        if (equal_pos == NULL)
            continue;
        *equal_pos = '\0';
        if (strlen(trim(tmp)) == 0 || strlen(trim(equal_pos+1)) == 0)
            continue;
        
        // 将key-value添加到section中
        iniAddKvp(section_ptr, trim(tmp), trim(equal_pos+1));
    }
    return p_stat;
}

/**
 * @brief 释放inParseStat内存
 * 
 * @param p_stat 需要释放的iniParseStat指针
 * @return iniStat 返回INI_OK表示成功释放，
 *                 返回INI_ERR_STAT_NOT_FOUND表示p_stat为空指针
 */
iniStat iniFreeStat(iniParseStat* p_stat) {
    // 判断p_stat是否为空指针
    if (p_stat == NULL)
        return INI_ERR_STAT_NOT_FOUND;

    // 释放警告和错误信息内存
    free(p_stat->error_infos);
    free(p_stat->error_lines);
    free(p_stat->warn_infos);
    free(p_stat->warn_lines);

    free(p_stat);
    return INI_OK;
}

/**
 * @brief 释放iniParseStat内存，并将p_stat置为NULL
 * 
 * @param p_stat 需要释放的iniParseStat指针
 * @return iniStat 返回INI_OK表示成功释放，
 *                 返回INI_ERR_STAT_NOT_FOUND表示p_stat为空指针
 */
iniStat iniDestroyStat(iniParseStat** p_stat) {
    iniStat stat = iniFreeStat(*p_stat);
    *p_stat = (stat == INI_OK) ? NULL : *p_stat;
    return stat;
}

/**
 * @brief 释放ini内存
 * 
 * @param ini_ptr 需要释放的ini
 * @return iniStat 返回INI_OK表示成功释放，
 *                 返回INI_ERR_INI_NOT_FOUND表示该ini_ptr为空指针
 */
iniStat iniFree(ini* ini_ptr) {
    if (ini_ptr == NULL) {
        return INI_ERR_INI_NOT_FOUND;
    }

    // 遍历并释放所有section
    for (int i = 0; i < ini_ptr->section_num; i++) {
        section* sec = ini_ptr->sections[i];
        
        // 遍历并释放所有key-value对
        for (int j = 0; j < sec->kvp_num; j++) {
            kvp* kv = sec->kvps[j];
            free(kv->key); // 释放key内存
            free(kv->value); // 释放value内存
            free(kv); // 释放kvp结构体内存
        }
        
        free(sec->kvps); // 释放kvp数组内存
        free(sec->name); // 释放section名称内存
        free(sec); // 释放section结构体内存
    }
    
    free(ini_ptr->sections); // 释放section数组内存
    free(ini_ptr); // 最后释放ini结构体内存

    return INI_OK;
}

/**
 * @brief 释放ini内存，并将ini_ptr置为NULL
 * 
 * @param ini_ptr 需要释放的ini
 * @return iniStat 返回INI_OK表示成功释放，
 *                 返回INI_ERR_INI_NOT_FOUND表示ini_ptr为空指针
 */
iniStat iniDestroy(ini** ini_ptr) {
    iniStat stat = iniFree(*ini_ptr);
    *ini_ptr = (stat == INI_OK) ? NULL : *ini_ptr;
    return stat;
}

/**
 * @brief 获取ini_ptr中最后一个同section_name的section
 * 
 * @param ini_ptr 需要操作的ini指针
 * @param section_name 目标section的名称
 * @return section* 如果存在则返回该section指针，否则返回NULL
 */
section* iniGetSection(ini* ini_ptr ,char* section_name) {
    // 检查ini_ptr是否为空指针
    if (ini_ptr == NULL)
        return NULL;
    
    // 遍历ini中的section
    for (int i = ini_ptr->section_num-1; i >= 0; i--) {
        if (strcmp(ini_ptr->sections[i]->name, section_name) == 0) {
            return ini_ptr->sections[i];
        }
    }

    return NULL;
}

/**
 * @brief 获取section_ptr中最后一个同key的键值对的value
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 目标键值对的key名
 * @return char* 如果存在则返回该value指针，否则返回NULL
 */
char* iniGetValue(section* section_ptr ,char* key) {
    // 检查section_ptr是否为空指针
    if (section_ptr == NULL)
        return NULL;

    // 遍历section中的key-value对
    for (int i = section_ptr->kvp_num-1; i >= 0; i--) {
        if (strcmp(section_ptr->kvps[i]->key, key) == 0) {
            return section_ptr->kvps[i]->value;
        }
    }

    return NULL;
}

/**
 * @brief 获取section_ptr中最后一个同key的键值对
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 目标键值对的key名
 * @return kvp* 如果存在则返回该kvp指针，否则返回NULL
 */
kvp* iniGetKvp(section* section_ptr ,char* key) {
    // 检查section_ptr是否为空指针
    if (section_ptr == NULL)
        return NULL;

    // 遍历section中的key-value对
    for (int i = section_ptr->kvp_num-1; i >= 0; i--) {
        if (strcmp(section_ptr->kvps[i]->key, key) == 0) {
            return section_ptr->kvps[i];
        }
    }
    return NULL;
}

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
iniStat iniSetSection(ini* ini_ptr ,char* section_name, char* target_name) {
    // 检查ini_ptr是否为空指针
    if (ini_ptr == NULL)
        return INI_ERR_INI_NOT_FOUND;

    // 检查section_name和target_name是否为空指针
    if (section_name == NULL || target_name == NULL)
        return INI_ERR_STR_NULL;
    
    // 遍历ini中的section
    for (int i = ini_ptr->section_num-1; i >= 0; i--) {
        if (strcmp(ini_ptr->sections[i]->name, section_name) == 0) {
            // 更新section名称
            free(ini_ptr->sections[i]->name);
            ini_ptr->sections[i]->name = strdup(target_name);
            return INI_OK;
        }
    }

    return INI_ERR_SECTION_NOT_FOUND;
}

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
iniStat iniSetValue(section* section_ptr ,char* key ,char* value) {
    // 检查section_ptr是否为空指针
    if (section_ptr == NULL)
        return INI_ERR_SECTION_NOT_FOUND;
    
    // 检查key和value是否为空指针
    if (key == NULL || value == NULL)
        return INI_ERR_STR_NULL;

    // 遍历section中的key-value对
    for (int i = section_ptr->kvp_num-1; i >= 0; i--) {
        if (strcmp(section_ptr->kvps[i]->key, key) == 0) {
            // 更新value
            free(section_ptr->kvps[i]->value);
            section_ptr->kvps[i]->value = strdup(value);
            return INI_OK;
        }
    }

    return INI_ERR_KEY_NOT_FOUND;
}

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
iniStat iniAddSection(ini* ini_ptr , char* section_name) {
    // 检查ini_ptr是否为空指针
    if (ini_ptr == NULL)
        return INI_ERR_INI_NOT_FOUND;
    // 检查section_name是否为空指针
    if (section_name == NULL)
        return INI_ERR_STR_NULL;
    
    // 创建section
    section* section_ptr = (section*)malloc(sizeof(section));
    memset(section_ptr, 0, sizeof(section));
    section_ptr->name = strdup(section_name);

    // 添加section
    ini_ptr->section_num++;
    ini_ptr->sections = (section**)realloc(ini_ptr->sections, sizeof(section*) * ini_ptr->section_num);
    ini_ptr->sections[ini_ptr->section_num - 1] = section_ptr;
    return INI_OK;
}

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
iniStat iniAddKvp(section* section_ptr ,char* key ,char* value) {
    // 检查section_ptr是否为空指针
    if (section_ptr == NULL) 
        return INI_ERR_SECTION_NOT_FOUND;

    // 检查key和value是否为空指针
    if (key == NULL || value == NULL)
        return INI_ERR_STR_NULL;
    
    // 添加key-value对
    section_ptr->kvp_num++;
    section_ptr->kvps = (kvp**)realloc(section_ptr->kvps, sizeof(kvp*) * section_ptr->kvp_num);
    section_ptr->kvps[section_ptr->kvp_num - 1] = (kvp*)malloc(sizeof(kvp));
    section_ptr->kvps[section_ptr->kvp_num - 1]->key = strdup(key);
    section_ptr->kvps[section_ptr->kvp_num - 1]->value = strdup(value);
    return INI_OK;
}

/**
 * @brief 删除ini_ptr中最后一个同section_name的section
 * 
 * @param ini_ptr 需要操作的ini指针
 * @param section_name 需要删除的section名称
 * @return iniStat 返回INI_OK表示成功删除，
 *                 返回INI_ERR_INI_NOT_FOUND表示该ini_ptr为空指针，
 *                 返回INI_ERR_SECTION_NOT_FOUND表示该section_name对应的section不存在，或者该section_name为空指针
 */
iniStat iniDelSection(ini* ini_ptr, char* section_name) {
    // 检查ini_ptr是否为空指针
    if (ini_ptr == NULL)
        return INI_ERR_INI_NOT_FOUND;
    // 检查section_ptr是否为空指针
    if (section_name == NULL)
        return INI_ERR_SECTION_NOT_FOUND;

    // 遍历INI结构体中的sections数组，找到要删除的section的位置
    for (int i = 0; i < ini_ptr->section_num; i++) {
        if (strcmp(ini_ptr->sections[i]->name, section_name) == 0) {
            section* section_ptr = ini_ptr->sections[i];
            // 释放section中的kvps
            for (int j = 0; j < section_ptr->kvp_num; j++) {
                free(section_ptr->kvps[j]->key);
                free(section_ptr->kvps[j]->value);
                free(section_ptr->kvps[j]);
            }
            free(section_ptr->kvps);
            free(section_ptr->name);
            free(section_ptr);

            // 移动后续的sections以填补空缺
            for (int k = i; k < ini_ptr->section_num - 1; k++) {
                ini_ptr->sections[k] = ini_ptr->sections[k + 1];
            }

            // 减少section的数量
            ini_ptr->section_num--;

            // 重新分配sections数组的大小
            ini_ptr->sections = (section**)realloc(ini_ptr->sections, sizeof(section*) * ini_ptr->section_num);

            return INI_OK; // 假设INI_SUCCESS是成功的状态码
        }
    }
    return INI_ERR_SECTION_NOT_FOUND; // 假设INI_SECTION_NOT_FOUND是未找到section的状态码
}

/**
 * @brief 删除section_ptr中最后一个同key名的键值对
 * 
 * @param section_ptr 需要操作的section指针
 * @param key 需要删除的键值对的key名
 * @return iniStat 返回INI_OK表示成功删除，
 *                 返回INI_ERR_SECTION_NOT_FOUND表示该section_ptr为空指针，
 *                 返回INI_ERR_KEY_NOT_FOUND表示该key对应的键值对不存在，或者该key为空指针
 */
iniStat iniDelKvp(section* section_ptr, char* key) {
    // 检查section_ptr是否为空指针
    if (section_ptr == NULL)
        return INI_ERR_SECTION_NOT_FOUND;
    // 检查key是否为空指针
    if (key == NULL)
        return INI_ERR_KEY_NOT_FOUND;

    // 遍历section中的kvps数组，查找给定的key
    for (int i = 0; i < section_ptr->kvp_num; i++) {
        if (strcmp(section_ptr->kvps[i]->key, key) == 0) {
            // 释放key和value的内存
            free(section_ptr->kvps[i]->key);
            free(section_ptr->kvps[i]->value);
            // 释放kvp结构体的内存
            free(section_ptr->kvps[i]);

            // 移动后续的kvps以填补空缺
            for (int j = i; j < section_ptr->kvp_num - 1; j++) {
                section_ptr->kvps[j] = section_ptr->kvps[j + 1];
            }

            // 减少kvp的数量
            section_ptr->kvp_num--;

            // 重新分配kvps数组的大小
            section_ptr->kvps = (kvp**)realloc(section_ptr->kvps, sizeof(kvp*) * section_ptr->kvp_num);

            return INI_OK; // 成功删除key-value对
        }
    }
    return INI_ERR_KEY_NOT_FOUND; // 未找到指定的key
}

/**
 * @brief 将ini_ptr中的ini数据保存至file_path文件中
 * 
 * @param ini_ptr 需要保存的ini指针
 * @param file_path 保存的文件路径
 * @return iniStat 返回INI_OK表示成功保存，
 *                 返回INI_ERR_FILE_OPEN表示该file_path为空指针或者无法打开该文件，
 */
iniStat iniSaveFile(ini* ini_ptr, char* file_path) {
    // 检查ini_ptr是否为空指针
    if (ini_ptr == NULL) 
        return INI_ERR_INI_NOT_FOUND;

    // 检查file_path是否为空指针
    if (file_path == NULL) 
        return INI_ERR_FILE_OPEN;

    // 打开文件
    FILE* fp = fopen(file_path, "w");
    if (fp == NULL) 
        return INI_ERR_FILE_OPEN;

    // 遍历sections数组，将每个section写入文件
    for (int i = 0; i < ini_ptr->section_num; i++) {
        section* sec = ini_ptr->sections[i];
        fprintf(fp, "[%s]\n", sec->name);

        // 遍历kvps数组，将每个key-value对写入文件
        for (int j = 0; j < sec->kvp_num; j++) {
            kvp* kv = sec->kvps[j];
            fprintf(fp, "%s=%s\n", kv->key, kv->value);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return INI_OK;
}

/**
 * @brief 将ini_ptr中的ini数据保存至字符串中并返回
 * 
 * @param ini_ptr 需要保存的ini指针
 * @return char* 保存成功返回的字符串指针，否则返回NULL
 */
char* iniSaveStr(ini* ini_ptr) {
    // 检查ini_ptr是否为空
    if (ini_ptr == NULL)
        return NULL;

    // 第一次遍历，计算字符串长度
    int str_len = 0;
    for (int i = 0; i < ini_ptr->section_num; i++) {
        section* sec = ini_ptr->sections[i];
        //         [                   ]   \n   \n(每个section结尾的\n)
        str_len += 1+strlen(sec->name)+1 + 1  + 1;
        for (int j = 0; j < sec->kvp_num; j++) {
            kvp* kv = sec->kvps[j];
            //                           =                      \n
            str_len += strlen(kv->key) + 1 + strlen(kv->value) + 1;
        }
    }

    // 第二次遍历，生成字符串
    char* str = (char*)malloc(str_len + 1);
    memset(str, 0, str_len + 1);
    for (int i = 0; i < ini_ptr->section_num; i++) {
        section* sec = ini_ptr->sections[i];
        strcat(str, "[");
        strcat(str, sec->name);
        strcat(str, "]\n");
        for (int j = 0; j < sec->kvp_num; j++) {
            kvp* kv = sec->kvps[j];
            strcat(str, kv->key);
            strcat(str, "=");
            strcat(str, kv->value);
            strcat(str, "\n");
        }
        strcat(str, "\n");
    }

    return str;
}