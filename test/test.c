#include "ini.h"

#include <stdio.h>
#include "ini.h"
int main(){
    // 初始化ini
    char* str = "[section1]\nkey1=value1\nkey2=value2\n[section2]\nkey3=value3\nkey4=value4\n[section3]\nkey5=value5\nkey6=value6\n[section4]\nkey7=value7\nkey8=value8\n[section5]";
    ini *ini_ptr = iniParseStr(str);
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
}