#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include"game_obj_collision.h"
#undef max
#undef min
class PLAYER :public game_obj_collision
{
private:
	cv::Mat role_img;
	cv::Mat temp_role_img;//用于朝向绘制
	cv::Size role_size;
	//cv::Point role_position;//没必要再声明一个位置属性，角色的位置属性是和碰撞框的位置属性相同的，则共用碰撞类的位置属性即可
	//cv::Point direction;//用传入的pos去判断方向，再调整绘图
	int speed;
public:
	int life_time;
public:
	PLAYER(cv::Point role_pos);
	~PLAYER();
	cv::Size get_role_size() const;
	void role_move(cv::Point dir, const cv::Size & bk_size);
	void role_draw(const cv::Mat & frame) const;
};

