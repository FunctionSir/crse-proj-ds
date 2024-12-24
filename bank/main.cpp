#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
int tolnumber,vipnumber,toltime,starthour,startminute,finalhour,finalminute,vistime,tal;
typedef struct  //信息
{
    char name[20];
    char cardnumber[6];
    int deposit;
    int vip;
    int starttimehour;
    int starttimeminute;
    int durtime;
}customer;
vector<customer>s1;
typedef struct
{
    customer cs[300];
    int front;
    int rear;
    int size;
}Queue;
customer vipfile[400];
Queue que[3];
void clear_screen() {
    system("cls");
}
void init(Queue *p)
{
    p->front=0;
    p->rear=0;
    p->size=0;
}
int push(Queue *p,customer x)
{
    if(p->size>=300)
    {
        return 0;
    }
    p->cs[p->rear]=x;
    p->rear=(p->rear+1)%300;
    p->size++;
    return 1;
}
void pop(Queue *p)
{
    customer x=p->cs[p->front];
    p->front=(p->front+1)%300;
    p->size--;
}
int emptyf(Queue *p)
{
    return p->size==0;
}
void update(int x,int y)
{
    int sum=x*60+y;
    for(int i=0;i<3;i++)
    {
        while(!emptyf(&que[i]))
        {
            customer t=que[i].cs[que[i].front];
            int s=t.starttimehour*60+t.starttimeminute+t.durtime;
            if(sum>=s)
            {
                toltime+=t.durtime;
                pop(&que[i]);
                if(!emptyf(&que[i]))
                {
                    toltime+=s-que[i].cs[que[i].front].starttimehour*60-que[i].cs[que[i].front].starttimeminute;
                    que[i].cs[que[i].front].starttimehour=s/60;
                    que[i].cs[que[i].front].starttimeminute=s%60;
                }
            }
            else
                break;
        }
    }
}
void insertque(Queue *p,customer x)
{
    if(p->size>300)
        return;
    if(p->size==0)
    {
         p->cs[p->front]=x;
         p->rear=(p->rear+1)%300;
         p->size++;
         return;
    }
    int i=1;
    while(p->front+i<p->rear)
    {
        if(p->cs[p->front+i].vip>=x.vip)
            i++;
        else
            break;
    }
    int j=p->rear;
    if(p->front+i==j)
    {
        p->cs[p->front+i]=x;
        p->rear=(p->rear)%300;
        p->size++;
        return;
    }
    while(j>=p->front+i)
    {
        p->cs[j]=p->cs[j-1];
        j--;
    }
    p->cs[p->front+i]=x;
    p->rear=(p->rear+1)%300;
    p->size++;
}
void over()
{
    int t=0;
    for(int i=0;i<3;i++)
    {
        t=0;
        while(!emptyf(&que[i]))
        {
            int s=que[i].cs[que[i].front].starttimehour*60+que[i].cs[que[i].front].starttimeminute+que[i].cs[que[i].front].durtime;
            t=s;
            toltime+=que[i].cs[que[i].front].durtime;
            pop(&que[i]);
            if(!emptyf(&que[i]))
            {
                toltime+=t-que[i].cs[que[i].front].starttimehour*60-que[i].cs[que[i].front].starttimeminute;
                que[i].cs[que[i].front].starttimehour=s/60;
                que[i].cs[que[i].front].starttimeminute=s%60;
            }
        }
    }
}
void sit()
{
    for(int i=0;i<3;i++)
    {
        init(&que[i]);
    }
    printf("\n开始营业:\n");
    int vis=0;
    int totaltime=0;
    while(1)
    {
        printf("\n请选择你的操作:\n");
        printf("\n1-新进来一位客户(按时间的先后顺序)\n2-下班结束\n3-统计有多少客户\n4-统计每天客户的平均逗留时间(单位:分钟/人)\n5-打印vip客户档案\n6-退出系统\n7-清屏\n");
        int opt;
        scanf("%d",&opt);
        if(opt==6)
        {
            printf("\n成功退出系统\n");
            return;
        }
        switch (opt)
        {
        case 1:
        {
            if(vis==1)
            {
                printf("\n抱歉,我行已经下班,请明天再来\n");
                break;
            }
            printf("\n欢迎客户,请输入客户的信息\n");
            customer t;
            printf("\n请输入该用户的名字:\n");
            scanf(" %s",t.name);
            printf("\n请输入该用户的卡号:\n");
            scanf(" %s",t.cardnumber);
            printf("\n请输入该用户的存款:\n");
            scanf("%d",&t.deposit);
            printf("\n请输入该客户进入银行的时刻(小时):\n");
            scanf("%d",&t.starttimehour);
            printf("\n请输入该客户进入银行的时刻(分钟):\n");
            scanf("%d",&t.starttimeminute);
            printf("\n请输入该客户办理业务所需要的时间(分钟):\n");
            scanf("%d",&t.durtime);
            if(t.deposit<=500000&&t.deposit>=200000)
                t.vip=1;
            else if(t.deposit>=510000&&t.deposit<=1000000)
                t.vip=2;
            else if(t.deposit>=1010000)
                t.vip=3;
            else
                t.vip=0;
            if(t.starttimehour*60+t.starttimeminute<=totaltime)
            {
                printf("\n读入信息失败,后来的顾客不能比先来的顾客来的早!!!!\n");
                break;
            }
            totaltime=t.starttimehour*60+t.starttimeminute;
            int u=0;
            if(t.vip>0)
            {
                    for(int i=1; i<=tal; i++)
                    {
                        if(strcmp(vipfile[i].name,t.name)==0&&strcmp(t.cardnumber,vipfile[i].cardnumber)==0)
                        {
                            u=1;
                        }
                    }
                    if(u==0)
                    {
                        printf("\n读入信息失败,该用户信息与vip档案冲突!!!\n");
                        break;
                    }
            }
            tolnumber++;
            update(t.starttimehour,t.starttimeminute);//跟新三个窗口的队伍的进度
            if(t.vip>0)
            {
                vipnumber++;
                s1.push_back(t);
            }
            int minque=0;
            for(int i=1; i<3; i++)
            {
                if(que[i].size<que[minque].size)
                    minque=i;
            }
            insertque(&que[minque],t);
            printf("\n此时三个窗口的人数分别是:\n1窗口-%d人\n2窗口-%d人\n3窗口-%d人\n",que[0].size,que[1].size,que[2].size);
        }
        break;
        case 2:
        {
            over();
            printf("\n下班了!!!\n");
            vis=1;
        }
        break;
        case 3:
        {
            if(vis==0)
            {
                printf("\n还未下班\n");
                break;
            }
            printf("\n总客户数为:%d\nvip客户数为:%d\n",tolnumber,vipnumber);
            printf("\nvip客户的信息:\n");
            for(int i=0;i<s1.size();i++)
            {
                printf("客户姓名:%s 客户卡号:%s\n",s1[i].name,s1[i].cardnumber);
            }
        }
        break;
        case 4:
        {
            if(vis==0)
            {
                printf("\n还未下班\n");
                break;
            }
            int k=toltime;
            double ans=(double)k/(double)tolnumber;
            printf("\n总逗留时间为:%d\n总客户数为:%d\n平均逗留时间为:%lf(单位:分钟/人)\n",k,tolnumber,ans);
        }
        break;
        case 5:
            {
                for(int i=1;i<=tal;i++)
                {
                    printf("\n名字:%s\n卡号:%s\n的存款:%d\n",vipfile[i].name,vipfile[i].cardnumber,vipfile[i].deposit);
                    if(vipfile[i].vip==1)
                    {
                        printf("优先级:I级\n");
                    }
                    else if(vipfile[i].vip==2)
                    {
                        printf("优先级:II级\n");
                    }
                    else
                    {
                        printf("优先级:III级\n");
                    }
                }
            }
        break;
        case 7:
            {
                clear_screen();
            }
        break;
        }
    }
}
int main()
{
    printf("\n欢迎来到多窗口银行业务模拟系统\n");
    printf("\n建立vip客户档案\n");
    srand(time(NULL)); // 设置随机种子
    int totalvipcs= rand()%6+5; // 随机生成vip客户数量在50到150之间
    tal=totalvipcs;
    printf("\nvip客户数量为:%d\n",totalvipcs);
    for(int i=1;i<=totalvipcs;i++)
    {
        printf("\n请输入第%d位vip客户的信息\n",i);
        printf("\n请输入该vip用户的名字:");
        scanf(" %s",vipfile[i].name);
        printf("\n请输入该客户的卡号:\n");
        scanf(" %s",vipfile[i].cardnumber);
        printf("\n请输入该客户的存款:\n");

        scanf(" %d",&vipfile[i].deposit);
        if(vipfile[i].deposit<=500000&&vipfile[i].deposit>=200000)
            vipfile[i].vip=1;
        else if(vipfile[i].deposit>=510000&&vipfile[i].deposit<=1000000)
            vipfile[i].vip=2;
        else if(vipfile[i].deposit>=1010000)
            vipfile[i].vip=3;
        else
            printf("\n该vip客户数据有误!!!\n");
    }
    sit();
    return 0;
}
