#pragma once

#include "PicReader.h"
struct BitString {
	int length;
	int value;
};
class square {
private:
	int WriteByte = 0, BytePos = 7;
	FILE* fp;
	char FileName[9] = { "lena.jpg" };
	BitString Y_DC_Huffman_Table[12];
	BitString Y_AC_Huffman_Table[256];
	BitString CbCr_DC_Huffman_Table[12];
	BitString CbCr_AC_Huffman_Table[256];
	char yData[64];
	char CbData[64];
	char CrData[64]; // 记录色彩空间数据
	int PrevDC_Y = 0, PrevDC_Cb = 0, PrevDC_Cr = 0;
	void writeBitString(int counts, BitString* output);
	void write_2byte(const unsigned short num);
	void write_head();
	void encodeHuffmanTable();
	void computeHuffmanTable(const char* nr_codes, const unsigned char* std_table, BitString* huffman_table);
	void write_byte(int value);
	int GetNumberLength(int number);
	int Getrlelength(int zeros, int length);
	void cleanBitString(BitString* target, int length);
	void encodebit(BitString* rle, int count);
	void encodeFDC(const char* channel, short* result, const unsigned char* Quantization_Table);
	void encoderle(short* channel, BitString* rle_result, int& count);
	void encodeHuffman(int PrezDC, int& outcount, BitString* rle, int& PrevDC, int count, BitString* output, BitString* DC_Huffman_Table, BitString* AC_Huffman_Table);
public:
	int height, width;
	square(int x, int y);
	~square();
	void Getdata(BYTE* data, int coordinate, int length);
	void encodeJPG();
};