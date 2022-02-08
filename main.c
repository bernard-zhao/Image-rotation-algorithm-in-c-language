
#define CV_PI 3.1415926

void aod_png_res_rotate(float angle, unsigned char* src_img, int src_width, int src_height,
					unsigned char* dst_img, int buf_width, int buf_height)
{
	int x,y,i,j;
	unsigned int *p;
	unsigned int *p1, *p2;
	float src_x1, src_y1, src_x2, src_y2, src_x3, src_y3, src_x4, src_y4;
	float dst_x1, dst_y1, dst_x2, dst_y2, dst_x3, dst_y3, dst_x4, dst_y4;
	int dst_width = 0;
	int dst_height = 0;
	float theta = angle*CV_PI / 180.0;
	int src_lineByte = src_width*4;
	int dst_lineByte = 0;
	float dx, dy;
	int x_offcet, y_offcet;
	
	// 源图四个角的坐标（以图像中心为坐标系原点）
	src_x1 = (float) (- (src_width  - 1) / 2);
	src_y1 = (float) (  (src_height - 1) / 2);
	src_x2 = (float) (  (src_width  - 1) / 2);
	src_y2 = (float) (  (src_height - 1) / 2);
	src_x3 = (float) (- (src_width  - 1) / 2);
	src_y3 = (float) (- (src_height - 1) / 2);
	src_x4 = (float) (  (src_width  - 1) / 2);
	src_y4 = (float) (- (src_height - 1) / 2);

	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	dst_x1 =  cos(theta) * src_x1 + sin(theta) * src_y1;
	dst_y1 = -sin(theta) * src_x1 + cos(theta) * src_y1;
	dst_x2 =  cos(theta) * src_x2 + sin(theta) * src_y2;
	dst_y2 = -sin(theta) * src_x2 + cos(theta) * src_y2;
	dst_x3 =  cos(theta) * src_x3 + sin(theta) * src_y3;
	dst_y3 = -sin(theta) * src_x3 + cos(theta) * src_y3;
	dst_x4 =  cos(theta) * src_x4 + sin(theta) * src_y4;
	dst_y4 = -sin(theta) * src_x4 + cos(theta) * src_y4;

	dst_width  = (fmax(fabs(dst_x4 - dst_x1), fabs(dst_x3 - dst_x2)) + 0.5);
	dst_height = (fmax(fabs(dst_y4 - dst_y1), fabs(dst_y3 - dst_y2)) + 0.5);

	dx = -0.5*dst_width*cos(theta) - 0.5*dst_height*sin(theta) + 0.5*src_width;
	dy = 0.5*dst_width*sin(theta) - 0.5*dst_height*cos(theta) + 0.5*src_height;

	//if (dst_height >= buf_height)
		//AOD_DEBUG("[bernard] aod_png_res_rotate Not Enough Memory! dst_height:%d buf_height:%d\n", dst_height, buf_height);
	
	dst_lineByte = buf_width * 4;
	x_offcet = (buf_width - dst_width)/2;
	y_offcet = (buf_height - dst_height)/2;
	//AOD_DEBUG("[bernard] aod_png_res_rotate offcet x_offcet:%d y_offcet:%d\n", x_offcet, y_offcet);
	
	for (i = 0; i < dst_height; i++)
	{
		for (j = 0; j < dst_width; j++)
		{
			x = (float)(j)*cos(theta) + (float)(i)*sin(theta) + dx;
			y = (float)(-j)*sin(theta) + (float)(i)*cos(theta) + dy;

			if ((x<0) || (x>=src_width) || (y<0) || (y>=src_height))
			{
				//填充区域
				p = (unsigned int *)(dst_img + (i+y_offcet)*dst_lineByte + (j+x_offcet)*4);
				(*p) = 0xff0000ff;
				//AOD_DEBUG("[bernard] aod_png_res_rotate p addr %x=%x\n", p, *p);
			}
			else
			{
				p1 = (unsigned int *)(dst_img + (i+y_offcet)*dst_lineByte + (j+x_offcet)*4); // 新图像
				p2 = (unsigned int *)(src_img + y*src_lineByte + x*4);  // 原图像
				(*p1) = (*p2);
				//AOD_DEBUG("[bernard] aod_png_res_rotate p1 addr %x=%x, p2 addr %x=%x\n", p1, *p1, p2, *p2);
			}
		}
	}

	//AOD_DEBUG("[bernard] aod_png_res_rotate finished\n");
}
