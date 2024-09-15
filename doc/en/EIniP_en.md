### <div align="center">|[简体中文](../cn/EIniP.md)|[English](EIniP_en.md)|</div>

## <div align="center">Directory</div>
- [EIniP library usage](#einip-library-usage)
- [Function resolution](#function-resolution)
- [IniStat enumeration type](#inistat-enumeration-type)
- [Ini related structures](#ini-related-structures)

## EIniP library usage
#### 1. EIniP Quick Use Example
```c
#include "ini.h"
int main(){
    // Initialize ini
    ini* ini_ptr = iniParseFile("test.ini");

    // Retrieve the specified section from ini
    section* section_ptr = iniGetSection(ini_ptr,"section1");

    // Retrieve the specified key from the section
    char* value = iniGetValue(section_ptr,"key1");

    // Free ini
    iniFree(ini_ptr);
    return 0;
}
```
#### 2. Traverse ini
#### You can use the method provided here to traverse the data in the ini and perform the required operations, or you can use the functions provided by the einip library (please refer to [Function resolution](#function-resolution) for details)
```c
#include <stdio.h>
#include "ini.h"
int main(){
    // Initialize ini
    ini *ini_ptr = iniParseFile("test.ini");
    if (ini_ptr == NULL){
        printf("iniParseFile error\n");
        exit(1);
    }

    // Traverse the sections in ini
    printf("section_num: %d\n\n",ini_ptr->section_num);
    for(int i = 0; i < ini_ptr->section_num; i++){
        section* section_ptr = ini_ptr->sections[i];
        printf("section_name: %s, kvp_num: %d\n",section_ptr->name, section_ptr->kvp_num);

        // Traverse the kvps in the section
        for(int j = 0; j < section_ptr->kvp_num; j++){
            kvp* kvp_ptr = section_ptr->kvps[j];
            printf("key: %s, value: %s\n",kvp_ptr->key, kvp_ptr->value);
        }

        printf("\n");
    }

    // Free ini
    iniFree(ini_ptr);
    return 0;
}
```

## Function resolution
#### 1、 Retrieve and release operation related functions
|Function Declaration|Function role|Note|
|:-|:-|:-|
|ini* iniParseFile(char* file_path)|Parse the file and return the parsing result. During parsing, sections are merged and key value pairs are overwritten phonily.||
|ini* iniParseStr(char* str)|Parse the string and return the parsing result. During parsing, sections are merged and key value pairs are overwritten phonily.||
|iniParseStat* iniParse(FILE* stream, ini* ini_ptr)|Parse the stream and save the parsing result to ini_ptr. During parsing, sections are merged and key value pairs are overwritten phonily.|The returned iniParseStat provides a detailed explanation of warning and error situations|
|iniStat iniFree(ini* ini_ptr)| Release the memory pointed to by ini_ptr||
|iniStat iniDestroy(ini** ini_ptr)|Release ini memory and set ini_ptr to NULL||
|iniStat iniFreeStat(iniParseStat* p_stat)|Release inParseStat memory||
|iniStat iniDestroyStat(iniParseStat** p_stat)|Release iniParseStat memory and set p_stat to NULL||

#### 2、 Get operation related functions
|Function Declaration|Function role|Note|
|:-|:-|:-|
|section* iniGetSection(ini* ini_ptr ,char* section_name)|Returns the section with the specified section_name in ini_ptr. If ini_ptr is illegal or the section does not exist, returns NULL. If the same section_name appears, return the last section with the same section_name||
|char* iniGetValue(section* section_ptr ,char* key)| Return the address of the value of the specified key in section_ptr. If it does not exist, return NULL. If the same key name appears, return the last value with the same key name||
|kvp* iniGetKvp(section* section_ptr ,char* key)| Return the address of the kvp with the specified key in section_ptr. If it does not exist, return NULL. If the same key name appears, return the last kvp with the same key name||

#### 3、 Set operation related functions
|Function Declaration|Function role|Note|
|:-|:-|:-|
|iniStat iniSetSection(ini* ini_ptr ,char* section_name, char* target_name)|Set the section name of section_name in ini_ptr to target_name. If the same section_name appears, only modify the name of the last section with the same section_name. If the section does not exist, return INI_ERR_SECTION_NOT_FOUND||
|iniStat iniSetValue(section* section_ptr ,char* key ,char* value)|Set the value of the specified key in section_ptr to value. If the same key name appears, only modify the value of the last kvp with the same key name. If the section does not exist, return INI_ERR_SECTION_NOT_FOUND. If the key value pair does not exist, return INI_ERR_KEY_NOT_FOUND||

#### 4、 Add operation related functions
|Function Declaration|Function role|Note|
|:-|:-|:-|
|iniStat iniAddSection(ini* ini_ptr , char* section_name)|Add a section to ini_ptr and append it directly to the end of the sections. If the same section_name appears, obtain the last section with the same section_name when use the iniGetSection function, achieving a similar overlay effect||
|iniStat iniAddKvp(section* section_ptr ,char* key ,char* value)|Add a key value pair in section_ptr, and the new kvp will be added at the end of kvps. If the same kvp appears, obtain the last kvp with the same name when use the iniGetKvp and iniGetValue functions, achieving a similar coverage effect||

#### 5、 Delete operation related functions
|Function Declaration|Function role|Note|
|:-|:-|:-|
|iniStat iniDelSection(ini* ini_ptr , char* section_name)|Delete the section with the specified section_name in ini_ptr. If it does not exist, return INI_ERR_SECTION_NOT_FOUND. If there are sections with the same section_name, only the last section with the same name will be deleted||
|iniStat iniDelKvp(section* section_ptr ,char* key)|Delete the key value pair for the specified key in section_ptr. If it does not exist, return INI_ERR_KEY_NOT_FOUND. If the same key name appears, only the last kvp with the same key name will be deleted||

#### 6、 Save operation related functions
|Function Declaration|Function role|Note|
|:-|:-|:-|
|iniStat iniSaveFile(ini* ini_ptr ,char* file_path)|Save the ini data in ini_ptr to the file_path file||
|char* iniSaveStr(ini* ini_ptr)|Returns the ini data in ini_ptr as a string, and returns NULL if the operation fails|The returned string is dynamically allocated, please release it promptly after use|

## IniStat enumeration type
#### Function: Used to indicate the return value of most einip functions and to indicate the execution status of einip functions
|Member Name|Meaning|Value|
|:-|:-|:-|
|INI_OK|The ini function operation was successful|0x00 / 0000_0000|
|INI_WARN|Ini function operation warning|0x40 / 0100_0000|
|INI_WARN_KVP_NOT_BELONG_SECTION|When parsing, there is a kvp that does not belong to any section|
|INI_WARN_SECTION_EXIST_SPACE|There are spaces in section_name|
|INI_WARN_KEY_EXIST_SPACE|There are spaces in the key|
|INI_WARN_VALUE_EXIST_SPACE|There are spaces in the value|
|INI_ERR|Ini function operation error|0x80 / 1000_0000|
|INI_ERR_STAT_NOT_FOUND|IniStat is an illegal or invalid value||
|INI_ERR_UNKNOWN_LINE|Unknown string appears|
|INI_ERR_KEY_IS_EMPTY|The key in kvp is empty|
|INI_ERR_VALUE_IS_EMPTY|The value in kvp is empty|
|INI_ERR_STR_NULL|The string is NULL|
|INI_ERR_STREAM_NOT_FOUND|Stream does not exist|
|INI_ERR_INI_NOT_FOUND|Ini does not exist|
|INI_ERR_SECTION_NOT_FOUND|Section does not exist|
|INI_ERR_KEY_NOT_FOUND|Key does not exist|
|INI_ERR_FILE_OPEN|file open failed|

## Ini related structures
```c
// The following structure stores key value pairs, sections, and ini in sequence
/**
 * @brief Key value pair
 * 
 */
typedef struct KVP{
    char* key;
    char* value;
}kvp;

/**
 * @brief Section of ini file
 * 
 */
typedef struct SECTION{
    char* name;
    int kvp_num;
    struct KVP** kvps;
}section;

/**
 * @brief  Structure for storing ini file content
 * 
 */
typedef struct INI{ 
    int section_num;
    struct SECTION** sections;
}ini;
```