2018/01 ~ 2018/12 experience
# Service Framework For Multiple Sub-Projects
1. **Frontend**: 前端
    + provide GUI
    + friendly interaction
    + `js, website, mobile-phone-app`
2. **Backend**: 服务器, 监听器
    + daemons running continuously,
    + listen and response
    + `php, py, go, java, c++`
3. **Task-Handler**: 脚本实体
    + scripts to perform actual operation
    + called by **Backend**
    + called via console directly without **Backend**
    + `py, shell-scripts`
4. **Sub-Projects Source**: 工程源码
    + source, resource, project-directory
    + build/develop/debug on local-host

# More Components needed to be considered
+ 配置 问题
    1. 不同 **Task-Handler** 部署机器上 的 local-config, common-config
        如*XCode 路径, android-sdk路径*
    2. 不同 **Backend** 部署机器上 的 local-config, common-config
    2. 不同 子项目 [ 如游戏, 专服] 的 self-config, common-config

+ 子项目 共同的 **Task Handler**
+ 子项目 共同的 **Backernd**
+ 子项目 共同的 **Frontend**