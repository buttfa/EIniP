#include "gtest/gtest.h"
#include "ini.h"
#include "check.h"

TEST(testIniWarnAndErr, test0) {
    // 初始化测试用例
    iniParseStat* p_stat= (iniParseStat*)malloc(sizeof(iniParseStat));
    memset(p_stat, 0, sizeof(iniParseStat));
    int expections_warn_num = 0; // 预期警告
    int expections_warn_line[] = {};
    iniStat expections_warns[] = {};
    int expections_err_num = 0; // 预期错误
    int expections_err_line[] = {};
    iniStat expections_errs[] = {};

    // 运行

    // 检测结果
    checkWarnAndErr(p_stat,\
\
                    expections_warn_num,\
                    expections_warn_line,\
                    expections_warns,\
\
                    expections_err_num,\
                    expections_err_line,\
                    expections_errs);
    iniDestroyStat((iniParseStat**)&p_stat);
}


TEST(testIniWarnAndErr, test1) {
    // 初始化测试用例
    iniParseStat* p_stat= (iniParseStat*)malloc(sizeof(iniParseStat));
    memset(p_stat, 0, sizeof(iniParseStat));
    int expections_warn_num = 0; // 预期警告
    int expections_warn_line[] = {};
    iniStat expections_warns[] = {};
    int expections_err_num = 1; // 预期错误
    int expections_err_line[] = {1};
    iniStat expections_errs[] = {INI_ERR_VALUE_IS_EMPTY};

    // 运行待测函数
    section section_ptr;
    handleIniWarnAndErr(p_stat, &section_ptr, 1, (char*)"key1=");

    // 检测结果
    checkWarnAndErr(p_stat,\
\
                    expections_warn_num,\
                    expections_warn_line,\
                    expections_warns,\
\
                    expections_err_num,\
                    expections_err_line,\
                    expections_errs);
    iniDestroyStat((iniParseStat**)&p_stat);
}

TEST(testIniWarnAndErr, test2) {
    // 初始化测试用例
    iniParseStat* p_stat= (iniParseStat*)malloc(sizeof(iniParseStat));
    memset(p_stat, 0, sizeof(iniParseStat));
    int expections_warn_num = 0; // 预期警告
    int expections_warn_line[] = {};
    iniStat expections_warns[] = {};
    int expections_err_num = 1; // 预期错误
    int expections_err_line[] = {3};
    iniStat expections_errs[] = {INI_ERR_UNKNOWN_LINE};

    // 运行待测函数
    section section_ptr;
    handleIniWarnAndErr(p_stat, &section_ptr, 3, (char*)"key1");

    // 检测结果
    checkWarnAndErr(p_stat,\
\
                    expections_warn_num,\
                    expections_warn_line,\
                    expections_warns,\
\
                    expections_err_num,\
                    expections_err_line,\
                    expections_errs);
    iniDestroyStat((iniParseStat**)&p_stat);
}

TEST(testIniWarnAndErr, test3) {
    // 初始化测试用例
    iniParseStat* p_stat= (iniParseStat*)malloc(sizeof(iniParseStat));
    memset(p_stat, 0, sizeof(iniParseStat));
    int expections_warn_num = 0; // 预期警告
    int expections_warn_line[] = {};
    iniStat expections_warns[] = {};
    int expections_err_num = 2; // 预期错误
    int expections_err_line[] = {1, 3};
    iniStat expections_errs[] = {INI_ERR_VALUE_IS_EMPTY, INI_ERR_UNKNOWN_LINE};

    // 运行待测函数
    section section_ptr;
    handleIniWarnAndErr(p_stat, &section_ptr, 1, (char*)"key1=");
    handleIniWarnAndErr(p_stat, &section_ptr, 3, (char*)"key1");

    // 检测结果
    checkWarnAndErr(p_stat,\
\
                    expections_warn_num,\
                    expections_warn_line,\
                    expections_warns,\
\
                    expections_err_num,\
                    expections_err_line,\
                    expections_errs);
    iniDestroyStat((iniParseStat**)&p_stat);
}

TEST(testIniWarnAndErr, test4) {
    // 初始化测试用例
    iniParseStat* p_stat= (iniParseStat*)malloc(sizeof(iniParseStat));
    memset(p_stat, 0, sizeof(iniParseStat));
    int expections_warn_num = 1; // 预期警告
    int expections_warn_line[] = {1};
    iniStat expections_warns[] = {INI_WARN_KVP_NOT_BELONG_SECTION};
    int expections_err_num = 2; // 预期错误
    int expections_err_line[] = {2, 3};
    iniStat expections_errs[] = {INI_ERR_VALUE_IS_EMPTY, INI_ERR_UNKNOWN_LINE};

    // 运行待测函数
    section section_ptr;
    handleIniWarnAndErr(p_stat, NULL, 1, (char*)"key1=value1");
    handleIniWarnAndErr(p_stat, &section_ptr, 2, (char*)"key2=");
    handleIniWarnAndErr(p_stat, &section_ptr, 3, (char*)"key3");

    // 检测结果
    checkWarnAndErr(p_stat,\
\
                    expections_warn_num,\
                    expections_warn_line,\
                    expections_warns,\
\
                    expections_err_num,\
                    expections_err_line,\
                    expections_errs);
    iniDestroyStat((iniParseStat**)&p_stat);
}

TEST(testIniWarnAndErr, test5) {
    // 初始化测试用例
    iniParseStat* p_stat= (iniParseStat*)malloc(sizeof(iniParseStat));
    memset(p_stat, 0, sizeof(iniParseStat));
    int expections_warn_num = 3; // 预期警告
    int expections_warn_line[] = {1, 1, 2};
    iniStat expections_warns[] = {INI_WARN_VALUE_EXIST_SPACE, INI_WARN_KVP_NOT_BELONG_SECTION, INI_WARN_KEY_EXIST_SPACE};
    int expections_err_num = 2; // 预期错误
    int expections_err_line[] = {2, 3};
    iniStat expections_errs[] = {INI_ERR_VALUE_IS_EMPTY, INI_ERR_UNKNOWN_LINE};

    // 运行待测函数
    section section_ptr;
    handleIniWarnAndErr(p_stat, NULL, 1, (char*)"key1=val ue1");
    handleIniWarnAndErr(p_stat, &section_ptr, 2, (char*)"k ey2=");
    handleIniWarnAndErr(p_stat, &section_ptr, 3, (char*)"key3");

    // 检测结果
    checkWarnAndErr(p_stat,\
\
                    expections_warn_num,\
                    expections_warn_line,\
                    expections_warns,\
\
                    expections_err_num,\
                    expections_err_line,\
                    expections_errs);
    iniDestroyStat((iniParseStat**)&p_stat);
}