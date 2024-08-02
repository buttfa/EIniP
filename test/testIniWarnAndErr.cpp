#include "gtest/gtest.h"
#include "ini.h"
#include "check.h"

TEST(testIniWarnAndErr, test0) {
    // 初始化测试用例
    iniParseStat p_stat; memset(&p_stat, 0, sizeof(iniParseStat));
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
}


TEST(testIniWarnAndErr, test1) {
    // 初始化测试用例
    iniParseStat p_stat; memset(&p_stat, 0, sizeof(iniParseStat));
    int expections_warn_num = 1; // 预期警告
    int expections_warn_line[] = {1};
    iniStat expections_warns[] = {INI_WARN_VALUE_IS_EMPTY};
    int expections_err_num = 0; // 预期错误
    int expections_err_line[] = {};
    iniStat expections_errs[] = {};

    // 运行待测函数
    section section_ptr;
    handleIniWarnAndErr(&p_stat, &section_ptr, 1, "key1=");

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
}
