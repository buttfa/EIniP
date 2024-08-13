### <div align="center">|[简体中文](EIniP.md)|[English](../en/EIniP_en.md)|</div>

## <div align="center">目录</div> 
- [EIniP库使用](#einip库使用)
- [函数解析](#函数解析)
- [iniStat枚举类型](#inistat枚举类型)
- [ini相关结构体](#ini相关结构体)

## EIniP库使用
#### 1. EIniP快速使用示例
```c
#include "ini.h"
int main(){
    // 初始化ini
    ini* ini_ptr = iniParseFile("test.ini");

    // 获取ini中的指定section
    section* section_ptr = iniGetSection(ini_ptr,"section1");

    // 获取section中的指定key
    char* value = iniGetValue(section_ptr,"key1");

    // 释放ini
    iniFree(ini_ptr);
    return 0;
}
```
#### 2. 遍历ini
#### 可以使用此处给出的方法遍历ini中的数据并进行所需操作，也可以使用einip库提供的函数（详情请参考[函数解析](#函数解析)）。
```c
#include <stdio.h>
#include "ini.h"
int main(){
    // 初始化ini
    ini *ini_ptr = iniParseFile("test.ini");
    if (ini_ptr == NULL){
        printf("iniParseFile error\n");
        exit(1);
    }

    // 遍历ini中的section
    printf("section_num: %d\n\n",ini_ptr->section_num);
    for(int i = 0; i < ini_ptr->section_num; i++){
        section* section_ptr = ini_ptr->sections[i];
        printf("section_name: %s, kvp_num: %d\n",section_ptr->name, section_ptr->kvp_num);

        // 遍历section中的kvps
        for(int j = 0; j < section_ptr->kvp_num; j++){
            kvp* kvp_ptr = section_ptr->kvps[j];
            printf("key: %s, value: %s\n",kvp_ptr->key, kvp_ptr->value);
        }

        printf("\n");
    }

    // 释放ini
    iniFree(ini_ptr);
    return 0;
}
```

## 函数解析
#### 一、获取和释放操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|ini* iniParseFile(char* file_path)|解析file_path文件，并返回解析结果。解析时，section真合并、键值对假覆盖。||
|ini* iniParseStr(char* str)|解析str字符串，并返回解析结果。解析时，section真合并、键值对假覆盖。||
|iniParseStat iniParse(FILE* stream, ini* ini_ptr)|解析stream流，并将解析结果保存至ini_ptr中。解析时，section真合并、键值对假覆盖。|返回的iniParseStat详细说明了警告和错误情况|
|iniStat iniFree(ini* ini_ptr)|释放ini_ptr指向的内存||

#### 二、获取操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|section* iniGetSection(ini* ini_ptr ,char* section_name)|返回ini_ptr中的指定section_name的section，若ini_ptr非法或段不存在则返回NULL。如果出现相同的section_name则返回最后一个同section_name的section||
|char* iniGetValue(section* section_ptr ,char* key)|返回section_ptr中指定key的值的地址，若不存在则返回NULL。如果出现相同的键名则返回最后一个同键名的value||
|kvp* iniGetKvp(section* section_ptr ,char* key)|返回section_ptr中指定key的kvp的地址，若不存在则返回NULL。如果出现相同的键名则返回最后一个同键名的kvp||

#### 三、设置操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniSetSection(ini* ini_ptr ,char* section_name, char* target_name)|将ini_ptr中section_name的section名设置为target_name。如果出现相同的section_name则只修改最后一个同section_name的section的name。若段不存在则返回INI_ERR_SECTION_NOT_FOUND||
|iniStat iniSetValue(section* section_ptr ,char* key ,char* value)|将section_ptr中指定key的值设置为value。如果出现相同的键名只修改最后一个同键名的kvp的value。若段不存在则返回INI_ERR_SECTION_NOT_FOUND，若键值对不存在则返回INI_ERR_KEY_NOT_FOUND||

#### 四、添加操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniAddSection(ini* ini_ptr , char* section_name)|在ini_ptr中添加section，直接追加到sections尾部，如果出现相同的section_name则使用iniGetSection函数时获取最后一个同section_name的section，实现类似覆盖的效果。||
|iniStat iniAddKvp(section* section_ptr ,char* key ,char* value)|在section_ptr中添加key-value pair，新的kvp会被添加在kvps尾部。如果出现相同的kvp则使用iniGetKvp和iniGetValue函数时获取最后一个同名的kvp，实现类似覆盖的效果。||

#### 五、删除操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniDelSection(ini* ini_ptr , char* section_name)|删除ini_ptr中指定section_name的section，若不存在则返回INI_ERR_SECTION_NOT_FOUND。如果出现相同section_name的section，则只删除最后一个同名的section。||、
|iniStat iniDelKvp(section* section_ptr ,char* key)|删除section_ptr中指定key的key-value pair，若不存在则返回INI_ERR_KEY_NOT_FOUND。如果出现相同的键名，则只删除最后一个同键名的kvp。||

#### 六、保存操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniSaveFile(ini* ini_ptr ,char* file_path)|将ini_ptr中的ini数据保存至file_path文件中||
|char* iniSaveStr(ini* ini_ptr)|以字符串形式返回ini_ptr中的ini数据，操作失败则返回NULL。|返回的字符串为动态分配所得，请在使用完后及时释放。|

## iniStat枚举类型
#### 作用：用于指示大部分einip函数的返回值，用于指示einip函数执行状态
|成员名称|含义|值|
|:-|:-|:-|
|INI_OK|ini函数操作成功|0x00 / 0000_0000|
|INI_WARN|ini函数操作警告|0x40 / 0100_0000|
|INI_WARN_KVP_NOT_BELONG_SECTION|解析时，出现不署于任何一个section的kvp|
|INI_WARN_SECTION_EXIST_SPACE|section_name中存在空格|
|INI_WARN_KEY_EXIST_SPACE|key中存在空格|
|INI_WARN_VALUE_EXIST_SPACE|value中存在空格|
|INI_ERR|ini函数操作错误|0x80 / 1000_0000|
|INI_ERR_UNKNOWN_LINE|出现未知字符串|
|INI_ERR_KEY_IS_EMPTY|kvp中的key为空|
|INI_ERR_VALUE_IS_EMPTY|kvp中的value为空|
|INI_ERR_STR_NULL|str为空指针|
|INI_ERR_STREAM_NOT_FOUND|流不存在|
|INI_ERR_INI_NOT_FOUND|ini不存在|
|INI_ERR_SECTION_NOT_FOUND|section不存在|
|INI_ERR_KEY_NOT_FOUND|key不存在|
|INI_ERR_FILE_OPEN|文件打开失败|

## ini相关结构体
```c
// 以下结构体，依次存储key-value对、section、ini
typedef struct KVP{
    char* key;
    char* value;
}kvp;

typedef struct SECTION{
    char* name;
    int kvp_num;
    struct KVP* kvps[];
}section;

typedef struct INI{ 
    int section_num;
    struct SECTION* sections[];
}ini;
```