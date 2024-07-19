allTest:
	make testIniParse
	make testIniDel
	
testIniParse:
	@echo "test iniParse function"
# 编译testIniParse测试单元
	g++ -o testIniParse testIniParse.cpp ../lib/ini/ini.c -I ../lib/ini -lgtest -lgtest_main -w
# 执行testIniParse测试单元
	./testIniParse
# 删除testIniParse测试单元
	rm testIniParse

testIniDel:
	@echo "test iniDel function"
# 编译testIniDel测试单元
	g++ -o testIniDel testIniDel.cpp ../lib/ini/ini.c -I ../lib/ini -lgtest -lgtest_main -w
# 执行testIniDel测试单元
	./testIniDel
# 删除testIniDel测试单元
	rm testIniDel