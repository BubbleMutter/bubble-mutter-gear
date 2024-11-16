#!/bin/bash
git_config() {
    # windows os ignores file mode
    git config --global --add core.filemode false

    # username and password
    git config --global user.email  ""
    git config --global user.name   ""
    git config --global credential.helper 'cache --timeout=3600'
    git config --global credential.helper store   # remember password
    git config --global --unset credential.helper # discard password

    # proxy setting
    git config --global http.proxy # show
    git config --global http.proxy 'socks5://127.0.0.1:1080'   # set https
    git config --global https.proxy 'socks5://127.0.0.1:1080'  # set https
    git config --global --unset http.proxy   # disable
    git config --global --unset https.proxy  # disable
}

git_branch() {
    git checkout $branch    # switch branch
    git branch # show current branch
    git branch --delete $branch
    git branch --remotes --delete origin/$branch # discard un-push commits
    git push origin --delete $branch             # delete remote branch
}

git_diff_tips() {
    # disable pager globally
    git config --global pager.log false

    # show changed files of lastest commit
    git diff --name-only HEAD HEAD~1
    # show changed files of particular commit
    git diff --name-only $hash^ $hash

    # show commits on particular file
    git log -- $file
    # diff of commit on particular file
    git show $hash^ $hash -- $file

    # `rollback` a single file
    git checkout $hash -- $file
    # `rollback` the whole project
    git reset --hard $hash

    # show commits in pretty way
    alias gitpeak='git log --pretty=format:"%C(yellow)%h %C(white)%s %Cred%d %C(green)[%aN] %Cblue%aD"'
    gitpeak

    # show uncommited changes in pretty way
    git status -s # 形同 svn status
}

git clone "https://your/url/to/git/repo"
git pull  # `update` your local-repo
git pull  --allow-unrelated-histories # 远程有别人push了，导致本地push不成功
git add $f              # `add` your change to the local-repo.
git commit -m "message" # `record` your commit for your new changing.
git push                # `update` your local-disk change to the remote-repo.

# rebase
git rebase master
git rebase --continue
git rebase --abort

# firsh add remote
git remote add origin $url && git push -u origin master

# clone particular version of kernel
git clone -b linux-5.15.y --single-branch http://mirrors.ustc.edu.cn/linux.git

git rev-parse --short HEAD
git rev-parse --abbrev-ref HEAD
