//	By Hu Zhenyu	2021.10.04	hunter_hu163@163.com

#include <iostream>

#define ORDER		6
#define OUTLET_W	2
#define OUTLET_H	0
#if 1
int routePointDepth = 8;
char routePoint[ORDER][ORDER] = {
 { 1, 0, 0, 0, 1, 0},
 { 1,-1,-1,-1, 1, 0},
 {-1,-1, 0, 0, 1, 0},
 { 2, 0, 0, 0,-1,-1},
 { 2, 0, 8, 1, 0, 0},
 {-1,-1, 8, 1, 0, 0}
};
#endif


int attempt = 0;
int total;
void dump(char a[][ORDER], int depth)
{
#if 1
	printf("Step: %d, attempt = %d\n", total - depth + 1, attempt);

	for (int h = 0; h < ORDER; h++)				//height
	{
		for (int w = 0; w < ORDER; w++)			//width
			printf("%2d", a[h][w]);
		printf("\n");
	}
#endif
}

int checkRoute(char a[][ORDER], int depth)
{
	if (depth != routePointDepth)
		return 0;
	for (int h = 0; h < ORDER; h++)					//height
		for (int w = 0; w < ORDER; w++)				//width
			if (a[h][w] != routePoint[h][w])
				return 0;
	return 1;
}

int checkUnblock(char a[][ORDER], char b[][ORDER], int h, int w)
{
	int i, j, start, end;
	if (a[h][w] == 0)
		printf("checkUnblock error");
	if (a[h][w] < 0)		// horizontal block
	{
		for (i = 0; i < ORDER; i++)
			if (a[h][i] != b[h][i])
			{
				start = i;
				break;
			}
		for (i = ORDER - 1; i >= 0; i--)
			if (a[h][i] != b[h][i])
			{
				end = i;
				break;
			}
		for (i = start; i <= end; i++)
			if (b[h][i] == 0)
			{
				for (j = h - 1; j > 0; j--)					// up side
					if (a[j][i] > 0)						// it blocks a vertical block at first
						return 1;
					else if (a[j][i] < 0)					// find a horizontal block, no need to proceed
						continue;
				for (j = h + 1; j < ORDER; j++)				// down side
					if (a[j][i] > 0)						// it blocks a vertical block at first
						return 1;
					else if (a[j][i] < 0)
						break;								// find a horizontal block, no need to proceed
				for (j = i - 1; j > 0; j--)					// left side
					if (b[h][j] < 0 && b[h][j] != a[h][w])	// it blocks a horizontal block at first 
						return 1;
					else if (b[h][j] > 0 || b[h][j] == a[h][w])	// find a vertical block or itself, no need to proceed
						break;
				for (j = i + 1; j < ORDER; j++)				// right side
					if (b[h][j] < 0 && b[h][j] != a[h][w])	// it blocks a horizontal block at first 
						return 1;
					else if (b[h][j] > 0 || b[h][j] == a[h][w])	// find a vertical block or itself, no need to proceed
						break;
			}
	}
	else    //vertical block or caocao
	{
		for (i = 0; i < ORDER; i++)
			if (a[i][w] != b[i][w])
			{
				start = i;
				break;
			}
		for (i = ORDER - 1; i >= 0; i--)
			if (a[i][w] != b[i][w])
			{
				end = i;
				break;
			}

		for (i = start; i <= end; i++)
			if (b[i][w] == 0)
			{
				for (j = w - 1; j > 0; j--)					// left side
					if (a[i][j] < 0)						// it blocks horizontal block at first
						return 1;
					else if (a[i][j] > 0)					// find a vertical block, no need to proceed
						break;
				for (j = w + 1; j < ORDER; j++)				// right side
					if (a[i][j] < 0)						// it blocks horizontal block at first
						return 1;
					else if (a[i][j] > 0)					// find a vertical block, no need to proceed
						break;
				for (j = i - 1; j > 0; j--)					// top side
					if (b[j][w] > 0 && b[j][w] != a[h][w])	// it blocks a vertical block at first 
						return 1;
					else if (b[j][w] > 0 || b[j][w] == a[h][w])	// find a horizontal block or itself, no need to proceed
						break;
				for (j = i + 1; j < ORDER; j++)				// down side
					if (b[j][w] > 0 && b[j][w] != a[h][w])	// it blocks a vertical block at first 
						return 1;
					else if (b[j][w] > 0 || b[j][w] == a[h][w])	// find a horizontal block or itself, no need to proceed
						break;
			}
	}
	return 0;
}

int step(char a[][ORDER], int depth, int last_h, int last_w)
{
	char b[ORDER][ORDER];
	int h, w, i, j, len;
	if (depth == 0)
		return 0;

	//dump(a, depth);
	attempt++;

	if (attempt % 1000000 == 0)
		printf("attempt = %d\n", attempt);
	if (checkRoute(a, depth) == 1)
		printf("11111");

	for (h = 0; h < ORDER; h++)					//height
		for (w = 0; w < ORDER; w++)				//width
			b[h][w] = a[h][w];					//copy a -> b

	for (h = 0; h < ORDER; h++)					//height
		for (w = 0; w < ORDER; w++)				//width
		{
			if (a[h][w] == 0)	//not a  block
				continue;
			if (h == last_h && w == last_w)				// do not move the same block as last time 
				continue;

			if (a[h][w] < 0)							//horizontal
			{
				if (w == 0 || a[h][w - 1] != a[h][w])	// for horizontal block, only operate the left end
				{
					len = 1;
					for (i = 1; w + i < ORDER; i++)
						if (a[h][w + i] == a[h][w])
							len++;

					for (i = 1; w - i >= 0 && a[h][w - i] == 0; i++)		//move left i step and not knock into anything
						;
					i--;
					if (i > 0)
					{
						for (j = 0; j < len; j++)
							b[h][w + j - i] = a[h][w + j];
						for (j = 0; j < i; j++)
							b[h][w + len - i + j] = 0;
						if (checkUnblock(a, b, h, w))
						{
							if (step(b, depth - 1, h, w - i) == 1)
							{
								dump(b, depth);
								return 1;
							}
						}
						for (j = 0; j < ORDER; j++)		//restore
							b[h][j] = a[h][j];
					}

					for (i = 1; w + i + len - 1 < ORDER && a[h][w + i + len - 1] == 0; i++)	//move right i step and not knock into anything
						;
					i--;
					if (i > 0)
					{
						for (j = 0; j < i; j++)
							b[h][w + j] = 0;
						for (j = 0; j < len; j++)
							b[h][w + j + i] = a[h][w + j];
						if (checkUnblock(a, b, h, w))
						{
							if (step(b, depth - 1, h, w + i) == 1)
							{
								dump(b, depth);
								return 1;
							}
						}
						for (j = 0; j < ORDER; j++)		//restore
							b[h][j] = a[h][j];
					}
				}
			}
			else if (a[h][w] > 0)						//vertical
			{
				len = 1;
				for (i = 1; h + i < ORDER; i++)
					if (a[h + i][w] == a[h][w])
						len++;
				if (h == 0 || a[h - 1][w] != a[h][w])	// for vertical block, only operate the top end
				{
					for (i = 1; h - i >= 0 && a[h - i][w] == 0; i++)			//move up i step and not knock into anything
						;
					i--;
					if (i > 0)
					{
						for (j = 0; j < len; j++)
							b[h + j - i][w] = a[h + j][w];
						for (j = 0; j < i; j++)
							b[h + len - i + j][w] = 0;
						if (a[h][w] == 8 && b[OUTLET_H][OUTLET_W] == 8)
						{
							dump(b, depth);
							return 1;
						}
						if (checkUnblock(a, b, h, w))
						{
							if (step(b, depth - 1, h - i, w) == 1)
							{
								dump(b, depth);
								return 1;
							}
						}
						for (j = 0; j < ORDER; j++)		//restore
							b[j][w] = a[j][w];
					}

					for (i = 1; h + i + len - 1 < ORDER && a[h + i + len - 1][w] == 0; i++)	//move down i step and not knock into anything
						;
					i--;
					if (i > 0)
					{
						for (j = 0; j < i; j++)
							b[h + j][w] = 0;
						for (j = 0; j < len; j++)
							b[h + j + i][w] = a[h + j][w];
						if (a[h][w] == 8 && b[OUTLET_H][OUTLET_W] == 8)
						{
							dump(b, depth);
							return 1;
						}
						if (checkUnblock(a, b, h, w))
						{
							if (step(b, depth - 1, h + i, w) == 1)
							{
								dump(b, depth);
								return 1;
							}
						}
						for (j = 0; j < ORDER; j++)		//restore
							b[j][w] = a[j][w];
					}
				}
			}
		}
	return 0;
}


int  main()
{
#if 0
	int depth = 3;
	char a[6][6] = { { 0, 0, 0, 0, 1, 1},
					 { 0, 0, 0, 0, 1, 1},
					 { 0, 1,-1,-1, 1, 1},
			 	 	 { 0, 1, 8,-1,-1,-1},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 { 0,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 4;
	char a[6][6] = { {-1,-1,-1, 0, 0, 0},
			 	 	 { 0,-1,-1,-1, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 1},
			 	 	 { 0, 0,-1,-1, 0, 1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0, 0, 8, 1,-1,-1}
	};
#endif
#if 0
	int depth = 5;
	char a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-2,-2, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-2,-2, 1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 1
	int depth = 7;
	char a[6][6] = { { 0,-1,-1,-1, 0, 0},
			 	 	 { 0, 1,-1,-1,-1, 1},
			 	 	 { 0, 1, 0, 1, 0, 1},
			 	 	 { 0, 2, 0, 1,-1,-1},
			 	 	 { 0, 2, 8, 0, 0, 0},
			 	 	 {-1,-1, 8, 0, 0, 0} };
#endif
#if 0
	int depth = 7;
	char a[6][6] = { { 0,-1,-1, 0, 1, 0},
			 	 	 { 1, 0,-1,-1, 1, 0},
			 	 	 { 1, 0, 8, 0, 1, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 {-1,-1,-1, 1, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 8;
	char a[6][6] = { {-1,-1,-2,-2, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 { 1,-1,-1,-2,-2, 0},
			 	 	 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 8;
	char a[6][6] = { {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 0, 1,-1,-1},
			 	 	 { 1, 0, 0, 1, 1, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 {-1,-1, 8, 0, 0, 0},
			 	 	 {-1,-1, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 9;
	char a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 1},
			 	 	 { 0, 0, 8, 0, 1, 1},
			 	 	 { 1, 0, 8, 0,-1,-1},
			 	 	 { 1, 0,-1,-1, 0, 2},
			 	 	 { 0, 0, 0, 0, 0, 2}
	};
#endif
#if 0
	int depth = 9;
	char a[6][6] = { { 1,-1,-1, 1,-2,-2},
			 	 	 { 1,-1,-1, 1, 0, 0},
			 	 	 { 1, 0,-1,-1, 0, 0},
			 	 	 {-1,-1, 8,-2,-2, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 9;
	char a[6][6] = { { 1, 0,-1,-1, 0, 0},
			 	 	 { 1, 0,-1,-1, 0, 0},
			 	 	 { 2, 0, 0, 0, 1, 0},
			 	 	 { 2,-1,-1, 0, 1, 0},
			 	 	 {-1,-1, 8, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1,-1}
	};
#endif
#if 0
	int depth = 9;
	char a[6][6] = { { 0, 1, 0, 1, 0, 0},
			 	 	 { 0, 1, 0, 1,-1,-1},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 9;
	char a[6][6] = { { 0, 0, 0, 0, 0, 0},
					 { 0, 0, 0, 1,-1,-1},
					 { 1,-1,-1, 1, 0, 0},
					 { 1,-1,-1,-1, 0, 1},
					 { 0, 0, 8, 0, 0, 1},
					 { 0, 0, 8,-1,-1,-1}
	};
#endif
#if 0
	int depth = 10;
	char a[6][6] = { {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 { 0, 1, 8,-1,-1,-1},
			 	 	 { 1, 1,-1,-1, 0, 1},
			 	 	 { 1, 0, 0, 0, 0, 1},
			 	 	 {-1,-1,-1, 0, 0, 1}
	};
#endif
#if 0
	int depth = 10;
	char a[6][6] = { { 1, 1, 0, 0, 0, 0},
			 	 	 { 1, 1,-1,-1,-1, 1},
			 	 	 { 0, 0, 8, 1, 0, 1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 10;
	char a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 10;
	char a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 {-1,-1,-1, 1, 1, 0},
			 	 	 { 0, 0, 8, 1, 1, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 { 1,-1,-1, 0, 0, 0},
			 	 	 { 1, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 10;
	char a[6][6] = { {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 1, 0, 8,-1,-1, 1},
			 	 	 { 1,-1,-1, 0, 0, 1},
			 	 	 { 1,-1,-1, 0, 0, 1}
	};
#endif
#if 0
	int depth = 11;
	char a[6][6] = { {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1, 0, 0, 0, 0},
			 	 	 { 1, 1, 8,-1,-1, 0},
			 	 	 { 1, 1, 8, 0, 0, 0},
			 	 	 { 0, 1,-1,-1,-1, 0}
	};
#endif
#if 0
	int depth = 11;
	char a[6][6] = { { 0, 1,-1,-1,-1, 0},
					 { 0, 1, 0, 0, 1, 0},
					 { 0, 1,-1,-1, 1, 0},
					 {-1,-1, 0, 0, 1, 0},
					 { 0, 0, 8,-1,-1, 0},
					 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0  //ok
	int depth = 11;
	char a[6][6] = { { 0,-1,-1,-1, 0, 1},
			 	 	 { 0,-1,-1,-1, 0, 1},
			 	 	 { 0, 0, 0, 1, 0, 0},
			 	 	 { 0,-1,-1, 1,-2,-2},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 { 0, 0,-8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 11;
	char a[6][6] = { { 0, 1,-1,-1,-2,-2},
			 	 	 { 0, 1, 0, 1,-1,-1},
			 	 	 { 0, 0, 0, 1, 0, 0},
			 	 	 { 0,-1,-1,-1, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 1}
	};
#endif
#if 0
	int depth = 11;
	char a[6][6] = { { 1,-1,-1,-1, 0, 1},
			 	 	 { 1, 0, 0, 0, 0, 1},
			 	 	 { 0, 0, 0, 0,-1,-1},
			 	 	 {-1,-1,-1, 1, 1, 2},
			 	 	 { 0, 0, 8, 1, 1, 2},
			 	 	 { 0, 0, 8, 0, 1, 0}
	};
#endif
#if 0
	int depth = 11;
	char a[6][6] = { { 0, 1, 0, 0, 0, 0},
			 	 	 { 0, 1, 0,-1,-1, 0},
			 	 	 {-1,-1,-1, 0, 2, 0},
			 	 	 {-1,-1, 8, 0, 2, 0},
			 	 	 { 0, 0, 8, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0}
	}
#endif
#if 0
	int depth = 11;
	char a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 { 1, 0,-1,-1, 1, 0},
			 	 	 { 2, 0, 0, 1,-1,-1},
			 	 	 { 2, 0, 8, 1, 0, 0},
			 	 	 {-1,-1, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 12;
	char a[6][6] = { { 0,-1,-1,-1, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 1,-1,-1},
			 	 	 { 0,-2,-2, 1,-1,-1},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 8,-1,-1, 0}
	};
#endif
#if 0
	int depth = 12;
	char a[6][6] = { {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 0, 1, 0, 0},
			 	 	 { 1,-1,-1,-1, 0, 0},
			 	 	 { 1,-2,-2,-1,-1, 1},
			 	 	 { 1, 0, 8, 0, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 1}
	};
#endif
#if 0
	int depth = 12;
	char a[6][6] = { {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1, 8, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 1, 0, 0, 0, 0},
			 	 	 { 0, 1,-1,-1,-2,-2}
	};
#endif
#if 0
	//Level 31
	int depth = 12;
	char a[6][6] = { { 0,-1,-1,-1, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 1,-1,-1},
			 	 	 { 0,-1,-1, 1,-2,-2},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 8,-1,-1, 0}
	};
#endif
#if 0
	int depth = 12;
	char a[6][6] = { {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1, 8, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 1, 0, 0, 0, 0},
			 	 	 { 0, 1,-1,-1,-2,-2}
	};
#endif
#if 0
	int depth = 12;
	char a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 { 1, 0, 8, 1, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	char a[6][6] = { { 0, 0, 0, 0, 0, 1},
			 	 	 { 1,-1,-1,-1, 0, 1},
			 	 	 { 1, 0, 8, 1,-1,-1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0, 0, 0, 1, 0, 2},
			 	 	 { 0, 0,-1,-1, 0, 2}
	};
#endif
#if 0
	int depth = 13;
	char a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-2,-2, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0,-1,-1, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	char a[6][6] = { { 0, 0,-1,-1,-1, 0},
			 	 	 { 0, 0,-1,-1,-1, 0},
			 	 	 {-1,-1,-2,-2, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	char a[6][6] = { { 0, 0,-1,-1,-1, 0},
			 	 	 { 0, 0,-1,-1,-1, 0},
			 	 	 {-1,-1,-2,-2, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	char a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-2,-2, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0,-1,-1, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	char a[6][6] = { { 1, 0, 0, 0, 0, 0},
			 	 	 { 1,-1,-1, 0, 1, 0},
			 	 	 { 1, 0, 8, 0, 1, 0},
			 	 	 {-1,-1, 8,-2,-2, 0},
			 	 	 { 0, 1,-1,-1, 0, 0},
			 	 	 { 0, 1, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 18;	//attempt = 20785555	depth = 16 is ok
	char a[6][6] = { {-1,-1,-1, 0, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 0, 0},
			 	 	 { 1,-1,-1,-1, 1, 1},
			 	 	 { 1, 0, 8, 0, 1, 1},
			 	 	 { 1, 0, 8, 0,-1,-1}
	};
#endif
#if 0
	int depth = 18;
	char a[6][6] = { { 0, 0,-1,-1,-1, 0},
			 	 	 { 0,-1,-1, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 1, 1},
			 	 	 { 0,-1,-1, 0, 1, 1},
			 	 	 { 0, 0, 8, 0,-1,-1},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	char a[6][6] = { { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1,-1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0,-1,-1, 1, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	char a[6][6] = { { 1,-1,-1,-1, 0, 0},
					 { 1,-1,-1,-1, 0, 1},
					 {-1,-1,-1, 1, 0, 1},
					 { 0, 0, 8, 1,-1,-1},
					 { 0, 0, 8, 1, 0, 0},
					 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	char a[6][6] = { { 0, 0, 0, 0, 1, 0},
					 { 0,-1,-1,-1, 1, 0},
					 { 0, 0, 8,-1,-1,-1},
					 { 0, 0, 8,-1,-1, 0},
					 { 0, 0, 0, 1, 0, 1},
					 {-1,-1,-1, 1, 0, 1}
	};
#endif
#if 0
	int depth = 19;
	char a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 { 1, 0, 0, 1,-1,-1},
			 	 	 { 1, 0, 8, 1, 0, 0},
			 	 	 {-1,-1, 8, 1, 0, 0}
	};
#endif
#if 0
	int depth = 19;
	char a[6][6] = { { 0,-1,-1,-1, 0, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 { 1, 0, 0, 0, 1, 0},
			 	 	 { 1, 0, 0, 1,-1,-1},
			 	 	 {-1,-1, 8, 1, 0, 0},
			 	 	 { 0, 0, 8, 1, 0, 0}
	};
#endif
#if 0
	int depth = 19;
	char a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 {-1,-1, 8,-2,-2, 1},
			 	 	 { 1,-1,-1,-1, 0, 1},
			 	 	 { 1, 0, 0, 0, 0, 1}
	};
#endif
#if 0
	int depth = 19;
	char a[6][6] = { { 0,-1,-1, 0,-2,-2},
					 { 0,-1,-1, 0, 0, 1},
					 { 0,-1,-1,-1, 0, 1},
					 { 0,-1,-1,-2,-2, 1},
					 {-1,-1, 8, 1,-2,-2},
					 {-1,-1, 8, 1,-2,-2} };
#endif
#if 0
	int depth = 19;
	char a[6][6] = { { 0,-1,-1, 0,-2,-2},
					 { 0,-1,-1, 0, 0, 1},
					 { 0,-1,-1,-1, 0, 1},
					 { 0,-1,-1,-2,-2, 1},
					 {-1,-1, 8, 1,-2,-2},
					 {-1,-1, 8, 1,-2,-2}
	};
#endif
#if 0
	int depth = 20;
	char a[6][6] = { {-1,-1,-1, 0, 0, 1},
			 	 	 { 1,-1,-1, 0, 1, 1},
			 	 	 { 1,-1,-1, 0, 1, 1},
			 	 	 { 2,-2,-2, 0, 1, 0},
			 	 	 { 2, 0, 8,-1,-1,-1},
			 	 	 {-1,-1, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 20;
	char a[6][6] = { { 0,-1,-1, 0, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 1},
			 	 	 { 0, 0, 8, 0, 0, 1},
			 	 	 {-1,-1,-1, 1, 0, 1},
			 	 	 { 0, 0, 0, 1, 0, 0}
	};
#endif
#if 0
	int depth = 21;
	char a[6][6] = { {-1,-1, 0, 1,-2,-2},
			 	 	 {-1,-1, 0, 1, 0, 0},
			 	 	 { 1,-1,-1,-2,-2, 1},
			 	 	 { 1, 0, 0, 0, 0, 1},
			 	 	 {-1,-1, 8, 2, 0, 1},
			 	 	 {-1,-1, 8, 2,-2,-2}
	};
#endif
#if 0
	int depth = 23;
	char a[6][6] = { {-1,-1, 0, 1, 0, 0},
			 	 	 {-1,-1, 0, 1, 0, 0},
			 	 	 { 1, 1,-1,-1,-2,-2},
			 	 	 { 1, 1, 8,-1,-1, 1},
			 	 	 { 2, 0, 8,-1,-1, 1},
			 	 	 { 2,-1,-1,-1, 0, 1}
	};
#endif
#if 0
	int depth = 24;
	char a[6][6] = { { 0, 1,-1,-1,-1, 0},
			 	 	 { 0, 1, 0, 0, 0, 0},
			 	 	 {-1,-1, 0, 1, 0, 1},
			 	 	 {-1,-1,-1, 1, 0, 1},
			 	 	 { 0, 0, 8, 1, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 24;  //example
	char a[6][6] = { { 1, 1,-1,-1,-1, 1},
			 	 	 { 1, 1, 0, 0, 0, 1},
			 	 	 { 1,-1,-1, 0, 1, 2},
			 	 	 {-1,-1, 8, 0, 1, 2},
			 	 	 { 0, 2, 8,-1,-1,-1},
			 	 	 { 0, 2,-1,-1,-2,-2}
	};
#endif
#if 0
	int depth = 21; // 23;  //example
	char a[6][6] = { {-1,-1, 0, 1, 0, 0},
					 {-1,-1, 0, 1, 0, 0},
					 { 1, 1,-1,-1,-2,-2},
					 { 1, 1, 8,-1,-1, 1},
					 { 2, 0, 8,-1,-1, 1},
					 { 2,-1,-1,-1, 0, 1}
	};
#endif
#if 0
	int depth = 21;  //24->21
	char a[6][6] = { { 0,-1,-1,-2,-2, 0},
					{-1,-1,-1, 1, 0, 0},
					{ 0, 0, 8, 1,-1,-1},
					{-1,-1, 8,-2,-2, 1},
					{-1,-1,-1, 0, 0, 1},
					{ 0, 0,-1,-1, 0, 1}
	};
#endif
#if 0
	int depth = 19;  //23->19
	char a[6][6] = {{ 1, 0, 0, 0, 0, 0},
					{ 1,-1,-1,-2,-2, 0},
					{-1,-1,-1, 0, 1, 0},
					{-1,-1,-1, 0, 1, 0},
					{ 0, 0, 8,-1,-1, 0},
					{ 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 21;  //23->19
	char a[6][6] = {{ 1, 0,-1,-1, 1, 0},
					{ 1,-1,-1,-1, 1, 1},
					{-1,-1, 8, 0, 1, 1},
					{ 0, 0, 8, 1,-1,-1},
					{ 0,-1,-1, 1, 0, 0},
					{ 0,-1,-1, 1,-2,-2}
	};
#endif
#if 0
	int depth = 24;  //24->22
	int a[6][6] = { { 1,-1,-1,-1, 0, 0},
					{ 1, 0, 0, 1,-1,-1},
					{ 1,-1,-1, 1, 0, 0},
					{-1,-1, 8, 1,-2,-2},
					{-1,-1, 8, 0, 0, 1},
					{-1,-1,-2,-2, 0, 1}
	};
#endif
	total = depth;
	step(a, depth, -1, -1);
	//dump(a, depth);
	return 0;
}