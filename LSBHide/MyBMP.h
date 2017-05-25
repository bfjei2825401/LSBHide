//仅限（！）于在刘铎老师的“数据结构与算法综合实践”课堂的学生使用
//严禁传播（包括给其他同学），避免造成没有必要、但是可能非常严重的麻烦
//否则，将直接导致之后我不会再帮大家做这类费力而且招惹麻烦的工作了

// MyBMP.h: interface for the MyBMP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBMP_H__2885B02E_A32F_464B_87F9_A07E5A8FFBAC__INCLUDED_)
#define AFX_MYBMP_H__2885B02E_A32F_464B_87F9_A07E5A8FFBAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class MyBMP
{
public:

	void save(const char*);

	void savelsb(const char*);

	const char* readlsb();

	int length();

    int getWidth();

    int getHeight();

    MyBMP(const char *filename);			//从一个文件名生成一个对象，目前为了处理上的简单性，要求图片宽度是4的倍数
    
	virtual ~MyBMP();						//析构函数
    
	MyBMP(const MyBMP &bmp);				//拷贝构造函数
    
	MyBMP &operator = (const MyBMP &bmp);	//赋值运算符重载
    
	//================================================================================================
   
	BITMAPINFOHEADER getInfoheader();		//获取位图信息数据结构
   
	BITMAPFILEHEADER getFileheader();		//获取位图头文件数据结构
    
	int getnumQuad();						//获取调色板的大小
    
	RGBQUAD *getRGBQUAD();					//获取调色板数据
    
	BYTE *getImagedata();					//获取位图数据
    
	//================================================================================================
    
	void outputFileheader();				//在屏幕上输出位图头文件数据结构信息
    
	void outputInfoheader();				//在屏幕上输出位图信息数据结构
    
	//================================================================================================

protected:
    BITMAPFILEHEADER fileheader;			//位图头文件数据结构

    BITMAPINFOHEADER infoheader;			//位图信息数据结构

    int numQuad;
    //调色板的大小，即RGBQUAD结构的数量，这由BITMAPINFOHEADER结构中biBitCount的值来确定

    RGBQUAD *quad;							//调色板数据，即RGBQUAD结构的数组，数组大小为numQuad

    BYTE *imagedata;
    //位图数据，即调色板数据项的索引，每一个像素需要多少比特由BITMAPINFOHEADER结构中biBitCount的值来确定
};

#endif // !defined(AFX_MYBMP_H__2885B02E_A32F_464B_87F9_A07E5A8FFBAC__INCLUDED_)
