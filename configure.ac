# 初始化autoconf
AC_INIT([replace_uuid], [1.0], [youremail@example.com])

# 检查C编译器
AC_PROG_CC

# 使用automake工具
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# 检查libuuid库
AC_CHECK_LIB([uuid], [uuid_generate_random])

# 生成Makefile
AC_CONFIG_FILES([Makefile])

# 初始化配置脚本
AC_OUTPUT
