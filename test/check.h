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