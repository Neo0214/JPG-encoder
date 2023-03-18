#include <iostream>
#include "head.h"
#include "PicReader.h"
using namespace std;
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Wrong parament number!" << endl;
		return 0;
	}
	UINT x, y;
	BYTE* data = nullptr;
	if (!strcmp(argv[1], "-compress"))
	{
		PicReader imread;
		imread.readPic(argv[2]);
		imread.getData(data, x, y); // 获取图像RGBA信息, 长宽一定可被8整除
		if (x % 8 != 0 || y % 8 != 0)
		{
			cerr << "Pixel number wrong." << endl;
			exit(EXIT_FAILURE);
		}
		int i, j, length = y / 8, width = x / 8;
		square element(x, y);
		for (i = 0; i < length; i++)
		{
			for (j = 0; j < width; j++)
			{ // 当前所在块区：第i行第j列
				element.Getdata(data, i * x * 4 * 8 + j * 8 * 4, x); // 从当前块区获取8*8数据
				// 将获得的YCbCr继续处理
				element.encodeJPG();
			}
		}
		
	}
	else if (!strcmp(argv[1], "-read"))
	{
		PicReader imread;
		imread.readPic(argv[2]);
		imread.getData(data, x, y);
		imread.showPic(data, x, y);
	}
	else
	{
		cerr << "Wrong parament element" << endl;
		return 0;
	}
	return 0;
}