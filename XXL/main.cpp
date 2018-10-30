/*实现消消乐最优解
算法思想：
深度搜索+回溯剪枝

数据结构：
利用map<string, prestate>记录状态情况，其中
1. string = 字符串化 + 下一个交换点的坐标以及交换的方向，这样可以形成唯一的key。
2. perstate = 前一个状态的string 和 当前得到的分数，用一个结构体表示即可。

算法步骤：
1. 每次递归都找出那个时候矩阵的所有可交换点，进行每个点的深度搜索
2. 递归结束条件有两个
	①没有可交换点，回溯
	②剪枝，查看map是否有相同状态但分数更高的，如果有更高的，结束递归进行回溯
	  否则，替换掉分数低的进行下一步递归。
3. 判断顶点周围是否有可以形成消去的条件，可以通过判断整行可否消去或者整列可否消去
4. 每次消去之后上方的方块会掉下来，这时掉下来之后还要多一次判断是否可以自动再消，直到确认不能再消
5. 进入下一次递归之前要把当前状态记录到map上。

优化细节：
1. 字符0（表示空）不能交换，相同颜色交换无意义，交换后无消去情况的话不能交换
2. 深度搜索利用链表来存储可交换点，通过增删控制递归

*/

#include <iostream>   
#include <string>
#include <list>
#include <map>
#include<algorithm>//sort函数包含的头文件

using namespace std;

const int ROW = 8;        //行
const int COLUMN = 4;     //列

typedef struct state   
{ 
	int current_score;   //当前分数
	string previous;     //记录上一个状态矩阵 + 和交换的点
}state;

typedef struct point
{
	int i, j; //坐标
	int dir;   //方向
}point;

map<string, state> mymap;   //map存储所有不重复最优记录,如果在递归中发现有相同的key，更新state

void str_to_mat(string str, char mat[][COLUMN]) {    //字符串转二维字符矩阵
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			mat[i][j] = str[i*COLUMN + j];
		}
	}
}

void mat_to_str(string str, char mat[][COLUMN]) {    //二维字符矩阵转字符串
	for (int i = 0; i < ROW; i++)
	{
		for (int i = 0; i < COLUMN; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{
				str[i*COLUMN + j] = mat[i][j];
			}
		}
	}
}

void print(string str) {        //输出状态矩阵
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++)
		{
			cout << str[i*COLUMN+j] << ' ';
		}
		cout << endl;
	}
}

//判断交换后的点可否消除，如果可以，返回消除的分数，如果不行，返回零，并将消去后的记录保存到map里头。
int clear(string pre_key, int i, int j) {
	int score;
	return score;
}

//判断p(i,j)能否向dir方向交换
bool is_exchange(string pre_key, int i, int j, int dir) {
	//判断p(i,j)可否向右交换
	if (dir == 1 
		&& j+1 < COLUMN 
		&& pre_key[i*COLUMN+j] != '0' 
		&& pre_key[i*COLUMN +j+1] != '0' 
		&& pre_key[i*COLUMN + j] != pre_key[i*COLUMN + j+1]) 
	{

	}
	//判断p(i,j)可否向下交换
	if (dir == 2
		&& i+1 < ROW
		&& pre_key[i*COLUMN+j] != '0' 
		&& pre_key[(1+1)*COLUMN + j] != '0'
		&& pre_key[i*COLUMN + j] != pre_key[(1 + 1)*COLUMN + j])
	{

	}
	return false;
}

void DFS(string pre_key) {
	list<point> list;  //记录所有可交换的节点
	string now_key;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (is_exchange(pre_key, i, j, 1)) {  //判断是否可以往右交换
				now_key = pre_key;   //临时字符串now_key用来处理
				int score = clear(now_key, i, j);
				if (score) {       //判断能否消去并消去
					state now_state;
					now_state.current_score = score;
					now_state.previous = pre_key;

				}
			}
			if (is_exchange(pre_key, i, j, 2)) {  //判断是否可以往下交换

			}
		}
	}
}

int main() {
	//字符串最后三位是 i+j+交换方向，000表示初始状态
	string str0 = "33433233243413433311343314431232000"; //矩阵的字符串表示
	state mat;
	mat.previous = "";
	mat.current_score = 0;
	mymap[str0] = mat;  //map里面记录最原始的矩阵
	print(str0);
	getchar();
	return 0;
}
