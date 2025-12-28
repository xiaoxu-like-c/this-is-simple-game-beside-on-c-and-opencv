#include "game.h"

GAME::GAME()
    :player(cv::Point(1450,1450)),//初始化玩家位置
    bk_img_size(3000,3000)//初始地图尺寸
{
    // 获取屏幕宽度和高度
    wind_width = GetSystemMetrics(SM_CXSCREEN);
    wind_height = GetSystemMetrics(SM_CYSCREEN);
    //初始化地图属性
    create_bk_img();
    bk_img_copy = bk_img.clone();
    bk_img_size = bk_img.size();
    //初始化视口位置
    cv::Size role_size = player.get_role_size();
    game_viewport_position.x = player.pos.x + role_size.width / 2 - game_viewport_size.width / 2;
    game_viewport_position.y = player.pos.y + role_size.height / 2 - game_viewport_size.height / 2;
    game_viewport_size = cv::Size(800, 650);

    //初始化怪物//用c++的随机数生成
    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());
    std::uniform_int_distribution<> dis1(0, 3000-50);//左右范围内(地图的w-怪物的w)
    std::uniform_int_distribution<> dis2(0, 3000 - 50);//上下范围内(地图的h-怪物的h)
    for (int i=0;i<25;i++)
    {
        monster.emplace_back(cv::Point(dis1(gen), dis2(gen)));
    }

    bullet_dir = cv::Point(0, -1);//记录子弹朝向默认向上
    //初始化游戏对象时就开始战斗播放音乐
    mciSendString(TEXT("open G:/game_resource/music.mp3"), NULL, 0, NULL);//打开音效文件
    mciSendString(TEXT("play G:/game_resource/music.mp3 repeat"), NULL, 0, NULL);//重复播发音效
}

GAME::~GAME()
{
}

void GAME::create_bk_img()
{
    bk_img = cv::imread("G:/game_resource/kb1.jpg");
    cv::resize(bk_img, bk_img, cv::Size(3000, 3000));
    if (bk_img.empty())//如果加载不出图片，就加载下面绘制的图片
    {
        bk_img = cv::Mat(bk_img_size, CV_8UC3, cv::Scalar(0, 0, 0, 0));
        // 添加网格线
        for (int x = 0; x < bk_img_size.width; x += 100) {
            cv::line(bk_img, cv::Point(x, 0), cv::Point(x, bk_img_size.height), cv::Scalar(100, 100, 100), 2);
        }
        for (int y = 0; y < bk_img_size.height; y += 100) {
            cv::line(bk_img, cv::Point(0, y), cv::Point(bk_img_size.width, y), cv::Scalar(100, 100, 100), 2);
        }
        // 添加恒星和行星
        cv::Point sun_center(1000, 1000);
        cv::circle(bk_img, sun_center, 50, cv::Scalar(0, 255, 255), -1);

        cv::Point planet_center(1200, 1000);
        cv::circle(bk_img, planet_center, 30, cv::Scalar(255, 100, 0), -1);

        cv::Size planetary_rings_axis(80, 10);
        cv::ellipse(bk_img, planet_center, planetary_rings_axis, 25, 0, 360, cv::Scalar(0, 0, 255), -1);
    }
}

void GAME::handle_input(int key)
{
    switch (key)
    {
    case'w':case'W':
        player.role_move(cv::Point(0, -1),bk_img_size);
        bullet_dir = cv::Point(0, -1);
        break;
    case's':case'S':
        player.role_move(cv::Point(0, 1), bk_img_size);
        bullet_dir = cv::Point(0, 1);
        break;
    case'a':case'A':
        player.role_move(cv::Point(-1, 0), bk_img_size);
        bullet_dir = cv::Point(-1, 0);
        break;
    case'd':case'D':
        player.role_move(cv::Point(1, 0), bk_img_size);
        bullet_dir = cv::Point(1, 0);
        break;
    case'j':case'J':
        //子弹发射代码
        all_music::PlayShoot(); //播发发射子弹音效
        bullet.emplace_back(player.get_role_size(), player.pos, bullet_dir);
        break;
    }
}

void GAME::update()
{
    //更新视口
    cv::Size role_size = player.get_role_size();
    game_viewport_position.x = player.pos.x + role_size.width / 2 - game_viewport_size.width / 2;
    game_viewport_position.y = player.pos.y + role_size.height / 2 - game_viewport_size.height / 2;
    // 视口在地图的边界检查
    game_viewport_position.x = cv::max(0, cv::min(game_viewport_position.x, bk_img_size.width - game_viewport_size.width));
    game_viewport_position.y = cv::max(0, cv::min(game_viewport_position.y, bk_img_size.height - game_viewport_size.height));
    // 更新子弹
    for (auto it = bullet.begin(); it != bullet.end(); )//遍历子弹数组
    {
        it->bullet_move(bk_img_size);//更新当前对象的移动数据
        if (it->life_time == 0)//删除生命周期为0的对象
        {
            it = bullet.erase(it);
        }
        else//如果生命周期不为0，更新迭代器指向下一个对象
        {
            it++;
        }
    }
    // 更新怪物并检测碰撞
    for (auto it = monster.begin(); it != monster.end(); )//遍历怪物数组
    {
        bool monster_deleted = false;  // 标记怪物是否被删除
        it->monster_move(bk_img_size);//更新当前对象的移动数据
        for (auto skill = bullet.begin(); skill != bullet.end(); ) // 检测子弹与怪物碰撞
        {//遍历所有子弹对象是否与第一个怪物碰撞，是就删除当前子弹对象和第一个怪物对象，依次循环
            if (skill->check_c(*it))
            {
                all_music::PlayExplosion();//播放爆炸音效
                skill = bullet.erase(skill);  // 删除子弹
                it = monster.erase(it); // 删除怪物
               monster_deleted = true;  // 标记第一个怪物和子弹发射碰撞已删除，在下面检测第一怪物与玩家的碰撞时外层的迭代器失效了，就不用与玩家检测碰撞而是更新下一个迭代器
                break; // 跳出内层循环
            }
            else
            {
                skill++;
            }
        }
        // 如果怪物未被子弹删除就检测与玩家碰撞，如果怪物已被子弹在上面删除则更新怪物迭代器
        if (!monster_deleted)
        {
            if (it->check_c(player))
            {
                all_music::PlayExplosion();//播放爆炸音效
                it = monster.erase(it); 
                monster_deleted = true;
            }
            else
            {
                it++;
            } // 如果怪物被子弹删除了，it已经更新，不需要再操作
        }
    }
}

void GAME::draw()
{
    //绘制角色
    player.role_draw(bk_img);
    //绘制每个怪物
    for (auto& it : monster)
    {
        it.monster_draw(bk_img);
    }
    //绘制每个子弹
    for (auto& it : bullet)
    {
        it.bullet_draw(bk_img);
    }
    //显示视口
    cv::namedWindow("迷失太空");
    cv::moveWindow("迷失太空", wind_width / 2 - game_viewport_size.width / 2, wind_height/20);
    cv::Mat viewport_frame = bk_img(cv::Rect(game_viewport_position,game_viewport_size)).clone();
    cv::imshow("迷失太空", viewport_frame);
    bk_img_copy.copyTo(bk_img);
}


