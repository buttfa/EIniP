#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

/**
 * @brief 打印ini结构体
 * 
 * @param ini_ptr 
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
 * @param file_path 
 * @return ini* 
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
    iniParseStat p_stat = iniParse(stream, &ini_ptr);
    fclose(stream);
    
    // 返回ini_ptr
    if (p_stat.stat & INI_ERR)
        return NULL; 
    return ini_ptr;
}

/**
 * @brief 解析str字符串，并返回解析结果。解析时，section合并、键值对覆盖。
 * 
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
    iniParseStat p_stat = iniParse(stream, &ini_ptr);
    fclose(stream);
    
    // 返回ini_ptr
    if (p_stat.stat & INI_ERR)
        return NULL; 
    return ini_ptr;
}

/**
 * @brief 向p_stat中添加警告信息
 * 
 * @param p_stat 
 * @param line 
 * @param stat 
 */
void addIniWarning(iniParseStat* p_stat, int line, iniStat stat) {
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
 * @param p_stat 
 * @param line 
 * @param stat 
 */
void addIniError(iniParseStat* p_stat, int line, iniStat stat) {
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
 * @brief 判断line_str中是否有需要警告的内容，如果有，则将相关信息写入p_stat结构体
 * 
 * @param p_stat 
 * @param line 
 * @param line_str 
 */
void handleIniWarnAndErr(iniParseStat* p_stat, section* section_ptr, int line, char* lineStr) {
    // 拷贝数据
    char* line_str = strdup(lineStr);

    // 如果line_str没有等号，则产生INI_ERR_UNKNOWN_LINE错误
    if (strchr(line_str, '=') == NULL)
        addIniError(p_stat, line, INI_ERR_UNKNOWN_LINE);
    
    // 如果line_str有等号，但等号右边没有值，则产生INI_WARN_VALUE_IS_EMPTY警告
    if (strchr(line_str, '=') != NULL && strlen(trim(strchr(line_str, '=')+1))==0) 
        addIniWarning(p_stat, line, INI_WARN_VALUE_IS_EMPTY);

    // 获取键和值
    char* key = trim(strtok(line_str, "="));
    char* value = trim(strchr(line_str, '=')+1);
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
 * @param stream 
 * @param ini_ptr 
 * @return iniParseStat 
 */
iniParseStat iniParse(FILE* stream, ini** ini_ptr) {
    // 创建iniParseStat结构体
    iniParseStat p_stat;
    memset(&p_stat, 0, sizeof(iniParseStat));

    // 检查stream是否为空
    if (stream == NULL) {
        p_stat.stat = INI_ERR_STREAM_NOT_FOUND;
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

            // 清空缓存区
            memset(buf, 0, sizeof(buf));
            continue;
        }

        int index = 0, key_index = 0, value_index = 0;
        // 找到=字符
        char* equal_pos = strchr(tmp, '=');
        if (equal_pos == NULL)
            continue;
        
        // 判断是否是key-value，如果不满足条件，则跳过
        *equal_pos = '\0';
        if (strlen(trim(tmp)) == 0 || strlen(trim(equal_pos+1)) == 0)
            continue;
        
        // 将key-value添加到section中
        if (iniGetKvp(section_ptr, trim(tmp)) == NULL) {
            // 添加key-value对
            iniAddKey(section_ptr, trim(tmp), trim(equal_pos+1));
        } else {
            // 更新key-value对
            iniSetValue(section_ptr, trim(tmp), trim(equal_pos+1));
        }

        // 清空缓存区
        memset(buf, 0, sizeof(buf));
    }
    return p_stat;
}

/**
 * @brief 释放ini内存
 * 
 * @param ini_ptr 
 * @return iniStat 
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
 * @brief 获取指定section
 * 
 * @param ini_ptr 
 * @param section_name 
 * @return section* 
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
 * @brief 获取指定key对应的value
 * 
 * @param section_ptr 
 * @param key 
 * @return char* 
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
 * @brief 获取指定key对应的kvp
 * 
 * @param section_ptr 
 * @param key 
 * @return kvp* 
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
 * @brief 将ini_ptr中section_name的section名设置为target_name。如果
 *        出现相同的section_name则只修改最后一个同section_name的section
 *        的name。若段不存在则返回INI_ERR_SECTION_NOT_FOUND
 * 
 * @param ini_ptr 
 * @param section_name 
 * @param target_name 
 * @return iniStat 
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
 * @brief 设置指定key对应的value
 * 
 * @param section_ptr 
 * @param key 
 * @param value 
 * @return iniStat 
 */
iniStat iniSetValue(section* section_ptr ,char* key ,char* value) {
    // 检查section_ptr是否为空指针
    if (section_ptr == NULL)
        return INI_ERR_SECTION_NOT_FOUND;

    // 遍历section中的key-value对
    for (int i = section_ptr->kvp_num-1; i >= 0; i--) {
        if (strcmp(section_ptr->kvps[i]->key, key) == 0) {
            // 更新value
            free(section_ptr->kvps[i]->value);
            section_ptr->kvps[i]->value = strdup(value);
            return INI_OK;
        }
    }
}

/**
 * @brief 将section添加到ini中
 * 
 * @param ini_ptr 
 * @param section_ptr 
 * @return iniStat 
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
}

/**
 * @brief 将key-value对添加到section中
 * 
 * @param section_ptr 
 * @param key 
 * @param value 
 * @return iniStat 
 */
iniStat iniAddKey(section* section_ptr ,char* key ,char* value) {
    // 检查section_ptr是否为空指针
    if (section_ptr == NULL) 
        return INI_ERR_SECTION_NOT_FOUND;
    
    // 添加key-value对
    section_ptr->kvp_num++;
    section_ptr->kvps = (kvp**)realloc(section_ptr->kvps, sizeof(kvp*) * section_ptr->kvp_num);
    section_ptr->kvps[section_ptr->kvp_num - 1] = (kvp*)malloc(sizeof(kvp));
    section_ptr->kvps[section_ptr->kvp_num - 1]->key = strdup(key);
    section_ptr->kvps[section_ptr->kvp_num - 1]->value = strdup(value);
}

/**
 * @brief 删除ini中的section
 * 
 * @param ini_ptr 指向INI结构体的指针
 * @param section_ptr 指向SECTION结构体的指针，即要删除的section
 * @return iniStat 返回操作的状态
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
 * @brief 删除section中的key-value对
 * 
 * @param section_ptr 指向SECTION结构体的指针
 * @param key 要删除的key值
 * @return iniStat 返回操作的状态
 */
iniStat iniDelKey(section* section_ptr, char* key) {
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
 * @param ini_ptr 
 * @param file_path 
 * @return iniStat 
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
 * @brief 以字符串形式返回ini_ptr中的ini数据
 * 
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

/**
 * @brief 去除字符串前后的空格
 * 
 * @param str 
 * @return char* 
 */
char* trim(char *str) {
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