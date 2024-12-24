#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;
const int inf=0x3f3f3f3f;
int head[2][100],cnt[2];//链式前向星
int lu[2],fang[2];//路线和方位
int dfsv[100]={0},top,way,g[100];
int floydp[100],floydw[100][100],pathw[100],pcnt;
int n;

struct scenery//景点信息
{
    char name[100],intro[100],direct[2][100][100];
}sce[100];
struct S
{
    int next,to,w;
}d[2][100];
void add(int x,int y,int w,int f)//创建无向图
{
    d[f][++cnt[f]].to=y;
    d[f][cnt[f]].w=w;
    d[f][cnt[f]].next=head[f][x];
    head[f][x]=cnt[f];
}
void scene(int k)//查询任意景点信息
{
    printf("该景点的代号为%d\n名称：%s\n简介：%s\n",k,sce[k].name,sce[k].intro);

}
void dij(int x,int y,int f)//查找任意两个景点的最短路径
{
    int c[100],v[100],p[100],i,j,k,t[100];
    memset(c,inf,sizeof(c));
    memset(p,inf,sizeof(p));
    memset(v,0,sizeof(v));
    c[x]=0;
    while(1)
    {
        k=-1,j=inf;
        for(i=1;i<=n;i++)
            if(c[i]<j&&v[i]==0)
            {
                k=i;
                j=c[i];
            }
        if(k==-1)
            break;
        v[k]=1;
        for(i=head[f][k];i;i=d[f][i].next)
        {
            if(!v[d[f][i].to]&&((c[k]+d[f][i].w)<c[d[f][i].to]))
            {
                c[d[f][i].to]=c[k]+d[f][i].w;
                p[d[f][i].to]=k;
            }
        }
    }
    k=0,j=y;
    while(p[j]!=inf)
    {
        t[++k]=p[j];
        j=p[j];
    }
    if(!k)
    {
        printf("\n这两个景点之间不相通\n");
        return;
    }
    printf("\n两景点最短路线为：");
    for(i=k,t[0]=y;i>0;i--)
    {
        printf(" %d",t[i]);
        printf(" %s->",sce[t[i]].direct[f][t[i-1]]);
    }
    printf(" %d",y);
    printf("\n两景点间的距离为：%d\n",c[y]);
}

void path(int g[],int top,int f)//打印路径
{
    printf("路径%d:",++way);
    int i;
    for(i=1;i<top;i++)
    {
        printf(" %d ",g[i]);
        printf("%s->",sce[g[i]].direct[f][g[i+1]]);
    }
    printf(" %d\n",g[top]);
}
void dfs(int x,int y,int f,int t)//深搜任意两个景点的所有路径
{
    dfsv[x]=1;
    g[++top]=x;
    if(g[top]==y)
        path(g,top,f);
    for(int i=head[f][x];i;i=d[f][i].next)
        if(!dfsv[d[f][i].to])
            dfs(d[f][i].to,y,f,top);
    dfsv[x]=0;
    top--;
}
void start(int x,int y,int f)//初始化栈
{
    way=top=0;
    memset(dfsv,0,sizeof(dfsv));
    dfs(x,y,f,top);
    if(!way)
        printf("\n这两个景点之间不相通\n");
}

void pathway(int l,int r)
{
    if(l==r)
        return ;
    if(floydw[l][r]==-1)
        pathw[++pcnt]=r;
    else
    {
        pathway(l,floydw[l][r]);
        pathway(floydw[l][r],r);
    }
}
void floyd(int flag)
{
    int i,j,k,p,dp[100][100];
    memset(dp,inf,sizeof(dp));
    memset(floydw,-1,sizeof(floydw));
    for(i=1;i<=n;i++)
        dp[i][i]=0;
    for(i=1;i<=n;i++)
    {
        for(j=head[flag][i];j;j=d[flag][j].next)
            dp[i][d[flag][j].to]=d[flag][j].w;
    }
    for(k=1;k<=n;k++)
        for(i=1;i<=n;i++)
            for(j=1;j<=n;j++)
                if((dp[i][k]+dp[k][j])<dp[i][j])
                {
                    dp[i][j]=dp[i][k]+dp[k][j];
                    floydw[i][j]=k;
                }
    for(i=2,k=1,pcnt=0;floydp[i];i++)
    {
        for(j=1,p=0;j<=pcnt;j++)
            if(floydp[i]==pathw[j])
                p=1;
        if(dp[floydp[k]][floydp[i]]==inf)
        {
            printf("  您查询的景点之间不相通\n");
            return ;
        }
        if(!p)
        {
            pathway(floydp[k],floydp[i]);
            k=i;
        }
    }
    pathw[0]=floydp[1];
    for(i=0;i<pcnt;i++)
    {
        printf("%d",pathw[i]);
        printf(" %s -> ",sce[pathw[i]].direct[flag][pathw[i+1]]);
    }
    printf("%d",pathw[i]);
}
int main()
{
    int i,m;
    printf("请输入景点个数\n");
    scanf("%d",&n);
    printf("\n请输入景点的信息（代号，名称，简介）:\n");
    for(i=1;i<=n;i++)
        scanf("%d%s%s",&m,sce[i].name,sce[i].intro);
    int x,y,w;

    printf("\n请输入景点步行路线数：\n");
    scanf("%d",&lu[0]);
    printf("\n请依次输入所有的步行路线：\n");
    for(i=1;i<=lu[0];i++)
    {
        scanf("%d%d%d",&x,&y,&w);
        add(x,y,w,0);
        add(y,x,w,0);
    }

    printf("\n请输入景点汽车路线数：\n");
    scanf("%d",&lu[1]);
    printf("\n请依次输入所有的汽车路线：\n");
    for(i=1;i<=lu[1];i++)
    {
        scanf("%d%d%d",&x,&y,&w);
        add(x,y,w,1);
        add(y,x,w,1);
    }

    printf("\n请输入步行路线景点间的方位数\n");
    scanf("%d",&fang[0]);
    printf("\n请依次输入景点间的步行方位：\n");
    for(i=1;i<=fang[0];i++)
    {
        scanf("%d%d",&x,&y);
        scanf("%s",sce[x].direct[0][y]);
    }

    printf("\n请输入汽车路线景点间的方位数\n");
    scanf("%d",&fang[1]);
    printf("\n请依次输入景点间的汽车方位：\n");
    for(i=1;i<=fang[1];i++)
    {
        scanf("%d%d",&x,&y);
        scanf("%s",sce[x].direct[1][y]);
    }

    int v;
    while(1)
    {
        printf("请输入:\n0.退出\n1.查询任意景点信息\n2.查询任意两个景点的最短路线\n");
        printf("3.查询任意两个景点的所有路线\n4.查找多个景点的最佳路线\n5.导游仿真图\n");
        scanf("%d",&v);
        if(v==0)
            break;
        switch(v)
        {
            case 1:
            {
                printf("\n请输入要查询的景点代号:\n");
                scanf("%d",&v);
                scene(v);
                break;
            }
            case 2:
            {
                printf("\n请输入要查询的路线类型：\n0.步行\n1.汽车\n");
                scanf("%d",&v);
                printf("\n请输入两个景点的代号（输入的第一个景点为起点）:\n");
                scanf("%d%d",&x,&y);
                dij(x,y,v);
                break;
            }
            case 3:
            {
                printf("\n请输入要查询的路线类型：\n0.步行\n1.汽车\n");
                scanf("%d",&v);
                printf("\n请输入两个景点的代号（输入的第一个景点为起点）:\n");
                scanf("%d%d",&x,&y);
                start(x,y,v);
                break;
            }
            case 4:
            {
                printf("\n请输入要查询的路线类型：\n0.步行\n1.汽车\n");
                scanf("%d",&v);
                printf("\n请依次输入所有景点的代号（输入的第一个景点为起点）,输入0以结束:\n");
                for(i=1;;i++)
                {
                    scanf("%d",&floydp[i]);
                    if(floydp[i]==0)
                        break;
                }
                floyd(v);
                break;
            }
            case 5:
            {
                printf("步行导游仿真图\n");
                printf("            9菜鸟驿站                 \n");
                printf("          *     *                     \n");
                printf("        *       *                     \n");
                printf("      *         *                     \n");
                printf(" 8女生宿舍*****5食堂    7男生宿舍     \n");
                printf("     *        * *         *  *        \n");
                printf("     *      *   *       *    *        \n");
                printf("     *    *     *     *      *        \n");
                printf("   4操场*******3水库*     6实验楼     \n");
                printf("     *          *        *   *        \n");
                printf("     *          *       *    *        \n");
                printf("     *          *      *     *        \n");
                printf("  10图书馆***2教学楼*****11地下通道   \n");
                printf("     *          *                     \n");
                printf("       *        *                     \n");
                printf("         *      *                     \n");
                printf("           *    *                     \n");
                printf("             *1校门口                 \n");
                printf("汽车导游仿真图\n");
                printf(" 8女生宿舍*****5食堂    7男生宿舍     \n");
                printf("     *        * *         *  *        \n");
                printf("     *      *   *       *    *        \n");
                printf("     *    *     *     *      *        \n");
                printf("   4操场*******3水库*     6实验楼     \n");
                printf("     *          *        *   *        \n");
                printf("     *          *       *    *        \n");
                printf("     *          *      *     *        \n");
                printf("  10图书馆***2教学楼*****11地下通道   \n");
                printf("     *          *                     \n");
                printf("       *        *                     \n");
                printf("         *      *                     \n");
                printf("           *    *                     \n");
                printf("             *1校门口                 \n");
                break;
            }
        }
    }

    return 0;
}
