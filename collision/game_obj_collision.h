#include "game_obj_collision.h"
game_obj_collision::game_obj_collision(cv::Point current_position, cv::Size obj_size)//构造函数画框为每个对象
{
    pos = current_position;
    c_rect = cv::Rect(pos, obj_size);//初始化框的大小
}

game_obj_collision::~game_obj_collision()
{
}

bool game_obj_collision::check_c(const game_obj_collision& other) const//传入另一个对象，这个对象是继承了这个类的派生类对象
{
    cv::Rect thisRect(pos.x, pos.y, c_rect.width, c_rect.height);
    cv::Rect otherRect(other.pos.x, other.pos.y, other.c_rect.width, other.c_rect.height);
    return (thisRect & otherRect).area() > 0;//检测2个矩形框是否出现了交集
}
