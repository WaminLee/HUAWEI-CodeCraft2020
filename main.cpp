#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>
#include <stack>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <string>
#include <string.h>
#include <cstring>
#include <set>
#include <unordered_set>
#include <atomic>
#include <unistd.h>
using namespace std;

//#define TEST
#define FIND_MUL_THD 4
#define WRITE_MUL_THD 4
#define MAX_NODE_NUM 2000000

struct note
{
	int no; //对方ID
	int money;//转账金额
	note(int no, int money) : no(no), money(money) {}
};

struct original //原始数据
{
	int giver; //本方ID
	int receiver; //对方ID
	int money;//转账金额
//	int nouse;

};
struct adj //原始数据
{
	int receiver; //ID
	int money;//转账金额

};
struct adj_in //原始数据
{
	int giver; //ID
	int money;//转账金额

};
struct xx23
{
	int x2;
	int x3;
	int n7_n1; //n7-n1的金额，成环时需要跟n1-n2金额比较
	int n5_n6;	
	xx23(int x2, int x3,int n7_n1,int n5_n6) : x2(x2), x3(x3),n7_n1(n7_n1),n5_n6(n5_n6) {}
	xx23() {}
};


int cycle_1_3[3*20000000]; int cycle_1_4[4*20000000]; int cycle_1_5[5*20000000]; int cycle_1_6[6*20000000]; int cycle_1_7[7*20000000];//0.52g
int cycle_2_3[3*20000000]; int cycle_2_4[4*20000000]; int cycle_2_5[5*20000000]; int cycle_2_6[6*20000000]; int cycle_2_7[7*20000000];
int cycle_3_3[3*20000000]; int cycle_3_4[4*20000000]; int cycle_3_5[5*20000000]; int cycle_3_6[6*20000000]; int cycle_3_7[7*20000000];
int cycle_4_3[3*20000000]; int cycle_4_4[4*20000000]; int cycle_4_5[5*20000000]; int cycle_4_6[6*20000000]; int cycle_4_7[7*20000000];

int* cycle[] = {
cycle_1_3,cycle_2_3,cycle_3_3,cycle_4_3,
cycle_1_4,cycle_2_4,cycle_3_4,cycle_4_4,
cycle_1_5,cycle_2_5,cycle_3_5,cycle_4_5,
cycle_1_6,cycle_2_6,cycle_3_6,cycle_4_6,
cycle_1_7,cycle_2_7,cycle_3_7,cycle_4_7
};

int cycle_1_3_soldier[1]; int cycle_1_4_soldier[1]; int cycle_1_5_soldier[1]; int cycle_1_6_soldier[1]; int cycle_1_7_soldier[1];
int cycle_2_3_soldier[1]; int cycle_2_4_soldier[1]; int cycle_2_5_soldier[1]; int cycle_2_6_soldier[1]; int cycle_2_7_soldier[1];
int cycle_3_3_soldier[1]; int cycle_3_4_soldier[1]; int cycle_3_5_soldier[1]; int cycle_3_6_soldier[1]; int cycle_3_7_soldier[1];
int cycle_4_3_soldier[1]; int cycle_4_4_soldier[1]; int cycle_4_5_soldier[1]; int cycle_4_6_soldier[1]; int cycle_4_7_soldier[1];
int* cycle_soldier[] = {
cycle_1_3_soldier,cycle_2_3_soldier,cycle_3_3_soldier,cycle_4_3_soldier,
cycle_1_4_soldier,cycle_2_4_soldier,cycle_3_4_soldier,cycle_4_4_soldier,
cycle_1_5_soldier,cycle_2_5_soldier,cycle_3_5_soldier,cycle_4_5_soldier,
cycle_1_6_soldier,cycle_2_6_soldier,cycle_3_6_soldier,cycle_4_6_soldier,
cycle_1_7_soldier,cycle_2_7_soldier,cycle_3_7_soldier,cycle_4_7_soldier
};

int nNodes;

int node_index[MAX_NODE_NUM];//ID>20w再用map
unordered_map<int, int> node_index_e;


char node_str[MAX_NODE_NUM][11];
int node_str_len[MAX_NODE_NUM];

char ansbuf1[1000000000];
char ansbuf2[1000000000];
char ansbuf3[1000000000];
char ansbuf4[1000000000];
char ansbuf5[1000000000];
char ansbuf6[1540000000];//1.43g
char* ansbuf[]= {ansbuf1,ansbuf2,ansbuf3,ansbuf4,ansbuf5,ansbuf6};


int OriRecNum;
int OriRecNum_TH[4];
original OutRecord_ori_TH[4][2000000];
int recordsort_TH[4][2000000];
original OutRecord_ori[2000000];
original InRecord_ori[2000000];
adj OutRecord[2000000];
adj_in InRecord[2000000];
int adjlist_start[2000000];
int adjlist[2000000];
int adjlist_in[2000000][2];
int recordsort[2000000];


int reach1[MAX_NODE_NUM];
int adjRecord1[MAX_NODE_NUM];
bool Reach5_1[MAX_NODE_NUM];
bool visit1[MAX_NODE_NUM];

int reach2[MAX_NODE_NUM];
int adjRecord2[MAX_NODE_NUM];
bool Reach5_2[MAX_NODE_NUM];
bool visit2[MAX_NODE_NUM];

int reach3[MAX_NODE_NUM];
int adjRecord3[MAX_NODE_NUM];
bool Reach5_3[MAX_NODE_NUM];
bool visit3[MAX_NODE_NUM];

int reach4[MAX_NODE_NUM];
int adjRecord4[MAX_NODE_NUM];
bool Reach5_4[MAX_NODE_NUM];
bool visit4[MAX_NODE_NUM];


string infile = "/data/test_data.txt";
string outfile = "/projects/student/result.txt";


inline bool cmp_OriRec1(original &a, original &b)
{
	return a.giver < b.giver;
}
inline bool cmp_adjlist(note& x1, note& x2)
{
	return x1.no < x2.no;
}
inline bool cmp_pathThree(xx23& a, xx23& b)
{
	return  a.x2 != b.x2 ? a.x2 < b.x2 : a.x3 < b.x3;
}

inline bool cmp_data_out(original &a, original &b)
{
	if (a.giver != b.giver) return a.giver < b.giver;//giver
	return a.receiver < b.receiver;//receiver

}

inline bool cmp_data_in(original &a, original &b)
{
	return a.receiver < b.receiver;//receiver
}



atomic_int record_signal(0);
atomic_int record_num(0);

void TH_date_read(char *buf,int FileLen,int th)
{
	char* thisBuf = buf;
	int x1, x2, x3;
	int cnt = 0;	
	while (thisBuf - buf + 1 < FileLen)
	{
		x1 = 0;x2 = 0;x3 = 0;
		while (*thisBuf != ',')
		{
			x1 = x1*10  + *thisBuf - '0';
			++thisBuf;
		}
		++thisBuf;
		while (*thisBuf != ',')
		{
			x2 = x2*10 + *thisBuf - '0';
			++thisBuf;
		}
		++thisBuf ;

		while (*thisBuf != '\r' && *thisBuf != '\n')
		{
			x3 = x3*10 + *thisBuf - '0';
			++thisBuf;
		}
		++thisBuf;
		if(*thisBuf == '\n')++thisBuf;

		recordsort_TH[th][cnt] = x2;

		OutRecord_ori_TH[th][cnt].giver = x1;
		OutRecord_ori_TH[th][cnt].receiver = x2;
		OutRecord_ori_TH[th][cnt].money = x3;

		++cnt;
	}	
	OriRecNum_TH[th]=cnt;
	sort(recordsort_TH[th], recordsort_TH[th]+cnt);//预排序，减小之后单线程时的排序压力
	
	while(record_signal!=th);
	x1 = record_num;
	record_num += cnt;
	record_signal++;
	memcpy(recordsort+x1,recordsort_TH[th],cnt*4);
	memcpy(OutRecord_ori+x1,OutRecord_ori_TH[th],cnt*12);
	memcpy(InRecord_ori+x1,OutRecord_ori_TH[th],cnt*12);
	
}


void date_read()
{
	//int fd = open(string("/data/test_data.N111314.E200W.A19630345.txt").c_str(), O_RDONLY);
	//int fd = open(string("/data/test_data_SFN.N1560268.E200W.A18875018.txt").c_str(), O_RDONLY);
	//int fd = open(string("/data/test_data697518.txt").c_str(), O_RDONLY);
	int fd = open(infile.c_str(), O_RDONLY);

	int FileLen = lseek(fd, 0, SEEK_END);
	char* buf = (char*)mmap(NULL, FileLen, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	int x1=FileLen/4;
	int cnt=x1;
	int start;
	while(*(buf+cnt)!='\n')++cnt;++cnt;
	thread t1(TH_date_read,buf,cnt,0);

	start=cnt;
	cnt+=x1;
	while(*(buf+cnt)!='\n')++cnt;++cnt;
	thread t2(TH_date_read,buf+start,cnt-start,1);

	start=cnt;
	cnt+=x1;
	while(*(buf+cnt)!='\n')++cnt;++cnt;
    thread t3(TH_date_read,buf+start,cnt-start,2);

    thread t4(TH_date_read,buf+cnt,FileLen-cnt,3);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

	OriRecNum = OriRecNum_TH[0]+OriRecNum_TH[1]+OriRecNum_TH[2]+OriRecNum_TH[3];

	sort(recordsort, recordsort+OriRecNum);
	nNodes = unique(recordsort, recordsort+OriRecNum) - recordsort;
	//cout << "tyu:" << clock()-tyu << endl;
	
	cnt = lower_bound(recordsort, recordsort+nNodes, MAX_NODE_NUM)-recordsort;
	

	for(int i=0; i<cnt;++i) //对node_str赋值
	{
		x1 = recordsort[i];
		node_index[x1] = i;
	}
	for(int i=cnt; i<nNodes;++i) //对node_str赋值
	{
		x1 = recordsort[i];
		node_index_e[x1] = i;
	}

}

//volatile int MagicNode = 0;
atomic_int MagicNode(-1);
atomic_int FC_signal(0);
int n1_last_1=0;
int n1_last_2=0;
int n1_last_3=0;
int n1_last_4=0;
xx23 path_three1[16384][256];
unsigned int path_three1_num[16384];
unsigned int p_index_1[2000000];
void TD_findCycle1()
{
	 int i, j, n1, n2;
	 int k=0;
	sort(OutRecord_ori, OutRecord_ori + OriRecNum, cmp_data_out);
	for(i=0; i<nNodes;++i)//adjlist
	{
		//pos=0;
		n1 = recordsort[i];
		while(OutRecord_ori[k].giver < n1) ++k;

		adjlist_start[i] = k;
		while(OutRecord_ori[k].giver==n1)
		{
			j = OutRecord_ori[k].receiver;
			n2 = j < MAX_NODE_NUM ? node_index[j] : node_index_e[j];//receiver映射
			OutRecord[k].receiver = n2;
			OutRecord[k].money = OutRecord_ori[k].money;
			++k;
		}
		adjlist[i] = k;		
	}
	++FC_signal;

    int trace[8];
    int n3, n4, n5, firstBigPos,\
	node2, node3, node4, node5, mulUseA,mulUseB,\
	n7_money, n6_money, n5_money, n4_money, n3_money, n2_money, n1_money;
	long  n4_mx5, n3_mx5, n1_mx5, n4_mx3, n3_mx3, n2_mx3, mulUse_long_F,mulUse_long_G;
	float n1_money_div3,n2_money_div5;
	int n1_e, n2_e, n3_e,n4_e;
	while(FC_signal!=4);
	for (i = 0; i < nNodes; ++i)
	{
		adjRecord1[i] = adjlist_start[i];
	}

	while(true)
	{
		n1 = ++MagicNode;
		if(n1>=nNodes)return;
		for(i=n1_last_1;i<n1;i++)
		{
			j = adjlist_in[i][1];
        	for (k = adjlist_in[i][0]; k < j; ++k) 
        	{
				++adjRecord1[InRecord[k].giver];
			}	
		}
		n1_last_1=n1;		
		n1_e = adjlist[n1];
        if (adjRecord1[n1]==n1_e || OutRecord[n1_e-1].receiver < n1)
        {
            continue;
        }
		n4_e=0;//pos

		node5 = adjlist_in[n1][1];
        for (i = adjlist_in[n1][0]; i < node5; ++i) 
        {
            node2 = InRecord[i].giver;
            if (node2 > n1)
            {			
				n7_money = InRecord[i].money; //n7-n1
				mulUse_long_F = 5 * (long)(n7_money);	
                for (j = adjlist_in[node2][0]; j < adjlist_in[node2][1]; ++j)
                {
                    node3 = InRecord[j].giver;
					n6_money = InRecord[j].money;//n6-n7
                    if (node3 > n1)// &&node3!=node2
                    {
						mulUse_long_G = (long)(n6_money) * 3;
						if (n6_money > mulUse_long_F||n7_money > mulUse_long_G) continue;
						mulUse_long_G = (long)(n6_money) * 5;						
						for (k = adjlist_in[node3][0]; k < adjlist_in[node3][1]; ++k)
						{
							node4 = InRecord[k].giver;
							n5_money = InRecord[k].money;//n5-n6
							if (node4>n1&&node4!=node2){ //&&node3!=node4
								n4_mx3 = (long)(n5_money)*3;
								if (n5_money > mulUse_long_G||n6_money > n4_mx3) continue;						
								if (!Reach5_1[node4]) {
									Reach5_1[node4]=true;
									++reach1[0]; reach1[reach1[0]] = node4;
									p_index_1[node4] = n4_e;
									++n4_e;
								}								
								firstBigPos = p_index_1[node4];
								mulUseA = path_three1_num[firstBigPos];
								++path_three1_num[firstBigPos];
								path_three1[firstBigPos][mulUseA].x2 = node3;
								path_three1[firstBigPos][mulUseA].x3 = node2;
								path_three1[firstBigPos][mulUseA].n7_n1 = n7_money;
								path_three1[firstBigPos][mulUseA].n5_n6 = n5_money;
							}
						}
                    }
                }
            }
        }
		

		
		j= reach1[0];
		for (i = 1; i <= j; i++)
		{
			mulUseB = p_index_1[reach1[i]];
			sort(path_three1[mulUseB], path_three1[mulUseB]+path_three1_num[mulUseB], cmp_pathThree);			
		}
		

    
        visit1[n1] = true;
        trace[0] = n1;

        firstBigPos = adjRecord1[n1];
        for (n2 = firstBigPos; n2 <n1_e; ++n2) {
            node2 = OutRecord[n2].receiver;
			n1_money = OutRecord[n2].money;
			n2_e = adjlist[node2];
            if (adjRecord1[node2]==n2_e || visit1[node2] == true || OutRecord[n2_e-1].receiver < n1)
            {
                continue;
            }
			n1_money_div3 = float(n1_money)/3;
            visit1[node2] = true;
            trace[1] = node2;
			n1_mx5 = (long)(n1_money) * 5;//n1-n2
			mulUse_long_G = (long)(n1_money) * 3;	

            firstBigPos = adjRecord1[node2];	
            for (n3 = firstBigPos; n3 < n2_e; ++n3) {
                node3 = OutRecord[n3].receiver;
				n2_money = OutRecord[n3].money;
				n3_e = adjlist[node3];
                if (adjRecord1[node3] == n3_e || visit1[node3] == true || OutRecord[n3_e-1].receiver < n1)
                {
                    continue;
                }
				mulUse_long_F = (long)(n2_money) * 5;
				n2_mx3 = (long)(n2_money) * 3;	
				if (n1_money > mulUse_long_F||n2_money > mulUse_long_G) continue;

                firstBigPos = adjRecord1[node3];
                visit1[node3] = true;
                trace[2] = node3;			
                if (OutRecord[firstBigPos].receiver == n1) {
					n3_money = OutRecord[firstBigPos].money;//n3-n1
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;					
					if (n2_money <= n3_mx5&&n3_money <= n2_mx3) {//n2-n3
						if (n3_money <= n1_mx5&&n1_money <= n3_mx3) {				
                    		memcpy(cycle_1_3 + cycle_1_3_soldier[0], trace, 12);
                    		cycle_1_3_soldier[0] += 3;
						}
					}
                }

                        //环5j
						n2_money_div5 = float(n2_money)/5;
                        if (Reach5_1[node3])//等价之前的条件
                        {
							mulUseB = p_index_1[node3];
                            for (i = 0; i <path_three1_num[mulUseB]; ++i) 
                            {
	                            node5 = path_three1[mulUseB][i].x3;
								n5_money = path_three1[mulUseB][i].n7_n1;
								node4 = path_three1[mulUseB][i].x2;
								n3_money = path_three1[mulUseB][i].n5_n6;	
								if (!visit1[node4] && !visit1[node5] && n2_money_div5 <= n3_money&&n3_money <= n2_mx3 &&n5_money <= n1_mx5&&n1_money_div3 <= n5_money)
								{										
                                    		trace[3] = node4;
                                    		trace[4] = node5;
											mulUseA = cycle_1_5_soldier[0];
											cycle_1_5[mulUseA] = trace[0];
											cycle_1_5[++mulUseA] = trace[1];
											cycle_1_5[++mulUseA] = trace[2];
											cycle_1_5[++mulUseA] = trace[3];
											cycle_1_5[++mulUseA] = trace[4];																			
                                    		cycle_1_5_soldier[0] += 5;
                                }
                            }
                        }


                for (n4 = firstBigPos; n4 < n3_e; ++n4) {
                    node4 = OutRecord[n4].receiver;
					n3_money = OutRecord[n4].money;//n3-n4		
					n4_e = adjlist[node4];
                    if (adjRecord1[node4] == n4_e || visit1[node4] == true || OutRecord[n4_e-1].receiver < n1)
                    {
                        continue;
                    }
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;						
					if (n2_money > n3_mx5||n3_money > n2_mx3) continue;

                    firstBigPos = adjRecord1[node4];
                    visit1[node4] = true;
                    trace[3] = node4;				
                    if (OutRecord[firstBigPos].receiver == n1) {
						n4_money = OutRecord[firstBigPos].money;
						n4_mx5 = (long)(n4_money) * 5;
						n4_mx3 = (long)(n4_money) * 3;					
						if (n3_money <= n4_mx5&&n4_money <= n3_mx3) {//n2-n3
							if (n4_money <= n1_mx5&&n1_money <= n4_mx3) {	
								mulUseA = cycle_1_4_soldier[0];
								cycle_1_4[mulUseA] = trace[0];
								cycle_1_4[++mulUseA] = trace[1];
								cycle_1_4[++mulUseA] = trace[2];
								cycle_1_4[++mulUseA] = trace[3];														
                        		cycle_1_4_soldier[0] += 4;
							}
						}
                    }

					n2_money_div5 = float(n3_money)/5;						
					if (Reach5_1[node4])
					{
	
						mulUseB = p_index_1[node4];
						for (i = 0; i < path_three1_num[mulUseB]; ++i)
						{
                            mulUseA = path_three1[mulUseB][i].x3;
							n6_money = path_three1[mulUseB][i].n7_n1;
							node5 = path_three1[mulUseB][i].x2;
							n4_money = path_three1[mulUseB][i].n5_n6;	

							if (!visit1[node5] && !visit1[mulUseA] && n2_money_div5 <= n4_money&&n4_money <= n3_mx3 &&n6_money <= n1_mx5&&n1_money_div3 <= n6_money)	
							{								
										trace[4] = node5;
										trace[5] = mulUseA;
										mulUseA = cycle_1_6_soldier[0];
										cycle_1_6[mulUseA] = trace[0];
										cycle_1_6[++mulUseA] = trace[1];
										cycle_1_6[++mulUseA] = trace[2];
										cycle_1_6[++mulUseA] = trace[3];
										cycle_1_6[++mulUseA] = trace[4];
										cycle_1_6[++mulUseA] = trace[5];										
										cycle_1_6_soldier[0] += 6;
							}
						}
					}

                    for (n5 = firstBigPos; n5 < n4_e; ++n5) {
                        node5 = OutRecord[n5].receiver;
						if(!Reach5_1[node5]||visit1[node5])continue;

						n4_money = OutRecord[n5].money;						
						n4_mx5 = (long)(n4_money) * 5;
						if (n3_money > n4_mx5||n4_money > n3_mx3) continue;
						n4_mx3 = (long)(n4_money) * 3;	
						trace[4] = node5;

                        //环7
						//auto ttt=clock();
						n2_money_div5 = float(n4_money)/5;
						j = p_index_1[node5];
                        firstBigPos = path_three1_num[j];//path_three1_num[node5];
                            for (i = 0; i < firstBigPos; ++i) 
                            {
	                            mulUseB = path_three1[j][i].x3;//node7
								n7_money = path_three1[j][i].n7_n1;
								mulUseA = path_three1[j][i].x2;
								n5_money = path_three1[j][i].n5_n6;									
                                if (!visit1[mulUseA] && !visit1[mulUseB] && n2_money_div5 <= n5_money&&n5_money <= n4_mx3 &&n7_money <= n1_mx5&&n1_money_div3 <= n7_money)
								{								
                                    		trace[5] = mulUseA;
											mulUseA = cycle_1_7_soldier[0];
											cycle_1_7[mulUseA] = trace[0];
											cycle_1_7[++mulUseA] = trace[1];
											cycle_1_7[++mulUseA] = trace[2];
											cycle_1_7[++mulUseA] = trace[3];
											cycle_1_7[++mulUseA] = trace[4];
											cycle_1_7[++mulUseA] = trace[5];
											cycle_1_7[++mulUseA] = mulUseB;//trace[6];
                                    		cycle_1_7_soldier[0] += 7;    
                                }							
                            }

                    }

                    visit1[node4] = false;
                }

                visit1[node3] = false;
            }

            visit1[node2] = false;
        }

        j = reach1[0];
        for (i = 1; i <= j; ++i) //还可以放在第六层循环，减少次数（设置只执行一次）
        {
			mulUseA = p_index_1[reach1[i]];
			path_three1_num[mulUseA]=0;
			Reach5_1[reach1[i]]=false;
        }

        reach1[0] = 0;
        visit1[n1] = false;


		
#ifdef TEST
        if (n1 % 3000 == 0) {
            cout << n1 << "/" << nNodes  << endl;
        }

#endif
    }

}

xx23 path_three2[16384][256];
unsigned int path_three2_num[16384];
unsigned int p_index_2[2000000];
void TD_findCycle2()
{

	 int i, j, n1, n2;
	 int k=0;
	sort(InRecord_ori, InRecord_ori + OriRecNum, cmp_data_in);
	for (int i = 0; i < nNodes; ++i)//adjlist_in
	{
		n1 = recordsort[i];

		adjlist_in[i][0] = k;
		while (InRecord_ori[k].receiver == n1)
		{
			j = InRecord_ori[k].giver;
			n2 = j < MAX_NODE_NUM ? node_index[j] : node_index_e[j];//giver映射
			InRecord[k].giver = n2;
			InRecord[k].money = InRecord_ori[k].money;
								
			++k;
		}
		adjlist_in[i][1] = k;
	}

	++FC_signal;

    int trace[8];
    int n3, n4, n5, firstBigPos,\
	node2, node3, node4, node5, mulUseA,mulUseB,\
	n7_money, n6_money, n5_money, n4_money, n3_money, n2_money, n1_money;
	long  n4_mx5, n3_mx5, n1_mx5, n4_mx3, n3_mx3, n2_mx3, mulUse_long_F,mulUse_long_G;
	float n1_money_div3,n2_money_div5;
	int n1_e, n2_e, n3_e,n4_e;
	while(FC_signal!=4);
	for (i = 0; i < nNodes; ++i)
	{
		adjRecord2[i] = adjlist_start[i];
	}

	
	while(true)
	{
		n1 = ++MagicNode;
		if(n1>=nNodes)return;
		for(i=n1_last_2;i<n1;i++)
		{
			j = adjlist_in[i][1];
        	for (k = adjlist_in[i][0]; k < j; ++k) 
        	{
				++adjRecord2[InRecord[k].giver];
			}	
		}
		n1_last_2=n1;	
		n1_e = adjlist[n1];
        if (adjRecord2[n1]==n1_e || OutRecord[n1_e-1].receiver < n1)
        {
            continue;
        }
		n4_e=0;
		node5 = adjlist_in[n1][1];
        for (i = adjlist_in[n1][0]; i < node5; ++i) 
        {
            node2 = InRecord[i].giver;
            if (node2 > n1)
            {			
				n7_money = InRecord[i].money; //n7-n1
				mulUse_long_F = 5 * (long)(n7_money);	
                for (j = adjlist_in[node2][0]; j < adjlist_in[node2][1]; ++j)
                {
                    node3 = InRecord[j].giver;
					n6_money = InRecord[j].money;//n6-n7
                    if (node3 > n1)// &&node3!=node2
                    {
						mulUse_long_G = (long)(n6_money) * 3;
						if (n6_money > mulUse_long_F||n7_money > mulUse_long_G) continue;
						mulUse_long_G = (long)(n6_money) * 5;						
						for (k = adjlist_in[node3][0]; k < adjlist_in[node3][1]; ++k)
						{
							node4 = InRecord[k].giver;
							n5_money = InRecord[k].money;//n5-n6
							if (node4>n1&&node4!=node2){ //&&node3!=node4
								n4_mx3 = (long)(n5_money)*3;
								if (n5_money > mulUse_long_G||n6_money > n4_mx3) continue;					
								if (!Reach5_2[node4]) {
									Reach5_2[node4]=true;
									++reach2[0]; reach2[reach2[0]] = node4;
									p_index_2[node4] = n4_e;
									++n4_e;
								}								
								firstBigPos = p_index_2[node4];
								mulUseA = path_three2_num[firstBigPos];
								++path_three2_num[firstBigPos];
								path_three2[firstBigPos][mulUseA].x2 = node3;
								path_three2[firstBigPos][mulUseA].x3 = node2;
								path_three2[firstBigPos][mulUseA].n7_n1 = n7_money;
								path_three2[firstBigPos][mulUseA].n5_n6 = n5_money;

							}
						}
                    }
                }
            }
        }
		

		
		j= reach2[0];
		for (i = 1; i <= j; i++)
		{
			mulUseB = p_index_2[reach2[i]];
			sort(path_three2[mulUseB], path_three2[mulUseB]+path_three2_num[mulUseB], cmp_pathThree);			
		}
		

    
        visit2[n1] = true;
        trace[0] = n1;

        firstBigPos = adjRecord2[n1];
        for (n2 = firstBigPos; n2 <n1_e; ++n2) {
            node2 = OutRecord[n2].receiver;
			n1_money = OutRecord[n2].money;
			n2_e = adjlist[node2];
            //adLen2 = adjlist[node2].size();//adjlist_len[node2];
            if (adjRecord2[node2]==n2_e || visit2[node2] == true || OutRecord[n2_e-1].receiver < n1)
            {
                continue;
            }
			n1_money_div3 = float(n1_money)/3;
            visit2[node2] = true;
            trace[1] = node2;
			n1_mx5 = (long)(n1_money) * 5;//n1-n2
			mulUse_long_G = (long)(n1_money) * 3;	

            firstBigPos = adjRecord2[node2];
		
            for (n3 = firstBigPos; n3 < n2_e; ++n3) {
                node3 = OutRecord[n3].receiver;
				n2_money = OutRecord[n3].money;
				n3_e = adjlist[node3];
                //adLen3 = adjlist[node3].size();//adjlist_len[node3];
                if (adjRecord2[node3] == n3_e || visit2[node3] == true || OutRecord[n3_e-1].receiver < n1)
                {
                    continue;
                }
				mulUse_long_F = (long)(n2_money) * 5;
				n2_mx3 = (long)(n2_money) * 3;	
				if (n1_money > mulUse_long_F||n2_money > mulUse_long_G) continue;

                firstBigPos = adjRecord2[node3];

                visit2[node3] = true;
                trace[2] = node3;			
                if (OutRecord[firstBigPos].receiver == n1) {
					n3_money = OutRecord[firstBigPos].money;//n3-n1
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;					
					if (n2_money <= n3_mx5&&n3_money <= n2_mx3) {//n2-n3
						if (n3_money <= n1_mx5&&n1_money <= n3_mx3) {				
                    		memcpy(cycle_2_3 + cycle_2_3_soldier[0], trace, 12);
                    		cycle_2_3_soldier[0] += 3;
						}
					}
                }

                        //环5j
						n2_money_div5 = float(n2_money)/5;
                        if (Reach5_2[node3])//等价之前的条件
                        {
							mulUseB = p_index_2[node3];
                            for (i = 0; i <path_three2_num[mulUseB]; ++i) 
                            {
	                            node5 = path_three2[mulUseB][i].x3;
								n5_money = path_three2[mulUseB][i].n7_n1;
								node4 = path_three2[mulUseB][i].x2;
								n3_money = path_three2[mulUseB][i].n5_n6;	

								if (!visit2[node4] && !visit2[node5] && n2_money_div5 <= n3_money&&n3_money <= n2_mx3 &&n5_money <= n1_mx5&&n1_money_div3 <= n5_money)
								{										
                                    		trace[3] = node4;
                                    		trace[4] = node5;
											mulUseA = cycle_2_5_soldier[0];
											cycle_2_5[mulUseA] = trace[0];
											cycle_2_5[++mulUseA] = trace[1];
											cycle_2_5[++mulUseA] = trace[2];
											cycle_2_5[++mulUseA] = trace[3];
											cycle_2_5[++mulUseA] = trace[4];											
											//memcpy(cycle_2_5 + cycle_2_5_soldier[0], trace, 20);									
                                    		cycle_2_5_soldier[0] += 5;
                                }
                            }
                        }


                for (n4 = firstBigPos; n4 < n3_e; ++n4) {
                    node4 = OutRecord[n4].receiver;
					n3_money = OutRecord[n4].money;//n3-n4		
					n4_e = adjlist[node4];
                    //adLen4 = adjlist[node4].size();//adjlist_len[node4];
                    if (adjRecord2[node4] == n4_e || visit2[node4] == true || OutRecord[n4_e-1].receiver < n1)
                    {
                        continue;
                    }
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;						
					if (n2_money > n3_mx5||n3_money > n2_mx3) continue;

                    firstBigPos = adjRecord2[node4];

                    visit2[node4] = true;
                    trace[3] = node4;				
                    if (OutRecord[firstBigPos].receiver == n1) {
						n4_money = OutRecord[firstBigPos].money;
						n4_mx5 = (long)(n4_money) * 5;
						n4_mx3 = (long)(n4_money) * 3;					
						if (n3_money <= n4_mx5&&n4_money <= n3_mx3) {//n2-n3
							if (n4_money <= n1_mx5&&n1_money <= n4_mx3) {	
								mulUseA = cycle_2_4_soldier[0];
								cycle_2_4[mulUseA] = trace[0];
								cycle_2_4[++mulUseA] = trace[1];
								cycle_2_4[++mulUseA] = trace[2];
								cycle_2_4[++mulUseA] = trace[3];														
                        		//memcpy(cycle_2_4 + cycle_2_4_soldier[0], trace, 16);
                        		cycle_2_4_soldier[0] += 4;
							}
						}
                    }

					n2_money_div5 = float(n3_money)/5;						
					if (Reach5_2[node4])
					{
						mulUseB = p_index_2[node4];
						for (i = 0; i < path_three2_num[mulUseB]; ++i)
						{
                            mulUseA = path_three2[mulUseB][i].x3;
							n6_money = path_three2[mulUseB][i].n7_n1;
							node5 = path_three2[mulUseB][i].x2;
							n4_money = path_three2[mulUseB][i].n5_n6;
	
							if (!visit2[node5] && !visit2[mulUseA] && n2_money_div5 <= n4_money&&n4_money <= n3_mx3 &&n6_money <= n1_mx5&&n1_money_div3 <= n6_money)	
							{								
										trace[4] = node5;
										trace[5] = mulUseA;
										//memcpy(cycle_2_6 + cycle_2_6_soldier[0], trace, 24);
										mulUseA = cycle_2_6_soldier[0];
										cycle_2_6[mulUseA] = trace[0];
										cycle_2_6[++mulUseA] = trace[1];
										cycle_2_6[++mulUseA] = trace[2];
										cycle_2_6[++mulUseA] = trace[3];
										cycle_2_6[++mulUseA] = trace[4];
										cycle_2_6[++mulUseA] = trace[5];										
										cycle_2_6_soldier[0] += 6;
							}
						}
					}

                    for (n5 = firstBigPos; n5 < n4_e; ++n5) {
                        node5 = OutRecord[n5].receiver;
						if(!Reach5_2[node5]||visit2[node5])continue;

						n4_money = OutRecord[n5].money;						
						n4_mx5 = (long)(n4_money) * 5;
						if (n3_money > n4_mx5||n4_money > n3_mx3) continue;
						n4_mx3 = (long)(n4_money) * 3;	
						trace[4] = node5;

                        //环7
						//auto ttt=clock();
						n2_money_div5 = float(n4_money)/5;
						j = p_index_2[node5];
                        firstBigPos = path_three2_num[j];//path_three2_num[node5];
                            for (i = 0; i < firstBigPos; ++i) 
                            {
	                            mulUseB = path_three2[j][i].x3;//node7
								n7_money = path_three2[j][i].n7_n1;
								mulUseA = path_three2[j][i].x2;
								n5_money = path_three2[j][i].n5_n6;			
                                if (!visit2[mulUseA] && !visit2[mulUseB] && n2_money_div5 <= n5_money&&n5_money <= n4_mx3 &&n7_money <= n1_mx5&&n1_money_div3 <= n7_money)
								{								
                                    		trace[5] = mulUseA;
                                    		//trace[6] = mulUseB;
											mulUseA = cycle_2_7_soldier[0];
											cycle_2_7[mulUseA] = trace[0];
											cycle_2_7[++mulUseA] = trace[1];
											cycle_2_7[++mulUseA] = trace[2];
											cycle_2_7[++mulUseA] = trace[3];
											cycle_2_7[++mulUseA] = trace[4];
											cycle_2_7[++mulUseA] = trace[5];
											cycle_2_7[++mulUseA] = mulUseB;//trace[6];
                                    		cycle_2_7_soldier[0] += 7;    
                                }							
                            }

                    }

                    visit2[node4] = false;
                }

                visit2[node3] = false;
            }

            visit2[node2] = false;
        }

        j = reach2[0];
        for (i = 1; i <= j; ++i) //还可以放在第六层循环，减少次数（设置只执行一次）
        {
			mulUseA = p_index_2[reach2[i]];
			path_three2_num[mulUseA]=0;
			Reach5_2[reach2[i]]=false;
        }

        reach2[0] = 0;
        visit2[n1] = false;

		
#ifdef TEST
        if (n1 % 3000 == 0) {
            cout << n1 << "/" << nNodes  << endl;
        }

#endif
    }

}

xx23 path_three3[16384][256];
unsigned int path_three3_num[16384];
unsigned int p_index_3[2000000];
void TD_findCycle3()
{
	int i, j, n1;
	j = lower_bound(recordsort, recordsort+nNodes, MAX_NODE_NUM)-recordsort;
	string x_c;
	for(i=0; i<j;++i) //对node_str赋值
	{
		n1 = recordsort[i];

		x_c = to_string(n1)+',';
		node_str_len[i]=x_c.size();
		memcpy(node_str[i], x_c.c_str(), node_str_len[i]);
	}
	for(i=j; i<nNodes;++i) //对node_str赋值
	{
		n1 = recordsort[i];

		x_c = to_string(n1)+',';
		node_str_len[i]=x_c.size();
		memcpy(node_str[i], x_c.c_str(), node_str_len[i]);

	}

	++FC_signal;

    int trace[8];
    int k, n2, n3, n4, n5, firstBigPos,\
	node2, node3, node4, node5, mulUseA,mulUseB,\
	n7_money, n6_money, n5_money, n4_money, n3_money, n2_money, n1_money;
	long  n4_mx5, n3_mx5, n1_mx5, n4_mx3, n3_mx3, n2_mx3, mulUse_long_F,mulUse_long_G;
	float n1_money_div3,n2_money_div5;
	int n1_e, n2_e, n3_e,n4_e;
	while(FC_signal!=4);
	for (i = 0; i < nNodes; ++i)
	{
		adjRecord3[i] = adjlist_start[i];
	}


	while(true)
	{
		n1 = ++MagicNode;
		if(n1>=nNodes)return;
		for(i=n1_last_3;i<n1;i++)
		{
			j = adjlist_in[i][1];
        	for (k = adjlist_in[i][0]; k < j; ++k) 
        	{
				++adjRecord3[InRecord[k].giver];
			}	
		}
		n1_last_3=n1;	
		n1_e = adjlist[n1];
        if (adjRecord3[n1]==n1_e || OutRecord[n1_e-1].receiver < n1)
        {
            continue;
        }
		n4_e=0;
        //firstBigPos = adjlist_in[n1].size();//这里用firstBigPos代替原来的变量
		node5 = adjlist_in[n1][1];
        for (i = adjlist_in[n1][0]; i < node5; ++i) 
        {
            node2 = InRecord[i].giver;
            if (node2 > n1)
            {			
                //mulUseA = adjlist_in[node2].size();//adjlist_in_len[n7_re];
				n7_money = InRecord[i].money; //n7-n1
				mulUse_long_F = 5 * (long)(n7_money);	
                for (j = adjlist_in[node2][0]; j < adjlist_in[node2][1]; ++j)
                {
                    node3 = InRecord[j].giver;
					n6_money = InRecord[j].money;//n6-n7
                    if (node3 > n1)// &&node3!=node2
                    {
						mulUse_long_G = (long)(n6_money) * 3;
						if (n6_money > mulUse_long_F||n7_money > mulUse_long_G) continue;
						mulUse_long_G = (long)(n6_money) * 5;						
						//mulUseB = adjlist_in[node3].size();// adjlist_in_len[node3];
						for (k = adjlist_in[node3][0]; k < adjlist_in[node3][1]; ++k)
						{
							node4 = InRecord[k].giver;
							n5_money = InRecord[k].money;//n5-n6
							if (node4>n1&&node4!=node2){ //&&node3!=node4
								n4_mx3 = (long)(n5_money)*3;
								if (n5_money > mulUse_long_G||n6_money > n4_mx3) continue;
								if (!Reach5_3[node4]) {
									Reach5_3[node4]=true;
									++reach3[0]; reach3[reach3[0]] = node4;
									p_index_3[node4] = n4_e;
									++n4_e;
								}								
								firstBigPos = p_index_3[node4];
								mulUseA = path_three3_num[firstBigPos];
								++path_three3_num[firstBigPos];
								path_three3[firstBigPos][mulUseA].x2 = node3;
								path_three3[firstBigPos][mulUseA].x3 = node2;
								path_three3[firstBigPos][mulUseA].n7_n1 = n7_money;
								path_three3[firstBigPos][mulUseA].n5_n6 = n5_money;
							}
						}
                    }
                }
            }
        }
		

		
		j= reach3[0];
		for (i = 1; i <= j; i++)
		{
			mulUseB = p_index_3[reach3[i]];
			sort(path_three3[mulUseB], path_three3[mulUseB]+path_three3_num[mulUseB], cmp_pathThree);			
		}
		

    
        visit3[n1] = true;
        trace[0] = n1;

        firstBigPos = adjRecord3[n1];
        for (n2 = firstBigPos; n2 <n1_e; ++n2) {
            node2 = OutRecord[n2].receiver;
			n1_money = OutRecord[n2].money;
			n2_e = adjlist[node2];
            //adLen2 = adjlist[node2].size();//adjlist_len[node2];
            if (adjRecord3[node2]==n2_e || visit3[node2] == true || OutRecord[n2_e-1].receiver < n1)
            {
                continue;
            }
			n1_money_div3 = float(n1_money)/3;
            visit3[node2] = true;
            trace[1] = node2;
			n1_mx5 = (long)(n1_money) * 5;//n1-n2
			mulUse_long_G = (long)(n1_money) * 3;	

            firstBigPos = adjRecord3[node2];		
            for (n3 = firstBigPos; n3 < n2_e; ++n3) {
                node3 = OutRecord[n3].receiver;
				n2_money = OutRecord[n3].money;
				n3_e = adjlist[node3];
                //adLen3 = adjlist[node3].size();//adjlist_len[node3];
                if (adjRecord3[node3] == n3_e || visit3[node3] == true || OutRecord[n3_e-1].receiver < n1)
                {
                    continue;
                }
				mulUse_long_F = (long)(n2_money) * 5;
				n2_mx3 = (long)(n2_money) * 3;	
				if (n1_money > mulUse_long_F||n2_money > mulUse_long_G) continue;

                firstBigPos = adjRecord3[node3];

                visit3[node3] = true;
                trace[2] = node3;			
                if (OutRecord[firstBigPos].receiver == n1) {
					n3_money = OutRecord[firstBigPos].money;//n3-n1
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;					
					if (n2_money <= n3_mx5&&n3_money <= n2_mx3) {//n2-n3
						if (n3_money <= n1_mx5&&n1_money <= n3_mx3) {				
                    		memcpy(cycle_3_3 + cycle_3_3_soldier[0], trace, 12);
                    		cycle_3_3_soldier[0] += 3;
						}
					}
                }

                        //环5j
						n2_money_div5 = float(n2_money)/5;
                       //j = path_three3[node3].size();//path_three3_num[node3];
                        if (Reach5_3[node3])//等价之前的条件
                        {
							mulUseB = p_index_3[node3];
                            for (i = 0; i <path_three3_num[mulUseB]; ++i) 
                            {
	                            node5 = path_three3[mulUseB][i].x3;
								n5_money = path_three3[mulUseB][i].n7_n1;
								node4 = path_three3[mulUseB][i].x2;
								n3_money = path_three3[mulUseB][i].n5_n6;		

								if (!visit3[node4] && !visit3[node5] && n2_money_div5 <= n3_money&&n3_money <= n2_mx3 &&n5_money <= n1_mx5&&n1_money_div3 <= n5_money)
								{										
                                    		trace[3] = node4;
                                    		trace[4] = node5;
											mulUseA = cycle_3_5_soldier[0];
											cycle_3_5[mulUseA] = trace[0];
											cycle_3_5[++mulUseA] = trace[1];
											cycle_3_5[++mulUseA] = trace[2];
											cycle_3_5[++mulUseA] = trace[3];
											cycle_3_5[++mulUseA] = trace[4];											
											//memcpy(cycle_3_5 + cycle_3_5_soldier[0], trace, 20);									
                                    		cycle_3_5_soldier[0] += 5;
                                }
                            }
                        }


                for (n4 = firstBigPos; n4 < n3_e; ++n4) {
                    node4 = OutRecord[n4].receiver;
					n3_money = OutRecord[n4].money;//n3-n4		
					n4_e = adjlist[node4];
                    //adLen4 = adjlist[node4].size();//adjlist_len[node4];
                    if (adjRecord3[node4] == n4_e || visit3[node4] == true || OutRecord[n4_e-1].receiver < n1)
                    {
                        continue;
                    }
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;						
					if (n2_money > n3_mx5||n3_money > n2_mx3) continue;

                    firstBigPos = adjRecord3[node4];
                    // while (OutRecord[firstBigPos].receiver < n1)++firstBigPos;
                    // adjRecord3[node4] = firstBigPos;

                    visit3[node4] = true;
                    trace[3] = node4;				
                    if (OutRecord[firstBigPos].receiver == n1) {
						n4_money = OutRecord[firstBigPos].money;
						n4_mx5 = (long)(n4_money) * 5;
						n4_mx3 = (long)(n4_money) * 3;					
						if (n3_money <= n4_mx5&&n4_money <= n3_mx3) {//n2-n3
							if (n4_money <= n1_mx5&&n1_money <= n4_mx3) {	
								mulUseA = cycle_3_4_soldier[0];
								cycle_3_4[mulUseA] = trace[0];
								cycle_3_4[++mulUseA] = trace[1];
								cycle_3_4[++mulUseA] = trace[2];
								cycle_3_4[++mulUseA] = trace[3];														
                        		//memcpy(cycle_3_4 + cycle_3_4_soldier[0], trace, 16);
                        		cycle_3_4_soldier[0] += 4;
							}
						}
                    }

					n2_money_div5 = float(n3_money)/5;						
					//mulUseB = path_three3[node4].size();//path_three3_num[node4];
					if (Reach5_3[node4])
					{
						mulUseB = p_index_3[node4];
						for (i = 0; i < path_three3_num[mulUseB]; ++i)
						{
                            mulUseA = path_three3[mulUseB][i].x3;
							n6_money = path_three3[mulUseB][i].n7_n1;
							node5 = path_three3[mulUseB][i].x2;
							n4_money = path_three3[mulUseB][i].n5_n6;	
	
							if (!visit3[node5] && !visit3[mulUseA] && n2_money_div5 <= n4_money&&n4_money <= n3_mx3 &&n6_money <= n1_mx5&&n1_money_div3 <= n6_money)	
							{								
										trace[4] = node5;
										trace[5] = mulUseA;
										//memcpy(cycle_3_6 + cycle_3_6_soldier[0], trace, 24);
										mulUseA = cycle_3_6_soldier[0];
										cycle_3_6[mulUseA] = trace[0];
										cycle_3_6[++mulUseA] = trace[1];
										cycle_3_6[++mulUseA] = trace[2];
										cycle_3_6[++mulUseA] = trace[3];
										cycle_3_6[++mulUseA] = trace[4];
										cycle_3_6[++mulUseA] = trace[5];										
										cycle_3_6_soldier[0] += 6;
							}
						}
					}

                    for (n5 = firstBigPos; n5 < n4_e; ++n5) {
                        node5 = OutRecord[n5].receiver;
						//if(firstBigPos==0||visit3[node5])continue;
						if(!Reach5_3[node5]||visit3[node5])continue;

						n4_money = OutRecord[n5].money;						
						n4_mx5 = (long)(n4_money) * 5;
						if (n3_money > n4_mx5||n4_money > n3_mx3) continue;
						n4_mx3 = (long)(n4_money) * 3;	
						trace[4] = node5;

                        //环7
						//auto ttt=clock();
						n2_money_div5 = float(n4_money)/5;
						j = p_index_3[node5];
                        firstBigPos = path_three3_num[j];//path_three3_num[node5];
                            for (i = 0; i < firstBigPos; ++i) 
                            {
	                            mulUseB = path_three3[j][i].x3;//node7
								n7_money = path_three3[j][i].n7_n1;
								mulUseA = path_three3[j][i].x2;
								n5_money = path_three3[j][i].n5_n6;		
                                if (!visit3[mulUseA] && !visit3[mulUseB] && n2_money_div5 <= n5_money&&n5_money <= n4_mx3 &&n7_money <= n1_mx5&&n1_money_div3 <= n7_money)
								{								
                                    		trace[5] = mulUseA;
                                    		//trace[6] = mulUseB;
											mulUseA = cycle_3_7_soldier[0];
											cycle_3_7[mulUseA] = trace[0];
											cycle_3_7[++mulUseA] = trace[1];
											cycle_3_7[++mulUseA] = trace[2];
											cycle_3_7[++mulUseA] = trace[3];
											cycle_3_7[++mulUseA] = trace[4];
											cycle_3_7[++mulUseA] = trace[5];
											cycle_3_7[++mulUseA] = mulUseB;//trace[6];
                                    		//memcpy(cycle_3_7 + cycle_3_7_soldier[0], trace, 28);
                                    		cycle_3_7_soldier[0] += 7;    
                                }							
                            }

                    }

                    visit3[node4] = false;
                }

                visit3[node3] = false;
            }

            visit3[node2] = false;
        }

        j = reach3[0];
        for (i = 1; i <= j; ++i) //还可以放在第六层循环，减少次数（设置只执行一次）
        {
			mulUseA = p_index_3[reach3[i]];
			path_three3_num[mulUseA]=0;
			Reach5_3[reach3[i]]=false;
        }

        reach3[0] = 0;
        visit3[n1] = false;

		
#ifdef TEST
        if (n1 % 3000 == 0) {
            cout << n1 << "/" << nNodes  << endl;
        }

#endif
    }

}

xx23 path_three4[16384][256];
unsigned int path_three4_num[16384];
unsigned int p_index_4[2000000];
void TD_findCycle4()
{
	++FC_signal;	

    int trace[8];
    int i, j, k, n1, n2, n3, n4, n5, firstBigPos,\
	node2, node3, node4, node5, mulUseA,mulUseB,\
	n7_money, n6_money, n5_money, n4_money, n3_money, n2_money, n1_money;
	long  n4_mx5, n3_mx5, n1_mx5, n4_mx3, n3_mx3, n2_mx3, mulUse_long_F,mulUse_long_G;
	float n1_money_div3,n2_money_div5;
	int n1_e, n2_e, n3_e,n4_e;
	while(FC_signal!=4);
	for (i = 0; i < nNodes; ++i)
	{
		adjRecord4[i] = adjlist_start[i];
	}
	
    //for (n1 = 3; n1 < nNodes; n1+=4) {
	while(true)
	{
		//MagicMutex.lock();
		n1 = ++MagicNode;
		//MagicMutex.unlock();
		if(n1>=nNodes)return;
		for(i=n1_last_4;i<n1;i++)
		{
			j = adjlist_in[i][1];
        	for (k = adjlist_in[i][0]; k < j; ++k) 
        	{
				++adjRecord4[InRecord[k].giver];
			}	
		}
		n1_last_4=n1;	
        //adLen1 = adjlist[n1].size();//adjlist_len[n1];
		n1_e = adjlist[n1];
        if (adjRecord4[n1]==n1_e || OutRecord[n1_e-1].receiver < n1)
        {
            continue;
        }
		n4_e=0;
		node5 = adjlist_in[n1][1];
        for (i = adjlist_in[n1][0]; i < node5; ++i) 
        {
            node2 = InRecord[i].giver;
            if (node2 > n1)
            {			
                //mulUseA = adjlist_in[node2].size();//adjlist_in_len[n7_re];
				n7_money = InRecord[i].money; //n7-n1
				mulUse_long_F = 5 * (long)(n7_money);	
                for (j = adjlist_in[node2][0]; j < adjlist_in[node2][1]; ++j)
                {
                    node3 = InRecord[j].giver;
					n6_money = InRecord[j].money;//n6-n7
                    if (node3 > n1)// &&node3!=node2
                    {
						mulUse_long_G = (long)(n6_money) * 3;
						if (n6_money > mulUse_long_F||n7_money > mulUse_long_G) continue;
						mulUse_long_G = (long)(n6_money) * 5;						
						//mulUseB = adjlist_in[node3].size();// adjlist_in_len[node3];
						for (k = adjlist_in[node3][0]; k < adjlist_in[node3][1]; ++k)
						{
							node4 = InRecord[k].giver;
							n5_money = InRecord[k].money;//n5-n6
							if (node4>n1&&node4!=node2){ //&&node3!=node4
								n4_mx3 = (long)(n5_money)*3;
								if (n5_money > mulUse_long_G||n6_money > n4_mx3) continue;
								if (!Reach5_4[node4]) {
									Reach5_4[node4]=true;
									++reach4[0]; reach4[reach4[0]] = node4;
									p_index_4[node4] = n4_e;
									++n4_e;
								}								
								firstBigPos = p_index_4[node4];
								mulUseA = path_three4_num[firstBigPos];
								++path_three4_num[firstBigPos];
								path_three4[firstBigPos][mulUseA].x2 = node3;
								path_three4[firstBigPos][mulUseA].x3 = node2;
								path_three4[firstBigPos][mulUseA].n7_n1 = n7_money;
								path_three4[firstBigPos][mulUseA].n5_n6 = n5_money;
							}
						}
                    }
                }
            }
        }
		

		
		j= reach4[0];
		for (i = 1; i <= j; i++)
		{
			mulUseB = p_index_4[reach4[i]];
			sort(path_three4[mulUseB], path_three4[mulUseB]+path_three4_num[mulUseB], cmp_pathThree);			
		}
		

    
        visit4[n1] = true;
        trace[0] = n1;

        firstBigPos = adjRecord4[n1];
        for (n2 = firstBigPos; n2 <n1_e; ++n2) {
            node2 = OutRecord[n2].receiver;
			n1_money = OutRecord[n2].money;
			n2_e = adjlist[node2];
            if (adjRecord4[node2]==n2_e || visit4[node2] == true || OutRecord[n2_e-1].receiver < n1)
            {
                continue;
            }
			n1_money_div3 = float(n1_money)/3;
            visit4[node2] = true;
            trace[1] = node2;
			n1_mx5 = (long)(n1_money) * 5;//n1-n2
			mulUse_long_G = (long)(n1_money) * 3;	

            firstBigPos = adjRecord4[node2];	
            for (n3 = firstBigPos; n3 < n2_e; ++n3) {
                node3 = OutRecord[n3].receiver;
				n2_money = OutRecord[n3].money;
				n3_e = adjlist[node3];
                if (adjRecord4[node3] == n3_e || visit4[node3] == true || OutRecord[n3_e-1].receiver < n1)
                {
                    continue;
                }
				mulUse_long_F = (long)(n2_money) * 5;
				n2_mx3 = (long)(n2_money) * 3;	
				if (n1_money > mulUse_long_F||n2_money > mulUse_long_G) continue;

                firstBigPos = adjRecord4[node3];

                visit4[node3] = true;
                trace[2] = node3;			
                if (OutRecord[firstBigPos].receiver == n1) {
					n3_money = OutRecord[firstBigPos].money;//n3-n1
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;					
					if (n2_money <= n3_mx5&&n3_money <= n2_mx3) {//n2-n3
						if (n3_money <= n1_mx5&&n1_money <= n3_mx3) {				
                    		memcpy(cycle_4_3 + cycle_4_3_soldier[0], trace, 12);
                    		cycle_4_3_soldier[0] += 3;
						}
					}
                }

                        //环5j
						n2_money_div5 = float(n2_money)/5;
                        if (Reach5_4[node3])//等价之前的条件
                        {
							mulUseB = p_index_4[node3];
                            for (i = 0; i <path_three4_num[mulUseB]; ++i) 
                            {
	                            node5 = path_three4[mulUseB][i].x3;
								n5_money = path_three4[mulUseB][i].n7_n1;
								node4 = path_three4[mulUseB][i].x2;
								n3_money = path_three4[mulUseB][i].n5_n6;	

								if (!visit4[node4] && !visit4[node5] && n2_money_div5 <= n3_money&&n3_money <= n2_mx3 &&n5_money <= n1_mx5&&n1_money_div3 <= n5_money)
								{										
                                    		trace[3] = node4;
                                    		trace[4] = node5;
											mulUseA = cycle_4_5_soldier[0];
											cycle_4_5[mulUseA] = trace[0];
											cycle_4_5[++mulUseA] = trace[1];
											cycle_4_5[++mulUseA] = trace[2];
											cycle_4_5[++mulUseA] = trace[3];
											cycle_4_5[++mulUseA] = trace[4];											
											//memcpy(cycle_4_5 + cycle_4_5_soldier[0], trace, 20);									
                                    		cycle_4_5_soldier[0] += 5;
                                }
                            }
                        }


                for (n4 = firstBigPos; n4 < n3_e; ++n4) {
                    node4 = OutRecord[n4].receiver;
					n3_money = OutRecord[n4].money;//n3-n4		
					n4_e = adjlist[node4];
                    //adLen4 = adjlist[node4].size();//adjlist_len[node4];
                    if (adjRecord4[node4] == n4_e || visit4[node4] == true || OutRecord[n4_e-1].receiver < n1)
                    {
                        continue;
                    }
					n3_mx5 = (long)(n3_money) * 5;
					n3_mx3 = (long)(n3_money) * 3;						
					if (n2_money > n3_mx5||n3_money > n2_mx3) continue;

                    firstBigPos = adjRecord4[node4];

                    visit4[node4] = true;
                    trace[3] = node4;				
                    if (OutRecord[firstBigPos].receiver == n1) {
						n4_money = OutRecord[firstBigPos].money;
						n4_mx5 = (long)(n4_money) * 5;
						n4_mx3 = (long)(n4_money) * 3;					
						if (n3_money <= n4_mx5&&n4_money <= n3_mx3) {//n2-n3
							if (n4_money <= n1_mx5&&n1_money <= n4_mx3) {	
								mulUseA = cycle_4_4_soldier[0];
								cycle_4_4[mulUseA] = trace[0];
								cycle_4_4[++mulUseA] = trace[1];
								cycle_4_4[++mulUseA] = trace[2];
								cycle_4_4[++mulUseA] = trace[3];														
                        		//memcpy(cycle_4_4 + cycle_4_4_soldier[0], trace, 16);
                        		cycle_4_4_soldier[0] += 4;
							}
						}
                    }

					n2_money_div5 = float(n3_money)/5;						
					if (Reach5_4[node4])
					{
						mulUseB = p_index_4[node4];
						for (i = 0; i < path_three4_num[mulUseB]; ++i)
						{
                            mulUseA = path_three4[mulUseB][i].x3;
							n6_money = path_three4[mulUseB][i].n7_n1;
							node5 = path_three4[mulUseB][i].x2;
							n4_money = path_three4[mulUseB][i].n5_n6;
	
							if (!visit4[node5] && !visit4[mulUseA] && n2_money_div5 <= n4_money&&n4_money <= n3_mx3 &&n6_money <= n1_mx5&&n1_money_div3 <= n6_money)	
							{								
										trace[4] = node5;
										trace[5] = mulUseA;
										//memcpy(cycle_4_6 + cycle_4_6_soldier[0], trace, 24);
										mulUseA = cycle_4_6_soldier[0];
										cycle_4_6[mulUseA] = trace[0];
										cycle_4_6[++mulUseA] = trace[1];
										cycle_4_6[++mulUseA] = trace[2];
										cycle_4_6[++mulUseA] = trace[3];
										cycle_4_6[++mulUseA] = trace[4];
										cycle_4_6[++mulUseA] = trace[5];										
										cycle_4_6_soldier[0] += 6;
							}
						}
					}

                    for (n5 = firstBigPos; n5 < n4_e; ++n5) {
                        node5 = OutRecord[n5].receiver;
						//if(firstBigPos==0||visit4[node5])continue;
						if(!Reach5_4[node5]||visit4[node5])continue;

						n4_money = OutRecord[n5].money;						
						n4_mx5 = (long)(n4_money) * 5;
						if (n3_money > n4_mx5||n4_money > n3_mx3) continue;
						n4_mx3 = (long)(n4_money) * 3;	
						trace[4] = node5;

                        //环7
						//auto ttt=clock();
						n2_money_div5 = float(n4_money)/5;
						j = p_index_4[node5];
                        firstBigPos = path_three4_num[j];//path_three4_num[node5];
                            for (i = 0; i < firstBigPos; ++i) 
                            {
	                            mulUseB = path_three4[j][i].x3;//node7
								n7_money = path_three4[j][i].n7_n1;
								mulUseA = path_three4[j][i].x2;
								n5_money = path_three4[j][i].n5_n6;		
                                if (!visit4[mulUseA] && !visit4[mulUseB] && n2_money_div5 <= n5_money&&n5_money <= n4_mx3 &&n7_money <= n1_mx5&&n1_money_div3 <= n7_money)
								{								
                                    		trace[5] = mulUseA;
                                    		//trace[6] = mulUseB;
											mulUseA = cycle_4_7_soldier[0];
											cycle_4_7[mulUseA] = trace[0];
											cycle_4_7[++mulUseA] = trace[1];
											cycle_4_7[++mulUseA] = trace[2];
											cycle_4_7[++mulUseA] = trace[3];
											cycle_4_7[++mulUseA] = trace[4];
											cycle_4_7[++mulUseA] = trace[5];
											cycle_4_7[++mulUseA] = mulUseB;//trace[6];
                                    		//memcpy(cycle_4_7 + cycle_4_7_soldier[0], trace, 28);
                                    		cycle_4_7_soldier[0] += 7;    
                                }							
                            }

                    }

                    visit4[node4] = false;
                }

                visit4[node3] = false;
            }

            visit4[node2] = false;
        }

        j = reach4[0];
        for (i = 1; i <= j; ++i) //还可以放在第六层循环，减少次数（设置只执行一次）
        {
			mulUseA = p_index_4[reach4[i]];
			path_three4_num[mulUseA]=0;
			Reach5_4[reach4[i]]=false;
        }

        reach4[0] = 0;
        visit4[n1] = false;

		
#ifdef TEST
        if (n1 % 3000 == 0) {
            cout << n1 << "/" << nNodes  << endl;
        }

#endif
    }

}

void findCycle()
{
    thread t1(TD_findCycle1);
    thread t2(TD_findCycle2);
    thread t3(TD_findCycle3);
    thread t4(TD_findCycle4);
    t1.join();
    t2.join();
    t3.join();
    t4.join();    

}
int cycNum;
int cycnumsz;
atomic_int Lenflag[WRITE_MUL_THD-1];
atomic_int filesize(0);
atomic_int allopen(0);
//volatile int Lenflag[WRITE_MUL_THD-1];//每次从内存中读这个值，而不是从缓存中
void TD_save_fwrite(int *th_data, int id)
{
	int start_cyc = th_data[0];
	int start_node = th_data[1];
	int end_cyc = th_data[2];
	int end_node = th_data[3];
	int thisCyc0;

    int t, i, j, k, s, sz3;
    int pos = 0;
    int th, h, head;
    int number;	
    int thj_leni_size[FIND_MUL_THD];
    int thj_leni_cnt[FIND_MUL_THD];	

	if(id==0)
	{			
        string Csize = to_string(cycNum) + '\n';
        int cs_sz = Csize.size();		
        for (i = 0; i < cs_sz; i++) //写入环的个数
        {
            ansbuf[id][pos] = Csize[i];
			++pos;
        }
#ifdef TEST
        cout<<"cycNum: " << cycNum <<endl;
#endif		
	}


  
    thisCyc0 = (start_cyc << 2);
    t = start_cyc + 3;
    // cout << "start_cyc:" << th_data[0] << " start_node: " << th_data[1] << " end_cyc: " << th_data[2]<< " end_node: " << th_data[3]<<  endl;
    for (j = 0; j < FIND_MUL_THD; ++j)
    {
    	k = 0;
        thj_leni_size[j] = cycle_soldier[thisCyc0 + j][0];
        while (cycle[thisCyc0 + j][k] < start_node)k += t;
        thj_leni_cnt[j] = k;
    }
    //cout << "thisCyc0:" << thisCyc0 << " start_node: " << start_node<< " thj_leni_cnt[0]: " << thj_leni_cnt[0] << " thj_leni_cnt[1]: " << thj_leni_cnt[1]<< " thj_leni_cnt[2]: " << thj_leni_cnt[2]<< " thj_leni_cnt[3]: " << thj_leni_cnt[3]<<  endl;
    if (start_cyc == end_cyc) 
    {
        for (j = start_node; j < end_node; ++j)
        {
            //head = recordsort[j];
            for(th=0;th<FIND_MUL_THD;++th)
			{
            while (thj_leni_cnt[th] < thj_leni_size[th] && cycle[thisCyc0 + th][thj_leni_cnt[th]] == j)
            {
                h = thj_leni_cnt[th];
                for (k = 0; k < t; ++k)
                {
                    number = cycle[thisCyc0 + th][h + k];
                    sz3 = node_str_len[number];
                    memcpy(ansbuf[id] + pos, node_str[number], sz3);//密集大数据用比较快
                    pos += sz3;
                }
                ansbuf[id][pos - 1] = '\n';//末尾的','改为换行符
                thj_leni_cnt[th] += t;
            }
			}
        }
        
    }
    else
    {
        for (j = start_node; j < nNodes; ++j)
        {
            //head = recordsort[j];
            for(th=0;th<FIND_MUL_THD;++th)
			{
            while (thj_leni_cnt[th] < thj_leni_size[th] && cycle[thisCyc0 + th][thj_leni_cnt[th]] == j)
            {
                h = thj_leni_cnt[th];
                for (k = 0; k < t; ++k)
                {
                    number = cycle[thisCyc0 + th][h + k];
                    sz3 = node_str_len[number];

                    memcpy(ansbuf[id] + pos, node_str[number], sz3);
                    pos += sz3;
                }
                ansbuf[id][pos - 1] = '\n';//末尾的','改为换行符
                thj_leni_cnt[th] += t;
            }
			}
        }

        for (i = start_cyc + 1; i < end_cyc; ++i) //合并各线程、各环数的环，写入文件
        {
            for (j = 0; j < FIND_MUL_THD; ++j)
            {
                thj_leni_size[j] = cycle_soldier[(i << 2) + j][0];
                thj_leni_cnt[j] = 0;
            }
            t = i + 3;
            for (j = 0; j < nNodes; ++j)
            {
                //head = recordsort[j];
                for(th=0;th<FIND_MUL_THD;++th)
				{
                while (thj_leni_cnt[th] < thj_leni_size[th] && cycle[(i << 2) + th][thj_leni_cnt[th]] == j)
                {
                    h = thj_leni_cnt[th];
                    for (k = 0; k < t; ++k)
                    {
                        number = cycle[(i << 2) + th][h + k];
                        sz3 = node_str_len[number];
                        memcpy(ansbuf[id] + pos, node_str[number], sz3);//密集大数据用比较快
                        pos += sz3;
                    }
                    ansbuf[id][pos - 1] = '\n';//末尾的','改为换行符
                    thj_leni_cnt[th] += t;
                }
				}
            }
        }

        for (j = 0; j < FIND_MUL_THD; ++j)
        {
            thj_leni_size[j] = cycle_soldier[(end_cyc << 2) + j][0];
            thj_leni_cnt[j] = 0;
        }
        t = end_cyc + 3;
        for (j = 0; j < end_node; ++j)
        {
            //head = recordsort[j];
            for(th=0;th<FIND_MUL_THD;++th)
			{
            while (thj_leni_cnt[th] < thj_leni_size[th] && cycle[(end_cyc << 2) + th][thj_leni_cnt[th]] == j)
            {
                h = thj_leni_cnt[th];
                for (k = 0; k < t; ++k)
                {
                    number = cycle[(end_cyc << 2) + th][h + k];
                    sz3 = node_str_len[number];

                    memcpy(ansbuf[id] + pos, node_str[number], sz3);//密集大数据用比较快
                    pos += sz3;
                }
                ansbuf[id][pos - 1] = '\n';//末尾的','改为换行符
                thj_leni_cnt[th] += t;
            }
			}
        }
    }
    #ifdef TEST
	cout<<id<<" pos:"<<pos<<endl;
	#endif


     if(id==0)
     {
     	Lenflag[0]=pos;
        int fd = open(outfile.c_str(), O_RDWR | O_CREAT, 0666);
		//++allopen;
		while(filesize==0);
        lseek(fd, filesize - 1, SEEK_SET);
        write(fd, "0", 1);
        char* ans = (char*)mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
		memcpy(ans,ansbuf[id],pos);
     }
    
    else if(id==WRITE_MUL_THD-1)
    {
		while(Lenflag[id-1]==0);
		filesize=Lenflag[id-1]+pos;
        int fd = open(outfile.c_str(), O_RDWR | O_CREAT, 0666);
        lseek(fd, filesize - 1, SEEK_SET);
        write(fd, "0", 1);
        char* ans = (char*)mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
		//while(allopen!=WRITE_MUL_THD);
		memcpy(ans+Lenflag[id-1],ansbuf[id],pos);
		//cout<<"filesize:"<<filesize<<endl;
    }
    else
    {
		while(Lenflag[id-1]==0);
    	Lenflag[id]=Lenflag[id-1]+pos; 
		int fd = open(outfile.c_str(), O_RDWR | O_CREAT, 0666); 
		//++allopen; 
		while(filesize==0);
        lseek(fd, filesize - 1, SEEK_SET);
        write(fd, "0", 1);
        char* ans = (char*)mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
		//while(allopen!=WRITE_MUL_THD);
		memcpy(ans+Lenflag[id-1],ansbuf[id],pos);
		//cout<<"filesize:"<<filesize<<endl;

    }
    
    //cout<<"end id: "<<id<<endl;
}

void save_fwrite() {

    int t, i, j, k, s, sz3;
    int pos = 0;
    int th, h, head;
    int number;
        cycNum = cycle_1_3_soldier[0]/3+cycle_1_4_soldier[0]/4+cycle_1_5_soldier[0]/5+cycle_1_6_soldier[0]/6+cycle_1_7_soldier[0]/7
					+cycle_2_3_soldier[0]/3+cycle_2_4_soldier[0]/4+cycle_2_5_soldier[0]/5+cycle_2_6_soldier[0]/6+cycle_2_7_soldier[0]/7
					+cycle_3_3_soldier[0]/3+cycle_3_4_soldier[0]/4+cycle_3_5_soldier[0]/5+cycle_3_6_soldier[0]/6+cycle_3_7_soldier[0]/7
					+cycle_4_3_soldier[0]/3+cycle_4_4_soldier[0]/4+cycle_4_5_soldier[0]/5+cycle_4_6_soldier[0]/6+cycle_4_7_soldier[0]/7;
    if (cycNum < 10000) //小数据集单线程写入
    {		
        FILE* fp = fopen(outfile.c_str(), "wb");
        char buf[10];

#ifdef TEST
        cout<<"cycNum: " << cycNum <<endl;
#endif

    int idx = sprintf(buf, "%d\n", cycNum);
    buf[idx] = '\0';
    fwrite(buf, idx, sizeof(char), fp);
    cycnumsz  = ftell(fp);		
        for (i = 0; i < 5; ++i) //合并各线程、各环数的环，写入文件
        {
            int thj_leni_size[4];
            int thj_leni_cnt[4];
            for (j = 0; j < 4; ++j)
            {
                thj_leni_size[j] = cycle_soldier[(i << 2) + j][0];
                thj_leni_cnt[j] = 0;
            }
            t = i + 3;
            for (j = 0; j < nNodes; ++j)
            {
                //head = recordsort[j];
                for(th=0;th<FIND_MUL_THD;++th)
				{
                while (thj_leni_cnt[th] < thj_leni_size[th] && cycle[(i << 2) + th][thj_leni_cnt[th]] == j)
                {
                    h = thj_leni_cnt[th];
                    for (k = 0; k < t; ++k)
                    {
                        number = cycle[(i << 2) + th][h + k];
                        sz3 = node_str_len[number];
                        memcpy(ansbuf[3] + pos, node_str[number], sz3);//密集大数据用比较快
                        pos += sz3;
                    }
                    ansbuf[3][pos - 1] = '\n';//末尾的','改为换行符
                    thj_leni_cnt[th] += t;
                }
				}
            }
        }

        fwrite(ansbuf[3], sizeof(char), pos, fp);

        fclose(fp);
	}
	else
	{



    	   int len_loss[5];
    	   len_loss[0] = (cycle_1_3_soldier[0]+cycle_2_3_soldier[0]+cycle_3_3_soldier[0]+cycle_4_3_soldier[0]);
    	   len_loss[1] = (cycle_1_4_soldier[0]+cycle_2_4_soldier[0]+cycle_3_4_soldier[0]+cycle_4_4_soldier[0]);
    	   len_loss[2] = (cycle_1_5_soldier[0]+cycle_2_5_soldier[0]+cycle_3_5_soldier[0]+cycle_4_5_soldier[0]);
    	   len_loss[3] = (cycle_1_6_soldier[0]+cycle_2_6_soldier[0]+cycle_3_6_soldier[0]+cycle_4_6_soldier[0]);
    	   len_loss[4] =  cycle_1_7_soldier[0]+cycle_2_7_soldier[0]+cycle_3_7_soldier[0]+cycle_4_7_soldier[0];
    	  
        int len_acc[6];
        len_acc[0] = 0;
        for(i=0;i<5;++i)
        {
        	 len_acc[i+1] = len_acc[i] + len_loss[i];
        }

        int len_per_th = len_acc[5]/WRITE_MUL_THD;
        int len_cur=0;
        float proportion;
        int recordPos;
        int input_th[WRITE_MUL_THD][4];//<0.从哪个长度的环开始  1.从在recordsort的哪个位置  2.在哪个长度的环结束  3.在recordsort的哪个位置结束>

		auto ttt=clock();
        for (j = 0; j < WRITE_MUL_THD-1; ++j)
        {
            if (j == 0) { input_th[0][0] = 0; input_th[0][1] = 0; }
            else
            {
                input_th[j][0] = input_th[j-1][2];
                input_th[j][1] = input_th[j-1][3];
            }
            
            	 len_cur += len_per_th;
                for (i = 0; i < 5; ++i)
                {
                    if (len_acc[i + 1] >= len_cur)
                    {
                    	//cout<<"len_cur:"<<len_cur<<"  len_acc[i]:"<<len_acc[i]<<"  len_loss[i] :"<<len_loss[i]<<endl;
                        proportion = float((float)(len_cur - len_acc[i]) / (float)len_loss[i]);
                        //printf("proportion: %f\n",proportion);
                        input_th[j][2] = i;//结束环长，线程中应<=
						t = cycle_soldier[i * 4][0]* proportion;
						k = cycle_soldier[i * 4+1][0]* proportion;
						s = cycle_soldier[i * 4+2][0]* proportion;
						sz3 = cycle_soldier[i * 4+3][0]* proportion;

                        while (t % (i + 3) != 0) t--; //头节点位置
						while (k % (i + 3) != 0) k--; //头节点位置
						while (s % (i + 3) != 0) s--; //头节点位置
						while (sz3 % (i + 3) != 0) sz3--; //头节点位置
                        // cout<<"头节点位置: "<<recordPos<<endl;
                        recordPos = (cycle[i * 4][t]+cycle[i * 4+1][k]+cycle[i * 4+2][s]
									+cycle[i * 4+3][sz3])/4;//头节点ID（这里默认各数组大致均匀，就只取四个数组中的一个的值）

                        input_th[j][3] = recordPos;
                        break;
                    }
                }
			//cout << input_th[j][0]<<"------"<< input_th[j][1]<<"------"<< input_th[j][2]<<"------"<< input_th[j][3]<<endl;
        }
        input_th[WRITE_MUL_THD-1][0] = input_th[WRITE_MUL_THD-2][2];
        input_th[WRITE_MUL_THD-1][1] = input_th[WRITE_MUL_THD-2][3];		
        input_th[WRITE_MUL_THD-1][2] = 4; 
		input_th[WRITE_MUL_THD-1][3] = nNodes;
	
		//cout<<"ttttime : "<<clock() - ttt<<endl;

        thread t1(TD_save_fwrite, input_th[0], 0);
        thread t2(TD_save_fwrite, input_th[1], 1);
        thread t3(TD_save_fwrite, input_th[2], 2);
        thread t4(TD_save_fwrite, input_th[3], 3);
		//thread t5(TD_save_fwrite, input_th[4], 4);
		//thread t6(TD_save_fwrite, input_th[5], 5);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
		//t5.join();
        //t6.join();
    }
}

int main()
{
		#ifdef TEST
		auto t=clock();
	 	float tt=t;			
	date_read();
		cout<<"date_read:"<<clock()-t<<endl;
		t=clock();
	findCycle();
		cout<<"TD_findCycle1:"<<clock()-t<<endl;
		t=clock();
	
	save_fwrite();

		cout<<"save_fwrite:"<<clock()-t<<endl;
		cout<<"totol:"<<clock()-tt<<endl;
	#else
	date_read();
	usleep(200000);
	findCycle();
	save_fwrite();
	#endif

	return 0;
}


// auto start = std::chrono::system_clock::now();
// std::chrono::duration<double, std::milli> duration;

// duration = std::chrono::system_clock::now() - start;
// std::cout << "time elapsed: " << duration.count() << "ms" << std::endl;
