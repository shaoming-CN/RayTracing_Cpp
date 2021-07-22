# RayTracing_Cpp
Using Cpp to render the scene with the principle of raytacing

使用C++模拟光线与场景中各物体相交，并根据不同材质产生复杂光散射情况，为了加速生产最终的ppm图片，使用了BVH碰撞检测加速策略；
其中物体形状涉及静态球体和薄片矩形，以及用于实现动态模糊效果的动态球体，考虑到物体外观更具表现力，加入固定、格子及大理石纹理，以及物体形状不同而带来不同采集纹理方式的实现。

# 类设计关联图
![图片名称](https://github.com/shaoming-CN/RayTracing_Cpp/blob/main/Displaypicture/RayTracing_Note.png) 

# 光散射效果
本项目不同射效果实现主要依托不同材质类中，scatter函数对应功能，材质lambertian、metal、dielectric分别产生漫反射、反射和折射三种现象；
下图从左到右分别是折射、漫反射、反射效果；
![图片名称](https://github.com/shaoming-CN/RayTracing_Cpp/blob/main/Displaypicture/reflect_refract.png) 

# 可控制的相机
相机可设定世界空间中的位置，以及利用fov和宽高比来定义视野范围；

![图片名称](https://github.com/shaoming-CN/RayTracing_Cpp/blob/main/Displaypicture/Camera.png) 

# 动态模糊
动态模糊在游戏中的应用:在每一帧上的运动物体朝某个方向加入拖影和模糊，就能以每秒至少24帧形成连续画面；本例一个像素会射出多束样本光线，我们为给每个光线添加一个记录射出时间的参数，该时间作为插值属性得到动态球球心位置，最后平均结果作为该像素颜色。
![图片名称](https://github.com/shaoming-CN/RayTracing_Cpp/blob/main/Displaypicture/raytracing_movingsphere.png) 

#球体纹理采样
将球上的位置坐标转换到球系坐标下的天顶角和方位角，分别归一化到纹理中取样。
![图片名称](https://github.com/shaoming-CN/RayTracing_Cpp/blob/main/Displaypicture/sphere_getuv.png) 

#大理石纹理
原理利用柏林噪声；
![图片名称](![图片名称](https://github.com/shaoming-CN/RayTracing_Cpp/blob/main/Displaypicture/sphere_getuv.png)) 
