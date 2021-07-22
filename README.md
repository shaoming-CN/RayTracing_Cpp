# RayTracing_Cpp
Using Cpp to render the scene with the principle of raytacing
使用C++模拟光线与场景中各物体相交，并根据不同材质产生复杂光散射情况，为了加速生产最终的ppm图片，使用了BVH碰撞检测加速策略；
其中物体形状涉及静态球体和薄片矩形，以及用于实现动态模糊效果的动态球体，考虑到物体外观更具表现力，加入固定、格子及大理石纹理，以及物体形状不同而带来不同采集纹理方式的实现。

#类设计关联图
![图片名称](https://github.com/shaoming-CN/Learn-OpenGL/blob/main/Pictures/1616638222(1).jpg) 
