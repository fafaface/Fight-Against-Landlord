# Fight-Against-Landlord
斗地主

**程序的说明**
**1.已完成的打包程序,** 有小概率可能在开始游戏后闪退, 再次双击运行一般没有问题
    a.双击"ddzfinal_boxed.exe"
    b.点击"开始游戏"
    c.点击"抢地主", 或者点击"不抢"
    d.点击符合出牌规则的扑克牌, 若点错了, 再点一次,则取消选中扑克牌
    e.选中符合出牌规则的扑克牌后, 点击"出牌"
    留意左下角的出牌区域, 看上一个玩家的出牌情况
    f.如果不想出, 或者没有大过上家的牌, 则选择"不出"
    若弹出游戏结束的画面, 点击左上角电源按钮, 进入游戏结束界面
    g.点击"Exit"退出游戏

**2.ddzfinal的源程序**
   经过搜索,如果要在另一台电脑上导入现有的Cocos2d文件夹,则必须安装Cocos2d环境
   (具体截图可参考"课设报告.pdf"的" 三、技术方案及步骤 "

01.在https://www.python.org/downloads/下载安装 Python2.7,将 Python2.7 文件夹和下属的 Script 目录的两条绝对路
径分别添加至环境变量Path的最高位(如果当前电脑配置了Python3.X, 则必须顺序优先于 Python3.X)。

02.在https://www.cocos.com/cocos2dx下载 Cocos2d-x 4.0 压缩包,并解压。进入解压后的文件夹,找到 setup.py,并在此文件夹下用 cmd python 命令运行
setup.py 脚本,按回车键,完成初步的安装。

03.在https://cmake.org/download/下载 CMake, 并将 CMake/bin 所在的绝对路径添加至环境变量 Path 中

04.修改VisualStudio2019配置,保证有MSVC v142生成工具等模块, Windows10 SDK。

唤起命令行, 输入: 

cocos new ddz -d C:/ -l cpp , 即 ("cocos new 项目名 -d 目录名 -l cpp")
命令行切换到创建好的 ddz 项目文件夹,输入:
mkdir build 
cd build
cmake .. -G"Visual Studio 16 2019"-T v142 -A win32
(这取决于当前电脑VS的具体版本)
成功创建新项目后, 在ddz文件夹中会出现ddz.sln文件,同时将上交的ddzfinal文件夹内的同名文件夹覆盖到当前的项目里,
双击ddz.sln,进入VS2019,添加ddzfinal中的Classes文件和Resources文件,删除原有的AppDelegate.cpp/h HelloWorld.cpp/.h文件
查看代码有无基本报错, 比如头文件的缺失, 如无误, 则设置ddz为启动项目, 并且VS2019为Release模式
即可点击生成解决方案, 初始编译较缓慢, 之后点击开始调试,则能出现程序了。

