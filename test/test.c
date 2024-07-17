#include "ini.h"

int main() {
    // ini* ini = NULL;
    ini ini;
    iniParseStr("[section1]\nkey1=value1\nkey2=value2\n[section2]\nkey1=value1\nkey2=value2", &ini);
    // iniParseStat p_stat = iniParseStr(NULL, &ini);
    return 0;
}