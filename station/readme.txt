如需在其他环境中嵌入请提取transform.h/.cpp、TranClass.h/.cpp，在需要的地方包含transform.h
注意：默认情况下输出值仅有z轴单位为cm，其他均为像素，可在transform.cpp中修改
梯度下降法没合适的数据所以还没测试，暂时不确定能否正常运行
所有可被调用的函数在头文件中均有说明
main.cpp及其他文件是一个可以使用的demo，所附带的1.png为一张测试图，ddd.png为1.png拉伸后所得到的图片，方便对输出值和图进行比较
其余的1*.png与det*.png为不同高度平台的校正图，如有需要可以利用其重新调试参数
demo中的三个测试点依次为我方烧饼杆左右两侧支撑柱底部和中路高地上的步兵
若在VS中无法运行该demo请百度VS调用opencv并进行相关配置