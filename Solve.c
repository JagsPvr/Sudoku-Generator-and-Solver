int row(int i, int j, int n) {
	for(int g = 0; g < 9; g++)
		if(a[i][g] == n)
			return 0;
	return 1;
}

int col(int i, int j, int n) {
	for(int g = 0; g < 9; g++)
		if(a[g][j] == n)
			return 0;
	return 1;
}

int square(int i, int j, int n) {
	int r = ceil((i+1)/3.0);
	int c = ceil((j+1)/3.0);
	for(int g = (r-1)*3; g < ((r-1)*3+3); g++)
		for(int h = (c-1)*3; h < ((c-1)*3+3); h++)
			if(a[g][h] == n)
				return 0;
	return 1;
}
void solve() {
	int un=0,tmp;
	int b=1;
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			if(a[i][j]==0)
				un++;
	printf("%d\n", n);
	while(un && b) {
		b=0;
		for(int i=0;i<n;i++)
			for(int j=0;j<n;j++){
				if(a[i][j])
					continue;
				tmp=0;
				for(int x=1;x<=n;x++){
				printf("%d\t",x);
					if(row(i,j,x)&& col(i,j,x) && square(i,j,x)){
						if(!tmp)
							tmp = x;
						else{
							tmp = 0;
							break;
						}
					}
				}
				if(tmp){
					a[i][j]=tmp;
					b=1;
					un--;
				}
			printf("LO%d\t",a[i][j]);
			puts("");
			}
		}
		if(!b)
		puts("The sudoku cannot be solved,\nMaybe there are more solution");
	else
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
	char c[2];
	sprintf(c,"%d",a[i][j]);
	gtk_entry_set_text(GTK_ENTRY(wid[i][j]),c);
}
}
}
