#include <iostream>

using namespace std;

class XXL
{
public:
	XXL(int m0=8, int n0=4, int k0=4) {
		M = m0;
		N = n0;
		K = k0;
		matrix = new int*[M];
		for (int i = 0; i < M; i++)
		{
			matrix[i] = new int[N];
		}
	}
	void copy_matrix(int a[][4]) {
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				matrix[i][j] = a[i][j];
			}
		}
	}
	// 输出流<<重载
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
	~XXL() {
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