#include "ini.h"

int main() {
    // ini* ini = NULL;
    ini ini;
    char* source = "[section1]\n key1 =value1\nkey2=value2\n[section2]\nkey1=value1\nkey2=value2";
    FILE* stream = fmemopen(source, strlen(source), "r");
    iniParse(stream, &ini);
    printfIni(&ini);
    // iniParseStat p_stat = iniParseStr(NULL, &ini);
    return 0;
}