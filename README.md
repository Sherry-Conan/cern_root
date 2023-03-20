# cern_root
记录root代码学习

# jupyter上面跑的是C风味，不是python
# 李智焕1.1作业（核物理实验数据处理，吴鸿毅）

##1.1 要求

按照下列要求进行分析，将新参数存入新的ROOT文件。

1.1.0.参考1.1代码，编写模拟程序，生成ROOT文件。参考1.2的做法，读入ROOT文件，将tu,td以及ctof，cx存入新的ROOT文件。其中ctof=(tu+td)/2，cx=(tu-td)/2。

代码 homework_1_1.cpp

1.1.1.利用两端时间和能量信息求粒子入射位置： tx 和 Qx 。 (不得利用 λ，vsc )

找出 tu−td 的中心位置和两侧边界，根据几何条件确定  tx=a∗(tu−td)+b 中的a,b值（参照文献2.的做法）

利用两端 Qu,Qd 的信号，计算粒子入射位置 Qx 。

代码 homework_1_2.cpp

1.1.2.中子能量 ce

代码 homework_1_3.cpp

利用TOF中的  γ  峰的位置，对TOF进行绝对刻度（找到飞行时间零点）

对TOF进行常数修正，将飞行距离归一到100cm，生成新的TOF参数ntof

利用上一步TOF信息，计算中子能量ce。

在新文件中加入 tx，Qx ，ntof，ce参数，并保留原来的所有参数。

比较 tx，qx 哪种更好？。
