network:
    postman laragon heidisql
    vncviewer teamviewer fiezilla
    wireshark
    v2ray
    netlimiter
    fdm5
    NetRouteView

devel:
    spyder
    HxD
    idaq64
    kdiff3

shell:
    MobaXterm PAC Putty XShell
sys:
    cpuz

PAC patch
curl -s https://packagecloud.io/install/repositories/asbru-cm/asbru-cm/script.deb.sh | sudo bash
sudo apt install libgnome2-vte-perl
find /path/to/pac -name "Vte.so" -exec rm {} +

# 重启 windows 任务栏
cmd /c wmic process where name="explorer.exe" delete&explorer reg delete HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\StuckRects2 /f&taskkill /f /im explorer.exe&explorer

# 更换 windows 头像 桌面壁纸 登录背景

# 强杀进程
Taskkill /F /PID 5408
Taskkill /IM firefox.exe /F /T