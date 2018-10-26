#include <iostream>
#include <vector>

using namespace std;

class Point
{
public:
	int x, y;  //记录可交换的点的坐标，说白了就是二维数组的下标
	int direction;  //交换方向，只有下和右，因为从左上角开始的
	Point(int x0 = 0, int y0 = 0, int dir = 0){
		x = x0;
		y = y0;
		direction = dir;
	}
};

class XXL
{
public:
	XXL(int m0=8, int n0=4, int k0=4) {  //构造函数
		M = m0;
		N = n0;
		K = k0;
		score = 0;
		matrix = new int *[M];
		for (int i = 0; i < M; i++)
		{
			matrix[i] = new int[N];
		}
	}
	void copy_matrix(int a[][4]) {     //赋值函数
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				matrix[i][j] = a[i][j];
			}
		}
	}
	// 输出流<<重载，因为friend才可以访问XXL里面的私密数据
	friend ostream & operator<<(ostream &out, const XXL& xxl) {
		for (int i = 0; i < xxl.M; i++)
		{
			for (int j = 0; j < xxl.N; j++)
			{
				out << xxl.matrix[i][j] << ' ';
			}
			out << endl;
		}
		return out;
	}

	bool find_commutative_point(int i, int j, int dir, int matrix[][4]) {  //判断该点交换后有无结果
		if (dir == 1) {  //向右交换
			if (j+1 <N && matrix[i][j+1] > 0) //如果可交换
			{
				if (j + 3 < N) { //右边至少得有两个相同的
					if (matrix[i][j] == matrix[i][j+2] && matrix[i][j] == matrix[i][j+3])
					{
						return true;  //行有三个或以上，可交换
					}
				}
				if (i + 2 < M)
				{
					if (matrix[i][j] == matrix[i + 1][j + 1] && matrix[i][j] == matrix[i + 2][j + 1]) {
						return true; //列有三个或以上，可交换
					}
				}
			}
		}
		else  //向下交换
		{
			if (i + 1 < M && matrix[i + 1][j] > 0) { //如果可交换
				if (i + 3 < M) { //下边至少得有两个相同的
					if (matrix[i][j] == matrix[i + 2][j] && matrix[i][j] == matrix[i + 3][j]) {
						return true;  //交换后列满足条件
					}
				}
				if (j + 2 < N) {

				}
			}
		}
		return false;
	}

	void xiao(int m[][4]) {
		vector<Point> points;  //记录当前可交换的点
		int matrix[8][4];      //进行下一步时记录当前状态
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++)
			{
				matrix[i][j] = m[i][j];
			}
		}

		for (int i = 0; i < M; i++) //将所有可交换点找出来，然后进行深度搜索
		{
			for (int j = 0; j < N; j++)
			{
				if (matrix[i][j] > 0) { //进行右交换和下交换，判断是否可以进行消消乐
					if (find_commutative_point(i, j, 1, matrix)) {
						points.push_back(Point(i,j,1));
					}
					if (find_commutative_point(i,j,2, matrix))
					{
						points.push_back(Point(i, j, 2));
					}
				}
			}
		}
		for (int i = 0; i < points.size(); i++) {
			xiao(matrix);
		}
	}

	~XXL() {  //销毁动态数组
		for (int i = 0; i < M; i++)
		{
			delete []matrix[i];
		}
		delete []matrix;
	}

private:
	int M;        //行
	int N;        //列
	int K;        //种类
	int **matrix; //消消乐矩阵
	int score;    //当前得分
};

int main() {
	int m = 8;
	int n = 4;
	int k = 4;
	int matrix[8][4] = {
		{3, 3, 4, 3},
		{3, 2, 3, 3},
		{2, 4, 3, 4},
		{1, 3, 4, 3},
		{3, 3, 1, 1},
		{3, 4, 3, 3},
		{1, 4, 4, 3},
		{1, 2, 3, 2},
	};
	XXL xxl(m, n, k);
	xxl.copy_matrix(matrix);
	cout << xxl;
	getchar();
	return 0;
}