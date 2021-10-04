//	By Hu Zhenyu	2021.10.04	hunter_hu163@163.com

#include <iostream>

#define ORDER		6
#define OUTLET_W	2
#define OUTLET_H	0
#if 1
int routePointDepth = 9;
int routePoint[ORDER][ORDER] ={
  					 {-1,-1,-1, 1, 0, 1},
					 { 0, 0, 0, 1, 0, 1},
					 {-1,-1,-1, 0, 0, 1},
					 { 1,-2,-2,-1,-1, 0},
					 { 1, 0, 8, 0, 0, 0},
					 { 1, 0, 8, 0, 0, 0}
};
#endif
enum DIRECTION 
{
	LEFT, RIGHT, UP, DOWN 
}direction;

int attempt = 0;
int total;
void dump(int a[][ORDER], int depth)
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

int checkRoute(int a[][ORDER], int depth)
{
	if (depth != routePointDepth)
		return 0;
	for (int h = 0; h < ORDER; h++)					//height
		for (int w = 0; w < ORDER; w++)				//width
			if (a[h][w] != routePoint[h][w])
				return 0;
	return 1;
}

int checkOtherSide(int a[][ORDER], int h, int w, enum DIRECTION dir, int* head_h, int* head_w)
{
	int i, j, start, end, space_start = 0, space_end = 0;

	if (dir == UP || dir == DOWN)
	{
		if (dir == UP)
		{
			for (i = h + 1; i < ORDER; i++)
				if (a[i][w] != 0)
					break;
			if (i == ORDER || a[i][w] > 0)			//skip for vertical block
				return 0;
		}
		else	//DOWN
		{
			for (i = h - 1; i >= 0; i--)
				if (a[i][w] != 0)
					break;
			if (i == -1 || a[i][w] > 0)			//skip for vertical block
				return 0;
		}
		//try to empty a[i][w], check left & right
		start = end = w;
		for (j = w - 1; j >= 0; j--)
			if (a[i][j] == a[i][w])
				start = j;
		for (j = w + 1; j < ORDER; j++)
			if (a[i][j] == a[i][w])
				end = j;
		for (j = start - 1; j >= 0; j--)
			if (a[i][j] == 0)
				space_start++;
		for (j = end + 1; j < ORDER; j++)
			if (a[i][j] == 0)
				space_end++;
		if (end - w + 1 <= space_start || w - start + 1 <= space_end)	//need to queue
		{
			*head_h = i;
			*head_w = start;
			return 1;
		}
	}
	else	//LEFT || RITHG
	{
		if (dir == LEFT)
		{
			for (i = w + 1; i < ORDER; i++)
				if (a[h][i] != 0)
					break;
			if (i == ORDER || a[h][i] < 0)			//skip for horizontal block
				return 0;
		}
		else	//RIGHT
		{
			for (i = w - 1; i >= 0; i--)
				if (a[h][i] != 0)
					break;
			if (i == -1 || a[h][i] < 0)			//skip for horizontal block
				return 0;
		}
		//try to empty a[h][i], check up down
		start = end = h;
		for (j = h - 1; j >= 0; j--)
			if (a[j][i] == a[h][i])
				start = j;
		for (j = w + 1; j < ORDER; j++)
			if (a[j][i] == a[h][i])
				end = j;
		for (j = start - 1; j >= 0; j--)
			if (a[j][i] == 0)
				space_start++;
		for (j = end + 1; j < ORDER; j++)
			if (a[j][i] == 0)
				space_end++;
		if (end - w + 1 <= space_start || w - start + 1 <= space_end)	//need to queue
		{
			*head_h = start;
			*head_w = i;
			return 1;
		}
	}
	return 0;
}

int getUnBlock(int a[][ORDER], int b[][ORDER], int h, int w, int pri[][2])
{
	int i, j, k, head_h, head_w, start = 0, end = ORDER - 1, idx = 0, found = 0;

	if (a[h][w] == 0)
		printf("getUnblock error");
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
				found = 0;
				for (j = h - 1; j > 0 && found == 0; j--)	// up side
				{
					if (a[j][i] > 0)						// it blocks a vertical block at first
					{
						for (k = 0; k < j; k++)				// find the head of the vertical block from top
							if (b[k][i] == b[j][i])
							{
								pri[idx][0] = k;
								pri[idx][1] = i;
								idx++;
								found = 1;
								if (checkOtherSide(b, h, i, UP, &head_h, &head_w) == 1)
								{
									pri[idx][0] = head_h;
									pri[idx][1] = head_w;
									idx++;
								}
								break;
							}
					}
					else if (a[j][i] < 0)			// find a horizontal block, no need to proceed
					{
						break;
					}
				}

				for (j = h + 1; j < ORDER; j++)				// down side
				{
					if (a[j][i] > 0)						// it blocks a vertical block at first
					{
						pri[idx][0] = j;
						pri[idx][1] = i;
						idx++;
						if (checkOtherSide(b, h, i, DOWN, &head_h, &head_w) == 1)
						{
							pri[idx][0] = head_h;
							pri[idx][1] = head_w;
							idx++;
						}
						break;
					}
					else if (a[j][i] < 0)			// find a horizontal block, no need to proceed
					{
						break;
					}
				}
				
				found = 0;
				for (j = i - 1; j > 0 && found == 0 ; j--)	// left side
				{
					if (b[h][j] < 0 && b[h][j] != a[h][w])	// it blocks a horizontal block at first 
					{
						for (k = 0; k < j; k++)				// find the head of the horizontal block from left
							if (b[h][k] == b[h][j])
							{
								pri[idx][0] = h;
								pri[idx][1] = k;
								idx++;
								found = 1;
								break;
							}
					}
					else if (b[h][j] > 0 || b[h][j] == a[h][w])	// find a vertical block or itself, no need to proceed
					{
						break;
					}
				}

				for (j = i + 1; j < ORDER; j++)				// right side
				{
					if (b[h][j] < 0 && b[h][j] != a[h][w])	// it blocks a horizontal block at first 
					{
						pri[idx][0] = h;
						pri[idx][1] = j;
						idx++;
						break;
					}
					else if (b[h][j] > 0 || b[h][j] == a[h][w])	// find a vertical block or itself, no need to proceed
					{
						break;
					}
				}
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

		for (i = start; i <= end; i++)						//vertical trace not including the end position
			if (b[i][w] == 0)
			{
				found = 0;
				for (j = w - 1; j > 0 && found == 0; j--)	// left side
				{
					if (a[i][j] < 0)						// it blocks horizontal block at first
					{
						for (k = 0; k < j; k++)				// find the head of the horizontal block from left
							if (b[i][k] == b[i][j])
							{
								pri[idx][0] = i;
								pri[idx][1] = k;
								idx++;
								found = 1;
								if (checkOtherSide(b, i, w, LEFT, &head_h, &head_w) == 1)
								{
									pri[idx][0] = head_h;
									pri[idx][1] = head_w;
									idx++;
								}
								break;
							}
					}
					else if (a[i][j] > 0)					// find a vertical block, no need to proceed
					{
						break;
					}
				}
				for (j = w + 1; j < ORDER; j++)		// right side
				{
					if (a[i][j] < 0)				// it blocks horizontal block at first
					{
						pri[idx][0] = i;			// find the head of the horizontal block from left
						pri[idx][1] = j;
						idx++;
						if (checkOtherSide(b, i, w, RIGHT, &head_h, &head_w) == 1)
						{
							pri[idx][0] = head_h;
							pri[idx][1] = head_w;
							idx++;
						}
						break;
					}
					else if (a[i][j] > 0)			// find a vertical block, no need to proceed
					{
						break;
					}
				}
				found = 0;
				for (j = i - 1; j > 0 && found == 0; j--)	// top side
				{
					if (b[j][w] > 0 && b[j][w] != a[h][w])	// it blocks a vertical block at first 
					{
						for (k = 0; k < j; k++)				// find the head of the vertical block from top
							if (b[k][w] == b[j][w])
							{
								pri[idx][0] = k;
								pri[idx][1] = w;
								idx++;
								found = 1;
								break;
							}
					}
					else if (b[j][w] > 0 || b[j][w] == a[h][w])	// find a horizontal block or itself, no need to proceed
					{
						found = 1;
						break;
					}
				}

				for (j = i + 1; j < ORDER; j++)					// down side
				{
					if (b[j][w] > 0 && b[j][w] != a[h][w])		// it blocks a vertical block at first 
					{
						pri[idx][0] = j;
						pri[idx][1] = w;
						idx++;
						break;
					}
					else if (b[j][w] > 0 || b[j][w] == a[h][w])	// find a horizontal block or itself, no need to proceed
					{
						break;
					}
				}
			}
	}
	return idx;
}

int step(int a[][ORDER], int depth, int pri[][2], int priNum)
{
	int b[ORDER][ORDER];
	int h, w, i, j, p, len, skip;
	int my_pri[20][2];
	int my_priNum;
	if (depth == 0)
		return 0;

	for (h = 0; h < ORDER; h++)					//height
		for (w = 0; w < ORDER; w++)				//width
			b[h][w] = a[h][w];					//copy a -> b

	//dump(a, depth);
	attempt++;

	printf("attempt = %d\n", attempt);
	if (checkRoute(a, depth) == 1)
		printf("11111");
	//if (attempt == 30)
	//	printf("11111");

	if (priNum == 0)
	{
		printf("priNum == 0 ???\n");
		exit(0);
	}

	for (p = 0; p < priNum; p++)					// the priority list
	{
		//if (depth == 5 && p == 1)
		//	printf("111");
		h = pri[p][0];
		w = pri[p][1];
		if (a[h][w] < 0)							//horizontal
		{
			len = 1;
			for (i = 1; w + i < ORDER; i++)			//calculate block length
				if (a[h][w + i] == a[h][w])
					len++;

			for (i = 1; w - i >= 0 && a[h][w - i] == 0; i++)	//move left i step and not knock into anything, i is the max value
				;
			i--;
			if (i > 0)
			{
				for (j = 0; j < len; j++)
					b[h][w + j - i] = a[h][w + j];
				for (j = 0; j < i; j++)
					b[h][w + len - i + j] = 0;
				my_priNum = getUnBlock(a, b, h, w, my_pri);
				if (my_priNum > 0)
				{
					if (step(b, depth - 1, my_pri, my_priNum) == 1)
					{
						dump(b, depth);
						return 1;
					}
				}
				for (j = 0; j < ORDER; j++)		//restore
					b[h][j] = a[h][j];
			}

			for (i = 1; w + i + len - 1 < ORDER && a[h][w + i + len - 1] == 0; i++)	//move right i step and not knock into anything, i is the max value
				;
			i--;
			if (i > 0)
			{
				for (j = 0; j < i; j++)
					b[h][w + j] = 0;
				for (j = 0; j < len; j++)
					b[h][w + j + i] = a[h][w + j];
				my_priNum = getUnBlock(a, b, h, w, my_pri);
				if (my_priNum > 0)
				{
					if (step(b, depth - 1, my_pri, my_priNum) == 1)
					{
						dump(b, depth);
						return 1;
					}
				}
				for (j = 0; j < ORDER; j++)		//restore
					b[h][j] = a[h][j];
			}
		}
		else if (a[h][w] > 0)						//vertical
		{
			len = 1;
			for (i = 1; h + i < ORDER; i++)
				if (a[h + i][w] == a[h][w])
					len++;

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
				my_priNum = getUnBlock(a, b, h, w, my_pri);
				if (my_priNum > 0)
				{
					if (step(b, depth - 1, my_pri, my_priNum) == 1)
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
				my_priNum = getUnBlock(a, b, h, w, my_pri);
				if (my_priNum > 0)
				{
					if (step(b, depth - 1, my_pri, my_priNum) == 1)
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
	
	return 0;
}

int  main()
{
	int pri[20][2];
	int priNum = 0;
#if 0
	int depth = 3;
	int  a[6][6] = { { 0, 0, 0, 0, 1, 1},
					 { 0, 0, 0, 0, 1, 1},
					 { 0, 1,-1,-1, 1, 1},
			 	 	 { 0, 1, 8,-1,-1,-1},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 { 0,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 4;
	int  a[6][6] = { {-1,-1,-1, 0, 0, 0},
			 	 	 { 0,-1,-1,-1, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 1},
			 	 	 { 0, 0,-1,-1, 0, 1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0, 0, 8, 1,-1,-1}
	};
#endif
#if 0
	int depth = 5;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-2,-2, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-2,-2, 1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 7;
	int  a[6][6] = { { 0,-1,-1,-1, 0, 0},
			 	 	 { 0, 1,-1,-1,-1, 1},
			 	 	 { 0, 1, 0, 1, 0, 1},
			 	 	 { 0, 2, 0, 1,-1,-1},
			 	 	 { 0, 2, 8, 0, 0, 0},
			 	 	 {-1,-1, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 7;
	int  a[6][6] = { { 0,-1,-1, 0, 1, 0},
			 	 	 { 1, 0,-1,-1, 1, 0},
			 	 	 { 1, 0, 8, 0, 1, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 {-1,-1,-1, 1, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 8;
	int  a[6][6] = { {-1,-1,-2,-2, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 { 1,-1,-1,-2,-2, 0},
			 	 	 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 8;
	int  a[6][6] = { {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 0, 1,-1,-1},
			 	 	 { 1, 0, 0, 1, 1, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 {-1,-1, 8, 0, 0, 0},
			 	 	 {-1,-1, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 9;
	int  a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 1},
			 	 	 { 0, 0, 8, 0, 1, 1},
			 	 	 { 1, 0, 8, 0,-1,-1},
			 	 	 { 1, 0,-1,-1, 0, 2},
			 	 	 { 0, 0, 0, 0, 0, 2}
	};
#endif
#if 0
	int depth = 10;		//9 -> 10
	int  a[6][6] = { { 1,-1,-1, 1,-2,-2},
			 	 	 { 1,-1,-1, 1, 0, 0},
			 	 	 { 1, 0,-1,-1, 0, 0},
			 	 	 {-1,-1, 8,-2,-2, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 9;
	int  a[6][6] = { { 1, 0,-1,-1, 0, 0},
			 	 	 { 1, 0,-1,-1, 0, 0},
			 	 	 { 2, 0, 0, 0, 1, 0},
			 	 	 { 2,-1,-1, 0, 1, 0},
			 	 	 {-1,-1, 8, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1,-1}
	};
#endif
#if 0
	int depth = 9;
	int  a[6][6] = { { 0, 1, 0, 1, 0, 0},
			 	 	 { 0, 1, 0, 1,-1,-1},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 9;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 0},
					 { 0, 0, 0, 1,-1,-1},
					 { 1,-1,-1, 1, 0, 0},
					 { 1,-1,-1,-1, 0, 1},
					 { 0, 0, 8, 0, 0, 1},
					 { 0, 0, 8,-1,-1,-1}
	};
#endif
#if 0
	int depth = 10;
	int  a[6][6] = { {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 { 0, 1, 8,-1,-1,-1},
			 	 	 { 1, 1,-1,-1, 0, 1},
			 	 	 { 1, 0, 0, 0, 0, 1},
			 	 	 {-1,-1,-1, 0, 0, 1}
	};
#endif
#if 0
	int depth = 10;
	int  a[6][6] = { { 1, 1, 0, 0, 0, 0},
			 	 	 { 1, 1,-1,-1,-1, 1},
			 	 	 { 0, 0, 8, 1, 0, 1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 10;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 10;
	int  a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 {-1,-1,-1, 1, 1, 0},
			 	 	 { 0, 0, 8, 1, 1, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 { 1,-1,-1, 0, 0, 0},
			 	 	 { 1, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 10;
	int  a[6][6] = { {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 1, 0, 8,-1,-1, 1},
			 	 	 { 1,-1,-1, 0, 0, 1},
			 	 	 { 1,-1,-1, 0, 0, 1}
	};
#endif
#if 0
	int depth = 11;
	int  a[6][6] = { {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1, 0, 0, 0, 0},
			 	 	 { 1, 1, 8,-1,-1, 0},
			 	 	 { 1, 1, 8, 0, 0, 0},
			 	 	 { 0, 1,-1,-1,-1, 0}
	};
#endif
#if 0
	int depth = 11;
	int  a[6][6] = { { 0, 1,-1,-1,-1, 0},
					 { 0, 1, 0, 0, 1, 0},
					 { 0, 1,-1,-1, 1, 0},
					 {-1,-1, 0, 0, 1, 0},
					 { 0, 0, 8,-1,-1, 0},
					 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0  //ok
	int depth = 11;
	int  a[6][6] = { { 0,-1,-1,-1, 0, 1},
			 	 	 { 0,-1,-1,-1, 0, 1},
			 	 	 { 0, 0, 0, 1, 0, 0},
			 	 	 { 0,-1,-1, 1,-2,-2},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 { 0, 0,-8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 11;
	int  a[6][6] = { { 0, 1,-1,-1,-2,-2},
			 	 	 { 0, 1, 0, 1,-1,-1},
			 	 	 { 0, 0, 0, 1, 0, 0},
			 	 	 { 0,-1,-1,-1, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 1}
	};
#endif
#if 0
	int depth = 11;
	int  a[6][6] = { { 1,-1,-1,-1, 0, 1},
			 	 	 { 1, 0, 0, 0, 0, 1},
			 	 	 { 0, 0, 0, 0,-1,-1},
			 	 	 {-1,-1,-1, 1, 1, 2},
			 	 	 { 0, 0, 8, 1, 1, 2},
			 	 	 { 0, 0, 8, 0, 1, 0}
	};
#endif
#if 0
	int depth = 11;
	int  a[6][6] = { { 0, 1, 0, 0, 0, 0},
			 	 	 { 0, 1, 0,-1,-1, 0},
			 	 	 {-1,-1,-1, 0, 2, 0},
			 	 	 {-1,-1, 8, 0, 2, 0},
			 	 	 { 0, 0, 8, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0}
	}
#endif
#if 0
	int depth = 11;
	int  a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 { 1, 0,-1,-1, 1, 0},
			 	 	 { 2, 0, 0, 1,-1,-1},
			 	 	 { 2, 0, 8, 1, 0, 0},
			 	 	 {-1,-1, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 12;
	int  a[6][6] = { { 0,-1,-1,-1, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 1,-1,-1},
			 	 	 { 0,-2,-2, 1,-1,-1},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 8,-1,-1, 0}
	};
#endif
#if 0
	int depth = 12;
	int a[6][6] = { {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 0, 1, 0, 0},
			 	 	 { 1,-1,-1,-1, 0, 0},
			 	 	 { 1,-2,-2,-1,-1, 1},
			 	 	 { 1, 0, 8, 0, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 1}
	};
#endif
#if 0
	int depth = 12;
	int  a[6][6] = { {-1,-1,-1, 0, 1, 0},
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
	int  a[6][6] = { { 0,-1,-1,-1, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 1,-1,-1},
			 	 	 { 0,-1,-1, 1,-2,-2},
			 	 	 { 0, 0, 8, 0, 0, 0},
			 	 	 { 0, 0, 8,-1,-1, 0}
	};
#endif
#if 0
	int depth = 12;
	int  a[6][6] = { {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 {-1,-1, 8, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 1, 0, 0, 0, 0},
			 	 	 { 0, 1,-1,-1,-2,-2}
	};
#endif
#if 0
	int depth = 12;
	int  a[6][6] = { { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 1, 0, 8,-1,-1, 0},
			 	 	 { 1, 0, 8, 1, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 1},
			 	 	 { 1,-1,-1,-1, 0, 1},
			 	 	 { 1, 0, 8, 1,-1,-1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0, 0, 0, 1, 0, 2},
			 	 	 { 0, 0,-1,-1, 0, 2}
	};
#endif
#if 0
	int depth = 13;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-2,-2, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0,-1,-1, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	int  a[6][6] = { { 0, 0,-1,-1,-1, 0},
			 	 	 { 0, 0,-1,-1,-1, 0},
			 	 	 {-1,-1,-2,-2, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	int  a[6][6] = { { 0, 0,-1,-1,-1, 0},
			 	 	 { 0, 0,-1,-1,-1, 0},
			 	 	 {-1,-1,-2,-2, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 13;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-2,-2, 0},
			 	 	 {-1,-1,-1, 0, 1, 0},
			 	 	 { 0, 0,-1,-1, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 0},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	int  a[6][6] = { { 1, 0, 0, 0, 0, 0},
			 	 	 { 1,-1,-1, 0, 1, 0},
			 	 	 { 1, 0, 8, 0, 1, 0},
			 	 	 {-1,-1, 8,-2,-2, 0},
			 	 	 { 0, 1,-1,-1, 0, 0},
			 	 	 { 0, 1, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 16;	//attempt = 20785555	depth = 16 is ok
	int a[6][6] = { {-1,-1,-1, 0, 0, 0},
			 	 	 { 0, 0, 0, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 0, 0},
			 	 	 { 1,-1,-1,-1, 1, 1},
			 	 	 { 1, 0, 8, 0, 1, 1},
			 	 	 { 1, 0, 8, 0,-1,-1}
	};
#endif
#if 0
	int depth = 18;
	int  a[6][6] = { { 0, 0,-1,-1,-1, 0},
			 	 	 { 0,-1,-1, 0, 0, 0},
			 	 	 {-1,-1,-1, 0, 1, 1},
			 	 	 { 0,-1,-1, 0, 1, 1},
			 	 	 { 0, 0, 8, 0,-1,-1},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	int  a[6][6] = { { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 0, 0, 1, 0},
			 	 	 { 0, 0, 8,-1,-1,-1},
			 	 	 { 0, 0, 8, 1,-1,-1},
			 	 	 { 0,-1,-1, 1, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	int  a[6][6] = { { 1,-1,-1,-1, 0, 0},
					 { 1,-1,-1,-1, 0, 1},
					 {-1,-1,-1, 1, 0, 1},
					 { 0, 0, 8, 1,-1,-1},
					 { 0, 0, 8, 1, 0, 0},
					 { 0, 0, 0, 0, 0, 0}
	};
#endif
#if 0
	int depth = 18;
	int  a[6][6] = { { 0, 0, 0, 0, 1, 0},
					 { 0,-1,-1,-1, 1, 0},
					 { 0, 0, 8,-1,-1,-1},
					 { 0, 0, 8,-1,-1, 0},
					 { 0, 0, 0, 1, 0, 1},
					 {-1,-1,-1, 1, 0, 1}
	};
#endif
#if 0
	int depth = 19;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 { 1, 0, 0, 1,-1,-1},
			 	 	 { 1, 0, 8, 1, 0, 0},
			 	 	 {-1,-1, 8, 1, 0, 0}
	};
#endif
#if 0
	int depth = 19;
	int  a[6][6] = { { 0,-1,-1,-1, 0, 0},
			 	 	 { 1,-1,-1,-1, 1, 0},
			 	 	 { 1, 0, 0, 0, 1, 0},
			 	 	 { 1, 0, 0, 1,-1,-1},
			 	 	 {-1,-1, 8, 1, 0, 0},
			 	 	 { 0, 0, 8, 1, 0, 0}
	};
#endif
#if 0
	int depth = 19;
	int  a[6][6] = { { 0, 0, 0, 0, 0, 0},
			 	 	 {-1,-1,-1, 1, 0, 0},
			 	 	 { 0, 0, 8, 1, 0, 0},
			 	 	 {-1,-1, 8,-2,-2, 1},
			 	 	 { 1,-1,-1,-1, 0, 1},
			 	 	 { 1, 0, 0, 0, 0, 1}
	};
#endif
#if 0
	int depth = 19;
	int  a[6][6] = { { 0,-1,-1, 0,-2,-2},
					 { 0,-1,-1, 0, 0, 1},
					 { 0,-1,-1,-1, 0, 1},
					 { 0,-1,-1,-2,-2, 1},
					 {-1,-1, 8, 1,-2,-2},
					 {-1,-1, 8, 1,-2,-2} };
#endif
#if 0
	int depth = 19;
	int  a[6][6] = { { 0,-1,-1, 0,-2,-2},
					 { 0,-1,-1, 0, 0, 1},
					 { 0,-1,-1,-1, 0, 1},
					 { 0,-1,-1,-2,-2, 1},
					 {-1,-1, 8, 1,-2,-2},
					 {-1,-1, 8, 1,-2,-2}
	};
#endif
#if 0
	int depth = 20;
	int  a[6][6] = { {-1,-1,-1, 0, 0, 1},
			 	 	 { 1,-1,-1, 0, 1, 1},
			 	 	 { 1,-1,-1, 0, 1, 1},
			 	 	 { 2,-2,-2, 0, 1, 0},
			 	 	 { 2, 0, 8,-1,-1,-1},
			 	 	 {-1,-1, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 20;
	int  a[6][6] = { { 0,-1,-1, 0, 1, 0},
			 	 	 { 0,-1,-1,-1, 1, 0},
			 	 	 { 0, 0, 8,-1,-1, 1},
			 	 	 { 0, 0, 8, 0, 0, 1},
			 	 	 {-1,-1,-1, 1, 0, 1},
			 	 	 { 0, 0, 0, 1, 0, 0}
	};
#endif
#if 0
	int depth = 21;
	int  a[6][6] = { {-1,-1, 0, 1,-2,-2},
			 	 	 {-1,-1, 0, 1, 0, 0},
			 	 	 { 1,-1,-1,-2,-2, 1},
			 	 	 { 1, 0, 0, 0, 0, 1},
			 	 	 {-1,-1, 8, 2, 0, 1},
			 	 	 {-1,-1, 8, 2,-2,-2}
	};
#endif
#if 0
	int depth = 23;
	int  a[6][6] = { {-1,-1, 0, 1, 0, 0},
			 	 	 {-1,-1, 0, 1, 0, 0},
			 	 	 { 1, 1,-1,-1,-2,-2},
			 	 	 { 1, 1, 8,-1,-1, 1},
			 	 	 { 2, 0, 8,-1,-1, 1},
			 	 	 { 2,-1,-1,-1, 0, 1}
	};
#endif
#if 0
	int depth = 24;
	int  a[6][6] = { { 0, 1,-1,-1,-1, 0},
			 	 	 { 0, 1, 0, 0, 0, 0},
			 	 	 {-1,-1, 0, 1, 0, 1},
			 	 	 {-1,-1,-1, 1, 0, 1},
			 	 	 { 0, 0, 8, 1, 0, 1},
			 	 	 { 0, 0, 8, 0, 0, 0}
	};
#endif
#if 0
	int depth = 24;  //example
	int a[6][6] = { { 1, 1,-1,-1,-1, 1},
			 	 	 { 1, 1, 0, 0, 0, 1},
			 	 	 { 1,-1,-1, 0, 1, 2},
			 	 	 {-1,-1, 8, 0, 1, 2},
			 	 	 { 0, 2, 8,-1,-1,-1},
			 	 	 { 0, 2,-1,-1,-2,-2}
	};
#endif
#if 0
	int depth = 23; // 23;  //example
	int  a[6][6] = { {-1,-1, 0, 1, 0, 0},
					 {-1,-1, 0, 1, 0, 0},
					 { 1, 1,-1,-1,-2,-2},
					 { 1, 1, 8,-1,-1, 1},
					 { 2, 0, 8,-1,-1, 1},
					 { 2,-1,-1,-1, 0, 1}
	};
#endif
#if 0
	int depth = 25;		//29->25
	int a[6][6] = { { 0, 0, 0, 0, 0, 0},
					{ 1,-1,-1,-1, 1, 0},
				    { 1, 0, 0, 0, 1, 1},
					{ 2, 1, 8,-1,-1, 1},
					{ 2, 1, 8, 1, 0, 2},
					{-1,-1,-1, 1, 0, 2}
	};
#endif
#if 0
	int depth = 28;
	int a[6][6] = { { 0, 0,-1,-1, 1, 0},
					{ 1,-1,-1,-1, 1, 0},
					{ 1, 1, 0, 1, 0, 0},
					{ 1, 1, 0, 1,-1,-1},
					{-1,-1, 8, 2, 0, 1},
					{-1,-1, 8, 2, 0, 1}
	};
#endif
#if 0
	int depth = 20;  //24->20
	int a[6][6] = { { 0,-1,-1,-1, 0, 0},
					{-1,-1,-1, 1, 0, 0},
					{ 1, 0, 8, 1,-1,-1},
					{ 1, 0, 8,-1,-1, 1},
					{ 2,-1,-1,-1, 0, 1},
					{ 2, 0, 0, 0, 0, 1}
	};
#endif
#if 0
	int depth = 24;  //24->20
	int a[6][6] = { { 0,-1,-1,-2,-2, 0},
					{-1,-1,-1, 1, 0, 0},
					{ 0, 0, 8, 1,-1,-1},
					{-1,-1, 8,-2,-2, 1},
					{-1,-1,-1, 0, 0, 1},
					{ 0, 0,-1,-1, 0, 1}
	};
#endif
#if 1
	int depth = 24;  //24->22
	int a[6][6] = { { 1,-1,-1,-1, 0, 0},
					{ 1, 0, 0, 1,-1,-1},
					{ 1,-1,-1, 1, 0, 0},
					{-1,-1, 8, 1,-2,-2},
					{-1,-1, 8, 0, 0, 1},
					{-1,-1,-2,-2, 0, 1}
	};
#endif
	for (int h = 0; h < ORDER; h++)					//height
	{
		for (int w = 0; w < ORDER; w++)				//width
		{
			if (a[h][w] == 0)	//not a  block
				continue;

			if (a[h][w] < 0)							//horizontal
			{
				if (w == 0 || a[h][w - 1] != a[h][w])	// for horizontal block, only operate the left end
				{
					pri[priNum][0] = h;
					pri[priNum][1] = w;
					priNum++;
				}
			}
			else if (a[h][w] > 0)						//vertical
			{
				if (h == 0 || a[h - 1][w] != a[h][w])	// for vertical block, only operate the top end
				{
					pri[priNum][0] = h;
					pri[priNum][1] = w;
					priNum++;
				}
			}
		}
	}
	total = depth;
	step(a, depth, pri, priNum);
	//dump(a, depth);
	return 0;
}