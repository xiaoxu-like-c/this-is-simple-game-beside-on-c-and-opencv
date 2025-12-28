#include "bullet.h"

BULLET::BULLET(cv::Size role_size, cv::Point role_pos,cv::Point dir)//初始化传入角色的位置（在初始化子弹位置时要用）
	:game_obj_collision(role_pos,cv::Size(50,50))
{
	bullet_img = cv::imread("G:/game_resource/shoot.jpg");
	temp_bullet_img = bullet_img.clone();
	if (temp_bullet_img.empty() || bullet_img.empty())
	{
		std::cout << "not load img" << std::endl;
	}
	bullet_size = bullet_img.size();
	bullet_diretion = dir;
	speed = 20;
	life_time = 15;
	//生成子弹对象时，确定初始位置和朝向
	//计算位置根据角色位置计算子弹的发射位置（计算子弹位于角色图片的中间而不是角色的坐标位置）子弹对象共用碰撞类的pos
	cv::Point center(bullet_size.width / 2, bullet_size.height / 2);//旋转中心
	if (dir == cv::Point(0, -1))//up
	{
		cv::warpAffine(bullet_img, temp_bullet_img, cv::getRotationMatrix2D(center, 0, 1), bullet_img.size());
		game_obj_collision::pos.x = game_obj_collision::pos.x + role_size.width / 2 - bullet_size.width / 2;
		game_obj_collision::pos.y = game_obj_collision::pos.y - bullet_size.height;
	}
	else if (dir == cv::Point(0, 1))//down
	{
		cv::warpAffine(bullet_img, temp_bullet_img, cv::getRotationMatrix2D(center, 180, 1), bullet_img.size());
		game_obj_collision::pos.x = game_obj_collision::pos.x + role_size.width / 2 - bullet_size.width / 2;
		game_obj_collision::pos.y = game_obj_collision::pos.y + role_size.height + bullet_size.height;
	}
	else if (dir == cv::Point(-1, 0))//left
	{
		cv::warpAffine(bullet_img, temp_bullet_img, cv::getRotationMatrix2D(center, +90, 1), bullet_img.size());
		game_obj_collision::pos.x = game_obj_collision::pos.x - bullet_size.width;
		game_obj_collision::pos.y = game_obj_collision::pos.y + role_size.height / 2 - bullet_size.height / 2;
	}
	else if (dir == cv::Point(1, 0))//right
	{
		cv::warpAffine(bullet_img, temp_bullet_img, cv::getRotationMatrix2D(center, -90, 1), bullet_img.size());
		game_obj_collision::pos.x = game_obj_collision::pos.x + role_size.width;
		game_obj_collision::pos.y = game_obj_collision::pos.y + role_size.height / 2 - bullet_size.height / 2;
	}
}

BULLET::~BULLET()
{
}

cv::Size BULLET::get_bullet_size() const
{
	return cv::Size(bullet_size);
}

void BULLET::bullet_move(const cv::Size& bk_szie)
{
	//每一帧导弹都会自己在原来的数据上更新自己的数据(位置=朝向*速度)
	game_obj_collision::pos.x += bullet_diretion.x * speed;
	game_obj_collision::pos.y += bullet_diretion.y * speed;
	//子弹在地图的边界检查
	game_obj_collision::pos.x = cv::max(0, cv::min(game_obj_collision::pos.x, bk_szie.width - bullet_size.width));//左右边界(1000是地图的宽高)
	game_obj_collision::pos.y = cv::max(0, cv::min(game_obj_collision::pos.y, bk_szie.height - bullet_size.height));//上下边界
	life_time--;//更新一次生命周期减一次，为0就删除当前对象
}

void BULLET::bullet_draw(const cv::Mat& frame) const
{
	cv::Rect bullet_roi(game_obj_collision::pos, bullet_size);
	cv::Mat temp_bullet = frame(bullet_roi);
	temp_bullet_img.copyTo(temp_bullet);
}
