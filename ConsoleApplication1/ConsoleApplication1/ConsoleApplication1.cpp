#include "stdio.h"
int n,a,k;
int main()
{
	freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);	
	scanf("%d",&n);
	for(int i=0; i<n; i++)
	{
		for(int j=0;j<n;j++)
		{
			scanf("%d",&a);
			if (j<=i)
			{
				if (a==1) k++;
			}
		}
	}
	printf("%d",k);
	return 0;
}

