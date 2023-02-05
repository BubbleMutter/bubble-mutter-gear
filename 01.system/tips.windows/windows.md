### enable hibernate
+ 控制面板 > 电源选项 > 更改计划设置 > 高级电源设置 > "睡眠" > "允许混合睡眠" > "否"
+ cmd.exe > `powercfg hibernate on`
+ 设备管理器 > "鼠标 键盘 网卡 等设备" > 右键-属性 > "电源管理" > 允许此设备唤醒计算机(*不选*)
    + 解决睡眠休眠后 自动重启的问题
+ bios 开启 S1, S2, S3 待机模式

### 锁屏密码有u, 每次按下会弹窗
+ 鼠标点击 左下角那个button

### win7 panel configuration
1. 使用小图标
2. 拉成两行
3. 个性化->窗口颜色和外观->{ 不启用透明效果;颜色混合器亮度最低;颜色浓度最大 }

### background
+ redgist HKEY_LOCAL_MACHINE---SOFTWARE---Microsoft---Windows---CurrentVersion--- Authentication---LogonUI---Background,
    + OEMBackground : 1 (Hex)
+ C:\Windows\System32\oobe\info\backgrounds\backgroundDefault.jpg
+ C:\Windows\System32\oobe\background.bmp

### 修改 Path
py：
from winreg import *
import os, sys, win32gui, win32con

修改键值对的例子：
```python
pathRegEnv = r'SYSTEM\CurrentControlSet\Control\Session Manager\Environment'
key = OpenKey(HKEY_LOCAL_MACHINE, pathRegEnv, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY)
ret = SetValueEx(key, 'PATH', 0, REG_EXPAND_SZ, n_val)
win32gui.SendMessageTimeout(win32con.HWND_BROADCAST,
    win32con.WM_SETTINGCHANGE, 0, 'Environment', 0, 5)
```

### IE主页问题：
3个主页面键值，1个开机自启动项
1 HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Internet Explorer\Main
   Start Page的键值改为“about:blank”
2 HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\Main
“Start Page”的键值改为“about:blank”
3 HKEY_LOCAL_MACHINE\Software\Microsoft\Internet Explorer\Main\Default_Page_URL
“Default_Page_URL”
4  HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\Current Version\Run
下面是开机自启动项
5 快捷方式->属性->“目标”


+ 申请管理员权限 MSDN 中称为 UAC，就是 user-access-control
+ registry 中的value，utf-16, 写入时，注意填充'\0'; 读取时注意 循环增量为2
+ 软链接 mklink /D dest src


### C盘的缓存
+ C:\Users\A1-0560\AppData\Local\Temp
```s
/bin/rm *.zip`
/bin/rm *.tmp`
/bin/rm  dd_setup_*
/bin/rm *.gradle
```