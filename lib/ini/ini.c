#include "ini.h"

/**
 * @brief 打印ini结构体
 * 
 * @param ini_ptr 
 */
void printfIni(ini* ini_ptr) {
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
        // 跳过注释行
        if (tmp[0] == '#' || tmp[0] == ';') {
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
 * @brief 提取input_str中的子字符串，以空格为分隔符
 * 
 * @param input_str 
 * @param out_num_tokens 
 * @return char** 
 */
char** splitStringBySpace(char* input_str, int* out_num_tokens) {
    if (input_str != NULL) {
        int count = 0;
        char* p = input_str;
        
        // 跳过开头的空格
        while (isspace(*p)) {
            ++p;
        }
        
        // 计算非空格分隔的单词数量
        while (*p != '\0') {
            if (!isspace(*p)) {
                ++count;
                while (*p != '\0' && !isspace(*p)) {
                    ++p;
                }
            } else {
                ++p; // 跳过空格
            }
        }

        // 分配足够的空间来存储指针数组和每个子字符串
        char** tokens = (char**)malloc((count + 1) * sizeof(char*));
        *out_num_tokens = 0;

        // 回退指针p到第一个单词的开始位置
        p = input_str;
        while (isspace(*p)) {
            ++p;
        }

        for(int i = 0; i < count; ++i) {
            while (isspace(*p)) {
                ++p; // 跳过前导空格
            }
            if (*p == '\0') break; // 如果到达字符串末尾，跳出循环

            // 找到单词的结束位置
            char* token_end = p;
            while (*token_end != '\0' && !isspace(*token_end)) {
                ++token_end;
            }

            // 分配内存并复制单词
            size_t len = token_end - p;
            tokens[(*out_num_tokens)++] = (char*)malloc(len + 1);
            strncpy(tokens[*out_num_tokens - 1], p, len);
            tokens[*out_num_tokens - 1][len] = '\0';

            // 移动到下一个单词的开始位置
            p = token_end;
            while (isspace(*p)) {
                ++p; // 跳过后导空格
            }
        }

        tokens[*out_num_tokens] = NULL; // 以NULL结束指针数组
        return tokens;
    }
    else {
        *out_num_tokens = 0;
        return NULL;
    }
}

/**
 * @brief 释放分割字符串的数组。
 * 
 * @param splitResult 
 */
void freeSplitResult(char **splitResult) {
    if (splitResult != NULL) {
        for (int i = 0; splitResult[i] != NULL; ++i) {
            free(splitResult[i]);
        }
        free(splitResult);
    }
}

/**
 * @brief 去除字符串前后的空格
 * 
 * @param str 
 * @return char* 
 */
char* trim(char *str) {
    if (str == NULL)
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