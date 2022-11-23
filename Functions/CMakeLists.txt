#将文件夹下所有的cpp文件放入到functions中
file(GLOB_RECURSE tempFuns ./ *.cpp)

#将functions中所有的cpp编译为funcs这个lib库
add_library(Functions ${tempFuns})