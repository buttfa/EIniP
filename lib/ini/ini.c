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

            
            // 否则，分配新section内存
            section_ptr = (section*)malloc(sizeof(section));
            memset(section_ptr, 0, sizeof(section));
            section_ptr->name = strdup(tmp + 1);
            // 将section添加到ini中
            iniAddSection(*ini_ptr, section_ptr);

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
iniStat iniAddSection(ini* ini_ptr , section* section_ptr) {
    // 检查ini_ptr是否为空指针
    if (ini_ptr == NULL)
        return INI_ERR_INI_NOT_FOUND;
    
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