#!/bin/bash

# 远程有别人push了，导致本地push不成功
git pull  --allow-unrelated-histories

# At the first time, to `Obtain` the whole remote-repo.
git clone "https://your/url/to/git/repo"

# To `Save` your username and passwd after your first push and pull.
git config --global credential.helper store

# A real-day routine is showed as follow
# [1] `Sync` the remote-repo before you change the repo.
git pull
:<<! [2] editing
    Edit your code, debug, ...
!
# [3] `Add` your change to the local-repo.
git add -A
# [4] `Record` your commit for your new changing.
git commit
# [5] `Update` your local-disk change to the remote-repo.
git push

# roll back
# Note: Be careful, the un-pushed changes will be discarded.
# [1] `rollback` a single file
git checkout "<hash code>"  "<path/to/file>"
# [2] `rollback` the whole project
git reset --hard "<hash code>"

# commit editor
git config --global core.editor vim

# save password
git config --global user.email  ""
git config --global user.name   ""
git config --global credential.helper store
git config --global credential.helper 'cache --timeout=3600'
git config --global --unset credential.helper # clear password

# using git as clond note
git add -A && git commit --allow-empty-message -m "" && git push --force

# init
git init && git add -A && git commit -m "" --allow-empty-message
git remote add origin $url && git push -u origin master

# win7 ignore mode difference
git config --global --add core.filemode false

# proxy setting
git config --global http.proxy # show
git config --global http.proxy 'socks5://127.0.0.1:1080'   # set https
git config --global https.proxy 'socks5://127.0.0.1:1080'  # set https
git config --global --unset http.proxy   # disable
git config --global --unset https.proxy  # disable

# git 忽略文件权限
git config core.fileMode false

# 仅显示最新的 hash id
# 通常用于记录某个节点是否存在更新
git log -1 --pretty=format:%h .

# 形同 svn status
git status -s

# 从 master 转移到 main 分支; 新的代码在 master 中
git checkout main # 切到 main 中
# 在 main 中拉 master 的代码
git pull origin master --allow-unrelated-histories
# 推送到 main
git push origin main:main

# 删除本地分支
git branch --delete $branch
# 删除远程分支在本地的记录; 用于 discard 已 commit 但未 push 的场景
git branch --remotes --delete origin/$branch
# 删除远程分支
git push origin --delete $branch

# rebase
git rebase master
git rebase --continue
git rebase --abort

