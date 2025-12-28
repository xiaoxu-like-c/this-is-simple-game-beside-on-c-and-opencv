#include "monster.h"

MONSTER::MONSTER(cv::Point monster_pos)
	:game_obj_collision(monster_pos,cv::Size(50,50))
{
	monster_img = cv::imread("G:/game_resource/monster2.jpg");
	cv::resize(monster_img, monster_img, cv::Size(50, 50));
	monster_img_copy = monster_img.clone();
	if (monster_img_copy.empty() || monster_img.empty())
	{
		std::cout << "not load img" << std::endl;
	}
	monster_size = monster_img.size();
	speed = 25;
	life_time = 200;
}

MONSTER::~MONSTER() {}

void MONSTER::monster_move(const cv::Size & bk_size)
{
	//c++随机数
	std::random_device rd;
	std::mt19937 gen = std::mt19937(rd());
	std::uniform_int_distribution<> dis(0, 4);//0-4范围
	int monster_direction = dis(gen);
	cv::Point center(monster_img.cols / 2, monster_img.rows / 2);//旋转中心
	switch (monster_direction)//随机移动和旋转怪物朝向
	{
	case 0:
		cv::warpAffine(monster_img, monster_img_copy, cv::getRotationMatrix2D(center, -90, 1), monster_img.size());//旋转方向
		game_obj_collision::pos.x += speed;//right direction
		break;
	case 1:
		cv::warpAffine(monster_img, monster_img_copy, cv::getRotationMatrix2D(center, 90, 1), monster_img.size());
		game_obj_collision::pos.x -= speed;//left directiom
		break;
	case 2:
		cv::warpAffine(monster_img, monster_img_copy, cv::getRotationMatrix2D(center, 180, 1), monster_img.size());
		game_obj_collision::pos.y += speed;//down direction
		break;
	case 3:
		cv::warpAffine(monster_img, monster_img_copy, cv::getRotationMatrix2D(center, 0, 1), monster_img.size());
		game_obj_collision::pos.y -= speed;//up direction
		break;
	}
	//范围检只能在地图内生成，上下0-地图h-怪物h，左右0-地图w-怪物w
	game_obj_collision::pos.x = cv::max(0, cv::min(game_obj_collision::pos.x, bk_size.width - monster_size.width));
	game_obj_collision::pos.y = cv::max(0, cv::min(game_obj_collision::pos.y, bk_size.height - monster_size.height));
}

void MONSTER::monster_draw(const cv::Mat & frame) const
{
	cv::Rect monster_roi(game_obj_collision::pos, monster_size);
	cv::Mat temp_monster_img = frame(monster_roi);
	monster_img_copy.copyTo(temp_monster_img);
}

