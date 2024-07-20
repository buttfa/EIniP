#include "gtest/gtest.h"
#include "ini.h"
#include "check.h"

TEST(testParse, test0) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", (char*)"key1", (char*)"value1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);
    
    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testParse, test1) {
    // 初始化测试用例
    char* str = (char*)"  [section1]   \nkey1 = value1  \n key2= value2\n";
    char* expections[] = {(char*)"section1", (char*)"key1", (char*)"value1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);
    
    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testParse, test2) {
    // 初始化测试用例
    char* str = (char*)"key1 = value1\n  [section1]   \n  \n key2= value2\n";
    char* expections[] = {(char*)"section1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testParse, test3_the_same_section_name) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1 = value1\n  [section1]   \n  \n key2= value2\n";
    char* expections[] = {(char*)"section1",(char*)"key1",(char*)"value1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testParse, test4_the_same_key_name) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1 = value1\n    \n  \n key1= Change\n";
    char* expections[] = {(char*)"section1",(char*)"key1",(char*)"Change"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testParse, test5_the_same_key_and_section_name) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1 = value1\n key2=value2\n [section1]  \n  \n key2= Change\n";
    char* expections[] = {(char*)"section1",(char*)"key1",(char*)"value1",(char*)"key2",(char*)"Change"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testParse, test6) {
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
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testParse, empty_str) {
    // 初始化测试用例
    char* str = (char*)"    \n   \n   \n   \n";
    char** expections = NULL;
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    checkIni(ini_t, expections, 0);
    iniFree(ini_t);
}

TEST(testParse, empty_ini) {
    // 初始化测试用例
    char* str = (char*)" \n key2= value2\n   \n  key1 = value1\n \n   \n   \n";
    char** expections = NULL;
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    checkIni(ini_t, expections, 0);
    iniFree(ini_t);
}

TEST(testParse, note) {
    // 初始化测试用例
    char* str = (char*)"# This is note\n key1 = value1\n  [section1]   \n  \n key2= value2\n ; This is also note.\n";
    char* expections[] = {(char*)"section1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}