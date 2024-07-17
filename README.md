## 项目名称
#### INI / Easy initialization file parser

## 项目介绍
#### 轻量级、无依赖（指无标准库以外的依赖）Initialization file（INI文件）解析器，适用于C/C++。

## 使用方法
#### （一）将lib文件夹放至工程根目录，将其中的ini文件夹添加到工程（由于不同项目工程其添加库的方式不同，此处自行处理）
#### （二）在工程中添加头文件#include "ini.h"
#### （三）调用相关函数即可，详情参见下述函数说明

## 重要函数解析
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniParseStat iniParse(FILE* stream, ini* ini_ptr)|解析stream流，并将解析结果保存至ini_ptr中||
|iniStat iniFree(ini* ini_ptr)|释放ini_ptr指向的内存||
|iniStat iniFreeSection(section* section_ptr)|释放ini_ptr指向的ini中section_name的section的内存||
 
## 对ini的遍历方法
#### 可以使用此处给出的方法遍历ini中的数据并进行所需操作，也可以使用einip库提供的函数。
```c
#include "ini.h"
int main(){
    // 初始化ini
    ini* ini = NULL;
    if (iniParseFile("test.ini", &ini) != INI_OK){
        printf("iniParseFile error\n");
        exit(0);
    }
    
    
    // 遍历ini的段
    for(int i = 0; i < ini->section_num; i++){
        printf("section name:%s\n",ini->sections[i]->name);
        
        // 遍历ini的键值对
        for(int j = 0; j < ini->sections[i]->kvp_num; j++){
            printf("key:%s value:%s\n",ini->sections[i]->kvps[j]->key,ini->sections[i]->kvps[j]->value);
        }
        printf("\n");
    }
}
```

## iniStat 枚举类型
#### 作用：用于指示大部分einip函数的返回值，用于指示einip函数执行状态
|成员名称|含义|值|
|:-|:-|:-|
|INI_OK|ini函数操作成功|0x00 / 0000_0000|
|INI_WARN|ini函数操作警告|0x40 / 0100_0000|
|INI_WARN_VALUE_WITH_SPACE|键值对中的值存在空格|
|INI_WARN_VALUE_IS_EMPTY|键值对中的值为空|
|INI_ERR|ini函数操作错误|0x80 / 1000_0000|
|INI_ERR_STREAM_NOT_FOUND|流不存在|
|INI_ERR_FILE_NOT_FOUND|文件不存在|
|INI_ERR_FILE_OPEN|文件打开失败|
|INI_ERR_FILE_READ|文件读取失败|
|INI_ERR_FILE_WRITE|文件写入失败|
|INI_ERR_MEM_ALLOC|内存分配失败|
|INI_ERR_MEM_FREE|内存释放失败|
|INI_ERR_INI_NOT_FOUND|ini不存在|
|INI_ERR_SECTION_NOT_FOUND|section不存在|
|INI_ERR_KEY_NOT_FOUND|key不存在|

## 结构体及枚举类型解析
```c
// 绝大多数einip函数的返回值，用于指示einip函数执行状态
typedef enum INISTAT{
    INI_OK = 0x00, // 0000_0000
    ....
    INI_WARN = 0x40, // 0100_0000
    ....
    INI_ERR = 0x80, // 1000_0000
    ....
}iniStat;



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

## 额外函数解析
#### 一、获取键值对操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|section* iniGetSection(ini* ini_ptr ,char* section_name)|返回ini_ptr中的指定section_name的section，若ini_ptr非法或段不存在则返回NULL||
|char* iniGetValue(section* section_ptr ,char* key)|返回section_ptr中指定key的值，若不存在则返回NULL||
<!--|char* iniGetValue(ini* ini_ptr ,char* section_name ,char* key)|返回ini_ptr中指定section_name的指定key的值，若不存在则返回NULL||-->


#### 二、设置键值对操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniSetValue(section* section_ptr ,char* key ,char* value)|将section_ptr中指定key的值设置为value，若段不存在则返回INI_ERR_SECTION_NOT_FOUND，若键值对不存在则返回INI_ERR_KEY_NOT_FOUND||
<!--|iniStat iniSetValue(ini* ini_ptr ,char* section_name ,char* key ,char* value)|将ini_ptr中指定section_name的指定key的值设置为value，若段不存在则返回INI_ERR_SECTION_NOT_FOUND，若键值对不存在则返回INI_ERR_KEY_NOT_FOUND||-->



#### 二、添加操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniCreateSection(ini* ini_ptr ,char* section_name)|在ini_ptr中创建section_name的section，若已存在则返回INI_ERR_SECTION_EXIST||
|iniStat iniAddSection(ini* ini_ptr , section* section_ptr)|在ini_ptr中添加section_ptr，若已存在则返回INI_ERR_SECTION_EXIST||
|iniStat iniAddKey(section* section_ptr ,char* key ,char* value)|在section_ptr中添加key-value pair，若已存在则修改value||
<!--|iniStat iniAddKey(ini* ini_ptr ,char* section_name ,char* key ,char* value)|在ini_ptr中添加section_name的section，并添加key-value pair，若已存在则修改value||-->
<!--|initStat iniCreate(ini* ini_ptr)|创建ini_ptr指向的ini结构体，并初始化其成员||-->

#### 三、删除操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniDelSection(ini* ini_ptr ,section* section_ptr)|删除ini_ptr中指定section_name的section，若不存在则返回INI_ERR_SECTION_NOT_FOUND||、
|iniStat iniDelKey(section* section_ptr ,char* key)|删除section_ptr中指定key的key-value pair，若不存在则返回INI_ERR_KEY_NOT_FOUND||
<!--|iniStat iniDelKey(ini* ini_ptr ,char* section_name ,char* key)|删除ini_ptr中指定section_name的section中的key-value pair，若不存在则返回INI_ERR_KEY_NOT_FOUND||-->


#### 四、保存操作相关函数
|函数声明|函数作用|注意|
|:-|:-|:-|
|iniStat iniSaveIni(ini* ini_ptr ,char* file_path)|将ini_ptr中的ini数据保存至file_path文件中||
|iniStat iniSaveStr(ini* ini_ptr ,char* str)|将ini_ptr中的ini数据保存至str字符串中||