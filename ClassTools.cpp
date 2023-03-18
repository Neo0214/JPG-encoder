#define _CRT_SECURE_NO_WARNINGS
#include "head.h"
#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;
// 设置各类参数表
const unsigned char Y_Quantization_Table[64]{
	3,2,2,3,2,2,3,3,
	3,3,4,3,3,4,5,8,
	5,5,4,4,5,10,7,7,
	6,8,12,10,12,12,11,10,
	11,11,13,14,18,16,13,14,
	17,14,11,11,16,22,16,17,
	19,20,21,21,21,12,15,23,
	24,22,20,24,18,20,21,20,
};
const unsigned char C_Quantization_Table[64]{
	3,4,4,5,4,5,9,5,
	5,9,20,13,11,13,20,20,
	20,20,20,20,20,20,20,20,
	20,20,20,20,20,20,20,20,
	20,20,20,20,20,20,20,20,
	20,20,20,20,20,20,20,20,
	20,20,20,20,20,20,20,20,
	20,20,20,20,20,20,20,20,
};
const unsigned char ZigZag[64]{
	0, 1, 5, 6,14,15,27,28,
	2, 4, 7,13,16,26,29,42,
	3, 8,12,17,25,30,41,43,
	9,11,18,24,31,40,44,53,
	10,19,23,32,39,45,52,54,
	20,22,33,38,46,51,55,60,
	21,34,37,47,50,56,59,61,
	35,36,48,49,57,58,62,63
};
const char Standard_DC_Luminance_NRCodes[] = { 0, 0, 7, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
const unsigned char Standard_DC_Luminance_Values[] = { 4, 5, 3, 2, 6, 1, 0, 7, 8, 9, 10, 11 };
const char Standard_DC_Chrominance_NRCodes[] = { 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
const unsigned char Standard_DC_Chrominance_Values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const char Standard_AC_Luminance_NRCodes[] = { 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };
const unsigned char Standard_AC_Luminance_Values[] =
{
	0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
	0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
	0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
	0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
	0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
	0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
	0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
	0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
	0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
	0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
	0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
	0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
	0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa
};
const char Standard_AC_Chrominance_NRCodes[] = { 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };
const unsigned char Standard_AC_Chrominance_Values[] =
{
	0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
	0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
	0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
	0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
	0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
	0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
	0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
	0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
	0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
	0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
	0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
	0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa
};
int BitStringValuemask[16] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768 };
square::square(int x, int y)
{
	height = y;
	width = x;
	fp = fopen(FileName, "wb");
	if (fp == NULL)
	{
		cerr << "Can't open the file" << endl;
		exit(EXIT_FAILURE);
	}
	encodeHuffmanTable();
	write_head(); // 写入jpg开头部分
}
square::~square()
{
	write_byte(0xff);
	write_byte(0xd9);
	fclose(fp);
}
void square::write_2byte(const unsigned short num)
{
	unsigned short value = ((num >> 8) & 0xff) | ((num & 0xff) << 8);
	fwrite(&value, 1, 2, fp);
}
void square::writeBitString(int counts, BitString* output)
{
	int lengthleft = 0;
	int pos = 0;
	for (; counts > 0; counts--, pos++)
	{
		lengthleft = output[pos].length;
		for (; lengthleft > 0;)
		{
			if (output[pos].value & BitStringValuemask[lengthleft - 1])
			{ // 比对这一位，如果bit==1，获取进newbyte
				WriteByte = WriteByte | BitStringValuemask[BytePos--];
				lengthleft--;
			}
			else
			{
				lengthleft--;
				BytePos--;
			}
			// 对value该位的处理完成
			if (BytePos < 0)
			{ // 已经凑足一个字节
				write_byte(int(WriteByte));
				if (WriteByte == 0xff)
				{ // 不能与0xFFD9混淆
					write_byte(int(0x00));
				}
				WriteByte = 0; // 将该字节写入文件，然后重置
				BytePos = 7; // 将该字节写入文件，然后重置当前位置
			}
		}
	}
}
void square::computeHuffmanTable(const char* nr_codes, const unsigned char* std_table, BitString* huffman_table)
{
	unsigned char pos = 0;
	unsigned short value = 0;
	for (int k = 1; k <= 16; k++)
	{
		for (int j = 1; j <= nr_codes[k - 1]; j++)
		{
			huffman_table[std_table[pos]].value = value;
			huffman_table[std_table[pos]].length = k;
			pos++;
			value++;
		}
		value <<= 1;
	}
}
void square::encodeHuffmanTable()
{
	ZeroMemory(&Y_DC_Huffman_Table, sizeof(Y_DC_Huffman_Table));
	computeHuffmanTable(Standard_DC_Luminance_NRCodes, Standard_DC_Luminance_Values, Y_DC_Huffman_Table);
	ZeroMemory(&Y_AC_Huffman_Table, sizeof(Y_AC_Huffman_Table));
	computeHuffmanTable(Standard_AC_Luminance_NRCodes, Standard_AC_Luminance_Values, Y_AC_Huffman_Table);
	ZeroMemory(&CbCr_DC_Huffman_Table, sizeof(CbCr_DC_Huffman_Table));
	computeHuffmanTable(Standard_DC_Chrominance_NRCodes, Standard_DC_Chrominance_Values, CbCr_DC_Huffman_Table);
	ZeroMemory(&CbCr_AC_Huffman_Table, sizeof(CbCr_AC_Huffman_Table));
	computeHuffmanTable(Standard_AC_Chrominance_NRCodes, Standard_AC_Chrominance_Values, CbCr_AC_Huffman_Table);
}
void square::write_byte(int value)
{
	fwrite(&value, 1, 1, fp);
}
void square::write_head()
{
	// 写入SOI
	write_byte(0xff);
	write_byte(0xD8);

	// 写入APPO
	write_byte(0xff);
	write_byte(0xe0);

	write_byte(0x00);		
	write_byte(0x10);

	write_byte('J');
	write_byte('F');
	write_byte('I');
	write_byte('F');
	write_byte('\0');

	write_byte(0x01);
	write_byte(0x01);
	write_byte(0x00); // 密度单位

	write_byte(0x00);
	write_byte(0x01);
	write_byte(0x00);
	write_byte(0x01);

	write_byte(0x00);
	write_byte(0x00); // 无缩略图
	// 写入DQT
	write_byte(0xff);
	write_byte(0xdb);
	write_byte(0x00);
	write_byte(0x84);
	write_byte(0x00);
	fwrite(Y_Quantization_Table, 1, 64, fp);
	write_byte(0x01);	
	fwrite(C_Quantization_Table, 1, 64, fp);

	//SOFO
	write_byte(0xff);
	write_byte(0xc0);
	write_byte(0x00);
	write_byte(0x11);
	write_byte(0x08);
	write_2byte(height & 0xffff);
	write_2byte(width & 0xffff);
	write_byte(0x03);		

	write_byte(0x01);				
	write_byte(0x11);				
	write_byte(0x00);				

	write_byte(2);				
	write_byte(0x11);				
	write_byte(1);				

	write_byte(3);				
	write_byte(0x11);				
	write_byte(1);				

	//DHT
	write_2byte(0xFFC4);		
	write_2byte(0x01A2);		
	write_byte(0);			
									
									
	fwrite(Standard_DC_Luminance_NRCodes, 1, sizeof(Standard_DC_Luminance_NRCodes), fp);
	fwrite(Standard_DC_Luminance_Values, 1, sizeof(Standard_DC_Luminance_Values), fp);		
	write_byte(0x10);			

	fwrite(Standard_AC_Luminance_NRCodes, 1, sizeof(Standard_AC_Luminance_NRCodes), fp);
	fwrite(Standard_AC_Luminance_Values,1, sizeof(Standard_AC_Luminance_Values), fp); 
	write_byte(0x01);			
	fwrite(Standard_DC_Chrominance_NRCodes,1, sizeof(Standard_DC_Chrominance_NRCodes), fp);
	fwrite(Standard_DC_Chrominance_Values,1, sizeof(Standard_DC_Chrominance_Values), fp);
	write_byte(0x11);			
	fwrite(Standard_AC_Chrominance_NRCodes, 1,sizeof(Standard_AC_Chrominance_NRCodes), fp);
	fwrite(Standard_AC_Chrominance_Values, 1,sizeof(Standard_AC_Chrominance_Values), fp);

	//SOS
	write_2byte(0xFFDA);		
	write_2byte(12);			
	write_byte(3);			

	write_byte(1);			
	write_byte(0);			
									
	write_byte(2);			
	write_byte(0x11);			

	write_byte(3);			
	write_byte(0x11);			

	write_byte(0);			
	write_byte(0x3F);			
	write_byte(0);			

}
void square::cleanBitString(BitString* target, int length)
{
	for (int i = 0; i < length; i++)
		target[i] = { 0,0 };
}
void square::encoderle(short* channel, BitString* rle_result, int& count)
{
	cleanBitString(rle_result, 128);
	int i, total = 0, length = 0;
	int pos = 0;
	count = 0;
	for (i = 1; i < 64; i++)
	{
		if (channel[i] != 0)
			total++;
	}
	for (i = 1; i < 64; i++)
	{
		if (total == 0)
		{
			rle_result[pos].length = 0;
			rle_result[pos].value = 0; // EOB
			count++;
			break;
		}
		if (channel[i] == 0)
		{
			length++;
			if (length == 15)
			{
				rle_result[pos].length = 15;
				rle_result[pos].value = channel[i + 1];
				pos++;
				length = 0;
				if (channel[i + 1] != 0)
					total--;
				i++;
				count++;
			}
		}
		else
		{
			rle_result[pos].length = length;
			rle_result[pos].value = channel[i];
			length = 0;
			pos++;
			total--;
			count++;
		}
	}
}
int square::GetNumberLength(int number)
{
	number = number >= 0 ? number : -number;
	int counts = 0;
	while (number != 0)
	{
		counts++;
		number = number >> 1;
	}
	return counts;
}
int square::Getrlelength(int zeros, int length)
{
	int result = 0;
	result = zeros << 4;
	result = result + length;
	return result;
}
void square::encodebit(BitString* rle, int count)
{
	int pos = 0;
	int zero_counts = 0;
	int length = 0;
	for (pos = 0; pos < count; pos++)
	{
		if (rle[pos].length == 15 && rle[pos].value == 0)
			continue;
		zero_counts = rle[pos].length; // 零的个数是rle存储的长度值
		length = GetNumberLength(rle[pos].value); // 获取占位长度
		// 获取转换后的value值
		rle[pos].length = Getrlelength(zero_counts, length);
		// value值不变
	}
	// 函数完成后，rle内存储的是length=0x**，value为bit码
	// EOB部分不处理，但原count仍记录了这一信息
}
void square::encodeHuffman(int PrezDC, int& outcount, BitString* rle, int& PrevDC, int count, BitString* output, BitString* DC_Huffman_Table, BitString* AC_Huffman_Table)
{
	int pos = 0;
	// DC编码
	int DcDiff = 0;
	DcDiff = PrezDC - PrevDC;
	PrevDC = PrezDC;
	if (DcDiff == 0)
	{
		output[0] = DC_Huffman_Table[0];
		outcount++;
		pos = 1;
	}
	else
	{
		outcount += 2;
		pos = 2;
		output[0] = DC_Huffman_Table[GetNumberLength(DcDiff)];
		output[1] = { GetNumberLength(DcDiff),DcDiff >= 0 ? DcDiff : ((1 << GetNumberLength(DcDiff)) + DcDiff - 1) };
	}
	// AC编码
	int pos_inrle = 0;
	for (; count > 0; count--)
	{
		if (rle[pos_inrle].length == 0 && rle[pos_inrle].value == 0)
		{
			outcount++;
			output[pos++] = AC_Huffman_Table[0x00];
			break;
		}
		if (rle[pos_inrle].length == 15 && rle[pos_inrle].value == 0)
		{
			outcount++;
			output[pos++] = AC_Huffman_Table[0xf0];
			pos_inrle++;
			continue;
		}
		output[pos++] = AC_Huffman_Table[rle[pos_inrle].length];
		output[pos++] = { GetNumberLength(rle[pos_inrle].value),rle[pos_inrle].value>=0?rle[pos_inrle].value: ((1 << GetNumberLength(rle[pos_inrle].value)) + rle[pos_inrle].value - 1) };
		pos_inrle++;
		outcount += 2;
	}
	
}
void square::encodeJPG()
{
	short yZigZag[64], CbZigZag[64], CrZigZag[64];
	BitString rle[128], output[128];
	int count = 0, outcount = 0;
	// 首先处理Y
	encodeFDC(yData, yZigZag, Y_Quantization_Table); // FDC，量化，蛇形
	encoderle(yZigZag, rle, count); // 计算rle,共count个有效结果!!DC不进行rle处理
	encodebit(rle, count); // rle结果进行bit编码！！DC仍在外，未处理
	encodeHuffman(yZigZag[0], outcount, rle, PrevDC_Y, count, output, Y_DC_Huffman_Table, Y_AC_Huffman_Table);
	writeBitString(outcount, output);
	// 然后处理Cb
	outcount = 0;
	encodeFDC(CbData, CbZigZag, C_Quantization_Table);
	encoderle(CbZigZag, rle, count);
	encodebit(rle, count);
	encodeHuffman(CbZigZag[0], outcount, rle, PrevDC_Cb, count, output, CbCr_DC_Huffman_Table, CbCr_AC_Huffman_Table);
	writeBitString(outcount, output);
	// 最后处理Cr
	outcount = 0;
	encodeFDC(CrData, CrZigZag, C_Quantization_Table);
	encoderle(CrZigZag, rle, count);
	encodebit(rle, count);
	encodeHuffman(CrZigZag[0], outcount, rle, PrevDC_Cr, count, output, CbCr_DC_Huffman_Table, CbCr_AC_Huffman_Table);
	writeBitString(outcount, output);
}
void square::encodeFDC(const char* channel, short* result, const unsigned char* Quantization_Table)
{
	const float PI = 3.1415926f;
	int v, u, x, y;
	float alpha_u, alpha_v,tmp;
	for (v = 0; v < 8; v++)
	{
		for (u = 0; u < 8; u++)
		{
			alpha_u = (u == 0) ? 1 / sqrt(8.0f) : 0.5f;
			alpha_v = (v == 0) ? 1 / sqrt(8.0f) : 0.5f;
			tmp = 0;
			for (x = 0; x < 8; x++)
			{
				for (y = 0; y < 8; y++)
				{
					float data = channel[y * 8 + x];

					data *= cos((2 * x + 1) * u * PI / 16.0f);
					data *= cos((2 * y + 1) * v * PI / 16.0f);
					tmp += data;
				}
			}
			tmp *= alpha_u * alpha_v / Y_Quantization_Table[ZigZag[v * 8 + u]];
			result[ZigZag[v * 8 + u]] = short(round(tmp));
		}
	}
	
}
void square::Getdata(BYTE* data, int coordinate, int length)
{ // 从当前位置读取一个8*8块
	int i, j, coordinate_now;
	unsigned short r, g, b;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 32; j += 4)
		{
			// 当前位置data[coordinate+i*length*4+j]
			coordinate_now = coordinate + i * length * 4 + j;
			r = data[coordinate_now];
			g = data[coordinate_now + 1];
			b = data[coordinate_now + 2];
			yData[i * 8 + j / 4] = (char)(0.299f *r + 0.587f * g + 0.114f * b - 128);
			CbData[i * 8 + j / 4] = (char)(-0.1687f * r - 0.3313f * g + 0.5f * b);
			CrData[i * 8 + j / 4] = (char)(0.5f * r - 0.4187f * g - 0.0813f * b);
		}
	}
}
