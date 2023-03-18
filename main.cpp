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
		imread.getData(data, x, y); // ��ȡͼ��RGBA��Ϣ, ����һ���ɱ�8����
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
			{ // ��ǰ���ڿ�������i�е�j��
				element.Getdata(data, i * x * 4 * 8 + j * 8 * 4, x); // �ӵ�ǰ������ȡ8*8����
				// ����õ�YCbCr��������
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