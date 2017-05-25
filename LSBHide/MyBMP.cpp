// MyBMP.cpp: implementation of the MyBMP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include "MyBMP.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyBMP::MyBMP(const char *filename)
{
    //从一个文件名生成一个对象，目前为了处理上的简单性，要求图片宽度是4的倍数

	fstream readfile(filename, ios::binary|ios::in);

    if (!readfile)
    {
        cout << "Fail to open." << endl;
        exit(1);
    }
    else
    {
        readfile.read((char*)(&fileheader), sizeof(fileheader));//读取位图头文件数据结构
        readfile.read((char*)(&infoheader), sizeof(infoheader));//读取位图信息数据结构

        if (infoheader.biBitCount < 24)							//判断是否有调色板，如果有的话读取调色板
        {
            //表示颜色查找表中表的行数 即一共多少种颜色（1位代表2种颜色，biBitCount位可以表示2^biBitCount种颜色）
            // 1<<x = 2^x
            numQuad = 1 << infoheader.biBitCount;
            quad = new RGBQUAD[numQuad];
            readfile.read((char*)(quad), sizeof(RGBQUAD)*numQuad);
        }
        else
        {
            quad = NULL;
            numQuad = 0;
        }

        // numPixel ：所需的字节数 = 总像素数 * 每个像素所需字节数
        // 每个像素所需字节数 = 每个像素所需的位数 / 8
		// 总像素数 = 图片扩展后的宽度 * 图片高度
		// 图片扩展后的宽度 = 不小于图片实际宽度的最小的4 的倍数
		int biWidthEx = int((infoheader.biWidth + 3) / 4) * 4;
        int numPixel = infoheader.biHeight * biWidthEx * infoheader.biBitCount / 8;
        imagedata = new BYTE[numPixel];
        readfile.read((char*)(imagedata), numPixel);						//读取位图数据
    }

    readfile.close();
}

MyBMP::MyBMP(const MyBMP &bmp)
{
    //拷贝构造函数
    fileheader = bmp.fileheader;
    infoheader = bmp.infoheader;
    numQuad = bmp.numQuad;

    quad = new RGBQUAD[numQuad];
    memcpy(quad, bmp.quad, sizeof(RGBQUAD) * numQuad);

    int numPixel = infoheader.biHeight * infoheader.biWidth * infoheader.biBitCount / 8;
    imagedata = new BYTE[numPixel];
    memcpy(imagedata, bmp.imagedata, numPixel);
}

MyBMP &MyBMP::operator =(const MyBMP &bmp)
{
    //赋值运算符重载
    fileheader = bmp.fileheader;
    infoheader = bmp.infoheader;
    numQuad = bmp.numQuad;

    quad = new RGBQUAD[numQuad];
    memcpy(quad, bmp.quad, sizeof(RGBQUAD) * numQuad);

    int numPixel = infoheader.biHeight * infoheader.biWidth * infoheader.biBitCount / 8;
    imagedata = new BYTE[numPixel];
    memcpy(imagedata, bmp.imagedata, numPixel);
    return *this;
}

MyBMP::~MyBMP()
{
    if (numQuad > 0)
        delete []quad;
    delete []imagedata;
}

BITMAPFILEHEADER MyBMP::getFileheader()
{
    //此方法功能为获取位图信息数据结构

    BITMAPFILEHEADER r = fileheader;
    return r;
}

BITMAPINFOHEADER MyBMP::getInfoheader()
{
    //此方法功能为获取位图信息数据结构

    BITMAPINFOHEADER r = infoheader;
    return r;
}

int MyBMP::getnumQuad()
{
    //此方法功能为获取调色板的大小

    return numQuad;
}

RGBQUAD *MyBMP::getRGBQUAD()
{
    //此方法功能为获取调色板数据

    if (numQuad > 0)
    {
        RGBQUAD *r;
        r = new RGBQUAD[numQuad];
        memcpy(r, quad, sizeof(RGBQUAD) *numQuad);
        return r;
    }
    else
        return NULL;
}

BYTE *MyBMP::getImagedata()
{
    //此方法功能为获取位图数据

    BYTE *r;
    int numPixel = infoheader.biHeight * infoheader.biWidth * infoheader.biBitCount / 8;
    r = new BYTE[numPixel];
    memcpy(r, imagedata, sizeof(BYTE) * numPixel);
    return r;
}

void MyBMP::outputFileheader()
{
    //此方法功能为输出位图信息数据结构

    cout << "该图片的：" << endl;
    cout << "    文件的类型(bfType)是：" << fileheader.bfType << endl;
    cout << "    文件大小(bfSize)是：" << fileheader.bfSize << endl;
    cout << "    保留区一(bfReserved1)是：" << fileheader.bfReserved1 << endl;
    cout << "    保留区二(bfReserved2)是：" << fileheader.bfReserved2 << endl;
    cout << "    位图文件的偏移地址(bfOffBits)是：" << fileheader.bfOffBits << endl;
}

void MyBMP::outputInfoheader()
{
    //此方法功能为输出位图信息数据结构

    cout << "该图片的：" << endl;
    cout << "    本结构所占用字节数(biSize)是（应该是40）：" << infoheader.biSize << endl;
    cout << "    位图宽度的像素数(biWidth)是：" << infoheader.biWidth << endl;
    cout << "    位图高度的像素数(biHeight)是：" << infoheader.biHeight << endl;
    cout << "    目标设备的级别(biPlanes)，必须为1：" << infoheader.biPlanes << endl;
    cout << "    表示每个像素时所需的位数(biBitCount)是：" << infoheader.biBitCount << endl;
    cout << "    位图压缩类型(biCompression)是：" << infoheader.biCompression << endl;
    cout << "    位图的字节数(biSizeImage)是（当不压缩时可设置为0）：" << infoheader.biSizeImage << endl;
    cout << "    位图的水平分辨率，即水平方向每米像素数(biXPelsPerMeter)是（可设为0）：" << infoheader.biXPelsPerMeter << endl;
    cout << "    位图的垂直分辨率，即垂直方向每米像素数(biYPelsPerMeter)是（可设为0）：" << infoheader.biYPelsPerMeter << endl;
    cout << "    实际使用的调色板中的颜色数(biClrUsed)是（默认为0）是：" << infoheader.biClrUsed << endl;
    cout << "    显示过程中重要的颜色数(biClrImportant)是（默认为0）是：" << infoheader.biClrImportant << endl;
}

int MyBMP::getHeight()
{
    return infoheader.biHeight;
}

int MyBMP::getWidth()
{
    return infoheader.biWidth;
}


int MyBMP::length()
{
    return infoheader.biHeight * infoheader.biWidth * infoheader.biBitCount / 8;
}

void MyBMP::save(const char* filename)
{
	fstream writefile(filename, ios::binary|ios::out);
	writefile.write((char*)(&fileheader), sizeof(fileheader));//读取位图头文件数据结构
	writefile.write((char*)(&infoheader), sizeof(infoheader));//读取位图头文件数据结构
	//int biWidthEx = int((infoheader.biWidth + 3) / 4) * 4;
	int biWidthEx = infoheader.biWidth;
	int numPixel = infoheader.biHeight * biWidthEx * infoheader.biBitCount / 8;
	writefile.write((char*)(imagedata), numPixel);						//读取位图数据
	writefile.close();
}

void MyBMP::savelsb(const char *s)
{
	char *str = new char[strlen(s) + 1];
    memcpy(str, s, sizeof(char)*strlen(s));
	str[strlen(s)] = '\0';
	int len = strlen(str);
	int count = 0;
	for (int i = 0; i < len + 1; i++) {
		for (int j = 7; j >= 0; j--) {
			char sign = ((str[i] >> j) & (char)0x1);
			imagedata[count] &= ~(char)(0x1);
			imagedata[count++] |= sign;
		}
	}
}

const char* MyBMP::readlsb()
{
	int len = length();
	int count = 0;
	int datacount = 0;
	char* data = new char[len];

	while (count < len) {
		char c = (char)0;
		for (int i = 7; i >= 0 && count < len; i--) {
			char rd = imagedata[count++] & (char)0x1;
			rd <<= i;
			c |= rd;
		}
		data[datacount++] = c;
		if (c == '\0') {
			break;
		}
	}
	return data;
}