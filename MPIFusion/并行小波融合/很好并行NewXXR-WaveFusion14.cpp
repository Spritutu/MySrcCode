#include <stdio.h>
#include <stdlib.h>

#include "math.h"

#include "mpi.h"


//bool Convert(unsigned char *&pColor, unsigned char *&pGray, int length)
bool Convert(unsigned char *&pColor, unsigned char *&pGray,double *&pIHS, int length)
{
    length=length*3;
    for (int i=0;i<length;)
    {
        double B=pColor[i++];
        double G=pColor[i++];
        double R=pColor[i++];
        
        int position=(i-3)/3;
        
        double I,H,S,min;
        
        min=B<G?(R<B?R:B):(R<G?R:G);
        
        I=(B+G+R)/3;
        if(min==B)
        {
            H=(G-B)/(3*(I-B));
            S=1.0-(B/I);
        //    I=pGray[position];
            
           /* B=(1.0-S)*I;
            G=3*H*I+(1.0-3*H)*B;
            R=3*I-B-G; */
        }

        else if(min==R)
        {
            H=(B-R)/(3*(I-R));
            S=1.0-(R/I);
           // I=pGray[position];
            
         /*   R=(1.0-S)*I;
            B=3*H*I+(1.0-3*H)*R;
            G=3*I-R-B; */
        }
        
        else{
            H=(R-G)/(3*(I-G));
            S=1.0-(G/I);
          //  I=pGray[position];
            
           /* G=(1.0-S)*I;
            R=3*H*I+(1.0-3*H)*G;
            B=3*I-R-G; */           
        }
      

	/*	pColor[i-3]=(unsigned char)I;
        pColor[i-2]=(unsigned char)H;
        pColor[i-1]=(unsigned char)S; */

		pIHS[i-3]=I;
        pIHS[i-2]=H;
        pIHS[i-1]=S; 


    }
    return true;
}







bool ReadColorBmp(char *name, unsigned char *&body, unsigned char *&head, int &height,int &width,int &length)
//bool ReadColorBmp(char *name, unsigned char *&body, unsigned char *&head,int height,int width, int &length)
{
    FILE *fp=fopen(name,"rb");
    if(fp==NULL)
    {
        perror("Open file failure");
        return false;
    }

    head=(unsigned char*)malloc(sizeof(unsigned char)*54);
    fread(head,sizeof(unsigned char),54,fp);    
    unsigned int OffBits=(head[10]&0x000000ff)+((head[11]&0x000000ff)<<8)+((head[12]&0x000000ff)<<16)+((head[13]&0x000000ff)<<24);
	width=(head[18]&0x000000ff)+((head[19]&0x000000ff)<<8)+((head[20]&0x000000ff)<<16)+((head[21]&0x000000ff)<<24);
	height=(head[22]&0x000000ff)+((head[23]&0x000000ff)<<8)+((head[24]&0x000000ff)<<16)+((head[25]&0x000000ff)<<24);
    
    length=width*height;
    fseek(fp, OffBits, SEEK_SET);    
    body=(unsigned char*)malloc(sizeof(unsigned char)*length*3);    
    fread(body, sizeof(unsigned char),length*3,fp);
    
    fclose(fp);
	
    return true;
}



bool WriteColorBmp(char *name, unsigned char *&body, unsigned char *&head, int length)
{
    FILE *fp=fopen(name,"wb");
    if(fp==NULL)
    {
        perror("Open file failure");
        return false;
    }
    
    fwrite(head, sizeof(unsigned char),54,fp);
    fwrite(body, sizeof(unsigned char),length*3,fp);
    fclose(fp);
    return true;
}

bool ReadGrayBmp(char *name, unsigned char *&body, int &length)
{
    FILE *fp=fopen(name,"rb");
    if(fp==NULL)
    {
        perror("Open file failure");
        return false;
    }
    
    unsigned char head[54];
    fread(head,sizeof(unsigned char),54,fp);    
    unsigned int OffBits=(head[10]&0x000000ff)+((head[11]&0x000000ff)<<8)+((head[12]&0x000000ff)<<16)+((head[13]&0x000000ff)<<24);
    unsigned int width=(head[18]&0x000000ff)+((head[19]&0x000000ff)<<8)+((head[20]&0x000000ff)<<16)+((head[21]&0x000000ff)<<24);
    unsigned int height=(head[22]&0x000000ff)+((head[23]&0x000000ff)<<8)+((head[24]&0x000000ff)<<16)+((head[25]&0x000000ff)<<24);

    length=width*height;
    fseek(fp, OffBits, SEEK_SET);    
    body=(unsigned char*)malloc(sizeof(unsigned char)*length);    
    fread(body, sizeof(unsigned char),length,fp);
    
    fclose(fp);
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**********************************************************************
//* �ļ�����: Wavelet.h
//* �ļ�����: С���任
//**********************************************************************
//* ��������:Hori_Transform()
//* ��������:���һ��ͼ��ˮƽ�����С���任	
//* �� �� ֵ:
//* ����˵��:psOriginData��άָ�룬ָ��ԭʼͼ������				
//*          psTransData���һ��ˮƽ�任���С��ϵ��	
//*          fRadiusС���任���� 
//**********************************************************************
void Hori_Transform(short** psOriginData, short** psTransData, int iWidth, int iHeight, int layer,double fRadius)
{
	int x,y;
	short Trans_Coeff0;			//С���任ϵ��
    signed short Trans_Coeff1;

	//�б任
    for(y=0; y<iHeight; y++)            
	{
		for(x=0; x<iWidth/2; x++)
		{
			if(layer == 1)
			{
				if (fRadius==2)
				{
					psTransData[y][x] = (psOriginData[y][x*2]);
                    psTransData[y][iWidth/2+x] = (psOriginData[y][x*2+1]);
				}
				else
				{
                    psTransData[y][x] = (psOriginData[y][x*2]-128);		
                    psTransData[y][iWidth/2+x] = (psOriginData[y][x*2+1]-128);	
				}
			}
// 			if(layer>1)//���任��������1,���������Ƶ��С��ϵ��
// 			{				
// 				psTransData0[y][x] = psTransData[y][x*2];
// 				psTransData0[y][iWidth/2+x] = psTransData[y][x*2+1];
// 			}
		}
	}
	//ͨ��ͼ��Ĳ�֣����С���任
	for(y=0; y<iHeight; y++)
	{
		//�Ұ�ͼ������ż������ϵ��
		for(x=0; x<iWidth/2-1; x++)
		{
			Trans_Coeff1 = (psTransData[y][x]+psTransData[y][x+1])>>1;
			//Ԥ��ϵ��
			psTransData[y][iWidth/2+x] -= Trans_Coeff1;	
		}
		//�ұ߽紦��
		Trans_Coeff1 = (psTransData[y][iWidth/2-2]+psTransData[y][iWidth/2-1])>>1;
		psTransData[y][iWidth-1] -=Trans_Coeff1;
		
		
		//��߽紦��
		Trans_Coeff0 = (psTransData[y][iWidth/2]+psTransData[y][iWidth/2+1])>>2;
		psTransData[y][0] +=Trans_Coeff0;
		//���ͼ��������������ϵ��
		for(x=1; x<iWidth/2; x++)
		{
			Trans_Coeff0 = (psTransData[y][iWidth/2+x-1]+psTransData[y][iWidth/2+x])>>2;
			psTransData[y][x] +=Trans_Coeff0;
		}
	}	
    //��ȡˮƽ�任��С��ϵ��
	for(y=0; y<iHeight; y++)
	{
		for(x=0; x<iWidth/2; x++)
		{
			psTransData[y][x]=short(psTransData[y][x]*fRadius);
			psTransData[y][x+iWidth/2]=short(psTransData[y][x+iWidth/2]/fRadius);
		}
	}
}
//**********************************************************************
//* ��������:Vert_Transform()
//* ��������:���һ��ͼ����ֱ�����С���任	
//* �� �� ֵ:
//* ����˵��:psOriginData��άָ�룬ָ��ԭʼͼ������				
//*          psTransData���һ����ֱ�任���С��ϵ��
//*          fRadiusС���任����   
//**********************************************************************
void Vert_Transform(short** psOriginData, short** psTransData, int iWidth,int iHeight, int layer,double fRadius)
{
	int x,y;
	short Trans_Coeff0;			//С���任ϵ��
    signed short Trans_Coeff1;

	//��ģ����ɱ任ϵ���ĸ�ֵ����
	for(x = 0; x < iWidth; x ++)            
	{
			for(y = 0; y < iHeight/2; y ++)           
			{
	            if (fRadius == 2)
				{
					psTransData[y][x] = (psOriginData[y*2][x]);
                    psTransData[iHeight/2 + y][x] = (psOriginData[y*2+1][x]);
				}
	            else
				{
                    psTransData[y][x] = (psOriginData[y*2][x]-128);		//even
                    psTransData[iHeight/2 + y][x] = (psOriginData[y*2+1][x]-128);	//odd
				}
	   		}
	}
	//ͨ��ͼ��Ĳ�֣����С���任
	for(x=0; x<iWidth; x++)
	{
		for(y=0; y<iHeight/2-1; y++)
		{
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((psTransData[y][x]+psTransData[y+1][x])/2);	
			//ȡ�෴��
			Trans_Coeff1=~Trans_Coeff1+1;	
			//ϵ��Ԥ��
			psTransData[iHeight/2 + y][x] = psTransData[iHeight/2 + y][x]+Trans_Coeff1;
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((psTransData[iHeight/2-1][x]+psTransData[iHeight/2-2][x])/2);
		Trans_Coeff1=~Trans_Coeff1+1;
		psTransData[iHeight-1][x] = psTransData[iHeight-1][x]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((psTransData[iHeight/2][x]+psTransData[iHeight/2+1][x])/4);
		psTransData[0][x] = psTransData[0][x]+Trans_Coeff0;
		//�����������������ı任
		for(y=1; y<iHeight/2; y++)
		{
			Trans_Coeff0 = ((psTransData[iHeight/2 + y][x]+psTransData[iHeight/2 + y -1][x])/4);
			psTransData[y][x] = psTransData[y][x]+Trans_Coeff0;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//* ��������:XXR-NewVert_Transform()
//* ��������:���һ��ͼ���С���任
//* �� �� ֵ:
//* ����˵��:psTransData0��άָ�룬ָ���б任����				
//*          psTransData ���һ��С�����С��ϵ��
//*          layerС���任�Ĳ���
//*          fRadiusС���任���� 
//////////////////////////////////////////////////////////////////////////////////////////
void NewVert_Transform(short** psTransData0,short** psTransData,int iWidth, int iHeight,int layer,double fRadius)

{
	short Trans_Coeff0;			//С���任ϵ��
    signed short Trans_Coeff1;
	

//��ֱ����ı任��ʼ	
	for(int x=0; x<iWidth/2; x++)
	{
		//�б任
		for(int y=0; y<iHeight/2; y++)
		{
			//Ƶ��LL����
			psTransData[y][x] = psTransData0[y*2][x];
			//Ƶ��HL����
			psTransData[iHeight/2+y][x] = psTransData0[y*2+1][x];
			//Ƶ��LH����
			psTransData[y][iWidth/2+x] = psTransData0[y*2][iWidth/2+x];	
			//Ƶ��HH����
			psTransData[iHeight/2+y][iWidth/2+x] = psTransData0[y*2+1][iWidth/2+x];
		}

		//�ϰ�ͼ��������������ϵ��
		for(int y=0; y<iHeight/2-1; y++)
		{
		    //���ϲ��� HL
			Trans_Coeff1 = (psTransData[y][x]+psTransData[y+1][x])>>1;
			psTransData[iHeight/2+y][x] -=Trans_Coeff1;
			//���ϲ��� HH
			Trans_Coeff1 = (psTransData[y][iWidth/2+x]+psTransData[y+1][iWidth/2+x])>>1;
			psTransData[iHeight/2+y][iWidth/2+x] -= Trans_Coeff1;
		}
		//���ϱ߽紦��
		Trans_Coeff1 = (psTransData[iHeight/2-2][x]+psTransData[iHeight/2-1][x])>>1;
		psTransData[iHeight-1][x] -=Trans_Coeff1;
	    //���ϱ߽紦��
		Trans_Coeff1 = (psTransData[iHeight/2-2][iWidth/2+x]+psTransData[iHeight/2-1][iWidth/2+x])>>1;
		psTransData[iHeight-1][iWidth/2+x] -=Trans_Coeff1;
		
	
        //���±߽紦��
		Trans_Coeff0 = ((psTransData[iHeight/2][x]+psTransData[iHeight/2+1][x])>>2);
		psTransData[0][x] +=Trans_Coeff0;
		//���±߽紦��
		Trans_Coeff0 = ((psTransData[iHeight/2][iWidth/2+x]+psTransData[iHeight/2+1][iWidth/2+x])>>2);
		psTransData[0][iWidth/2+x] +=Trans_Coeff0;
		//�°�ͼ������ż������ϵ��
		for(int y=1; y<iHeight/2; y++)
		{
			//���²��� LL
			Trans_Coeff0 = (psTransData[iHeight/2+y-1][x]+psTransData[iHeight/2+y][x])>>2;
			psTransData[y][x] +=Trans_Coeff0;

			//���²��� LH
			Trans_Coeff0 = (psTransData[iHeight/2+y-1][iWidth/2+x]+psTransData[iHeight/2+y][iWidth/2+x])>>2;
			psTransData[y][iWidth/2+x] +=Trans_Coeff0;
		}
	}
	//���С��ϵ����LLƵ����ϵ�����з�ֵ��ǿ����������ƵƵ����ϵ�����������ֵ
	for(int x=0; x<iWidth; x++)
	{
		for(int y=0; y<iHeight/2; y++)
		{
			psTransData[y][x]=short(psTransData[y][x]*fRadius);
			psTransData[y+iHeight/2][x]=short(psTransData[y+iHeight/2][x]/fRadius);
		}
	}
	//�ͷ���ʱ�ڴ棬ע���ͷ��Ⱥ�˳��
/*	for(y = 0; y<iHeight; y++)
	{
		SAFE_DELETE_ARRAY(psTransData0[y]);
	}
	SAFE_DELETE_ARRAY(psTransData0);*/


}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


//**********************************************************************
//* ��������:DWT()
//* ��������:���һ��ͼ���С���任
//* �� �� ֵ:
//* ����˵��:psOriginData��άָ�룬ָ��ԭʼͼ������				
//*          psTransData ���һ��С�����С��ϵ��
//*          layerС���任�Ĳ���
//*          fRadiusС���任���� 
//**********************************************************************
void DWT(short** psOriginData,short** psTransData,int iWidth, int iHeight,int layer,double fRadius)
{
	int x,y;
	short Trans_Coeff0;			//С���任ϵ��
    signed short Trans_Coeff1;
	short ** psTransData0 = new short* [iHeight];//���һ��ˮƽ�任���С��ϵ��
	for(y =0; y <iHeight; y++)
	{
		psTransData0[y] = new short [iWidth];
	}

	//�б任
    for(y=0; y<iHeight; y++)            
	{
		for(x=0; x<iWidth/2; x++)
		{
			if(layer == 1)
			{
				if (fRadius==2)
				{
					psTransData0[y][x] = (psOriginData[y][x*2]);
                    psTransData0[y][iWidth/2+x] = (psOriginData[y][x*2+1]);
				}
				else
				{
                    psTransData0[y][x] = (psOriginData[y][x*2]-128);		
                    psTransData0[y][iWidth/2+x] = (psOriginData[y][x*2+1]-128);	
				}
			}
			if(layer>1)//���任��������1,���������Ƶ��С��ϵ��
			{				
				psTransData0[y][x] = psTransData[y][x*2];
				psTransData0[y][iWidth/2+x] = psTransData[y][x*2+1];
				
			}
		}
	}
	//ͨ��ͼ��Ĳ�֣����С���任
	for(y=0; y<iHeight; y++)
	{
		//�Ұ�ͼ������ż������ϵ��
		for(x=0; x<iWidth/2-1; x++)
		{
			Trans_Coeff1 = (psTransData0[y][x]+psTransData0[y][x+1])>>1;
			//Ԥ��ϵ��
			psTransData0[y][iWidth/2+x] -= Trans_Coeff1;	
		}
		//�ұ߽紦��
		Trans_Coeff1 = (psTransData0[y][iWidth/2-2]+psTransData0[y][iWidth/2-1])>>1;
		psTransData0[y][iWidth-1] -=Trans_Coeff1;

		
		//��߽紦��
		Trans_Coeff0 = (psTransData0[y][iWidth/2]+psTransData0[y][iWidth/2+1])>>2;
		psTransData0[y][0] +=Trans_Coeff0;
		//���ͼ��������������ϵ��
		for(x=1; x<iWidth/2; x++)
		{
			Trans_Coeff0 = (psTransData0[y][iWidth/2+x-1]+psTransData0[y][iWidth/2+x])>>2;
			psTransData0[y][x] +=Trans_Coeff0;
		}
	}	
    //��ȡˮƽ�任��С��ϵ��
	for(y=0; y<iHeight; y++)
	{
		for(x=0; x<iWidth/2; x++)
		{
			psTransData0[y][x]=short(psTransData0[y][x]*fRadius);
			psTransData0[y][x+iWidth/2]=short(psTransData0[y][x+iWidth/2]/fRadius);
		}
	}
    //ˮƽ����ı任����

	//��ֱ����ı任��ʼ	
	for(x=0; x<iWidth/2; x++)
	{
		//�б任
		for(y=0; y<iHeight/2; y++)
		{
			//Ƶ��LL����
			psTransData[y][x] = psTransData0[y*2][x];
			//Ƶ��HL����
			psTransData[iHeight/2+y][x] = psTransData0[y*2+1][x];
			//Ƶ��LH����
			psTransData[y][iWidth/2+x] = psTransData0[y*2][iWidth/2+x];	
			//Ƶ��HH����
			psTransData[iHeight/2+y][iWidth/2+x] = psTransData0[y*2+1][iWidth/2+x];
		}

		//�ϰ�ͼ��������������ϵ��
		for(y=0; y<iHeight/2-1; y++)
		{
		    //���ϲ��� HL
			Trans_Coeff1 = (psTransData[y][x]+psTransData[y+1][x])>>1;
			psTransData[iHeight/2+y][x] -=Trans_Coeff1;
			//���ϲ��� HH
			Trans_Coeff1 = (psTransData[y][iWidth/2+x]+psTransData[y+1][iWidth/2+x])>>1;
			psTransData[iHeight/2+y][iWidth/2+x] -= Trans_Coeff1;
		}
		//���ϱ߽紦��
		Trans_Coeff1 = (psTransData[iHeight/2-2][x]+psTransData[iHeight/2-1][x])>>1;
		psTransData[iHeight-1][x] -=Trans_Coeff1;
	    //���ϱ߽紦��
		Trans_Coeff1 = (psTransData[iHeight/2-2][iWidth/2+x]+psTransData[iHeight/2-1][iWidth/2+x])>>1;
		psTransData[iHeight-1][iWidth/2+x] -=Trans_Coeff1;
		
	
        //���±߽紦��
		Trans_Coeff0 = ((psTransData[iHeight/2][x]+psTransData[iHeight/2+1][x])>>2);
		psTransData[0][x] +=Trans_Coeff0;
		//���±߽紦��
		Trans_Coeff0 = ((psTransData[iHeight/2][iWidth/2+x]+psTransData[iHeight/2+1][iWidth/2+x])>>2);
		psTransData[0][iWidth/2+x] +=Trans_Coeff0;
		//�°�ͼ������ż������ϵ��
		for(y=1; y<iHeight/2; y++)
		{
			//���²��� LL
			Trans_Coeff0 = (psTransData[iHeight/2+y-1][x]+psTransData[iHeight/2+y][x])>>2;
			psTransData[y][x] +=Trans_Coeff0;

			//���²��� LH
			Trans_Coeff0 = (psTransData[iHeight/2+y-1][iWidth/2+x]+psTransData[iHeight/2+y][iWidth/2+x])>>2;
			psTransData[y][iWidth/2+x] +=Trans_Coeff0;
		}
	}
	//���С��ϵ����LLƵ����ϵ�����з�ֵ��ǿ����������ƵƵ����ϵ�����������ֵ
	for(x=0; x<iWidth; x++)
	{
		for(y=0; y<iHeight/2; y++)
		{
			psTransData[y][x]=short(psTransData[y][x]*fRadius);
			psTransData[y+iHeight/2][x]=short(psTransData[y+iHeight/2][x]/fRadius);
		}
	}
	//�ͷ���ʱ�ڴ棬ע���ͷ��Ⱥ�˳��
/*	for(y = 0; y<iHeight; y++)
	{
		SAFE_DELETE_ARRAY(psTransData0[y]);
	}
	SAFE_DELETE_ARRAY(psTransData0);*/
}
//**********************************************************************
//* ��������:DWTi()
//* ��������:���һ��ͼ��С���任����任
//* �� �� ֵ:
//* ����˵��:psOriginData��άָ�룬�����ݿռ���С����任���ͼ������				
//*          psTransData���һ��С���任���С��ϵ��
//*          layerС���任�Ĳ���
//*          fRadiusС���任����   
//**********************************************************************
void DWTi(short **psTransData, short **psOriginData, int iWidth, int iHeight,int layer, double fRadius)
{
	int x,y;
	short WvltCoeff0;			//С���任ϵ��
    signed short WvltCoeff1;
	short ** psTransData0 = new short* [iHeight];//���һ����ֱ��任�������
	for(y =0; y <iHeight; y++)
	{
		psTransData0[y] = new short [iWidth];
	}

	//��任��һ������̣������ִ���ֱ����ʼ
	for(x=0; x<iWidth; x++)
	{
		for(y=0; y<iHeight/2; y++)
		{
			psTransData[y][x]= short(psTransData[y][x]/fRadius);
			psTransData[y+iHeight/2][x]= short(psTransData[y+iHeight/2][x]*fRadius);
		}
	}
	for(x=0; x<iWidth/2; x++)
	{
        //���±߽紦��
		WvltCoeff0 = (psTransData[iHeight/2][x]+psTransData[iHeight/2+1][x])>>2;
		psTransData[0][x] -=WvltCoeff0;
        //���±߽紦��
		WvltCoeff0 = (psTransData[iHeight/2][iWidth/2+x]+psTransData[iHeight/2+1][iWidth/2+x])>>2;
		psTransData[0][iWidth/2+x] -=WvltCoeff0;
		//�°�ͼ����ֱ����ż������С��ϵ������任
		for(y=1; y<iHeight/2; y++)
		{
			//���²��� LL
			WvltCoeff0 = (psTransData[iHeight/2+y-1][x]+psTransData[iHeight/2+y][x])>>2;
			psTransData[y][x] -=WvltCoeff0;
			//���²��� LH
			WvltCoeff0 = (psTransData[iHeight/2+y-1][iWidth/2+x]+psTransData[iHeight/2+y][iWidth/2+x])>>2;
			psTransData[y][iWidth/2+x] -=WvltCoeff0;
		}

		//�ϰ�ͼ����ֱ������������С��ϵ������任
		for(y=0; y<iHeight/2-1; y++)
		{
            //���ϲ��� HL
			WvltCoeff1 =(psTransData[y][x]+psTransData[y+1][x])>>1;
			psTransData[iHeight/2+y][x] +=WvltCoeff1;
            //���ϲ��� HH
			WvltCoeff1 = (psTransData[y][iWidth/2+x]+psTransData[y+1][iWidth/2+x])>>1;
			psTransData[iHeight/2+y][iWidth/2+x] +=WvltCoeff1;
		}
		//���ϱ߽紦��
		WvltCoeff1 = (psTransData[iHeight/2-2][x]+psTransData[iHeight/2-1][x])>>1;
		psTransData[iHeight-1][x] +=WvltCoeff1;
	    //���ϱ߽紦��		
		WvltCoeff1 = (psTransData[iHeight/2-2][iWidth/2+x]+psTransData[iHeight/2-1][iWidth/2+x])>>1;
		psTransData[iHeight-1][iWidth/2+x] +=WvltCoeff1;

		//�任ϵ���Ĳ����������
		for(y=0; y<iHeight/2; y++)
		{
			psTransData0[y*2][x] = psTransData[y][x];
			psTransData0[y*2+1][x] = psTransData[iHeight/2+y][x];
			psTransData0[y*2][iWidth/2+x] = psTransData[y][iWidth/2+x];
			psTransData0[y*2+1][iWidth/2+x]= psTransData[iHeight/2+y][iWidth/2+x];
		}
	}
    //��ֱ�������任����

	//�������ˮƽ�������任
	for(y=0; y<iHeight; y++)
	{
		for(x=0; x<iWidth/2; x++)
		{
			psTransData0[y][x]= short(psTransData0[y][x]/fRadius);
			psTransData0[y][x+iWidth/2] = short(psTransData0[y][x+iWidth/2]*fRadius);
		}
	}	
	for(y=0; y<iHeight; y++)
	{
		//��߽紦��
		WvltCoeff0 = (psTransData0[y][iWidth/2]+psTransData0[y][iWidth/2+1])>>2;
		psTransData0[y][0] -=WvltCoeff0;
		//���ͼ��ż������С��ϵ������任
		for(x=1; x<iWidth/2; x++)
		{
			WvltCoeff0 = (psTransData0[y][iWidth/2+x-1]+psTransData0[y][iWidth/2+x])>>2;
			psTransData0[y][x] -=WvltCoeff0;
		}

		//�Ұ�ͼ����������С��ϵ������任
		for(x=0; x<iWidth/2-1; x++)
		{
			WvltCoeff1 = (psTransData0[y][x]+psTransData0[y][x+1])>>1;
			psTransData0[y][iWidth/2+x] += WvltCoeff1;
		}
		//�ұ߽紦��
		WvltCoeff1 = (psTransData0[y][iWidth/2-2]+psTransData0[y][iWidth/2-1])>>1;
		psTransData0[y][iWidth-1] += WvltCoeff1;
		if(layer > 1)
		{
			for(x=0; x<iWidth/2; x++)
			{
				psTransData[y][x*2] = psTransData0[y][x];
				psTransData[y][x*2+1] = psTransData0[y][iWidth/2+x];
			}
		}
		if(layer==1)
		{
			for(x=0; x<iWidth/2; x++)
			{
				if(fRadius!=2)
				{
					psTransData0[y][x]=psTransData0[y][x]+128;
					psTransData0[y][iWidth/2+x]=psTransData0[y][iWidth/2+x]+128;
				}
				if(psTransData0[y][x]>255) 
				{
					psTransData0[y][x]=255;
				}
				if(psTransData0[y][x]<0)  
				{
					psTransData0[y][x]=0;
				}
				if(psTransData0[y][iWidth/2+x]>255) 
				{
					psTransData0[y][iWidth/2+x]=255;
				}
				if(psTransData0[y][iWidth/2+x]<0) 
				{
					psTransData0[y][iWidth/2+x]=0;
				}
				psOriginData[y][x*2] = psTransData0[y][x];
			    psOriginData[y][x*2+1] =psTransData0[y][iWidth/2+x];
			}
		}
	}
	//�ͷ���ʱ�ڴ棬ע���ͷ��Ⱥ�˳��
/*	for(int i = 0; i <iHeight; i++)
	{
		SAFE_DELETE_ARRAY(psTransData0[i]);
	}
	SAFE_DELETE_ARRAY(psTransData0); */
}
/*********************************************************************************
*����������	Window_WvltFusion���ͼ��С��ϵ�����ںϲ������õ���Ƶ����С���ں�����*
*����������	short **spWvltData0 ����άָ�룬�������һ��ͼ���ԭʼС��ϵ��		 *
*			short **spWvltData1 ����άָ�룬�����������һ��ͼ���ԭʼС��ϵ��	 *
*			int   Scan_y		��ɨ������ʼ������								 *
*			int   Scan_x		��ɨ������ʼ������								 *
*			int   End_y			��ɨ������ֹ������								 *
*			int   End_x			��ɨ������ֹ������								 *
*********************************************************************************/
void Window_WvltFusion(short **spWvltData0, short **spWvltData1, int Scan_y, int Scan_x, int End_y, int End_x)
{
	int y,x;
	short WndSum0, WndSum1;
	for(y = Scan_y; y < End_y; y ++)
	{
		for(x = Scan_x; x < End_x; x ++)
		{
			//��ʼ��������С��ϵ���ĺ�
			WndSum0 = 0;	WndSum1 = 0;
			//���㴰����С��ϵ���ĺ�
			for(int i = -1; i <= 1; i++)
			{
				for(int j = -1; j <= 1; j++)
				{
					if( (y+i) < Scan_y || (x+j) < Scan_x || (y+i) >= End_y || (x+j) >= End_x)
					{
						WndSum0 += 0;
						WndSum1 += 0;
					}
					else
					{
						WndSum0 += abs((int)spWvltData0[y + i][x + j]);
						WndSum1 += abs((int)spWvltData1[y + i][x + j]);
					}
				}
			}
			if(WndSum0 < WndSum1)
				spWvltData0[y][x] = spWvltData1[y][x];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    int myid, numprocs;
	double starttime, endtime;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);	
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
	
	starttime = MPI_Wtime();
	
/*	unsigned char* colorData;
    unsigned char* grayData;
    unsigned char* colorHead;   
    unsigned char* pColor;
    unsigned char* pGray;  */
	
	unsigned char* colorData;
	unsigned char* grayData;
	unsigned char* colorHead;	
	
	unsigned char* colorBody;
	unsigned char* grayBody;
	
	int colorLength, grayLength;
	int HeightP,WidthP;
	
	int height,width;  	
	int heightNum,heightother,height0;	
	int length,length0,lengthother;	
	
	unsigned char  *OriginDataColor,*OriginDataGray;	
//	short    *OriginDataColor,*OriginDataGray;	

	printf("**********************************\n");

	fprintf(stdout,"Process %d of %d on %s\n",myid,numprocs,processor_name);
	

	if (myid==0)
	{
			
		unsigned char* pColor;
		unsigned char* pGray; 	
				
		ReadColorBmp("F:\\���뱸��\\MPIFusion\\TestData\\Mul.bmp", colorBody, colorHead, HeightP,WidthP,colorLength);
		ReadGrayBmp("F:\\���뱸��\\MPIFusion\\TestData\\Pan.bmp", grayBody, grayLength);

		while ((colorLength%numprocs)!=0)
		{
			numprocs--;
		}
		length=colorLength/numprocs;
		colorData=colorBody;
		grayData=grayBody;
		
     //   unsigned char  *OriginDataColor;		
     //   unsigned char  *OriginDataGray;	

		OriginDataColor=(unsigned char*)malloc(sizeof(unsigned char)*colorLength);
		OriginDataGray=(unsigned char*)malloc(sizeof(unsigned char)*colorLength); 

       // height=600;
     //  width=400;
		
	   height=HeightP;
       width=WidthP;

       pColor=colorBody;

	   int k=0;
	   int i=0;

		for (i=0;i<height;i++)
            for (int j=0;j<width;j++)
			{
              double B=pColor[k];
              double G=pColor[k+1];
              double R=pColor[k+2];
          	  OriginDataColor[i*width+j]=0.299*R+0.587*G+0.114*B;
			  k=k+3;
			}
       
		int m=OriginDataColor[k/3-1];
			
		printf("k=%d\n",k);	
			
		printf("OriginDataColor=%d\n",OriginDataColor[0]);	
			
		pGray=grayBody;

		 k=0;
		 for ( i=0;i<height;i++)
            for (int j=0;j<width;j++)              
          	{ k++;
			 OriginDataGray[i*width+j]=pGray[i*width+j];
			}
			printf("k=%d\n",k);

				printf("OriginDataGray=%d\n",OriginDataGray[0]);  

			heightNum=height/numprocs;

			length=heightNum*width;
		/*	heightother=heightNum+8;
			height0=heightNum+4;			
			length=heightNum*width;
			length0=height0*width;
			lengthother=heightother*width; */

		//	printf("height=%d, width=%d,length=%d\n",height, width,length); 

        
          MPI_Send(&heightNum,1,MPI_INT,0,101,MPI_COMM_WORLD);
		  MPI_Send(&width,1,MPI_INT,0,102,MPI_COMM_WORLD);
		  MPI_Send(&length,1,MPI_INT,0,103,MPI_COMM_WORLD);
		 

          MPI_Status s;
		  MPI_Recv(&height,1,MPI_INT,0,101,MPI_COMM_WORLD,&s);
		  MPI_Recv(&width,1,MPI_INT,0,102,MPI_COMM_WORLD,&s);
		  MPI_Recv(&length,1,MPI_INT,0,103,MPI_COMM_WORLD,&s); 
	
		  
		  printf("\n\nheight=%d  width=%d  length=%d\n",height, width,length);


		for (i=1;i<numprocs-1;i++)
		{
          MPI_Send(&heightNum,1,MPI_INT,i,201,MPI_COMM_WORLD);
		  MPI_Send(&width,1,MPI_INT,i,202,MPI_COMM_WORLD);
		  MPI_Send(&length,1,MPI_INT,i,203,MPI_COMM_WORLD);
		  
	      MPI_Send(OriginDataColor+i*heightNum*width,length, MPI_UNSIGNED_CHAR,i,204,MPI_COMM_WORLD);
          MPI_Send(OriginDataGray+i*heightNum*width,length, MPI_UNSIGNED_CHAR,i,205,MPI_COMM_WORLD);/**/
		}
        if(numprocs!=1)
		{
		MPI_Send(&heightNum,1,MPI_INT,numprocs-1,301,MPI_COMM_WORLD);
		MPI_Send(&width,1,MPI_INT,numprocs-1,302,MPI_COMM_WORLD);
		MPI_Send(&length,1,MPI_INT,numprocs-1,303,MPI_COMM_WORLD);		
	  

		MPI_Send(OriginDataColor+(numprocs-1)*heightNum*width,length, MPI_UNSIGNED_CHAR,i,304,MPI_COMM_WORLD);
        MPI_Send(OriginDataGray+(numprocs-1)*heightNum*width,length,  MPI_UNSIGNED_CHAR,i,305,MPI_COMM_WORLD);


		}

	//	height =height0;
	//	length=length0;
	//	printf("\n\nheight=%d  width=%d  length=%d\n",height, width,length);
	
		
	}


 else if(myid!=numprocs-1)
	{		
	 MPI_Status s;
	 
   //  unsigned char  *OriginDataColor;		
   //  unsigned char  *OriginDataGray;
	 
	 MPI_Recv(&height,1,MPI_INT,0,201,MPI_COMM_WORLD,&s);
	 MPI_Recv(&width,1,MPI_INT,0,202,MPI_COMM_WORLD,&s);
	 MPI_Recv(&length,1,MPI_INT,0,203,MPI_COMM_WORLD,&s);
	 
 
	 OriginDataColor=(unsigned char*)malloc(sizeof(unsigned char)*length);
	 OriginDataGray=(unsigned char*)malloc(sizeof(unsigned char)*length);	 
	 
	 MPI_Recv(OriginDataColor,length,MPI_UNSIGNED_CHAR,0,204,MPI_COMM_WORLD,&s);	 
	 
	 MPI_Recv(OriginDataGray,length,MPI_UNSIGNED_CHAR,0,205,MPI_COMM_WORLD,&s);  	

	}

	else
	{
	//	unsigned char  *OriginDataColor;		
	//	unsigned char  *OriginDataGray;
		
        MPI_Status s;
		MPI_Recv(&height,1,MPI_INT,0,301,MPI_COMM_WORLD,&s);
		MPI_Recv(&width,1,MPI_INT,0,302,MPI_COMM_WORLD,&s);
		MPI_Recv(&length,1,MPI_INT,0,303,MPI_COMM_WORLD,&s);	

		
		OriginDataColor=(unsigned char*)malloc(sizeof(unsigned char)*length);
		OriginDataGray=(unsigned char*)malloc(sizeof(unsigned char)*length);		
		
		MPI_Recv(OriginDataColor,length,MPI_UNSIGNED_CHAR,0,304,MPI_COMM_WORLD,&s);		
		
		MPI_Recv(OriginDataGray,length,MPI_UNSIGNED_CHAR,0,305,MPI_COMM_WORLD,&s);

	}
   // length=height*width;

	printf("\n�б任ʱ�ķֿ������height=%d,width=%d,length=%d\n\n",height,width,length);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////����С���б任///////////////////////////////////////////////////////////////////////////////
//	int   x;
	int 	m_iTranslayer =1;  	
	
	//���ͼ��1��m_iTranslayer��С���任
	
    short ** m_psTransData1=new short* [height];
    short ** m_psTransData2=new short* [height];	
	
    short ** psOriginDataColor=new short* [height];
    short ** psOriginDataGray=new short* [height];


	for(int y =0; y <height; y++)
	{
		m_psTransData1[y] = new short [width];
		m_psTransData2[y] = new short [width]; 
		psOriginDataColor[y]=new short [width];
		psOriginDataGray[y]= new short [width];
	}


   for (int i=0;i<height;i++)
        for (int j=0;j<width;j++)
			{
			 psOriginDataColor[i][j]=OriginDataColor[i*width+j];
			 psOriginDataGray[i][j]=OriginDataGray[i*width+j];
			}
		

	int  iWidth, iHeight,m_iHeight,m_iWidth;
	m_iHeight=height;
    m_iWidth=width;

	 iWidth  = m_iWidth;
	 iHeight = m_iHeight;

	 int RADIUS=1;
	for(int i=1; i<=m_iTranslayer; i++)
	{
		Hori_Transform(psOriginDataColor,m_psTransData1,iWidth,iHeight,i,RADIUS);
		//�ߴ����
		iHeight/=2;
		iWidth/=2;
	}



	iWidth  = m_iWidth;
	iHeight = m_iHeight;
	for(int i=1; i<=m_iTranslayer; i++)
	{
		Hori_Transform(psOriginDataGray,m_psTransData2,iWidth,iHeight,i,RADIUS);
		//�ߴ����
		iHeight/=2;
		iWidth/=2;
	}
	
	iWidth  = m_iWidth;
	iHeight = m_iHeight; 
	
	short * DataColorNewI;
	short * DataGrayNewI;

	short * TempDataColor;
	short * TempDataGray;


	if (myid==0)
	{

        printf("\n\ncolorLength=%d\n",colorLength);
		DataColorNewI=(short* )malloc(sizeof(short)*colorLength);
        DataGrayNewI=(short* )malloc(sizeof(short)*colorLength);
		TempDataColor=(short* )malloc(sizeof(short)*colorLength);
        TempDataGray=(short* )malloc(sizeof(short)*colorLength);
	//	printf("\nlength=%d\n",length);

	}
	else
	{
	//	printf("\nmyid=length=%d\n",length);
		DataColorNewI=(short* )malloc(sizeof(short)*length);
		DataGrayNewI=(short* )malloc(sizeof(short)*length);
	}

	int k=0;
	for(int i=0;i<=height-1;i++)
		for(int j=0;j<width;j++)
		{
		DataColorNewI[k++]=m_psTransData1[i][j];
		}

      k=0;

		for(int i=0;i<=height-1;i++)
		for(int j=0;j<width;j++)
		{
		DataGrayNewI[k++]=m_psTransData2[i][j];
		}


///////////////////���ղ���С���б任������///////////////////////////

	if (myid==0)
	{
				
		MPI_Status s; 		
		
		TempDataColor=(short*)malloc(sizeof(short)*colorLength);
		TempDataGray=(short*)malloc(sizeof(short)*colorLength); 	
		
		
		TempDataColor=DataColorNewI;
		
		TempDataGray=DataGrayNewI;



		for (int i=1;i<numprocs-1;i++)
		{
			MPI_Status s;
			MPI_Recv(TempDataColor+length*i,length,MPI_SHORT,i,401,MPI_COMM_WORLD,&s);
			
			MPI_Recv(TempDataGray+length*i,length,MPI_SHORT,i,403,MPI_COMM_WORLD,&s);	
		}

		if (numprocs!=1)
		{
			MPI_Status s;
			MPI_Recv(TempDataColor+length*(numprocs-1),length,MPI_SHORT,numprocs-1,402,MPI_COMM_WORLD,&s);
			MPI_Recv(TempDataGray+length*(numprocs-1),length,MPI_SHORT,numprocs-1,404,MPI_COMM_WORLD,&s);	
		}

	} 

	else if(myid!=numprocs-1 && myid!=0)
	{
		MPI_Send(DataColorNewI,length,MPI_SHORT,0,401,MPI_COMM_WORLD);
        MPI_Send(DataGrayNewI,length,MPI_SHORT,0,403,MPI_COMM_WORLD);
	}

	else
	{
		MPI_Send(DataColorNewI,length,MPI_SHORT,0,402,MPI_COMM_WORLD);
		MPI_Send(DataGrayNewI,length,MPI_SHORT,0,404,MPI_COMM_WORLD);
	} 

///////////////////����Ϊ���պ�ˮƽ�任������///////////////////////////

	short * ColorWK11;
	short * GrayWK11;
    int     widthV, lengthV;


	
	if (myid==0)
	{
		short **  ColorTempW12=new short* [height*numprocs];
		short **  GrayTempW12=new short* [height*numprocs];			
		
		for(int y =0; y <height*numprocs; y++)
		{
			ColorTempW12[y]   = new short [width];
			GrayTempW12[y]   = new short [width]; 		
		}
		int widthW1=width/numprocs;
		
		widthV=widthW1;
		lengthV=widthV*height*numprocs;	


				
		 for(int i=0; i<height*numprocs;i++)
			for(int j=0;j<width;j++)
			{
				ColorTempW12[i][j]=TempDataColor[i*width+j];
				GrayTempW12[i][j]=TempDataGray[i*width+j];
			}
					
			
		//	short * ColorWK11;
		//	short * GrayWK11;
				
			ColorWK11=(short* )malloc(sizeof(short)*lengthV);
			GrayWK11=(short* )malloc(sizeof(short)*lengthV);
			


		for (int m=1;m<numprocs;m++)	
			{
				for(int i=0; i<height*numprocs;i++)
					for(int j=0;j<widthW1;j++)
					{
						ColorWK11[i*widthW1+j]=ColorTempW12[i][m*widthW1+j];  
						GrayWK11[i*widthW1+j]=GrayTempW12[i][m*widthW1+j];
					}
					
					MPI_Send(&widthW1,1,MPI_INT,m,501,MPI_COMM_WORLD);
					MPI_Send(ColorWK11,lengthV,MPI_SHORT,m,502,MPI_COMM_WORLD);
					MPI_Send(GrayWK11,lengthV,MPI_SHORT,m,503,MPI_COMM_WORLD);	
					
			}			
		
			for(int i=0; i<height*numprocs;i++)
					for(int j=0;j<widthW1;j++)
					{
						ColorWK11[i*widthW1+j]=ColorTempW12[i][j];   
						GrayWK11[i*widthW1+j]=GrayTempW12[i][j];	
					}
				
			
		//	int widthV=widthW1;
		//			 widthV=widthW1;
		
	}			

	else
	{
		
	    MPI_Status s;
		MPI_Recv(&widthV,1,MPI_INT,0,501,MPI_COMM_WORLD,&s);
		lengthV=widthV*height*numprocs;		
		
		ColorWK11=(short* )malloc(sizeof(short)*lengthV);
		GrayWK11=(short* )malloc(sizeof(short)*lengthV);
		
		MPI_Recv(ColorWK11,lengthV,MPI_SHORT,0,502,MPI_COMM_WORLD,&s);
		MPI_Recv(GrayWK11,lengthV,MPI_SHORT,0,503,MPI_COMM_WORLD,&s);
	} 

printf("ChuiZhi*** widthV=%d, lengthV=%d,width=%d\n\n",widthV,lengthV,width);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	int heightV=height*numprocs;

//	int WidthV=width/numprocs;
//	int 	widthV=WidthV;

//	int lengthV=widthV*height*numprocs;
	
	short ** m_psTransData1V=new short* [heightV];
	short ** m_psTransData2V=new short* [heightV];	
	
    short ** psOriginDataColorV=new short* [heightV];
    short ** psOriginDataGrayV=new short* [heightV];


	for(int y =0; y <heightV; y++)
	{
		m_psTransData1V[y]   = new short [widthV];
		m_psTransData2V[y]   = new short [widthV]; 
		psOriginDataColorV[y]= new short [widthV];
		psOriginDataGrayV[y]=  new short [widthV];
	}


   for ( int i=0;i<heightV;i++)
        for (int j=0;j<widthV;j++)
			{
              psOriginDataColorV[i][j]=ColorWK11[i*widthV+j];
              psOriginDataGrayV[i][j]=GrayWK11[i*widthV+j];
			}

	int  iWidthV, iHeightV,m_iHeightV,m_iWidthV;
	m_iHeightV=heightV;
    m_iWidthV=widthV;

	 iWidthV  = m_iWidthV;
	 iHeightV = m_iHeightV;

	 RADIUS=1;
	for(int i=1; i<=m_iTranslayer; i++)
	{
	//	Vert_Transform(psOriginDataColorV,m_psTransData1V,iWidthV,iHeightV,i,RADIUS);

		NewVert_Transform(psOriginDataColorV,m_psTransData1V,iWidthV,iHeightV,i,RADIUS);

		//�ߴ����
		iHeightV/=2;
		iWidthV/=2;
	}


	iWidthV  = m_iWidthV;
	 iHeightV = m_iHeightV;

	for(int i=1; i<=m_iTranslayer; i++)
	{
	//	Vert_Transform(psOriginDataGrayV,m_psTransData2V,iWidthV,iHeightV,i,RADIUS);
        NewVert_Transform(psOriginDataGrayV,m_psTransData2V,iWidthV,iHeightV,i,RADIUS);        

		//�ߴ����
		iHeightV/=2;
		iWidthV/=2;
	}

  printf("After Vert***widthV=%d, lengthV=%d,width=%d\n",widthV,lengthV,width);
  printf("After Vert***iWidthV=%d, iHeightV=%d,m_iHeightV=%d, m_iWidthV=%d\n\n",iWidthV, iHeightV,m_iHeightV,m_iWidthV);



	
	iWidthV  = m_iWidthV;
	iHeightV = m_iHeightV;
	
	short * DataColorNewIV;
	short * DataGrayNewIV;

	short * TempDataColorV;
	short * TempDataGrayV;


	if (myid==0)
	{

        printf("\n\ncolorLength=%d\n",colorLength);
		DataColorNewIV=(short* )malloc(sizeof(short)*colorLength);
        DataGrayNewIV=(short* )malloc(sizeof(short)*colorLength);
		TempDataColorV=(short* )malloc(sizeof(short)*colorLength);
        TempDataGrayV=(short* )malloc(sizeof(short)*colorLength);
	//	printf("\n\nlength=%d\n",length);

	}
	else
	{
      int  lengthV=widthV*height*numprocs;
	//	printf("\n\nmyid=length=%d\n",lengthV);
		DataColorNewIV=(short* )malloc(sizeof(short)*lengthV);
		DataGrayNewIV=(short* )malloc(sizeof(short)*lengthV);
	}

	 k=0;
	for(int i=0;i<=height*numprocs-1;i++)
		for(int j=0;j<widthV;j++)
		{
		   DataColorNewIV[k++]=m_psTransData1V[i][j];
		}
      k=0;

	//	for(i=0;i<=height-1;i++)
	  for(int i=0;i<=height*numprocs-1;i++)
		for(int j=0;j<widthV;j++)
		{
		   DataGrayNewIV[k++]=m_psTransData2V[i][j];
		}


	if (myid==0)
	{		
		MPI_Status s;	    	
		TempDataColorV=DataColorNewIV;
		TempDataGrayV=DataGrayNewIV;
			
		
		for (int i=1;i<numprocs;i++)
		{
			MPI_Status s;
			MPI_Recv(TempDataColorV+lengthV*i,lengthV,MPI_SHORT,i,601,MPI_COMM_WORLD,&s);			
			MPI_Recv(TempDataGrayV+lengthV*i,lengthV,MPI_SHORT,i,602,MPI_COMM_WORLD,&s);	
		}

	} 
	

	else
	{
		MPI_Send(DataColorNewIV,lengthV,MPI_SHORT,0,601,MPI_COMM_WORLD);
		MPI_Send(DataGrayNewIV,lengthV,MPI_SHORT,0,602,MPI_COMM_WORLD);
	} 



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if (myid==0)
 {
	iHeight =HeightP;
	iWidth  =WidthP;
	
	printf("\n\n*@*@**m_iHeight=%d****#####***???^**m_iWidth=%d*******\n",HeightP,WidthP);


	int m_iWidth=iWidth;
	int m_iHeight=iHeight;


    short ** TempTransData1F=new short* [iHeight];
    short ** TempTransData2F=new short* [iHeight];

    short ** m_psOriginDataF=new short* [iHeight];

	printf("***width***=%d***\n",width);


	for(int y =0; y <iHeight; y++)
	{
		TempTransData1F[y] = new short [width];
		TempTransData2F[y] = new short [width]; 
		m_psOriginDataF[y]=new short [width];
	}


   for (int m=0;m<numprocs;m++)	
   {
	
	for(int y=0; y<iHeight; y++)
		for(int x=0; x<widthV; x++)
		{
			TempTransData1F[y][m*widthV+x]=TempDataColorV[m*iHeight*widthV+y*widthV+x];

			TempTransData2F[y][m*widthV+x]=TempDataGrayV[m*iHeight*widthV+y*widthV+x];
		}
   }


   	iHeight =HeightP;
	iWidth  =WidthP;
   
    //С��ϵ�����ںϴ���Ƶ����LL3,LH3,HL3,HH3,LH2,HL2,HH2,LH1,HL1,HH1
	//�ںϴ�����Ƶ�����У����������õ���3*3�Ĵ���
	//LL3Ƶ��С��ϵ�����ں�
	Window_WvltFusion(TempTransData1F,TempTransData2F,0,0,iHeight/2,iWidth/2);	
	//HL1Ƶ��С��ϵ�����ں�
	Window_WvltFusion(TempTransData1F,TempTransData2F,0,iWidth/2,iHeight/2,iWidth);
	//LH1Ƶ��С��ϵ�����ں�
	Window_WvltFusion(TempTransData1F,TempTransData2F,iHeight/2,0,iHeight,iWidth/2);
	//HH1Ƶ��С��ϵ�����ں�
	Window_WvltFusion(TempTransData1F,TempTransData2F,iHeight/2,iWidth/2,iHeight,iWidth);

	//��任�����ͼ����ں�
	iWidth  = m_iWidth/int(pow(float(2.0),m_iTranslayer-1));
	iHeight = m_iHeight/int(pow(float(2.0),m_iTranslayer-1));

   // printf("\n\n*@***m_iHeight=%d****#####***???^**m_iWidth=%d*******\n",TempTransData1F[10][1],TempTransData1F[12][00]);

	for(int i=m_iTranslayer;i>=1;i--)
	{
		DWTi(TempTransData1F,m_psOriginDataF,iWidth,iHeight,i,RADIUS);
		iHeight *=2;				
		iWidth *= 2;
	}

  
	
	  printf("\n\n*@***m_iHeight=%d****#####***???^**m_iWidth=%d*******\n",m_psOriginDataF[100][20],m_psOriginDataF[100][0]);
	  
		char  *m_pImageGreyOut;
		m_pImageGreyOut=(char*)malloc(sizeof(char)*colorLength);  	 
		

	//��ͼ�����ݷ�����ʾ������
	for(int y=0; y<m_iHeight; y++)
	  for(int x=0; x<m_iWidth; x++)
			m_pImageGreyOut[y*m_iWidth+x] = m_psOriginDataF[y][x];
	

	  double *pIHS;
	  pIHS=(double* )malloc(sizeof(double)*colorLength*3);

	  Convert(colorData, grayData,pIHS, colorLength);
	   k=0;
      printf("X=%f, Y=%f,Z=%f\n",pIHS[318],pIHS[13],pIHS[14]);


	  for(int y=0; y<m_iHeight; y++)
	     for(int x=0; x<m_iWidth; x++)
		     pIHS[y*m_iWidth*3+x*3+k] =m_psOriginDataF[y][x];

		 printf("X=%f, Y=%f,Z=%f\n",pIHS[318],pIHS[13],pIHS[14]);

   ReadColorBmp("C:\\XXR_Image\\tm.bmp", colorBody, colorHead, HeightP,WidthP,colorLength);

   unsigned char* colorTemp;
		colorTemp=(unsigned char*)malloc(sizeof(unsigned char)*colorLength*3);
		for( int i =0; i <colorLength*3; i++)
			colorTemp[i]=colorBody[i];

  // pColor=colorBody;

    unsigned char  *colorBodyNew;
	colorBodyNew=(unsigned char*)malloc(sizeof(unsigned char)*colorLength*3);  
 
   k=0;

    for (int i=0;i<colorLength;i++)
    {
       

		double B=colorTemp[k];
        double G=colorTemp[k+1];
        double R=colorTemp[k+2];

	
		double I=pIHS[k];
        double H=pIHS[k+1];
        double S=pIHS[k+2];

		double min;
        
        min=B<G?(R<B?R:B):(R<G?R:G);        
       
        if(min==B)
        {                      
            B=(1.0-S)*I;
            G=3*H*I+(1.0-3*H)*B;
            R=3*I-B-G; 
        }

        else if(min==R)
        {            
            R=(1.0-S)*I;
            B=3*H*I+(1.0-3*H)*R;
            G=3*I-R-B;
        }
        
        else{
            G=(1.0-S)*I;
            R=3*H*I+(1.0-3*H)*G;
            B=3*I-R-G;
            }

        if (B>=255)
        {
            B=255;
        }
        if (B<=0)
        {
            B=0;
        }
        if (G>=255)
        {
            G=255;
        }
        if (G<=0)
        {
            G=0;
        }
        if (R>=255)
        {
            R=255;
        }
        if (R<=0)
        {
            R=0;
        } 
        colorBodyNew[k]=(unsigned char)B;
        colorBodyNew[k+1]=(unsigned char)G;
        colorBodyNew[k+2]=(unsigned char)R; 
		k=k+3;
	}

	//printf("k=%d\n",k);
	printf("colorBodyNew***=%d\n",colorBodyNew[2]);

    WriteColorBmp("F:\\���뱸��\\MPIFusion\\TestData\\WaveFusion_Color.bmp",colorBodyNew, colorHead, colorLength);

}
	
  //  endtime = MPI_Wtime();
   // printf("That takes %f seconds:\n",endtime-starttime);	

    MPI_Finalize();  
    return 0;
}
