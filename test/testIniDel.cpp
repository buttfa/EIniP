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
    iniParseStat p_stat = iniParse(stream, (ini**)&ini_t);
    iniDelSection(ini_t, iniGetSection(ini_t, (char*)"section1"));


    // 检测结果
    checkIni(p_stat, ini_t, expections, 0);
    iniFree(ini_t);
}