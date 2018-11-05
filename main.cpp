#include<iostream>
#include<string>
#include<sstream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

//�����
//��
double ker1[3][3] = {0,1,0,
                    1,1,1,
                    0,1,0};
double ker2[5][5] = {1,0,0,0,0,
                    0,1,0,0,0,
                    0,0,1,0,0,
                    0,0,0,1,0,
                    0,0,0,0,1};
double ker3[3][3] = {-1,-1,-1,
                    -1, 8,-1,
                    -1,-1,-1};
double ker4[3][3] = {-1,-1,-1,
                    -1, 9,-1,
                    -1,-1,-1};
double ker5[3][3] = {-1,-1, 0,
                    -1, 0, 1,
                     0, 1, 1};
double ker6[5][5] = {0.0120,0.1253,0.2736,0.1253,0.0120,
                     0.1253,1.3054,2.8514,1.3054,0.1253,
                     0.2736,2.8514,6.2279,2.8514,0.2736,
                     0.1253,1.3054,2.8514,1.3054,0.1253,
                     0.0120,0.1253,0.2736,0.1253,0.0120};

///���ͼ����
/**
ͼ��ԭ·��  ͼ��Ŀ��·��  ѡ������ ����˳���
*/
void juanji(char* srcPath,char* dstPath,int s,int l){
    //ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen;
	//ͼ�񲨶���
	int i, bandNum;
    //�˻��������˻�֮��
    double z,t,*temp;
	//����ͼ��
	GDALDataset* poSrcDS;
	//���ͼ��
	GDALDataset* poDstDS;
	//ͼ���ڴ�洢
	GByte *buffTmp[2];
	//ע������
	GDALAllRegister();
	//��ͼ��
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
  	//��ȡͼ���ȣ��߶ȣ�������
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();
	//�����ȡ�Ľ��
	cout << "IMG  X Length:" << imgXlen << endl;
	cout << "IMG  Y Length:" << imgYlen << endl;
	cout << "Band Number:" << bandNum << endl;

	//����ͼ��Ŀ�Ⱥ͸߶ȷ����ڴ�
	for(i = 0 ; i < 2;i++){
        buffTmp[i] = (GByte *) CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	}
	//�������ͼ��
    poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
                            dstPath,imgXlen,imgYlen,bandNum,GDT_Byte,NULL);
    //��ʼ��
    for(i = 0 ; i < imgYlen ; i++){
        for(int j = 0;j < imgXlen ;j++){
            buffTmp[0][i * imgXlen + j] = buffTmp[1][i * imgXlen + j] = 0;
        }
    }
    //�ֲ��δ���ͼ��
    for(i = 1 ; i <= bandNum ; i++){
        //ȡ���ò��ε�����
        poSrcDS->GetRasterBand(i)->RasterIO(GF_Read,
                                               0,0,imgXlen,imgYlen,buffTmp[0],imgXlen,imgYlen,GDT_Byte,0,0);
        //������
        for(int j = l / 2;j < imgYlen - 1;j++){
            for(int k = l / 2; k < imgXlen - 1 ; k++){
                //�Ը������ؼ���
                t = 0;
                z = 1;
                for(int q = 0;q < l;q++){
                    for(int p = 0 ; p < l;p++){
                        //ѡ���
                        if(s == 1){
                            temp = ker1[0];
                            z = 0.2;
                        }
                        if(s == 2){
                            temp = ker2[0];
                            z = 0.2;
                        }
                        if(s == 3){
                            temp = ker3[0];
                        }
                        if(s == 4){
                            temp = ker4[0];
                        }
                        if(s == 5){
                            temp = ker5[0];
                        }
                        if(s == 6){
                            temp = ker6[0];
                            z = 0.04;
                        }
                        t +=(double) buffTmp[0][( j + q - l/2) * imgXlen + ( k + p - l/2)] * temp[q*l+p];
                    }
                }
                t *= z;
                if(t < 0)
                    t = 0;
                else if(t > 255)
                    t = 255;
                buffTmp[1][j * imgXlen + k] = (GByte)t;
            }
        }
        //������õ��������ͼƬ��
        poDstDS->GetRasterBand(i)->RasterIO(GF_Write,
            0,0,imgXlen,imgYlen,buffTmp[1],imgXlen,imgYlen,GDT_Byte,0,0);
        printf("... ... band %d processing ... ...\n",i);
    }
    //����ڴ�
	for(i = 0;i < 2;i++)
        CPLFree(buffTmp[i]);
    //�ر�dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
}

int main()
{
	//����ͼ��·��
	char* srcPath = "lena.jpg";
	//���ͼ���·��
	//char* dstPath = "lena2.tif";
	string dp = "lena";
    string dp2 = ".tif";
    string dp3 ;
    std::stringstream ss;
    int ll[] = {0,3,5,3,3,3,5};
    printf("welcome!\n");
    for( int x = 1;x <= 6;x++){
        ss.clear();
        ss << dp << x << dp2;
        ss >> dp3;
        cout << dp3 << endl;
        juanji(srcPath,&dp3[0],x,ll[x]);
        cout << dp3 << "  success!"  << endl <<  endl;
    }
	return 0;
}
