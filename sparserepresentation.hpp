#ifndef _SPARSEREPRESENTATION_HPP_
#define _SPARSEREPRESENTATION_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Eigen/Dense"

#ifdef _OPENMP

#if defined(__clang__)
#include <omp.h>
#elif defined(__GNUG__) || defined(__GNUC__)
#include <omp.h>
#endif

#endif

using namespace std;
using namespace Eigen;

template<class DataType>
vector<DataType> operator*(vector<DataType> vec, DataType data) {
	/*operator*			数组与数相乘
	*vec				数组（N）
	*data				数
	*/
	int len = vec.size();
	if(!len) {
		cerr << "operator*:eeror." << endl;
		throw runtime_error("operator*:error");
	}
	vector<DataType> result(len, 0.0);
	for(int i = 0; i < len; i++) {
		result[i] = (vec[i] * data);
	}
	return result;
}

//数组减法
template<class DataType>
vector<DataType> operator-(vector<DataType> vec1, vector<DataType> vec2) {
	/*operator-			数组相减（vec1-vec2）
	*vec1				数组（N）
	*vec2				数组（N）
	*/
	if (!vec1.size() || !vec2.size() || vec1.size() != vec2.size()){
		cerr << "operator-:error." << endl;
		throw runtime_error("operator-:error");
	}
	int len = vec1.size();
	vector<DataType> result(len, 0.0);
	for (int i = 0; i < len; i++) {
		result[i] = (vec1[i] - vec2[i]);
	}

	return result;
}

//数组加法
template<class DataType>
vector<DataType> operator+(vector<DataType> vec1, vector<DataType> vec2) {
	/*operator-			数组相减（vec1-vec2）
	*vec1				数组（N）
	*vec2				数组（N）
	*/
	if(!vec1.size() || !vec2.size() || vec1.size() != vec2.size()){
		cerr << "operator+:error." << endl;
		throw runtime_error("operator+:error");
	}
	int len = vec1.size();
	vector<DataType> result(len, 0.0);
	for(int i = 0; i < len; i++) {
		result[i] = (vec1[i] + vec2[i]);
	}

	return result;
}

//矩阵与数组乘法
template<class DataType>
vector<DataType> operator*(vector<vector<DataType>> mat, vector<DataType> vec) {
	/*operator*			矩阵与数组相乘
	*mat				矩阵（N*M）
	*vec				数组（N）
	*/
	int row = mat.size();
	if (!row || row != vec.size()) {
		cerr << "operator*:eeror." << endl;
		throw runtime_error("operator*:error");
	}
	int col = mat[0].size();
	if (!col) {
		cerr << "operator*:eeror." << endl;
		throw runtime_error("operator*:error");
	}

	vector<DataType> result(col, 0.0);
	DataType tmp;
	for(int i = 0; i < col; i++) {
		tmp = 0.0;
		for(int j = 0; j < row; j++) {
			tmp += mat[j][i] * vec[j];
		}
		result[i] = (tmp);
	}
	return result;
}


//"RETRIEVESPARSITY" : "9",
//"RETRIEVEMINRESIDUAL" : "1"

template<class DataType>
class LSparseRepresentation {
public:
	LSparseRepresentation(vector<vector<vector<DataType>>> dicts);
	~LSparseRepresentation();
	//稀疏表示分类（一组特征）
	int SRClassify(vector<DataType>& y, DataType min_residual, int sparsity);
	//稀疏表示分类（多组特征）
	bool SRClassify(vector<vector<DataType>>& y, DataType min_residual, int sparsity, vector<int> &srres);
	
private:
	vector<vector<DataType>> dic;		//存放字典 （vector<DataType>：列）
	vector<int> dicclassnum;			//每类字典的列数
	int	classnum;						//类的个数

private:
	//两个数组内积
	DataType Dot(vector<DataType>& vec1, vector<DataType>& vec2);

	//数组2范数
	DataType Norm(vector<DataType> vec);

	//求解最小二乘问题
	bool solve(vector<vector<DataType>>& phi, vector<DataType>& y, vector<DataType>& x);

	//OMP算法
	bool OrthMatchPursuit(vector<DataType>& y, DataType min_residual, int sparsity, vector<DataType>& x, vector<int>& patch_indices);
};

//构造函数
template<class DataType>
LSparseRepresentation<DataType>::LSparseRepresentation(vector<vector<vector<DataType>>> dicts) {
	classnum = 0;
	dicclassnum.clear();
	dic.clear();

	//convert the dicts to two dimentions
	this->classnum = dicts.size();
	for(auto matrix:dicts) {
		this->dicclassnum.push_back(matrix.size());
		for(auto item:matrix) {
			this->dic.push_back(item);
		}
	}
}

//稀疏表示分类（一组特征）
template<class DataType>
int LSparseRepresentation<DataType>::SRClassify(vector<DataType>& y, DataType min_residual, int sparsity) {
	/*SRClassify		稀疏表示识别
	*y					特征
	*min_residual		最小残差
	*sparsity			稀疏度
	*return：类别:（0, 1，2，3，...） -1：错误
	*/
	int fsize = y.size(); //特征维数
	if (!fsize) return -1; 
	int dcol = dic.size();      //字典的列数
	if (!dcol) return -1; 
	int drow = dic[0].size();   //字典的行数
	if (!drow || drow != fsize) return -1;
	int i, j;
	vector<int> patch_indices;	//稀疏系数的列号
	vector<DataType> coefficient;//稀疏系数

	//OMP求稀疏系数
	if(!OrthMatchPursuit(y, min_residual, sparsity, coefficient, patch_indices)) {
		vector<int>().swap(patch_indices);//释放内存
		vector<DataType>().swap(coefficient);//释放内存
		return -1;
	}

	vector<DataType> x(dcol, 0.0); //稀疏解
	for (i = 0; i < patch_indices.size(); ++i) {
		x[patch_indices[i]] = coefficient[i];
	}

	// vector<int>().swap(patch_indices);
	// vector<DataType>().swap(coefficient);
	
	int result = 0;//分类结果 属于字典对应的类

	int start = 0;//某一类开始位置
	DataType mindist = 100000000;
	for(i = 0; i < classnum; ++i) {
		vector<DataType> tmp(fsize, 0.0);
		for(j = start; j < start + dicclassnum[i]; ++j) {
			if (x[j] != 0.0) {
				tmp = tmp + dic[j] * x[j];		
			}
		}
		DataType dist = Norm(y - tmp);

		// vector<DataType>().swap(tmp);

		if (mindist > dist) {
			mindist = dist;
			result = i;
		}

		start += dicclassnum[i];
	}
	// vector<DataType>().swap(x);

	return result; //result（0，1，2，3，...）

}

//稀疏表示分类（多组特征）
template<class DataType>
bool LSparseRepresentation<DataType>::SRClassify(vector<vector<DataType>>& y,
												 DataType min_residual, int sparsity, 
												 vector<int> &srres){
	/*SRClassify		稀疏表示识别
	*y					特征
	*min_residual		最小残差
	*sparsity			稀疏度
	*srres              排序后的目标索引
	*/
	int i,j;
	int size = y.size();
	vector<int> result(this->classnum, 0);

	vector<int> results(size,0);
	
#pragma omp parallel for
	for (i = 0; i < size; ++i) {
		results[i] = SRClassify(y[i], min_residual, sparsity);
	}

	for(i = 0; i < size; ++i) {
		if (results[i] < 0) {
			return false;
		}
		result[results[i]]++;
	}

	srres.resize(this->classnum);
	for(i = 0; i < this->classnum; ++i) {
		srres[i] = i;
	}

	for(i = this->classnum-1; i >= 0; --i) { //将识别后为0的类别删除
		if(result[i] == 0) {
			result.erase(result.begin() + i);
			srres.erase(srres.begin() + i);
		}
	}

	int resnum = result.size(); //识别结果个数
	//对目标进行排序(递减)
	for(i = 0; i < resnum - 1; ++i) {
		int index = i;
		for(j = i + 1; j < resnum; ++j){
			if(result[index] < result[j]){
				index = j;
			}
		}
		swap(result[index], result[i]);
		swap(srres[index], srres[i]);
	}
	return true;
}

//析构函数
template<class DataType>
LSparseRepresentation<DataType>::~LSparseRepresentation() {
	int size = dic.size();
	for(int i = 0; i < size; ++i)  {
		vector<DataType>().swap(dic[i]);
	}
	vector<vector<DataType>>().swap(dic);
	vector<int>().swap(dicclassnum);
}

//两个数组内积
template<class DataType>
DataType LSparseRepresentation<DataType>::Dot(vector<DataType>& vec1, vector<DataType>& vec2) {
	/*Norm				求两个数组的内积
	*vec1				数组（N）
	*vec2				数组（N）
	*return：DataType
	*/
	if(!vec1.size() || !vec2.size() || vec1.size() != vec2.size()){
		cerr << "Dot:error." << endl;
		throw runtime_error("Dot:error");
	}
	DataType sum = 0;
	int len = vec1.size();
	for(int i = 0; i < len; ++i) {
		sum += vec1[i] * vec2[i];
	}

	return sum;
}

//数组2范数
template<class DataType>
DataType LSparseRepresentation<DataType>::Norm(vector<DataType> vec) {
	/*Norm				求数组的2范数
	*vec				数组（N）
	*return：DataType
	*/
	if (!vec.size()) {
		cerr << "Norm:error." << endl;
		throw runtime_error("Norm:error");
	}
	DataType norm = 0;
	int len = vec.size();
	for(int i = 0; i < len; ++i) {
		norm += vec[i] * vec[i];
	}

	return sqrt(norm);
}

//求解最小二乘问题
template<class DataType>
bool LSparseRepresentation<DataType>::solve(vector<vector<DataType>>& phi, 
											vector<DataType>& y, vector<DataType>& x) {
	/*solve				求解最小二乘问题
	*phi				矩阵（列*行）
	*y					特征
	*x					求解系数
	*/
	int col = phi.size();
	if (col <= 0) return false;
	int row = phi[0].size();
	if (row != y.size() || col != x.size()) return false;
	MatrixXd A(row, col);
	VectorXd b(row);
	for(int i = 0; i < row; ++i) {
		b(i) = y[i];
		for(int j = 0; j < col; ++j)
			A(i, j) = phi[j][i];
	}

	VectorXd result = A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);
	for(int i = 0; i < col; ++i) {
		x[i] = result(i);
	}

	return true;
}

//OMP算法
template<class DataType>
bool LSparseRepresentation<DataType>::OrthMatchPursuit(vector<DataType>& y, 
												DataType min_residual, int sparsity, 
												vector<DataType>& x, vector<int>& patch_indices) {
	/*OrthMatchPursuit	正交匹配追踪算法（OMP）
	*dic				字典（列*行）
	*y					特征
	*min_residual		最小残差
	*sparsity			稀疏度
	*x					返回每个原子对应的系数
	*patch_indices		返回选出的原子序号
	*/
	int fsize = y.size(); //特征维数
	if (!fsize) return false;
	int dcol = dic.size();      //字典的列数
	if (!dcol) return false;
	
	int drow = dic[0].size();   //字典的行数
	if (!drow || drow != fsize) return false;

	vector<DataType> residual(y); //残差
	vector<vector<DataType>> phi; //保存已选出的原子向量
	x.clear();

	DataType max_coefficient;
	unsigned int patch_index;
	vector<DataType> coefficient(dcol,0);
	
	for(;;) {
		max_coefficient = 0;
#pragma omp parallel for shared(coefficient)
		for (int i = 0; i < dcol; ++i) {
			coefficient[i] = (DataType)Dot(dic[i], residual);
		}

		for (int i = 0; i < dcol; ++i) {
			if (fabs(coefficient[i]) > fabs(max_coefficient)) {
				max_coefficient = coefficient[i];
				patch_index = i;
			}
		}

		patch_indices.push_back(patch_index);
		phi.push_back(dic[patch_index]);
		x.push_back(0.0);

		if( !solve(phi, y, x) ) {
			vector<DataType>().swap(residual);
			for (int i = 0; i < phi.size(); ++i) vector<DataType>().swap(phi[i]);
			vector<vector<DataType>>().swap(phi);
			return false;
		}

		residual = y - phi *x;
		DataType res_norm = (DataType)Norm(residual);

		//if (x.size() >= sparsity || res_norm <= min_residual) //根据稀疏个数和残差作为终止条件
		if (x.size() >= sparsity)
			break;
	}

	vector<DataType>().swap(residual);
	for(int i = 0; i < phi.size(); ++i) vector<DataType>().swap(phi[i]);
	vector<vector<DataType>>().swap(phi);

	return true;
}

#endif
