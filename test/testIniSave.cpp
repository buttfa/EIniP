#include "gtest/gtest.h"
#include "ini.h"
#include "check.h"


TEST(testSave, saveFile_test0) {
    // 1. 初始化测试用例
    char* test_str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", 
                            (char*)"key1", (char*)"value1",
                            (char*)"key2", (char*)"value2"};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    
    // 3. 保存和释放测试用ini
    iniSaveFile(test_ini, "test.ini");
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fopen("test.ini", "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}

TEST(testSave, saveFile_test1) {
    // 1. 初始化测试用例
    char* test_str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", 
                            (char*)"key2", (char*)"value2"};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    iniDelKey(iniGetSection(test_ini, "section1"), "key1");
    
    // 3. 保存和释放测试用ini
    iniSaveFile(test_ini, "test.ini");
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fopen("test.ini", "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}

TEST(testSave, saveFile_test2) {
    // 1. 初始化测试用例
    char* test_str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section2"};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    iniDelKey(iniGetSection(test_ini, "section1"), "key1");
    iniDelSection(test_ini, "section1");
    iniAddSection(test_ini, "section2");
    
    // 3. 保存和释放测试用ini
    iniSaveFile(test_ini, "test.ini");
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fopen("test.ini", "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}

TEST(testSave, saveStr_test0) {
    // 1. 初始化测试用例
    char* test_str = (char*)"[section1]\nkey1=value1\nkey2=value2\n[section2]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", 
                            (char*)"key1", (char*)"value1",
                            (char*)"key2", (char*)"value2", 
                        
                        (char*)"section2", 
                            (char*)"key1", (char*)"value1", 
                            (char*)"key2", (char*)"value2"};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    
    // 3. 保存和释放测试用ini
    char* target_str = iniSaveStr(test_ini); 
    ASSERT_EQ(target_str!=NULL, true);
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fmemopen(target_str, strlen(target_str), "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}

TEST(testSave, saveStr_test1) {
    // 1. 初始化测试用例
    char* test_str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", 
                            (char*)"key1", (char*)"value1",
                            (char*)"key2", (char*)"value2"};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    
    // 3. 保存和释放测试用ini
    char* target_str = iniSaveStr(test_ini); 
    ASSERT_EQ(target_str!=NULL, true);
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fmemopen(target_str, strlen(target_str), "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}

TEST(testSave, saveStr_test2) {
    // 1. 初始化测试用例
    char* test_str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", 
                            (char*)"key2", (char*)"value2"};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    iniDelKey(iniGetSection(test_ini, "section1"), "key1");
    
    // 3. 保存和释放测试用ini
    char* target_str = iniSaveStr(test_ini); 
    ASSERT_EQ(target_str!=NULL, true);
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fmemopen(target_str, strlen(target_str), "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}

TEST(testSave, saveStr_test3) {
    // 1. 初始化测试用例
    char* test_str = (char*)"[section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {(char*)"section1", 
                            (char*)"key2", (char*)"value2",
                            
                            (char*)"section2"};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    iniDelKey(iniGetSection(test_ini, "section1"), "key1");
    iniAddSection(test_ini, "section2");
    
    // 3. 保存和释放测试用ini
    char* target_str = iniSaveStr(test_ini); 
    ASSERT_EQ(target_str!=NULL, true);
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fmemopen(target_str, strlen(target_str), "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}

TEST(testSave, saveStr_test4) {
    // 1. 初始化测试用例
    char* test_str = (char*)"non=non [section1]\nkey1=value1\nkey2=value2\n";
    char* expections[] = {};

    // 2. 创建测试用ini
    FILE* test_stream = fmemopen(test_str, strlen(test_str), "r");
    ini* test_ini = NULL; 
    iniParseStat test_stat = iniParse(test_stream, (ini**)&test_ini);
    iniDelSection(test_ini, "section1");
    
    // 3. 保存和释放测试用ini
    char* target_str = iniSaveStr(test_ini); 
    ASSERT_EQ(target_str!=NULL, true);
    fclose(test_stream);
    iniFree(test_ini);

    // 4. 读取保存的ini
    FILE* target_stream = fmemopen(target_str, strlen(target_str), "r");
    ini* target_ini = NULL;
    iniParseStat target_stat = iniParse(target_stream, (ini**)&target_ini);

    // 5. 检测结果
    checkIni(target_ini, expections, sizeof(expections)/sizeof(*expections));
    fclose(target_stream);
    iniFree(target_ini);
    remove("test.ini");
}