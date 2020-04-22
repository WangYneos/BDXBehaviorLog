#BDXBehaviorLog
prebuilt versioncan be find in release page
open-source under GPL_V3
feel free to make pull request, this can make the project better
预编译版本可见Release页
GPLv3开源
如果需要添加、修正翻译包、提交bug修复 欢迎开pull req
##Load It
put ```config/behaviorlog.json``` to bdx ```config``` folder
put ```langpack/behaviorlog.json``` to bdx ```langpack``` folder
put the dll plugin to ```bdxmod``` folder
将 ```config/behaviorlog.json``` 放到 bdx 的 ```config``` 文件夹
将 ```langpack/behaviorlog.json``` 放到 bdx 的```langpack``` 文件夹
将dll插件放到 ```bdxmod``` 文件夹
##about config file
```
{
"LogUseItem" : true,
//log player use item 记录玩家使用物品
"LogChest" : true,
//Logs about chest 箱子相关记录
"LogPlace" : true,
//Log player place blocks 记录玩家放置方块
"LogDestory" : true,
//Log player destroy blocks 记录玩家破坏方块
"LogCDim" : true,
//log player change dismission 记录玩家切换维度
"LogDie": true,  
//log a mob die 记录实体死亡
"showLogUseItem": true, 
//log player use a item(console output) 记录玩家使用物品(控制台输出)
"showLogChest": true, 
//Logs about chest(console output) 箱子相关记录(控制台输出)
"showLogBlock": true,
//block logs(console output) 方块相关记录(控制台输出)
"showLogCDim": true,
//log player change dismission(console output) 记录玩家切换维度(控制台输出)
"showLogDie": true,
//log a mob die(console output) 记录实体死亡(控制台输出)
"LogFD": true,
//log file with date 日志文件名后面加上日期
"LogFH": false,
//log file with date and hour 日志文件名后加上日期和小时
"logfile" : "log/BehaviorLog",
//path to logfile 日志文件目录
"LogCMD":true,
//Log player use a command 记录玩家使用命令
"LogJoin":true,
//log player join the server 记录玩家进入服务器
"LogLeft":true,
//log player left 记录玩家离开
"LogChat":true,
//log player's chat 记录玩家聊天
"logCmdMlock":true,
//log command block and Minecrat_commandblock 命令块和命令块矿车相关记录
"CmdBlockWriteable":true
//command blok can be write 命令块可写入
}
```
##about langpack/behaviorlog
```
word.xxxxx
is about a word 就是一个单词的翻译
log.xxx
is about a the log function output(in behavior.h)在behavior.h中的日志相关
other doesn't have special mean 其他打头的没有特殊意义
```
##build the plugin by yourself
clone this project and BedrockXKit project
create a folder in project sln.
put the file to the folder 
add the vcporoj file to your sln.
switch to release profile
clone这个项目和BedrockKit项目
建个文件夹并把文件丢进去
切换到Release配置文件
将项目添加到BDXKit解决方案中