#递归将本文件夹下所有的cpp放到app变量中
file(GLOB_RECURSE maths ./ *.cpp)

#将app中所有cpp编译为applicationlib库
add_library(mathlib ${maths})