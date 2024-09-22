#include "gtest/gtest.h"
#include "ini.h"
#include "check.h"
#include <iostream>
using namespace std;


TEST(testDel, test0) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", (char*)"key1", (char*)"value1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat* p_stat = iniParse(stream, (ini**)&ini_t);
    iniDestroyStat((iniParseStat**)&p_stat);
    // 删除
    iniDelSection(ini_t, (char*)"section1");


    // 检测结果
    checkIni(ini_t, expections, 0);
    iniFree(ini_t);
}

TEST(testDel, test1) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat* p_stat = iniParse(stream, (ini**)&ini_t);
    iniDestroyStat((iniParseStat**)&p_stat);
    // 删除
    iniDelKvp(iniGetSection(ini_t, (char*)"section1"), (char*)"key1");

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testDel, test2) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", };
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat* p_stat = iniParse(stream, (ini**)&ini_t);
    iniDestroyStat((iniParseStat**)&p_stat);
    // 删除
    iniDelKvp(iniGetSection(ini_t, (char*)"section1"), (char*)"key1");
    iniDelKvp(iniGetSection(ini_t, (char*)"section1"), (char*)"key2");

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testDel, test3) {
    // 初始化测试用例
    char* str = (char*)"[section1]\n      \
                        key1=value1\n     \
                        key2=value2\n     \
                        [section2]\n      \
                        test1= thtest\n   \
                        [section3]\n      \
                        sec3 = 3\n        \
                        [section1]\n      \
                        key2 = Change";
    char* expections[] = {(char*)"section1",
                     (char*)"key2", (char*)"value2",
                     (char*)"key2", (char*)"Change", 
                     
                     (char*)"section2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat* p_stat = iniParse(stream, (ini**)&ini_t);
    iniDestroyStat((iniParseStat**)&p_stat);
    // 删除
    iniDelKvp(iniGetSection(ini_t, (char*)"section1"), (char*)"key1");
    iniDelKvp(iniGetSection(ini_t, (char*)"section2"), (char*)"test1");
    iniDelSection(ini_t, (char*)"section3");
    


    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testDel, test4) {
    // 初始化测试用例
    char* str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", (char*)"key2", (char*)"value2"};
    
    // 运行结果
    FILE* stream = fmemopen(str, strlen(str), "r");
    ini* ini_t = NULL;
    iniParseStat* p_stat = iniParse(stream, (ini**)&ini_t);
    iniDestroyStat((iniParseStat**)&p_stat);
    // 删除
    EXPECT_EQ(iniDelKvp(iniGetSection(ini_t, (char*)"section1"), (char*)"key1"),INI_OK);
    EXPECT_EQ(iniDelKvp(iniGetSection(ini_t, (char*)"section1"), (char*)"key3"), INI_ERR_KEY_NOT_FOUND);
    EXPECT_EQ(iniDelSection(ini_t, (char*)"section_null"), INI_ERR_SECTION_NOT_FOUND);

    // 检测结果
    checkIni(ini_t, expections, sizeof(expections)/sizeof(*expections));
    iniFree(ini_t);
}

TEST(testDel, test5) {
    // 尝试从不存在的ini中进行删除操作
    EXPECT_EQ(iniDelSection(NULL, (char*)"section1"), INI_ERR_INI_NOT_FOUND);
    EXPECT_EQ(iniDelKvp(NULL, (char*)"key1"), INI_ERR_SECTION_NOT_FOUND);
}