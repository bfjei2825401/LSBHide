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
    //��һ���ļ�������һ������ĿǰΪ�˴����ϵļ��ԣ�Ҫ��ͼƬ�����4�ı���

	fstream readfile(filename, ios::binary|ios::in);

    if (!readfile)
    {
        cout << "Fail to open." << endl;
        exit(1);
    }
    else
    {
        readfile.read((char*)(&fileheader), sizeof(fileheader));//��ȡλͼͷ�ļ����ݽṹ
        readfile.read((char*)(&infoheader), sizeof(infoheader));//��ȡλͼ��Ϣ���ݽṹ

        if (infoheader.biBitCount < 24)							//�ж��Ƿ��е�ɫ�壬����еĻ���ȡ��ɫ��
        {
            //��ʾ��ɫ���ұ��б������ ��һ����������ɫ��1λ����2����ɫ��biBitCountλ���Ա�ʾ2^biBitCount����ɫ��
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

        // numPixel ��������ֽ��� = �������� * ÿ�����������ֽ���
        // ÿ�����������ֽ��� = ÿ�����������λ�� / 8
		// �������� = ͼƬ��չ��Ŀ�� * ͼƬ�߶�
		// ͼƬ��չ��Ŀ�� = ��С��ͼƬʵ�ʿ�ȵ���С��4 �ı���
		int biWidthEx = int((infoheader.biWidth + 3) / 4) * 4;
        int numPixel = infoheader.biHeight * biWidthEx * infoheader.biBitCount / 8;
        imagedata = new BYTE[numPixel];
        readfile.read((char*)(imagedata), numPixel);						//��ȡλͼ����
    }

    readfile.close();
}

MyBMP::MyBMP(const MyBMP &bmp)
{
    //�������캯��
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
    //��ֵ���������
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
    //�˷�������Ϊ��ȡλͼ��Ϣ���ݽṹ

    BITMAPFILEHEADER r = fileheader;
    return r;
}

BITMAPINFOHEADER MyBMP::getInfoheader()
{
    //�˷�������Ϊ��ȡλͼ��Ϣ���ݽṹ

    BITMAPINFOHEADER r = infoheader;
    return r;
}

int MyBMP::getnumQuad()
{
    //�˷�������Ϊ��ȡ��ɫ��Ĵ�С

    return numQuad;
}

RGBQUAD *MyBMP::getRGBQUAD()
{
    //�˷�������Ϊ��ȡ��ɫ������

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
    //�˷�������Ϊ��ȡλͼ����

    BYTE *r;
    int numPixel = infoheader.biHeight * infoheader.biWidth * infoheader.biBitCount / 8;
    r = new BYTE[numPixel];
    memcpy(r, imagedata, sizeof(BYTE) * numPixel);
    return r;
}

void MyBMP::outputFileheader()
{
    //�˷�������Ϊ���λͼ��Ϣ���ݽṹ

    cout << "��ͼƬ�ģ�" << endl;
    cout << "    �ļ�������(bfType)�ǣ�" << fileheader.bfType << endl;
    cout << "    �ļ���С(bfSize)�ǣ�" << fileheader.bfSize << endl;
    cout << "    ������һ(bfReserved1)�ǣ�" << fileheader.bfReserved1 << endl;
    cout << "    ��������(bfReserved2)�ǣ�" << fileheader.bfReserved2 << endl;
    cout << "    λͼ�ļ���ƫ�Ƶ�ַ(bfOffBits)�ǣ�" << fileheader.bfOffBits << endl;
}

void MyBMP::outputInfoheader()
{
    //�˷�������Ϊ���λͼ��Ϣ���ݽṹ

    cout << "��ͼƬ�ģ�" << endl;
    cout << "    ���ṹ��ռ���ֽ���(biSize)�ǣ�Ӧ����40����" << infoheader.biSize << endl;
    cout << "    λͼ��ȵ�������(biWidth)�ǣ�" << infoheader.biWidth << endl;
    cout << "    λͼ�߶ȵ�������(biHeight)�ǣ�" << infoheader.biHeight << endl;
    cout << "    Ŀ���豸�ļ���(biPlanes)������Ϊ1��" << infoheader.biPlanes << endl;
    cout << "    ��ʾÿ������ʱ�����λ��(biBitCount)�ǣ�" << infoheader.biBitCount << endl;
    cout << "    λͼѹ������(biCompression)�ǣ�" << infoheader.biCompression << endl;
    cout << "    λͼ���ֽ���(biSizeImage)�ǣ�����ѹ��ʱ������Ϊ0����" << infoheader.biSizeImage << endl;
    cout << "    λͼ��ˮƽ�ֱ��ʣ���ˮƽ����ÿ��������(biXPelsPerMeter)�ǣ�����Ϊ0����" << infoheader.biXPelsPerMeter << endl;
    cout << "    λͼ�Ĵ�ֱ�ֱ��ʣ�����ֱ����ÿ��������(biYPelsPerMeter)�ǣ�����Ϊ0����" << infoheader.biYPelsPerMeter << endl;
    cout << "    ʵ��ʹ�õĵ�ɫ���е���ɫ��(biClrUsed)�ǣ�Ĭ��Ϊ0���ǣ�" << infoheader.biClrUsed << endl;
    cout << "    ��ʾ��������Ҫ����ɫ��(biClrImportant)�ǣ�Ĭ��Ϊ0���ǣ�" << infoheader.biClrImportant << endl;
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
	writefile.write((char*)(&fileheader), sizeof(fileheader));//��ȡλͼͷ�ļ����ݽṹ
	writefile.write((char*)(&infoheader), sizeof(infoheader));//��ȡλͼͷ�ļ����ݽṹ
	//int biWidthEx = int((infoheader.biWidth + 3) / 4) * 4;
	int biWidthEx = infoheader.biWidth;
	int numPixel = infoheader.biHeight * biWidthEx * infoheader.biBitCount / 8;
	writefile.write((char*)(imagedata), numPixel);						//��ȡλͼ����
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