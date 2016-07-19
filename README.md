# PL0-compiler

bin文件夹下面是生成的程序compiler 以及输出的文件输出的结果文件等 不过在windows没法运行

文件夹 src下面是源程序 
main.c 是主程序 调用scanner.h parser.h vm.h 里边声明的函数进行语法分析 词法分析 语义分析（这个做的不多）还有代码生成

glodef.h 声明了一些常量量 全局变量 以及对scanner.h parser.h vm.h将要定义的函数进行声明

scanner.h 词法分析部分 将pl0程序（bin文件夹下面的input.txt）读入 进行词法分析 并生成词法分析结果 scanout.txt 

parser.h 进行语法分析  语义分析 以及代码生成  读入scanout.txt 输出 parserout.txt(语法分析结果)以及 genout.txt（代码）

vm.h 将genout.txt 读入 在一个堆栈中存储变量 在模拟机器运行
