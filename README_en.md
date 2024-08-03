### <div align="center">|[简体中文](README.md)|[English](README_en.md)|</div>

## <div align="center">Directory</div>
- [Project Name](#project-name)
- [Project Introduction](#project-introduction)
- [Usage method](#usage-method)
- [Analysis of important functions](#analysis-of-important-functions)
- [Traversal method for ini](#traversal-method-for-ini)
- [IniStat enumeration type](#inistat-enumeration-type)
- [Structure and enumeration type parsing](#structure-and-enumeration-type-parsing)
- [Additional Function Analysis](#additional-function-analysis) 

## Project Name
#### EINIP / Easy initialization file parser

## Project Introduction
#### Lightweight, no dependencies (referring to dependencies outside of standard libraries) Initialization file (INI file) parser, suitable for C/C++.

## Usage method
#### （1） Place the lib folder in the root directory of the project, and add the ini folder to the project (due to different ways of adding libraries in different project projects, please handle it yourself here)
#### （2） Add header file # include "ini. h" to the engineering file
#### （3） Simply call the relevant functions, please refer to the following function instructions for details

## Analysis of important functions
|Function Declaration | Function Function Function | Attention|
|:-|:-|:-|
|Ini * iniParseFile (char * file_math) | Parses the file_math file and returns the parsing result. When parsing, merge sections and overwrite key value pairs||
|Ini * iniParseStr (char * str) | Parses the str string and returns the parsing result. When parsing, merge sections and overwrite key value pairs||
|IniStat iniFree (ini * ini_ptr) | Release the memory pointed to by ini_ptr||
<!--| IniStat iniFreeSection (ini * ini_ptr, char * section_2) | Release the memory of the section with section_2 in the ini pointed to by ini_ptr | | -->
 
## Traversal method for ini
#### You can use the methods provided here to traverse the data in the ini and perform the required operations, or you can use the functions provided by the einip library.
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
}
```

## IniStat enumeration type
#### Function: Used to indicate the return value of most einip functions and to indicate the execution status of einip functions
|Member Name | Meaning | Value|
|:-|:-|:-|
|INI_OK|The ini function operation was successful|0x00 / 0000_0000|
|INI_WARN|Ini function operation warning|0x40 / 0100_0000|
|INI_ERR|INI function operation error|0x80 / 1000_0000|
|INI_ERR_STR_NULL|The string is NULL|
|INI_ERR_STREAM_NOT_FOUND|Stream does not exist|
|INI_ERR_INI_NOT_FOUND|Ini does not exist|
|INI_ERR_SECTION_NOT_FOUND|Section does not exist|
|INI_ERR_KEY_NOT_FOUND|Key does not exist|
|INI_ERR_FILE_OPEN|file open failed|
<!--|INI_WARN_VALUE_WITH_SPACE|There are spaces in the values of key value pairs|-->
<!--|INI_ERR_VALUE_IS_EMPTY|The value in the key value pair is empty|-->
<!--|INI_ERR_FILE_NOT_FOUND|file does not exist|-->
<!--|INI_ERR_FILE_READ|file read failure|-->
<!--|INI_ERR_FILE_WRITE|File write failed|-->
<!--|INI_ERR_MEM_ALLOC|memory allocation failed|-->
<!--|INI_ERR_MEM_FREE|Memory release failed|-->

## Structure and enumeration type parsing
```c
//The return value of the vast majority of einip functions is used to indicate the execution status of the einip function
typedef enum INISTAT{
    INI_OK = 0x00, // 0000_0000
    ....
    INI_WARN = 0x40, // 0100_0000
    ....
    INI_ERR = 0x80, // 1000_0000
    ....
}iniStat;



//The following structure stores key value pairs, sections, and ini in sequence
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

## Additional Function Analysis
#### 1、 Get ini operation related functions
|Function Declaration | Function Function Function | Attention|
|:-|:-|:-|
<!--| IniParseStat iniParse (FILE * stream, ini * ini_ptr) | Parse the stream stream and save the parsing results to ini_ptr. When parsing, merge sections and overwrite key value pairs| The returned iniParseStat provides a detailed explanation of warning and error situations | -->

#### 2、 Get key value pair operation related functions
|Function Declaration | Function Function Function | Attention|
|:-|:-|:-|
|Section * iniGetSection (ini * ini_ptr, char * section_2) | Returns the section with the specified section_2 in ini_ptr. If ini_ptr is illegal or the section does not exist, returns NULL. If the same section_2 appears, return the last section with the same section_2||
|Char * iniGetValue (section * section_ptr, char * key) | Returns the value of the specified key in section_ptr. If it does not exist, returns NULL. If the same key name appears, return the last value with the same key name||
|Kvp * iniGetKvp (section * section_ptr, char * key) | Returns the kvp of the specified key in section_ptr. If it does not exist, returns NULL. If the same key name appears, return the last kvp with the same key name||
<!--| Char * iniGetValue (ini * ini_ptr, char * section_2, char * key) | Returns the value of the specified key for section_2 in ini_ptr. If it does not exist, returns NULL | | -->


#### 3、 Set key value pair operation related functions
|Function Declaration | Function Function Function | Attention|
|:-|:-|:-|
|IniStat initSection (ini * ini_ptr, char * section_2, char * target_2) | Set the section name of section_2 in ini_ptr to target_2. If the same section_2 appears, only modify the name of the last section with the same section_2. If the segment does not exist, return INI_SRR_SELECTION NOT NOT FOUND||
|IniStat iniSetValue (section * section_ptr, char * key, char * value) | Set the value of the specified key in section_ptr to value. If the same key name appears, only modify the value of the last kvp with the same key name. If the segment does not exist, return INI-ERR_SELECTION NOT NOT FOUND. If the key value pair does not exist, return INI-ERR_KEY_SNOT NOT FOUND||
<!--| IniStat iniSetValue (ini * ini_ptr, char * section_name, char * key, char * value) | Set the value of the specified key for section_name in ini_ptr to value. If the section does not exist, return INI_SRR_SECTON-NOT_SFOUND. If the key value pair does not exist, return INI_SRR_KEYNOT_SFOUND | | -->


 
#### 4、 Add operation related functions
|Function Declaration | Function Function Function | Attention|
|:-|:-|:-|
|IniStat iniAddSection (ini * ini_ptr, char * section_2) | Add a section to ini_ptr and append it directly to the end of the section. If the same section_2 appears, use the iniGets section function to obtain the last section with the same section_2, achieving a similar overlay effect||
|IniStat iniAddKey (section * section_ptr, char * key, char * value) | Add a key value pair in section_ptr, and the new kvp will be added at the end of kvps. If the same kvp appears, use the iniGetKvp and iniGetValue functions to obtain the last kvp with the same name, achieving a similar coverage effect||
<!--| IniStat iniAddKey (ini * ini_ptr, char * section_2, char * key, char * value) | Add a section_2 to ini_ptr and add a key value pair. If it already exists, modify the value | | -->
<!--| InitStat iniCreate (ini * ini_ptr) | Create the ini struct pointed to by ini_ptr and initialize its members | | -->
<!--| Section * iniCreateSection (ini * ini_ptr, char * sectionname) | Create an empty section with sectionname in ini_ptr. If it already exists, return INI_SRR_SELECTION EXEIST | | -->
#### 5、 Delete operation related functions
|Function Declaration | Function Function Function | Attention|
|:-|:-|:-|
|IniStat iniDelSection (ini * ini_ptr, char * section_2) | Delete the section with the specified section_2 in ini_ptr. If it does not exist, return INI-ERR_SELECTION. NOT_SFOUND. If there are sections with the same section_2, only the last section with the same name will be deleted
|IniStat iniDelKey (section * section_ptr, char * key) | Delete the key value pair for the specified key in section_ptr. If it does not exist, return INI-ERR_KEYSinoT_FOUND. If the same key name appears, only the last kvp with the same key name will be deleted||
<!--| IniStat iniDelKey (ini * ini_ptr, char * section_2, char * key) | Delete the key value pair in the section specified by section_2 in ini_ptr. If it does not exist, return INI-ERR_KEY∝T_FOUND | | -->
 
#### 6、 Save operation related functions
|Function Declaration | Function Function Function | Attention|
|:-|:-|:-|
|IniStat iniSaveFile (ini * ini_ptr, char * file_dath) | Save the ini data in ini_ptr to the file_dath file||
|Char * iniSaveStr (ini * ini_ptr) | Returns the ini data in ini_ptr as a string, and returns NULL if the operation fails| The returned string is dynamically allocated, please release it promptly after use|