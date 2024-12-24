#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;
const int inf=0x3f3f3f3f;
int head[2][100],cnt[2];//��ʽǰ����
int lu[2],fang[2];//·�ߺͷ�λ
int dfsv[100]={0},top,way,g[100];
int floydp[100],floydw[100][100],pathw[100],pcnt;
int n;

struct scenery//������Ϣ
{
    char name[100],intro[100],direct[2][100][100];
}sce[100];
struct S
{
    int next,to,w;
}d[2][100];
void add(int x,int y,int w,int f)//��������ͼ
{
    d[f][++cnt[f]].to=y;
    d[f][cnt[f]].w=w;
    d[f][cnt[f]].next=head[f][x];
    head[f][x]=cnt[f];
}
void scene(int k)//��ѯ���⾰����Ϣ
{
    printf("�þ���Ĵ���Ϊ%d\n���ƣ�%s\n��飺%s\n",k,sce[k].name,sce[k].intro);

}
void dij(int x,int y,int f)//��������������������·��
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
        printf("\n����������֮�䲻��ͨ\n");
        return;
    }
    printf("\n���������·��Ϊ��");
    for(i=k,t[0]=y;i>0;i--)
    {
        printf(" %d",t[i]);
        printf(" %s->",sce[t[i]].direct[f][t[i-1]]);
    }
    printf(" %d",y);
    printf("\n�������ľ���Ϊ��%d\n",c[y]);
}

void path(int g[],int top,int f)//��ӡ·��
{
    printf("·��%d:",++way);
    int i;
    for(i=1;i<top;i++)
    {
        printf(" %d ",g[i]);
        printf("%s->",sce[g[i]].direct[f][g[i+1]]);
    }
    printf(" %d\n",g[top]);
}
void dfs(int x,int y,int f,int t)//���������������������·��
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
void start(int x,int y,int f)//��ʼ��ջ
{
    way=top=0;
    memset(dfsv,0,sizeof(dfsv));
    dfs(x,y,f,top);
    if(!way)
        printf("\n����������֮�䲻��ͨ\n");
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
            printf("  ����ѯ�ľ���֮�䲻��ͨ\n");
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
    printf("�����뾰�����\n");
    scanf("%d",&n);
    printf("\n�����뾰�����Ϣ�����ţ����ƣ���飩:\n");
    for(i=1;i<=n;i++)
        scanf("%d%s%s",&m,sce[i].name,sce[i].intro);
    int x,y,w;

    printf("\n�����뾰�㲽��·������\n");
    scanf("%d",&lu[0]);
    printf("\n�������������еĲ���·�ߣ�\n");
    for(i=1;i<=lu[0];i++)
    {
        scanf("%d%d%d",&x,&y,&w);
        add(x,y,w,0);
        add(y,x,w,0);
    }

    printf("\n�����뾰������·������\n");
    scanf("%d",&lu[1]);
    printf("\n�������������е�����·�ߣ�\n");
    for(i=1;i<=lu[1];i++)
    {
        scanf("%d%d%d",&x,&y,&w);
        add(x,y,w,1);
        add(y,x,w,1);
    }

    printf("\n�����벽��·�߾����ķ�λ��\n");
    scanf("%d",&fang[0]);
    printf("\n���������뾰���Ĳ��з�λ��\n");
    for(i=1;i<=fang[0];i++)
    {
        scanf("%d%d",&x,&y);
        scanf("%s",sce[x].direct[0][y]);
    }

    printf("\n����������·�߾����ķ�λ��\n");
    scanf("%d",&fang[1]);
    printf("\n���������뾰����������λ��\n");
    for(i=1;i<=fang[1];i++)
    {
        scanf("%d%d",&x,&y);
        scanf("%s",sce[x].direct[1][y]);
    }

    int v;
    while(1)
    {
        printf("������:\n0.�˳�\n1.��ѯ���⾰����Ϣ\n2.��ѯ����������������·��\n");
        printf("3.��ѯ�����������������·��\n4.���Ҷ����������·��\n5.���η���ͼ\n");
        scanf("%d",&v);
        if(v==0)
            break;
        switch(v)
        {
            case 1:
            {
                printf("\n������Ҫ��ѯ�ľ������:\n");
                scanf("%d",&v);
                scene(v);
                break;
            }
            case 2:
            {
                printf("\n������Ҫ��ѯ��·�����ͣ�\n0.����\n1.����\n");
                scanf("%d",&v);
                printf("\n��������������Ĵ��ţ�����ĵ�һ������Ϊ��㣩:\n");
                scanf("%d%d",&x,&y);
                dij(x,y,v);
                break;
            }
            case 3:
            {
                printf("\n������Ҫ��ѯ��·�����ͣ�\n0.����\n1.����\n");
                scanf("%d",&v);
                printf("\n��������������Ĵ��ţ�����ĵ�һ������Ϊ��㣩:\n");
                scanf("%d%d",&x,&y);
                start(x,y,v);
                break;
            }
            case 4:
            {
                printf("\n������Ҫ��ѯ��·�����ͣ�\n0.����\n1.����\n");
                scanf("%d",&v);
                printf("\n�������������о���Ĵ��ţ�����ĵ�һ������Ϊ��㣩,����0�Խ���:\n");
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
                printf("���е��η���ͼ\n");
                printf("            9������վ                 \n");
                printf("          *     *                     \n");
                printf("        *       *                     \n");
                printf("      *         *                     \n");
                printf(" 8Ů������*****5ʳ��    7��������     \n");
                printf("     *        * *         *  *        \n");
                printf("     *      *   *       *    *        \n");
                printf("     *    *     *     *      *        \n");
                printf("   4�ٳ�*******3ˮ��*     6ʵ��¥     \n");
                printf("     *          *        *   *        \n");
                printf("     *          *       *    *        \n");
                printf("     *          *      *     *        \n");
                printf("  10ͼ���***2��ѧ¥*****11����ͨ��   \n");
                printf("     *          *                     \n");
                printf("       *        *                     \n");
                printf("         *      *                     \n");
                printf("           *    *                     \n");
                printf("             *1У�ſ�                 \n");
                printf("�������η���ͼ\n");
                printf(" 8Ů������*****5ʳ��    7��������     \n");
                printf("     *        * *         *  *        \n");
                printf("     *      *   *       *    *        \n");
                printf("     *    *     *     *      *        \n");
                printf("   4�ٳ�*******3ˮ��*     6ʵ��¥     \n");
                printf("     *          *        *   *        \n");
                printf("     *          *       *    *        \n");
                printf("     *          *      *     *        \n");
                printf("  10ͼ���***2��ѧ¥*****11����ͨ��   \n");
                printf("     *          *                     \n");
                printf("       *        *                     \n");
                printf("         *      *                     \n");
                printf("           *    *                     \n");
                printf("             *1У�ſ�                 \n");
                break;
            }
        }
    }

    return 0;
}
