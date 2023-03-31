# PlantVsZombie
项目实现：
主要用到了C++11新特性、SDL（Simple DirectMedia Layer）第三方图形库和上个项目的线程池，目前两个项目

项目涉及到的技术：
C++智能指针、继承多态、vector、map容器、lambda表达式、模板编程等；
SDL_Window（图形窗口）和SDL_Renderer（渲染器）的创建、图形的加载和纹理的创建等；

遇到的问题：
1、SDL渲染器缓冲区满溢问题

解决办法：
1、将每个图片都加载进来，创建一个空白图像，将加载的图片都绘制上去，然后让渲染器只渲染这一张图片，减少渲染器的压力。

因时间原因，这个植物大战僵尸项目暂时只实现了阳光、向日葵、豌豆射手和普通僵尸，已经为他们设置了血量等状态，能够正常运行和游戏。

该项目主要框架已经搭建完毕，植物类和僵尸类已经实现完成，后续只需对每个僵尸和植物进行补充，以及把音效结合进去便可初步实现，详细实现需要花费更多的时间。


