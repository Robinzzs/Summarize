#include <stdio.h>
#include <stdlib.h>       //malloc
#include <time.h>
#include <string.h>
#include <ctype.h>        //isprint 用来判断一个字符是否为打印字符
int main()
{
    FILE *tmreader, *fin, *fou;
    unsigned char tmbuf[16*4], buf[16];
    unsigned long count = 0;
    long fsize;
    int tmNsp[20], **rawdata;
    int stime=0; double starttime=0.;   //time function
    float smp = 0.;
    int year=0, month=0, day=0, hour=0, minute=0;
    float second=0.;
    int fnum, fline, n;
    int i=0, it=0, iline=0, endline=0;
    int FO = 1 , verb = 0;      //是否输出十进制原文件，是否打印到屏幕

    /*--------------------------------read time-------------------------------------------*/
    struct tm stm;
    //定向文件
    tmreader = fopen("181.TXT", "r");
    //读入十六进制文件前4行，每行16个十六进制数
    fread(tmbuf, sizeof(char), 16*4, tmreader);
    //显示读取的道头信息
    printf("%s\n",tmbuf);
    //读取时间信息和采样信息
    for (i=0; i<16*4; i++) {
        tmbuf[i] = (int)tmbuf[i];
        //'0'对应十进制ASCII码48，‘9’对应57
        if(tmbuf[i]>=48 && tmbuf[i]<=57) {
            //取出时间和采样数值 时间位数17 采样位数3
            tmNsp[it] = tmbuf[i] - 48;
            //显示数值
            if(verb) printf("num=%d %d \n",it, tmNsp[it]);
            it++;
        }
    }
    //将取出时间
    year += tmNsp[0]*1000;   //年
    year += tmNsp[1]*100;
    year += tmNsp[2]*10;
    year += tmNsp[3];
    month += tmNsp[4]*10;      //月
    month += tmNsp[5];
    day += tmNsp[6]*10;       //日
    day += tmNsp[7];
    hour += tmNsp[8]*10;        //时
    hour += tmNsp[9];
    minute += tmNsp[10]*10;          //分
    minute += tmNsp[11];
    second += (float)tmNsp[12]*10;   //秒
    second += (float)tmNsp[13];
    second += (float)tmNsp[14]/10;
    second += (float)tmNsp[15]/100;
    second += (float)tmNsp[16]/1000;
    //获取时间戳timestemp 利用mktime转换为 距1900年1月1日0时0分0秒时的时间   单位为秒
    stm.tm_year = year - 1900;
    stm.tm_mon = month - 1;
    stm.tm_mday = day;
    stm.tm_hour = hour;
    stm.tm_min = minute;
    stm.tm_sec = 0;
    stm.tm_isdst = 0;
    stime = mktime(&stm);
    starttime = (double)stime + second;
    printf("starttime = %f\n", starttime);
    //printf("%f \n", stime);
    //采样率 单位Hz
    smp += tmNsp[17]*100;
    smp += tmNsp[18]*10;
    smp += tmNsp[19];
    if (verb) printf("sample = %f Hz\n", smp);
    fclose(tmreader);

    /*--------------------------------read data-------------------------------------------*/
    //读取数据
    fin = fopen("181.TXT", "rb");
    //输出十进制格式数据
    if (FO) fou = fopen("DECfile.txt", "w");

    //确定文件大小
    fseek(fin, 0, SEEK_END);
    fsize = ftell(fin);
    //指针返回文件头
    rewind(fin);
    //开辟数据存储空间
    fnum = fsize/sizeof(char);  //个数
    fline = fnum/16;            //行数
    //分配二维数组空间 行数
    rawdata = (int**)malloc(sizeof(int)*(fline-32));
    //loop 按行读取
    for (iline=0; iline<fline; iline++) {
        //分配二维数组 列数
        if (iline>=32) rawdata[iline-32] = (int*)malloc(sizeof(int)*16);
        //按行读取数据，返回单行16进制数据个数（default=16个）
        n = fread(buf, sizeof(char), 16, fin);
        //显示每行数据次序
        if (verb) printf("%08lx ", count);
        if (FO) fprintf(fou, "%08lx ", count);
        //判断行数据是否完整
        if(n < 16) {printf("error read hex file"); break;}
        for (i=0; i<n; i++) {
            //显示单个十六进制数
            if (verb) printf("%02x ", (unsigned)buf[i]);
            //十进制显示
            //printf(" %03d ", (unsigned)buf[i]);
            if (FO) fprintf(fou, "%03d ", (unsigned)buf[i]);
            //数据矩阵赋值
            if (iline>=32) {
                rawdata[iline-32][i] = (int)buf[i];
            }
        }
        //printf("%03d ", rawdata[3]);
        //按字符串形式显示数据
        for (i=0; i<n; i++) {
            if (verb) putchar(isprint(buf[i]) ? buf[i] : '.');
            if (FO) fputc(isprint(buf[i]) ? buf[i] : '.', fou);
        }
        if (verb) putchar('\n');
        if (FO) fputc('\n', fou);
        count += 16;

        //判断非零数据结束位置
        if (iline>=32 && rawdata[iline-32][4]==0 && endline==0 ) {
            endline = iline-32-1 ;
            //printf("rawdata=%d\n", rawdata[iline-32][4]);
            printf("endline=%d\n", endline);
        }
    }
    fclose(fin);
    if (FO) fclose(fou);
    //测试数据是否读入正确
    if (verb) printf("%03d\n", rawdata[61229][8]);

    //原始数据rawdata  大小fnum 每16个一行 非零元素行数endline
    /*--------------------------------processing data----------------------------------------*/
    int freq=16, consN=32768;      //matlab 定义
    float num0, num1;              //num0结束时间 num1开始时间 在matlab程序中没有用处
    double date0, *date;                  //the time set
    float *dianliu1, *dianliu2;   //电流数据
    //num0 = rawdata[endline][0]*256*256*256+rawdata[endline][1]*256*256+rawdata[endline][2]*256+rawdata[endline][3];
    //num1 = rawdata[0][0]*256*256*256+rawdata[0][1]*256*256+rawdata[0][2]*256+rawdata[0][3];
    //printf("num0=%f, num1=%f", num0, num1);

    //matlab程序将时间换算为天 /12960000=/150*24*60*60  c将时间换算成秒/150
    date0 = (double)(rawdata[0][0]*256*256*256 + rawdata[0][1]*256*256 + rawdata[0][2]*256 +
            rawdata[0][3])/150 + starttime;
    //printf("date0=%f\n", date0);

    date = (double*)malloc(sizeof(double)*endline);
    dianliu1 = (float*)malloc(sizeof(float)*endline);
    dianliu2 = (float*)malloc(sizeof(float)*endline);
    for (i=0; i<endline; i++) {
        //计算date
        //matlab程序将时间换算为天 /12960000=/150*24*60*60  c将时间换算成秒/150
        date[i] = (double)(rawdata[i][0]*256*256*256 + rawdata[i][1]*256*256 + rawdata[i][2]*256 +
                   rawdata[i][3])/150 + starttime;

        //计算电流
        dianliu1[i] = 3.72*(rawdata[i][4]*256 + rawdata[i][5]) - 1902;
        dianliu2[i] = 0.5958*(rawdata[i][6]*256 + rawdata[i][7]) - 306.22;
    }
    printf("date[i]=%f\n", date[1]);
    printf("dianliu1[end]=%f\n", dianliu1[endline-1]);
    printf("dianliu2[0]=%f\n", dianliu2[0]);

    return 0;

}
