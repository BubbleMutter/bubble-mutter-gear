# "=" 进行赋值，变量的值是整个makefile中最后被指定的值
VIR_A = A
VIR_B = $(VIR_A) B # VIR_B 这里是 AAB
VIR_A = AA

# " := " 就表示直接赋值，赋予当前位置的值
# " ?= " 变量为空则赋值, 非空则不执行
# " += " 字符串追加赋值

$(error I am Makefile error, then I exit)
$(warning I am Makefile warning)
$(info  I am Makefile info)

# ====================================================
# 配合 shell 的 test 使用
ifeq ("$(shell test -f $(WANT_FILE) && echo yes || echo no)", "yes")
	$(info $(WANT_FILE) exists)
else
	$(error $(WANT_FILE) not found)
endif
# ====================================================

# ====================================================
# 兼用性地使用 ccache 编译内核
CCACHE_LIB_PATH="/tmp/ccache/lib"
CCACHE_BIN_PATH=$(shell which ccache)
ifeq ("$(shell ccache --version 1>/dev/null 2> /dev/null && echo yes || echo no)", "yes")
    $(shell mkdir -p /tmp/ccache/lib)
    CROSS_COMPILE := "$(shell basename $(CROSS_COMPILE))"  # 避免绝对路径引用
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)ar")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)cpp")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)gcc")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)g++")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)nm")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)objcopy")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)objdump")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)strip")
    $(shell ln -sf $(CCACHE_BIN_PATH) "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)ld")
    CROSS_COMPILE := "$(CCACHE_LIB_PATH)/$(CROSS_COMPILE)" # 避免递归; 使用 :=
    $(info using ccache $(CROSS_COMPILE))
endif

tasksel_compile_kernel:
	$(MAKE) -C $(KERNEL_PATH) silentoldconfig ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= V=$(V)
	$(MAKE) -C $(KERNEL_PATH) prepare modules_prepare ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= V=$(V)
	$(MAKE) -C $(KERNEL_PATH) headers_install ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= V=$(V)
	$(MAKE) -j4 -C $(KERNEL_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) KBUILD_OUTPUT= V=$(V)
# ====================================================

