allTest:
	make testIniParse
	make testIniDel
	make testIniSave
	make testIniWarnAndErr

testIniParse:
	@echo "test iniParse function"
# 编译testIniParse测试单元
	g++ -o testIniParse testIniParse.cpp ../einip/ini.c -I ../einip -lgtest -lgtest_main -w
# 执行testIniParse测试单元
	./testIniParse
# 删除testIniParse测试单元
	rm testIniParse

testIniDel:
	@echo "test iniDel function"
# 编译testIniDel测试单元
	g++ -o testIniDel testIniDel.cpp ../einip/ini.c -I ../einip -lgtest -lgtest_main -w
# 执行testIniDel测试单元
	./testIniDel
# 删除testIniDel测试单元
	rm testIniDel

testIniSave:
	@echo "test iniSave function"
# 编译testIniSave测试单元
	g++ -o testIniSave testIniSave.cpp ../einip/ini.c -I ../einip -lgtest -lgtest_main -w
# 执行testIniSave测试单元
	./testIniSave
# 删除testIniSave测试单元
	rm testIniSave

testIniWarnAndErr:
	@echo "test iniWarnAndErr function"
# 编译testIniWarnAndErr测试单元
	g++ -o testIniWarnAndErr testIniWarnAndErr.cpp ../einip/ini.c -I ../einip -lgtest -lgtest_main -w
# 执行testIniWarnAndErr测试单元
	./testIniWarnAndErr
# 删除testIniWarnAndErr测试单元
	rm testIniWarnAndErr