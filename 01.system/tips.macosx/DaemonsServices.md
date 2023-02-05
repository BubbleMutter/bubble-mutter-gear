# Daemons and Services
+ launchd
+ launchctl
+ /Library/LaunchDaemons, /Library/LaunchAgents

### 4 categories
1. Login items:
    + `start` when login
    + `stop` when logout
    + no launchd
2. XPC Services:
3. Launch Daemons: system context
4. Launch Agents:
    + per-user background processes
    + exec when user login

### some directories
+ System
    + /System/Library/LaunchDaemons/
    + /Library/LaunchDaemons/
+ User
    + /System/Library/LaunchAgents
    + /Library/LaunchAgents
    + $HOME/Library/LaunchAgents

## daemon plist
+ $>_ `man launchd.plist`
+ + $>_ `sudo launchctl load -w /Library/LaunchDaemons/com.startup.plist`
+ $>_ `sudo launchctl unload -w /Library/LaunchDaemons/com.startup.plist`
+ $>_ `sudo launchctl start -w /Library/LaunchDaemons/com.startup.plist`
+ $>_ `sudo launchctl stop /Library/LaunchDaemons/com.startup.plist`
###  recommended property
+ Label
+ ProgramArguments
+ inetdCompatibility
+ KeepAlive
### 至关重要 chmod 755 filename.plist
### Giant Bug
+ 使用自定义用户 /Library/LaunchDaemons/ 不能执行
    + `<key>UserName</key>`
    + `<key>GroupName</key>`
    + 建议在脚本里面 使用 su -m $USER -c "$CMD" 指定
### 关键 Key
```xml
<key>Disabled</key>          <false/>
<key>RunAtLoad</key>         <true/>
<key>KeepAlive</key>         <false/>
<key>SessionCreate</key>     <true/>
<key>LaunchOnlyOnce</key>    <false/>
```

### Example
+ /Library/LaunchDaemons/com.loop.random.plist
```plist
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1
.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>com.example.hello</string>

    <key>ProgramArguments</key>
    <array>
        <!-- <string>/bin/bash</string> -->
        <string>/Users/qdazzle/.Haorong/loop.sh</string>
    </array>

    <key>KeepAlive</key>
    <false/>

    <key>RunAtLoad</key>
    <true/>

    <key>LaunchOnlyOnce</key>
    <true/>

    <!-- /Library/LaunchDaemons can not specify User/Group
    <key>UserName</key>
    <string>qdazzle</string>

    <key>GroupName</key>
    <string>staff</string>

    <key>InitGroups</key>
    <true/>
     -->
</dict>
</plist>
```

### sucks
+ vnc
```s
/private/etc
/System/Library/LaunchAgents/com.apple.RemoteDesktop.plist：
/etc/RemoteManagement.launchd
/Library/Application Support/Apple/Remote Desktop/RemoteManagement.launchd
/System/Library/CoreServices/RemoteManagement/ARDAgent.app/Contents/MacOS/ARDAgent
com.apple.RemoteDesktop.agent
/System/Library/CoreServices/RemoteManagement

ALLUSERS_FOLDER
/Library/Application Support/FotoWare
 
PRIVATE_FOLDER
~/Library/Preferences/FotoWare

·  When using a separate configuration for each user
PRIVATE_FOLDER/FotoStation/Configuration
~/Library/Preferences/FotoWare/FotoStation/Configuration/Metadata/MetadataConfiguration.xml
```