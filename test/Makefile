allTest: testIniParse
	
testIniParse:
	@echo "test iniParse function"
# 编译testIniParse测试单元
	g++ -o testIniParse testIniParse.cpp ../lib/ini/ini.c -I ../lib/ini -lgtest -lgtest_main
# 执行testIniParse测试单元
	./testIniParse
# 删除testIniParse测试单元
	rm testIniParse