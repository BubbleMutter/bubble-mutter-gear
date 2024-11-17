[TOC]
# init.section
```log
  section           [macros]
+-----------------+
| .init.text      | __init
+-----------------+
| .init.data      | __initdata
+-----------------+
| .init.setup     | __setup_param
+-----------------+
| .initcall1.init |
+-----------------+
| .initcall2.init |
+-----------------+
| .initcall3.init |
+-----------------+
| ............... |
+-----------------+
```

# init pointer
```c++
extern char __init_begin[], __init_end[];

extern initcall_entry_t __initcall_start[];
extern initcall_entry_t __initcall0_start[];
extern initcall_entry_t __initcall1_start[];
extern initcall_entry_t __initcall2_start[];
extern initcall_entry_t __initcall3_start[];
extern initcall_entry_t __initcall4_start[];
extern initcall_entry_t __initcall5_start[];
extern initcall_entry_t __initcall6_start[];
extern initcall_entry_t __initcall7_start[];
extern initcall_entry_t __initcall_end[];
```

# free_initmem()
> `init.section` 在初始化结束后
> `free_initmem()` 中释放这部分内存

# init macro ( __init / __exit / __initdata )
| macro | linked | module |
| :-- | :-- | :-- |
| __init | init.section | real text |
| __exit | not compiled | real text |
| __initdata | init.section | real data |

# init macro ( __devinit / __devexit / __devinitdata )
| macro | CONFIG_HOTPLUG=n | CONFIG_HOTPLUG=y |
| :-- | :-- | :-- |
| __devinit | init.section | real text |
| __devexit | not compiled | real text |
| __devinitdata | init.section | real data |
1. 用于支持热插拔的设备驱动的内存优化
2. 当 CONFIG_HOTPLUG=y 时; 内核支持热插拔
   则 初始化方法 和 销毁方法 需要常驻; 设备插拔时运行
3. 当 CONFIG_HOTPLUG=n 时; 内核不支持热插拔
   即 初始化方法 仅运行一次, 销毁方法无需运行