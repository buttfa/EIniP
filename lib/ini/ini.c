#include "ini.h"

/**
 * @brief 读取stream流，并解析成ini结构体
 * 
 * @param stream 
 * @param ini_ptr 
 * @return iniParseStat 
 */
iniParseStat iniParse(FILE* stream, ini* ini_ptr) {
    // 创建iniParseStat结构体
    iniParseStat p_stat = {0};

    // 检查stream是否为空
    if (stream != NULL) {
        p_stat.stat = INI_ERR_STREAM_NOT_FOUND;
        return p_stat;
    }

    // 如果ini_ptr为空指针，则分配内存
    if (ini_ptr == NULL) {
        ini_ptr = (ini*)malloc(sizeof(ini));
    }

    char buf[1024];              // 缓存区
    section *section_ptr = NULL; // section指针
    int row = 0;                 // 行数
    while (fgets(buf, sizeof(buf), stream) != NULL) {
        row++;
        // 跳过注释行
        if (buf[0] == '#' || buf[0] == ';') {
            continue;
        }

        // 判断是否是section
        if (buf[0] == '[' && buf[strlen(buf) - 2] == ']') {
            // 提取section名称
            buf[strlen(buf) - 2] = '\0';
            
            // 如果section已存在，则使section_ptr指向对应的section
            if ((section_ptr=iniGetSection(ini_ptr, buf+1)) != NULL)
                continue;

            
            // 否则，分配新section内存
            section_ptr = (section*)malloc(sizeof(section));
            memset(section_ptr, 0, sizeof(section));
            section_ptr->name = strdup(buf + 1);
            // 将section添加到ini中
            iniAddSection(ini_ptr, section_ptr);
            ini_ptr->section_num++;
        }

        // 判断是否是key-value
        int num = 0;
        char** tokens = splitStringBySpace(buf, &num);
        if (num >= 3 && strcmp(tokens[1],"=")==0) {
            // 找到=后的第一个字符的地址
            int index = 0;
            while (buf[index] != '=') index++;
            while (buf[index] != ' ') index++;

            // 将key-value添加到section中
            iniAddKey(section_ptr, strdup(tokens[0]), strdup(buf + index));
            section_ptr->kvp_num++;
        }
        freeSplitResult(tokens);

        // 清空缓存区
        memset(buf, 0, sizeof(buf));
    }
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