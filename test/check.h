#include <iostream>
using namespace std;

/**
 * @brief ini 测试函数
 * 
 * @param p_stat 
 * @param ini_t 
 * @param expections 
 */
void checkIni(ini* ini_t, char**expections, int expections_num) {
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
    EXPECT_EQ(index, expections_num);
}

/**
 * @brief 检查iniParseStat的正确性
 * 
 */
void checkWarnAndErr(iniParseStat p_stat,\
\
                    int warn_num,\
                    int* warn_lines,\
                    iniStat* warns,\
\
                    int err_num,\
                    int* err_lines,\
                    iniStat* errs) {
    // 判断警告和错误的数量是否正确
    ASSERT_EQ(p_stat.warn_num, warn_num);
    ASSERT_EQ(p_stat.error_num, err_num);

    // 判断警告和错误的内容是否正确
    // 警告
    for (int i = 0; i < warn_num; i++) {
        EXPECT_EQ(p_stat.warn_lines[i], warn_lines[i]);
        EXPECT_EQ(p_stat.warn_infos[i], warns[i]);
    }
    // 错误
    for (int i = 0; i < err_num; i++) {
        EXPECT_EQ(p_stat.error_lines[i], err_lines[i]);
        EXPECT_EQ(p_stat.error_infos[i], errs[i]);
    }
}