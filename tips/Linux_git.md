# git的使用
[toc]
## Linux中使用git进行文档版本管理
### 安装及初始设置git
1) 安装：sudo apt-get install git
2) 初始设置git:  每一次commit都会产生一条log，这条log标记了提交人的姓名与邮箱
```
      git config --global user.name 'Xiangjiajia-new'  
      git config --global user.email '1261192853@qq.com'  
      git config --list   
```
4) github上添加SSH密钥  
   1) Linux生成SSH密钥: 输入 ```ssh-keygen -t rsa```  
   2) ```~/.ssh```目录下生成两个文件：密钥——```id_rsa```, 公钥——```id_rsa.pub```  
   3) 公钥——```id_rsa.pub```内容添加到GitHub（设置-ssh和gpg）上，进行配对才能提交代码  
   4) 添加后，输入```ssh -T git@github.com``` 进行测试  
5) 提交代码  
   1) 克隆要编辑的项目（已关联远程仓库）：```git clone git@github.com:Xiangjiajia-new/research-study.git```
   2) 查看项目的远程仓库：```git remote -v```
   3) 把远程仓库的最新代码拉下来：```git pull origin master```
   4) 修改当前项目，将创建的文件xxx.md提交到git仓库（缓存区）```git add xxx.md```（```git rm --cached```可以移除这个缓存）  
   5) 提交（commit，仅推送修改到本地git库中）：```git commit -m 'first commit'``` m后添加提交信息，提交后再输入 ```git status``` ，会提示 ```nothing to commit```
   6) git add & git commit：先 git add 是先把改动添加到一个「暂存区」，你可以理解成是一个缓存区域，临时保存你的改动，而 git commit 才是最后真正的提交。这样做的好处就是防止误提交，可以两步合并成一步
   7) 把本地代码推到远程仓库（同步）:```git push origin master```（push完才能更新在远程仓库）
      git push <远程主机名> <远程分支名>  把当前提交到git本地仓库的代码推送到远程主机的某个远程分之上
   8) 查看commit记录：```git log```
6) 其他：分支-合并
### 出现问题
1) ```git pull origin master```出现错误```fatal：could't find remote ref master```:  
   master现在被认为是有种族歧视的，github将其换成了main，所以现在使用pull可以写为：```git pull origin main```
2) Linux环境下的code无法输入中文
**原因**：系统自带的应用市场下载的code，安装的VSCode是snap的削减版本，不支持中文。
**解决方法**：直接在官网中下载.deb自己安装,安装完成需要重启
3) Linux安装软件.deb
   使用dpkg: ```sudo dpkg -i xxx.deb```
4) Linux卸载应用
   1) 输入dpkg --list, 找到要卸载的软件
   2) sudo apt-get （--purge） remove 包名（将软件及其配置文件一并删除）
## VSCode中使用Git
用SOURCE CONTOROL就可以了
hhh


在分支中创建了文件未提交的，文件会悬挂在所有分支上，只有提交之后，文件才会只能在特定分支找到
