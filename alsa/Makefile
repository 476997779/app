# 本机编译器，如果需要可以改成交叉编译器 -Wall -werror -g 都是在编译的时候有打印调试信息的参数，最好在正式编译的时候都带上

#GCC = gcc -Wall -Werror -g
GCC = mips-linux-gnu-gcc -Wall -Werror -g 
#GCC = mips-linux-gnu-gcc -Wall -Werror -g  -L/opt/mips-gcc472-glibc216-32bit/lib/gcc/mips-linux-gnu/4.7.2/install_path/lib/
#GCC = mips-linux-gnu-gcc -Wall -Werror -g  -r /opt/mips-gcc472-glibc216-32bit/lib/gcc/mips-linux-gnu/4.7.2/install_path/lib/
#GCC = mips-linux-gnu-gcc -Wall -Werror -g  -L/home/linux/SF16A18_EVB_V5/sf16a18-sdk-4.2.10/chaos_calmer_15_05_1/build_dir/target-mipsel_mips-interAptiv_uClibc-0.9.33.2/alsa-lib-1.0.28/ipkg-install/usr/lib/
# Makefile的内置规则，文件最前的为最高目标，执行make命令会自动编译此目标
main: clean alsa

.PHONY: clean alsa
# $@ 符号是Makefile中内置变量，代表目标文件
# $^ 符号是Makefile中内置变量，代表依赖文件，即源文件
alsa: alsa-myself-rw-wav-file-paly-music.c
	@echo "=== compile start! ==="
	$(GCC) $^ -o $@ -lasound -Iinclude 
	@echo "=== compile end! ==="


clean:
	@rm -rf *.o alsa
	@echo "clean success!"
