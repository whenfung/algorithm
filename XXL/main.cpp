/************************实现消消乐最优解********************************
算法思想：深度搜索+回溯剪枝

数据结构：
利用map<string, state>记录状态情况，其中
1. string = 消去操作的矩阵的字符串化，可以形成唯一的key。
2. state =  记录一些回溯信息，用一个结构体表示即可。

算法步骤：
1. 每次递归都找出那个时候矩阵的所有可交换点，进行每个点的深度搜索
2. 递归结束条件有两个
	①没有可交换点，回溯
	②剪枝，查看map是否有相同状态但分数更高的，如果有更高的，结束递归进行回溯
	  否则，替换掉分数低的进行下一步递归。
3. 判断顶点周围是否有可以形成消去的条件，可以通过判断整行可否消去或者整列可否消去
4. 每次消去之后上方的方块会掉下来
5. 这时掉下来之后还要多一次判断是否可以自动再消，直到确认不能再消
6. 进入下一次递归之前要把当前状态记录到map上。

优化细节：
1. 字符0（表示空）不能交换，相同颜色交换无意义，交换后无消去情况的话直接返回


**************************讲解结束******************************/

#include <iostream>  //IO流
#include <string>    //图的字符串表示
#include <map>       //记录不重复记录 // 剪枝用
#include <algorithm> //交换操作
#include <time.h>    //统计时间用
#include <vector>    //输出用

using namespace std;

//---------------变量声明部分--------------------//

const int ROW = 8;        //行
const int COLUMN = 4;     //列
int       max_K;          //最大步骤
int       max_score;      //最大得分
string    max_str;        //最大得分的字符串

typedef struct state   
{ 
	int score;           //当前得分
	int K;               //当前步数
	int i, j;            //当前可交换点坐标
	int dir;             //当前交换方向
	string previous;     //上一个状态矩阵
}state;

map<string, state> mymap;   //map存储所有不重复最优记录,如果在递归中发现有相同的key，更新state

//函数声明，第一次编译才能通过
void DFS(string pre_key);                        //对所有可交换点深度遍历
int  is_clear(string& now_key, int i0, int j0);  //对p(i0,j0)进行消去操作
void update_key(string& now_key);                //对消去后的矩阵进行更新
void print(string str);                          //输出函数
void str_to_mat(string str, char mat[][COLUMN]); 
void mat_to_str(char mat[][COLUMN], string & str);
int  get_point(int count);                                 //根据消去的个数判断一步得到的分数，可以统计哪一步最赚的
bool is_exchange(string pre_key, int i, int j, int dir);   //判断p(i,j)是否可以往dir方向交换
void clear(string pre_key, int i0, int j0, int dir);       //核心函数，综合以上函数
int check(string& now_key);                                //消去之后还要判断
													 
//---------------------------定义部分--------------------------------------//
void print(string str) {        //输出状态矩阵
	vector<state> route;
	state s = mymap[str];
	while (s.K > 0)
	{
		route.push_back(s);
		s = mymap[s.previous];
	}
	
	for (int i = route.size()-1; i >= 0; i--)
	{
		for (int j = 0; j < ROW; j++) {
			for (int k = 0; k < COLUMN; k++)
			{
				cout << route[i].previous[j*COLUMN + k] << ' ';
			}
			cout << endl;
		}
		cout << "第"<<route[i].K<<"步，节点(" << route[i].i << "," << route[i].j << ")向" << route[i].dir << "交换,得分为"<<route[i].score<< endl;
	}
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++)
		{
			cout << str[i*COLUMN+j] << ' ';
		}
		cout << endl;
	}
}

void str_to_mat(string str, char mat[][COLUMN]) {
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			mat[i][j] = str[i*COLUMN + j];
		}
	}
}

void mat_to_str(char mat[][COLUMN], string &str) {
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			str[i*COLUMN + j] = mat[i][j];
		}
	}
}

int get_point(int count) {
	if (count == 3) return 1;
	if (count == 4) return 4;
	if (count == 5) return 10;
	return 0;
}

int check(string& now_key) {
	int score = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j = j ++) {
			if (now_key[i*COLUMN+j] != '0' ) {
				score = is_clear(now_key, i, j);
				if (score > 0)
				{
					update_key(now_key);
					return score;
				}
			}
		}
	}
	return 0;
}

//消除操作,只负责消除，不负责上方的方块掉下来的操作
int is_clear(string& now_key, int i0, int j0) {
	//思路：假设可以消，消了再说
	int count = 1;
	int score = 0;
	char mat[ROW][COLUMN];
	str_to_mat(now_key, mat);
	char p = mat[i0][j0];

	int j = j0+1;
	while (j < COLUMN)
	{
		if (mat[i0][j] == p)
		{
			count++;
			j++;
		}
		else
		{
			break;
		}
	}
	j = j0 - 1;
	while (j >= 0) {
		if (mat[i0][j] == p)
		{
			count++;
			j--;
		}
		else
		{
			break;
		}
	}

	if (count > 2)
	{
		mat[i0][j0] = '0';
		j = j0 + 1;
		while (j < COLUMN && mat[i0][j] == p)
		{
			mat[i0][j] = '0';
			j++;
		}
		j = j0 - 1;
		while (j >=0 && mat[i0][j] == p)
		{
			mat[i0][j] = '0';
			j--;
		}
	}
	score += get_point(count);
	//判断列的
	int i = i0 + 1;
	count = 1;
	while (i<ROW)
	{
		if (mat[i][j0] == p)
		{
			count++;
			i++;
		}
		else {
			break;
		}
	}
	i = i0 - 1;
	while (i >= 0) {
		if (mat[i][j0] == p)
		{
			count++;
			i--;
		}
		else {
			break;
		}
	}
	if (count > 2)
	{
		mat[i0][j0] = '0';
		i = i0 + 1;
		while (i < ROW && mat[i][j0] == p)
		{
			mat[i][j0] = '0';
			i++;
		}
		i = i0 - 1;
		while (i >=0 && now_key[i*COLUMN + j0] == now_key[i0*COLUMN + j0])
		{
			mat[i][j0] = '0';
			i--;
		}
	}
	score += get_point(count);
	if (score > 0)
	{
		mat_to_str(mat, now_key);
	}
	return score;
}

void update_key(string& now_key) {  //将中间消去的部分用上面的方块填满
	for (int i = 1; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++)
		{
			if (now_key[i*COLUMN + j] == '0') {
				int k = i;
				while (k > 0)
				{
					swap(now_key[k*COLUMN + j], now_key[(k - 1)*COLUMN + j]);
					k--;
				}
			}
		}
	}
}

//消除操作，如果可以，深度递归，并将消去后的记录保存到map里头。
void clear(string pre_key, int i0, int j0, int dir) {  //pre_key是形参，不影响上一个状态
	//记录状态
	state pre_state = mymap[pre_key];
	state now_state;
	now_state.previous = pre_key;
	now_state.score = pre_state.score;
	now_state.K = pre_state.K + 1;
	now_state.i = i0;
	now_state.j = j0;
	now_state.dir = dir;

	if (now_state.K >= max_K) return;
	//----------开始消去操作-----------------//
	string now_key = pre_key;  //新的状态字符串初始化为上一个状态

	//先将两个点给交换了
	int i1 = i0;
	int j1 = j0; //记录和（i0,j0）交换的点
	if (dir == 1) { 
		j1 = j0 + 1;
		swap(now_key[i0*COLUMN + j0], now_key[i1*COLUMN + j1]); 
	}
	if (dir == 2) {
		i1 = i0 + 1;
		swap(now_key[i0*COLUMN + j0], now_key[i1*COLUMN + j1]);
	}
	//记录消去两个节点附近的区域得到的分数。
	int score = is_clear(now_key,i0,j0);   //这里的now_key传入的是实参，所以会改变
	score += is_clear(now_key, i1, j1);
	if (score > 0) {  //说明是一个有效交换点，更新map，进行下一步DFS查找
		//进行将中空的部分用上面的方块压下来
		update_key(now_key);
		int temp = check(now_key);
		while (temp)
		{
			score += temp;
			temp = check(now_key);
		}
		
		now_state.score = now_state.score + score;
		if (max_score < now_state.score)
		{
			max_score = now_state.score;
			max_str = now_key;
		}
		if (mymap.count(now_key) == 0)  //如果没有该记录，保存该记录,进行递归
		{
			mymap[now_key] = now_state;
			DFS(now_key);
		}
		else  if (mymap[now_key].score < now_state.score) { //如果有该记录，但是比较小，更新并递归
			mymap[now_key] = now_state;
			DFS(now_key);
		}
	}
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
		return true;
	}
	//判断p(i,j)可否向下交换
	if (dir == 2
		&& i+1 < ROW
		&& pre_key[i*COLUMN+j] != '0' 
		&& pre_key[(1+i)*COLUMN + j] != '0'
		&& pre_key[i*COLUMN + j] != pre_key[(1 + i)*COLUMN + j])
	{
		return true;
	}
	return false;
}

void DFS(string pre_key) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (is_exchange(pre_key, i, j, 1)) {  //判断是否可以往右交换
				clear(pre_key, i, j, 1);          //消去操作，如果可以消去就进行往下递归
			}
			if (is_exchange(pre_key, i, j, 2)) {  //判断是否可以往下交换
				clear(pre_key, i, j, 2);          //消去操作，进行交换消去后再深度遍历
			}
		}
	}
}

int main() {
	//-------------------初始化--------------------------

	string str0 = "33433233243413433311343314431232"; //矩阵的字符串表示 
	max_score = 0;                //初始化最大得分
	max_K = 8;

	state state0;                 //初始状态
	state0.previous = "";         //无前状态
	state0.score = 0;             //初始分数为0
	state0.K = 0;                 //初始步数为0
	state0.i = -1;                //-1代表初始化
	state0.j = -1;                
	state0.dir = 0;               //0代表无方向
	mymap[str0] = state0;         //map里面记录原始的矩阵
	
	//------------------------运行-----------------------
	
	clock_t start = clock();
	DFS(str0);
	clock_t end = clock();
	cout << "运行时间为：" << (double)(end - start) << "ms" << endl;

	//------------------------输出-----------------------

	print(max_str);    //输出结果
	getchar();
	return 0;
}
