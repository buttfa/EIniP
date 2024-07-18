#include "gtest/gtest.h"
#include "ini.h"
#include <iostream>
using namespace std;

/**
 * @brief iniParse 测试函数
 * 
 * @param p_stat 
 * @param ini_t 
 * @param expections 
 */
void testIniParse(iniParseStat &p_stat, ini* ini_t, char**expections);

TEST(testIniParse, test0) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", (char*)"key1", (char*)"value1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);
    
    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, test1) {
    // 初始化测试用例
    char* str = (char*)"  [section1]   \nkey1 = value1  \n key2= value2\n";
    char* expections[] = {(char*)"section1", (char*)"key1", (char*)"value1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);
    
    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, test2) {
    // 初始化测试用例
    char* str = (char*)"key1 = value1\n  [section1]   \n  \n key2= value2\n";
    char* expections[] = {(char*)"section1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, test3_the_same_section_name) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1 = value1\n  [section1]   \n  \n key2= value2\n";
    char* expections[] = {(char*)"section1",(char*)"key1",(char*)"value1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, test4_the_same_key_name) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1 = value1\n    \n  \n key1= Change\n";
    char* expections[] = {(char*)"section1",(char*)"key1",(char*)"Change"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, test5_the_same_key_and_section_name) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1 = value1\n key2=value2\n [section1]  \n  \n key2= Change\n";
    char* expections[] = {(char*)"section1",(char*)"key1",(char*)"value1",(char*)"key2",(char*)"Change"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, test6) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1 = value1\n key2=value2\n[another_section]\n pp_ke=qq_va \n[section1]  \n  \n key2= Change\n";
    char* expections[] = {(char*)"section1",
                        (char*)"key1",(char*)"value1",
                        (char*)"key2",(char*)"Change",
                        
                        (char*)"another_section",
                        (char*)"pp_ke",(char*)"qq_va"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, empty_str) {
    // 初始化测试用例
    char* str = (char*)"    \n   \n   \n   \n";
    char** expections = NULL;
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, empty_ini) {
    // 初始化测试用例
    char* str = (char*)" \n key2= value2\n   \n  key1 = value1\n \n   \n   \n";
    char** expections = NULL;
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

TEST(testIniParse, note) {
    // 初始化测试用例
    char* str = (char*)"# This is note\n key1 = value1\n  [section1]   \n  \n key2= value2\n ; This is also note.\n";
    char* expections[] = {(char*)"section1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    testIniParse(p_stat, ini_t, expections);
    iniFree(ini_t);
}

/**
 * @brief iniParse 测试函数
 * 
 * @param p_stat 
 * @param ini_t 
 * @param expections 
 */
void testIniParse(iniParseStat &p_stat, ini* ini_t, char**expections)
{
    // 判断是否解析成功
    EXPECT_EQ(p_stat.stat == INI_OK, true);

    // 比较结果
    int index = 0;
    cout << "section num: " << ini_t->section_num << endl;
    for (int i = 0; i < ini_t->section_num; i++)
    {
        // 比较section
        section *section_ptr = ini_t->sections[i];
        cout << "section name: " << section_ptr->name << ", kvp num:" << section_ptr->kvp_num << endl;
        EXPECT_STREQ(section_ptr->name, expections[index++]);
        for (int j = 0; j < section_ptr->kvp_num; j++)
        {
            // 比较key-value
            kvp *kvp_ptr = section_ptr->kvps[j];
            cout << "key: " << kvp_ptr->key << ", value: " << kvp_ptr->value << endl;
            EXPECT_STREQ(kvp_ptr->key, expections[index++]);
            EXPECT_STREQ(kvp_ptr->value, expections[index++]);
        }
    }
}