# Linux中使用git
[返回主页](../../research-study/readme.md)
[toc]
## 安装及初始设置git
1) 安装：sudo apt-get install git
2) 初始设置git:  每一次commit都会产生一条log，这条log标记了提交人的姓名与邮箱
```
      git config --global user.name 'Xiangjiajia-new'  
      git config --global user.email '1261192853@qq.com'  
      git config --list   
```
4) github上添加SSH密钥  
   1) Linux生成SSH密钥: 
   * ```ssh-keygen -t rsa```  
   2) 密钥公钥文件：```~/.ssh```目录下生成两个文件
   * 密钥——```id_rsa```, 公钥——```id_rsa.pub```  
   1) GitHub公钥添加：
   * 公钥——```id_rsa.pub```内容添加到GitHub（设置-ssh和gpg）上
   1) 测试连接：
   * ```ssh -T git@github.com``` 
5) 提交代码  
   1) 克隆要编辑的项目（已关联远程仓库）：
   * ```git clone git@github.com:Xiangjiajia-new/research-study.git```
   2) 查看项目的远程仓库：```git remote -v```
   3) 把远程仓库的最新代码拉下来：```git pull origin main```
   4) 修改项目，提交到git仓库缓存区：
   * ```git add xxx.md```（```git rm --cached```可以移除这个缓存）  
   5) 提交（commit，仅推送修改到本地git库中）：
   * ```git commit -m 'first commit'``` m后添加提交信息，提交后再输入 ```git status``` ，会提示 ```nothing to commit```
   1) 在vscode中，资源管理器里可以一键暂存，一键提交，一键同步，更方便
   2) 把本地代码推到远程仓库（同步）:
   * ```git push origin main```（push更新在远程仓库，贡献值才能刷新，不确定最新先pull）
   * ```git push <远程主机名> <远程分支名>```  把当前提交到git本地仓库的代码推送到远程主机的某个远程分支上
   3) 查看commit记录：```git log```
6) 其他：分支-合并

### 出现问题
1) Linux环境下的code无法输入中文
**原因1**：系统自带的应用市场下载的code，是削减版本，不支持中文。官方下载重装
**原因2**：vscode软件问题，关闭所有窗口，再重新打开vscode
   1. Linux安装软件.deb
   * 使用dpkg: ```sudo dpkg -i xxx.deb```
   2. Linux卸载应用
   * 输入```dpkg --list```, 找到要卸载的软件
   * `sudo apt-get （--purge） remove 包名`（将软件及其配置文件一并删除）
2) 合并冲突
