#include "player.h"

PLAYER::PLAYER(cv::Point role_pos)
	:game_obj_collision(role_pos,cv::Size(100,100))
{
	role_img = cv::imread("G:/game_resource/role1.jpg");
	temp_role_img = role_img.clone();
	if (temp_role_img.empty()|| role_img.empty())
	{
		std::cout << "not load img" << std::endl;
	}
	role_size = role_img.size();
	//在游戏类实例化角色类对象时再初始化它的位置
	//role_position.x =
	//	game_viewport_resources::bk_img_size.width / 2 - role_size.width / 2;
	//role_position.y =
	//	game_viewport_resources::bk_img_size.height / 2 - role_size.height / 2;
	speed = 20;
	life_time = 100;
	//初始化视口的坐标

}

PLAYER::~PLAYER() {}

cv::Size PLAYER::get_role_size() const
{
	return cv::Size(role_size);
}

void PLAYER::role_move(cv::Point dir, const cv::Size & bk_size)
{
	//调整角色朝向(根据pos的值判断)
	cv::Point center(role_size.width / 2, role_size.height / 2);//旋转中心
	if (dir==cv::Point(0,-1))//up
	{
		cv::warpAffine(role_img, temp_role_img, cv::getRotationMatrix2D(center, 0, 1), role_img.size());
	}
	else if (dir == cv::Point(0, 1))//down
	{
		cv::warpAffine(role_img, temp_role_img, cv::getRotationMatrix2D(center, 180, 1), role_img.size());
	}
	else if (dir == cv::Point(-1, 0))//left
	{
		cv::warpAffine(role_img, temp_role_img, cv::getRotationMatrix2D(center, +90, 1), role_img.size());
	}
	else if(dir == cv::Point(1, 0))//right
	{
		cv::warpAffine(role_img, temp_role_img, cv::getRotationMatrix2D(center, -90, 1), role_img.size());
	}

	// 移动角色(角色的xy和碰撞类的pos共用)
	game_obj_collision::pos.x += dir.x * speed;
	game_obj_collision::pos.y += dir.y * speed;

	// 角色边界检查
	game_obj_collision::pos.x = cv::max(0, cv::min(game_obj_collision::pos.x, bk_size.width - role_size.width));
	game_obj_collision::pos.y = cv::max(0, cv::min(game_obj_collision::pos.y, bk_size.height - role_size.height));

}

void PLAYER::role_draw(const cv::Mat& frame) const
{
	cv::Rect role_roi(game_obj_collision::pos, role_size);
	cv::Mat temp = frame(role_roi);
	temp_role_img.copyTo(temp);
}

