/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI (float) 3.14159265358979323846

void swap(float&, float&);
void sort_base_y(GzCoord, GzCoord, GzCoord);
int getcolor(int, float, GzLight*, GzLight, GzColor, GzColor, GzColor, GzCoord, GzColor, int, GzColor);

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/

	if (degree == NULL || mat == NULL) return GZ_FAILURE;
	double radian = (degree / 180) * PI;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 1;

	mat[1][1] = cosf(radian);
	mat[1][2] = -sinf(radian);

	mat[2][1] = sinf(radian);
	mat[2][2] = cosf(radian);

	mat[3][3] = 1;
	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/

	if (degree == NULL || mat == NULL) return GZ_FAILURE;
	double radian = (degree / 180) * PI;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = cosf(radian);
	mat[0][2] = sinf(radian);

	mat[1][1] = 1;

	mat[2][0] = -sinf(radian);
	mat[2][2] = cosf(radian);

	mat[3][3] = 1;

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/

	if (degree == NULL || mat == NULL) return GZ_FAILURE;
	double radian = (degree / 180) * PI;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = cosf(radian);
	mat[0][1] = -sinf(radian);

	mat[1][0] = sinf(radian);
	mat[1][1] = cosf(radian);

	mat[2][2] = 1;

	mat[3][3] = 1;

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/

	if (translate == NULL || mat == NULL) return GZ_FAILURE;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 1;
	mat[1][1] = 1;
	mat[2][2] = 1;

	mat[0][3] = translate[0];
	mat[1][3] = translate[1];
	mat[2][3] = translate[2];
	mat[3][3] = 1;

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/

	if (scale == NULL || mat == NULL) return GZ_FAILURE;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = scale[0];
	mat[1][1] = scale[1];
	mat[2][2] = scale[2];
	mat[3][3] = 1;

	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	xres = xRes;
	yres = yRes;

	int resolution = 0;
	resolution = xres * yres;

	//framebuffer = (char*)malloc(3 * sizeof(char) * xRes * yRes);
	//pixelbuffer = new GzPixel[resolution];
	framebuffer = (char*)malloc(sizeof(GzPixel) * xres * yres);
	pixelbuffer = (GzPixel*)malloc(sizeof(GzPixel) * xres * yres);

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	matlevel = -1;
	numlights = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Xsp[i][j] = 0;
			m_camera.Xiw[i][j] = 0;
			m_camera.Xpi[i][j] = 0;
		}
	}

	Xsp[0][0] = (float)(xres / 2.0);
	Xsp[0][3] = (float)(xres / 2.0);
	Xsp[1][1] = -((float)yres / 2.0);
	Xsp[1][3] = (float)(yres / 2.0);
	Xsp[2][2] = (float)MAXINT;
	Xsp[3][3] = 1.0;

	m_camera.FOV = DEFAULT_FOV;

	m_camera.lookat[0] = 0;
	m_camera.lookat[1] = 0;
	m_camera.lookat[2] = 0;

	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;

	m_camera.worldup[0] = 0;
	m_camera.worldup[1] = 1;
	m_camera.worldup[2] = 0;

	//shade

	//interp_mode = GZ_RGB_COLOR;

	numlights = 0;

	GzColor ka = DEFAULT_AMBIENT;
	GzColor kd = DEFAULT_DIFFUSE;
	GzColor ks = DEFAULT_SPECULAR;

	Ka[RED] = ka[RED];
	Ka[GREEN] = ka[GREEN];
	Ka[BLUE] = ka[BLUE];

	Kd[RED] = kd[RED];
	Kd[GREEN] = kd[GREEN];
	Kd[BLUE] = kd[BLUE];

	Ks[RED] = ks[RED];
	Ks[GREEN] = ks[GREEN];
	Ks[BLUE] = ks[BLUE];

	spec = DEFAULT_SPEC;

}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;

}

int GzRender::GzDefault()
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */

	for (int i = 0; i < xres * yres; i++)
	{
		pixelbuffer[i] = { 3200, 3500, 3700, 1, MAXINT };

		framebuffer[3 * i] = (char)3200;
		framebuffer[3 * i + 1] = (char)3500;
		framebuffer[3 * i + 2] = (char)3700;
	}

	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			m_camera.Xpi[i][j] = 0;
	}

	float di = tanf((m_camera.FOV * PI / 180.0) / 2.0);
	m_camera.Xpi[0][0] = 1.0;
	m_camera.Xpi[1][1] = 1.0;
	m_camera.Xpi[2][2] = di;
	m_camera.Xpi[3][2] = di;
	m_camera.Xpi[3][3] = 1.0;

	GzCoord cl;
	for (int i = 0; i < 3; i++)
		cl[i] = m_camera.lookat[i] - m_camera.position[i];

	GzCoord newZ;
	float sq = sqrt(cl[0] * cl[0] + cl[1] * cl[1] + cl[2] * cl[2]);
	for (int i = 0; i < 3; i++)
		newZ[i] = cl[i] / sq;

	//dz = up * z
	float dZ = m_camera.worldup[0] * newZ[0] + m_camera.worldup[1] * newZ[1] + m_camera.worldup[2] * newZ[2];

	GzCoord newup;//up' = up - dz*z

	for (int i = 0; i < 3; i++)
		newup[i] = m_camera.worldup[i] - dZ * newZ[i];

	GzCoord newY;
	float sq2 = sqrt(newup[0] * newup[0] + newup[1] * newup[1] + newup[2] * newup[2]);
	for (int i = 0; i < 3; i++)
		newY[i] = newup[i] / sq2;


	GzCoord newX;
	newX[0] = newY[1] * newZ[2] - newY[2] * newZ[1];
	newX[1] = newY[2] * newZ[0] - newY[0] * newZ[2];
	newX[2] = newY[0] * newZ[1] - newY[1] * newZ[0];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			m_camera.Xiw[i][j] = 0;
	}

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 4; i++) {
			if (j == 0) {
				if (i == 3) {
					m_camera.Xiw[j][i] = -1.0f * (newX[0] * m_camera.position[0] + newX[1] * m_camera.position[1] + newX[2] * m_camera.position[2]);
				}
				else
					m_camera.Xiw[j][i] = newX[i];
			}
			else if (j == 1) {
				if (i == 3) {
					m_camera.Xiw[j][i] = -1.0f * (newY[0] * m_camera.position[0] + newY[1] * m_camera.position[1] + newY[2] * m_camera.position[2]);
				}
				else
					m_camera.Xiw[j][i] = newY[i];
			}
			else if (j == 2) {
				if (i == 3) {
					m_camera.Xiw[j][i] = -1.0f * (newZ[0] * m_camera.position[0] + newZ[1] * m_camera.position[1] + newZ[2] * m_camera.position[2]);
				}
				else
					m_camera.Xiw[j][i] = newZ[i];
			}
		}
	}

	m_camera.Xiw[3][3] = 1.0;

	GzPushMatrix(Xsp);
	GzPushMatrix(m_camera.Xpi);

	GzPushMatrix(m_camera.Xiw);

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_camera.Xiw[i][j] = camera.Xiw[i][j];
			m_camera.Xpi[i][j] = camera.Xpi[i][j];
		}
	}

	m_camera.position[X] = camera.position[X];
	m_camera.position[Y] = camera.position[Y];
	m_camera.position[Z] = camera.position[Z];
	m_camera.lookat[X] = camera.lookat[X];
	m_camera.lookat[Y] = camera.lookat[Y];
	m_camera.lookat[Z] = camera.lookat[Z];
	m_camera.worldup[X] = camera.worldup[X];
	m_camera.worldup[Y] = camera.worldup[Y];
	m_camera.worldup[Z] = camera.worldup[Z];
	m_camera.FOV = camera.FOV;

	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/

	if (matlevel >= MATLEVELS || matrix == NULL) return GZ_FAILURE;
	if (matlevel < MATLEVELS)
	{
		//identity_mat
		GzMatrix identity_mat;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					identity_mat[i][j] = 1;
				}
				else
					identity_mat[i][j] = 0;
			}
		}



		if (matlevel != -1)
		{
			//multiply
			float temp = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					temp = 0;
					for (int k = 0; k < 4; k++) {
						temp = temp + Ximage[matlevel][i][k] * matrix[k][j];
					}
					Ximage[matlevel + 1][i][j] = temp;
				}
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					Ximage[0][i][j] = matrix[i][j];
				}
			}
		}

		if (matlevel == -1 || matlevel == 0) {

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Xnorm[matlevel + 1][i][j] = (float)identity_mat[i][j];
				}
			}
		}
		else {
			GzMatrix uni_res;
			GzMatrix uni_res_xnorm;
			uni_res[0][0] = matrix[0][0];
			uni_res[0][1] = matrix[0][1];
			uni_res[0][2] = matrix[0][2];
			uni_res[0][3] = 0;
			uni_res[1][0] = matrix[1][0];
			uni_res[1][1] = matrix[1][1];
			uni_res[1][2] = matrix[1][2];
			uni_res[1][3] = 0;
			uni_res[2][0] = matrix[2][0];
			uni_res[2][1] = matrix[2][1];
			uni_res[2][2] = matrix[2][2];
			uni_res[2][3] = 0;
			uni_res[3][0] = matrix[3][0];
			uni_res[3][1] = matrix[3][1];
			uni_res[3][2] = matrix[3][2];
			uni_res[3][3] = 1;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					uni_res_xnorm[i][j] = 0.0;
					for (int k = 0; k < 4; k++) {
						uni_res_xnorm[i][j] += Xnorm[matlevel][i][k] * uni_res[k][j];
					}
				}

			}

			float temp_k = 1.0 / sqrt((uni_res_xnorm[0][0] * uni_res_xnorm[0][0]) + (uni_res_xnorm[0][1] * uni_res_xnorm[0][1]) + (uni_res_xnorm[0][2] * uni_res_xnorm[0][2]));
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					uni_res_xnorm[i][j] *= temp_k;
				}
			}

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Xnorm[matlevel + 1][i][j] = (float)uni_res_xnorm[i][j];
				}
			}
		}

		matlevel++;

	}
	else
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/

	if (matlevel < 0)
		return GZ_FAILURE;
	matlevel--;
	return GZ_SUCCESS;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */

	int indexI, indexJ;
	GzIntensity red, green, blue, alpha;
	GzDepth depth;

	indexI = max(min(i, xres - 1), 0);
	indexJ = max(min(j, yres - 1), 0);

	red = max(min(r, 4095), 0);
	green = max(min(g, 4095), 0);
	blue = max(min(b, 4095), 0);

	alpha = a;
	depth = z;

	if (pixelbuffer == NULL) return GZ_FAILURE;

	if (z < pixelbuffer[ARRAY(indexI, indexJ)].z) {
		pixelbuffer[ARRAY(indexI, indexJ)].red = red;
		pixelbuffer[ARRAY(indexI, indexJ)].green = green;
		pixelbuffer[ARRAY(indexI, indexJ)].blue = blue;
		pixelbuffer[ARRAY(indexI, indexJ)].alpha = alpha;
		pixelbuffer[ARRAY(indexI, indexJ)].z = depth;
	}

	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */

	int indexI = max(min(i, xres - 1), 0);
	int indexJ = max(min(j, yres - 1), 0);

	if (pixelbuffer == NULL) return GZ_FAILURE;

	*r = pixelbuffer[ARRAY(indexI, indexJ)].red;
	*g = pixelbuffer[ARRAY(indexI, indexJ)].green;
	*b = pixelbuffer[ARRAY(indexI, indexJ)].blue;
	*a = pixelbuffer[ARRAY(indexI, indexJ)].alpha;
	*z = pixelbuffer[ARRAY(indexI, indexJ)].z;

	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */

	fprintf(outfile, "P6 %d %d 255\n", xres, yres);

	if (pixelbuffer == NULL) return GZ_FAILURE;

	for (int i = 0; i <= (xres * yres); i++) {

		GzPixel p = pixelbuffer[i];
		char pred, pgreen, pblue;
		size_t size = 1, count = 1;

		pred = p.red >> 4;
		pgreen = p.green >> 4;
		pblue = p.blue >> 4;

		fwrite(&pred, size, count, outfile);
		fwrite(&pgreen, size, count, outfile);
		fwrite(&pblue, size, count, outfile);
	}

	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
	/* HW1.7 write pixels to framebuffer:
		- put the pixels into the frame buffer
		- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red
		- NOT red, green, and blue !!!
	*/

	int index = 0;

	if (pixelbuffer == NULL || framebuffer == NULL) return GZ_FAILURE;

	for (int i = 0; i <= (xres * yres); i++) {

		GzPixel p = pixelbuffer[i];
		char fred, fgreen, fblue;

		fred = p.red >> 4;
		fgreen = p.green >> 4;
		fblue = p.blue >> 4;

		framebuffer[index++] = fblue;
		framebuffer[index++] = fgreen;
		framebuffer[index++] = fred;
	}

	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList)
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/

	/*
	- GzPutAttribute() must accept the following tokens/values:

	- GZ_RGB_COLOR					GzColor		default flat-shade color
	- GZ_INTERPOLATE				int			shader interpolation mode
	- GZ_DIRECTIONAL_LIGHT			GzLight
	- GZ_AMBIENT_LIGHT            	GzLight		(ignore direction)
	- GZ_AMBIENT_COEFFICIENT		GzColor		Ka reflectance
	- GZ_DIFFUSE_COEFFICIENT		GzColor		Kd reflectance
	- GZ_SPECULAR_COEFFICIENT       GzColor		Ks coef's
	- GZ_DISTRIBUTION_COEFFICIENT   float		spec power
	*/

	if (pixelbuffer == NULL || framebuffer == NULL || numAttributes == NULL || *nameList == NULL || *valueList == NULL) return GZ_FAILURE;

	int i = 0;
	//GzIntensity red, green, blue;

	while (i < numAttributes)
	{
		GzToken token = nameList[i];
		if (token == GZ_RGB_COLOR)
		{
			GzColor* c = (GzColor*)valueList[i];
			//float *c = (float*)*(valueList+i);
			flatcolor[0] = max(min(c[i][RED], 4095), 0);
			flatcolor[1] = max(min(c[i][GREEN], 4095), 0);
			flatcolor[2] = max(min(c[i][BLUE], 4095), 0);
		}
		else if (token == GZ_INTERPOLATE)
		{
			int *m = (int*)valueList[i];
			interp_mode = *m;
		}
		else if (token == GZ_DIRECTIONAL_LIGHT)
		{
			GzLight *dir = (GzLight*)valueList[i];
			lights[numlights].direction[0] = dir->direction[0];
			lights[numlights].direction[1] = dir->direction[1];
			lights[numlights].direction[2] = dir->direction[2];
			lights[numlights].color[0] = dir->color[0]; //le
			lights[numlights].color[1] = dir->color[1];
			lights[numlights].color[2] = dir->color[2];
			numlights++;
		}
		else if (token == GZ_AMBIENT_LIGHT)
		{
			GzLight *amb = (GzLight*)valueList[i];
			ambientlight.direction[0] = amb->direction[0];
			ambientlight.direction[1] = amb->direction[1];
			ambientlight.direction[2] = amb->direction[2];
			ambientlight.color[0] = amb->color[0]; //la
			ambientlight.color[1] = amb->color[1];
			ambientlight.color[2] = amb->color[2];
		}
		else if (token == GZ_AMBIENT_COEFFICIENT)
		{
			float *ac = (float*)valueList[i];
			Ka[0] = ac[0];
			Ka[1] = ac[1];
			Ka[2] = ac[2];
		}
		else if (token == GZ_DIFFUSE_COEFFICIENT)
		{
			float *dc = (float*)valueList[i];
			Kd[0] = dc[0];
			Kd[1] = dc[1];
			Kd[2] = dc[2];
		}
		else if (token == GZ_SPECULAR_COEFFICIENT)
		{
			float *sc = (float*)valueList[i];
			Ks[0] = sc[0];
			Ks[1] = sc[1];
			Ks[2] = sc[2];
		}
		else if (token == GZ_DISTRIBUTION_COEFFICIENT)
		{
			float *distrc = (float*)valueList[i];
			spec = *distrc;
		}
		else if (token == GZ_TEXTURE_MAP)
		{
			GzTexture uv = (GzTexture)valueList[i];
			tex_fun = uv;
		}
		else if (token == GZ_AASHIFTX)
		{
			float* offsetx = (float*)valueList[i];
			Xoffset = *offsetx;
		}
		else if (token == GZ_AASHIFTY)
		{
			float* offsety = (float*)valueList[i];
			Yoffset = *offsety;
		}
		i++;
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/
	/*
	-- Xform positions of verts using matrix on top of stack
	-- Clip - just discard any triangle with any vert(s) behind view plane
			- optional: test for triangles with all three verts off-screen (trivial frustum cull)
	-- invoke triangle rasterizer
	*/
	for (int i = 0; i < numParts; i++)
	{
		if (pixelbuffer == NULL || framebuffer == NULL) return GZ_FAILURE;
		GzCoord* vP = (GzCoord*)valueList[0];	//position
		GzCoord* nP = (GzCoord*)valueList[1];	//norm
		GzTextureIndex* uvP = (GzTextureIndex*)valueList[2];

		float v4d[3][4];
		float n4d[3][4];
		//GzCoord v4d[4];
		v4d[0][0] = vP[0][X];
		v4d[0][1] = vP[0][Y];
		v4d[0][2] = vP[0][Z];
		v4d[1][0] = vP[1][X];
		v4d[1][1] = vP[1][Y];
		v4d[1][2] = vP[1][Z];
		v4d[2][0] = vP[2][X];
		v4d[2][1] = vP[2][Y];
		v4d[2][2] = vP[2][Z];
		v4d[0][3] = 1.0f;
		v4d[1][3] = 1.0f;
		v4d[2][3] = 1.0f;

		n4d[0][0] = nP[0][X];
		n4d[0][1] = nP[0][Y];
		n4d[0][2] = nP[0][Z];
		n4d[1][0] = nP[1][X];
		n4d[1][1] = nP[1][Y];
		n4d[1][2] = nP[1][Z];
		n4d[2][0] = nP[2][X];
		n4d[2][1] = nP[2][Y];
		n4d[2][2] = nP[2][Z];
		n4d[0][3] = 1.0f;
		n4d[1][3] = 1.0f;
		n4d[2][3] = 1.0f;

		float t4d[3][4];
		float nt4d[3][4];

		float s = 0;
		float a = 0;

		//matrix multiply
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 4; k++) {
					s = s + Ximage[matlevel][i][k] * v4d[j][k];
					a = a + Xnorm[matlevel][i][k] * n4d[j][k];
				}
				t4d[j][i] = s;
				nt4d[j][i] = a;
				s = 0;
				a = 0;
			}
		}

		//if (t4d[0][2] < m_camera.position[2] || t4d[1][2] < m_camera.position[2] || t4d[2][2] < m_camera.position[2]) return GZ_FAILURE;

		/*
		// V1:
			vP[0][0], vP[0][1], vP[0][2];
		// V2:
			vP[1][0], vP[1][1], vP[1][2];
		// V3:
			vP[2][0], vP[2][1], vP[2][2];
		*/

		GzCoord v1 = { t4d[0][0] / t4d[0][3] - Xoffset, t4d[0][1] / t4d[0][3] - Yoffset, t4d[0][2] / t4d[0][3] }; //x, y, z
		GzCoord v2 = { t4d[1][0] / t4d[1][3] - Xoffset, t4d[1][1] / t4d[1][3] - Yoffset, t4d[1][2] / t4d[1][3] };
		GzCoord v3 = { t4d[2][0] / t4d[2][3] - Xoffset, t4d[2][1] / t4d[2][3] - Yoffset, t4d[2][2] / t4d[2][3] };

		GzCoord n1 = { nt4d[0][0] / nt4d[0][3], nt4d[0][1] / nt4d[0][3], nt4d[0][2] / nt4d[0][3] }; //x, y, z
		GzCoord n2 = { nt4d[1][0] / nt4d[1][3], nt4d[1][1] / nt4d[1][3], nt4d[1][2] / nt4d[1][3] };
		GzCoord n3 = { nt4d[2][0] / nt4d[2][3], nt4d[2][1] / nt4d[2][3], nt4d[2][2] / nt4d[2][3] };

		GzCoord n1_bak = { n1[0] / 1 , n1[1] / 1 , n1[2] / 1 };
		GzCoord n2_bak = { n2[0] / 1 , n2[1] / 1 , n2[2] / 1 };
		GzCoord n3_bak = { n3[0] / 1 , n3[1] / 1 , n3[2] / 1 };

		GzColor color_v1 = { 0, 0, 0 };
		GzColor color_v2 = { 0, 0, 0 };
		GzColor color_v3 = { 0, 0, 0 };

		GzTextureIndex uv1 = { uvP[0][0], uvP[0][1] };
		GzTextureIndex uv2 = { uvP[1][0], uvP[1][1] };
		GzTextureIndex uv3 = { uvP[2][0], uvP[2][1] };

		if (tex_fun != NULL && interp_mode == GZ_COLOR)
		{
			for (int k = 0; k < 3; k++) {
				Ka[k] = 1.0;
				Ks[k] = 1.0;
				Kd[k] = 1.0;
			}
		}

		getcolor(numlights, spec, lights, ambientlight, Ka, Kd, Ks, n1, color_v1, 0, NULL);
		getcolor(numlights, spec, lights, ambientlight, Ka, Kd, Ks, n2, color_v2, 0, NULL);
		getcolor(numlights, spec, lights, ambientlight, Ka, Kd, Ks, n3, color_v3, 0, NULL);

		//if ((v1[X] < 0 || v1[X] > xres) 
		//	&& (v1[Y] < 0 || v1[Y] > xres) 
		//	&& (v1[Z] < 0 || v1[Z] > xres)
		//	&& (v2[X] < 0 || v2[X] > xres)
		//	&& (v2[Y] < 0 || v2[Y] > xres)
		//	&& (v2[Z] < 0 || v2[Z] > xres)) {
		//	return GZ_FAILURE;
		//}

		//sort_base_y(v1, v2, v3);
		if (v1[1] > v2[1]) {
			for (int i = 0; i < 3; i++) {
				swap(v1[i], v2[i]); //xyz
				swap(n1[i], n2[i]); //xyz vector
				if (interp_mode != GZ_FLAT)
					swap(color_v1[i], color_v2[i]); //rgb
			}
			swap(uv1[0], uv2[0]);
			swap(uv1[1], uv2[1]);
		}

		if (v2[1] > v3[1]) {
			for (int i = 0; i < 3; i++) {
				swap(v2[i], v3[i]);
				swap(n2[i], n3[i]);
				if (interp_mode != GZ_FLAT)
					swap(color_v2[i], color_v3[i]);
			}
			swap(uv3[0], uv2[0]);
			swap(uv3[1], uv2[1]);
		}

		if (v1[1] > v2[1]) {
			for (int i = 0; i < 3; i++) {
				swap(v1[i], v2[i]);
				swap(n1[i], n2[i]);
				if (interp_mode != GZ_FLAT)
					swap(color_v1[i], color_v2[i]);
			}
			swap(uv1[0], uv2[0]);
			swap(uv1[1], uv2[1]);
		}
		//sort_base_y end

		int flag = -1;

		//divide special case
		if (v1[Y] == v2[Y]) //flag invert tri
		{
			if (v2[X] > v1[X]) {
				for (int i = 0; i < 3; i++) {
					swap(v1[i], v2[i]);
					swap(n1[i], n2[i]);
					if (interp_mode != GZ_FLAT)
						swap(color_v1[i], color_v2[i]);
				}
				swap(uv1[0], uv2[0]);
				swap(uv1[1], uv2[1]);
			}

			flag = 0; //invert tri
		}
		else if (v2[Y] == v3[Y]) //flag invert tri
		{
			if (v2[X] > v3[X]) {
				//v1 always on the upper left
				for (int i = 0; i < 3; i++) {
					swap(v3[i], v2[i]);
					swap(n3[i], n2[i]);
					if (interp_mode != GZ_FLAT)
						swap(color_v2[i], color_v3[i]);
				}
				swap(uv3[0], uv2[0]);
				swap(uv3[1], uv2[1]);
			}

			flag = 1; //tri 
		}
		else {

			flag = 2;
		}

		float uvVz;
		GzTextureIndex Puv[3];

		Puv[0][0] = uv1[0] / (v1[2] / (MAXINT - v1[2]) + 1.0);
		Puv[0][1] = uv1[1] / (v1[2] / (MAXINT - v1[2]) + 1.0);

		Puv[1][0] = uv2[0] / (v2[2] / (MAXINT - v2[2]) + 1.0);
		Puv[1][1] = uv2[1] / (v2[2] / (MAXINT - v2[2]) + 1.0);

		Puv[2][0] = uv3[0] / (v3[2] / (MAXINT - v3[2]) + 1.0);
		Puv[2][1] = uv3[1] / (v3[2] / (MAXINT - v3[2]) + 1.0);

		// Get the plane to interpolate u
		float uX_1 = v2[0] - v1[0], uY_1 = v2[1] - v1[1], uZ_1 = Puv[1][0] - Puv[0][0];
		float uX_2 = v3[0] - v1[0], uY_2 = v3[1] - v1[1], uZ_2 = Puv[2][0] - Puv[0][0];
		float uPanelA = (uY_1 * uZ_2 - uZ_1 * uY_2);
		float uPanelB = (uZ_1 * uX_2 - uX_1 * uZ_2);
		float uPanelC = (uX_1 * uY_2 - uY_1 * uX_2);
		float uPanelD = -1.0f * (uPanelA * v1[0] + uPanelB * v1[1] + uPanelC * Puv[0][0]);
		// Get the plane to interpolate v
		float vX_1 = v2[0] - v1[0], vY_1 = v2[1] - v1[1], vZ_1 = Puv[1][1] - Puv[0][1];
		float vX_2 = v3[0] - v1[0], vY_2 = v3[1] - v1[1], vZ_2 = Puv[2][1] - Puv[0][1];
		float vPanelA = (vY_1 * vZ_2 - vZ_1 * vY_2);
		float vPanelB = (vZ_1 * vX_2 - vX_1 * vZ_2);
		float vPanelC = (vX_1 * vY_2 - vY_1 * vX_2);
		float vPanelD = -1.0f * (vPanelA * v1[0] + vPanelB * v1[1] + vPanelC * Puv[0][1]);


		//render based on case
		if (flag == 2) {
			float s12x = (v2[X] - v1[X]) / (v2[Y] - v1[Y]);
			float s12z = (v2[Z] - v1[Z]) / (v2[Y] - v1[Y]);

			float s13x = (v3[X] - v1[X]) / (v3[Y] - v1[Y]);
			float s13z = (v3[Z] - v1[Z]) / (v3[Y] - v1[Y]);

			float s23x = (v3[X] - v2[X]) / (v3[Y] - v2[Y]);
			float s23z = (v3[Z] - v2[Z]) / (v3[Y] - v2[Y]);

			float deltaY = 0;

			float s12c_r, s13c_r, s23c_r, s12c_g, s13c_g, s23c_g, s12c_b, s13c_b, s23c_b;
			float s12n_x, s13n_x, s23n_x, s12n_y, s13n_y, s23n_y, s12n_z, s13n_z, s23n_z;

			if (interp_mode == GZ_COLOR) {
				s12c_r = (color_v2[RED] - color_v1[RED]) / (v2[Y] - v1[Y]);
				s13c_r = (color_v3[RED] - color_v1[RED]) / (v3[Y] - v1[Y]);
				s23c_r = (color_v3[RED] - color_v2[RED]) / (v3[Y] - v2[Y]);

				s12c_g = (color_v2[GREEN] - color_v1[GREEN]) / (v2[Y] - v1[Y]);
				s13c_g = (color_v3[GREEN] - color_v1[GREEN]) / (v3[Y] - v1[Y]);
				s23c_g = (color_v3[GREEN] - color_v2[GREEN]) / (v3[Y] - v2[Y]);

				s12c_b = (color_v2[BLUE] - color_v1[BLUE]) / (v2[Y] - v1[Y]);
				s13c_b = (color_v3[BLUE] - color_v1[BLUE]) / (v3[Y] - v1[Y]);
				s23c_b = (color_v3[BLUE] - color_v2[BLUE]) / (v3[Y] - v2[Y]);
			}
			else if (interp_mode == GZ_NORMALS) {
				s12n_x = (n2[X] - n1[X]) / (v2[Y] - v1[Y]);
				s13n_x = (n3[X] - n1[X]) / (v3[Y] - v1[Y]);
				s23n_x = (n3[X] - n2[X]) / (v3[Y] - v2[Y]);

				s12n_y = (n2[Y] - n1[Y]) / (v2[Y] - v1[Y]);
				s13n_y = (n3[Y] - n1[Y]) / (v3[Y] - v1[Y]);
				s23n_y = (n3[Y] - n2[Y]) / (v3[Y] - v2[Y]);

				s12n_z = (n2[Z] - n1[Z]) / (v2[Y] - v1[Y]);
				s13n_z = (n3[Z] - n1[Z]) / (v3[Y] - v1[Y]);
				s23n_z = (n3[Z] - n2[Z]) / (v3[Y] - v2[Y]);
			}


			for (int j = ceil(v1[Y]); j <= v3[Y]; j++) { //y

				deltaY = j - v1[Y];

				if (deltaY <= v2[Y] - v1[Y]) {
					float x1 = v1[X] + s12x * deltaY;
					float x2 = v1[X] + s13x * deltaY;
					float zValue1 = v1[Z] + s12z * deltaY;
					float zValue2 = v1[Z] + s13z * deltaY;

					float c_rValue1, c_rValue2, c_gValue1, c_gValue2, c_bValue1, c_bValue2;
					float n_xValue1, n_xValue2, n_yValue1, n_yValue2, n_zValue1, n_zValue2;

					if (interp_mode == GZ_COLOR) {
						c_rValue1 = color_v1[RED] + s12c_r * deltaY;
						c_rValue2 = color_v1[RED] + s13c_r * deltaY;

						c_gValue1 = color_v1[GREEN] + s12c_g * deltaY;
						c_gValue2 = color_v1[GREEN] + s13c_g * deltaY;

						c_bValue1 = color_v1[BLUE] + s12c_b * deltaY;
						c_bValue2 = color_v1[BLUE] + s13c_b * deltaY;
					}
					else if (interp_mode == GZ_NORMALS) {
						n_xValue1 = n1[X] + s12n_x * deltaY;
						n_xValue2 = n1[X] + s13n_x * deltaY;

						n_yValue1 = n1[Y] + s12n_y * deltaY;
						n_yValue2 = n1[Y] + s13n_y * deltaY;

						n_zValue1 = n1[Z] + s12n_z * deltaY;
						n_zValue2 = n1[Z] + s13n_z * deltaY;
					}

					if (x1 > x2) {
						swap(x1, x2);
						swap(zValue1, zValue2);
						if (interp_mode == GZ_COLOR) {
							swap(c_rValue1, c_rValue2);
							swap(c_gValue1, c_gValue2);
							swap(c_bValue1, c_bValue2);
						}
						else if (interp_mode == GZ_NORMALS) {
							swap(n_xValue1, n_xValue2);
							swap(n_yValue1, n_yValue2);
							swap(n_zValue1, n_zValue2);
						}
					}

					//cal z slope in one y
					float slopeZX = (zValue2 - zValue1) / (x2 - x1);
					float slopec_rX, slopec_gX, slopec_bX;
					float slopen_xX, slopen_yX, slopen_zX;
					if (interp_mode == GZ_COLOR) {
						slopec_rX = (c_rValue2 - c_rValue1) / (x2 - x1);
						slopec_gX = (c_gValue2 - c_gValue1) / (x2 - x1);
						slopec_bX = (c_bValue2 - c_bValue1) / (x2 - x1);
					}
					else if (interp_mode == GZ_NORMALS) {
						slopen_xX = (n_xValue2 - n_xValue1) / (x2 - x1);
						slopen_yX = (n_yValue2 - n_yValue1) / (x2 - x1);
						slopen_zX = (n_zValue2 - n_zValue1) / (x2 - x1);
					}

					for (int i = ceil(x1); i <= x2; i++) { //x		

						GzIntensity r = 0, g = 0, b = 0, a = 0;
						GzDepth z = MAXINT;



						if (!(i >= xres || i < 0 || j >= yres || j < 0)) {
							GzGet(i, j, &r, &g, &b, &a, &z);
							float newZ = zValue1 + slopeZX * (i - x1);
							
							GzTextureIndex cUV;
							float texture[3];

							if (tex_fun != NULL) {
								float para_uv = (float)newZ / ((float)MAXINT - (float)newZ);
								cUV[0] = -(uPanelA*i + uPanelB * j + uPanelD) / uPanelC;
								cUV[1] = -(vPanelA*i + vPanelB * j + vPanelD) / vPanelC;

								cUV[0] *= (para_uv + 1.0);
								cUV[1] *= (para_uv + 1.0);
								tex_fun(cUV[0], cUV[1], texture);
							}
							


							if (newZ < z && newZ >= 0) {
								//if (newZ < z) {
								if (interp_mode == GZ_FLAT) {
									r = max(min((GzIntensity)ctoi(color_v1[0]), 4095), 0);
									g = max(min((GzIntensity)ctoi(color_v1[1]), 4095), 0);
									b = max(min((GzIntensity)ctoi(color_v1[2]), 4095), 0);
								}
								else if (interp_mode == GZ_COLOR) {
									float newR = c_rValue1 + slopec_rX * (i - x1);
									float newG = c_gValue1 + slopec_gX * (i - x1);
									float newB = c_bValue1 + slopec_bX * (i - x1);
								
									if (tex_fun != NULL) {
										newR *= texture[0];
										newG *= texture[1];
										newB *= texture[2];
									}

									r = max(min((GzIntensity)ctoi(newR), 4095), 0);
									g = max(min((GzIntensity)ctoi(newG), 4095), 0);
									b = max(min((GzIntensity)ctoi(newB), 4095), 0);
								}
								else if (interp_mode == GZ_NORMALS) {
									float newX = n_xValue1 + slopen_xX * (i - x1);
									float newY = n_yValue1 + slopen_yX * (i - x1);
									float newZ = n_zValue1 + slopen_zX * (i - x1);
									GzCoord n_temp = { newX, newY, newZ };
									GzColor color_temp = { 0, 0, 0 };

									getcolor(numlights, spec, lights, ambientlight, Ka, Kd, Ks, n_temp, color_temp, (tex_fun != NULL) ? 1 : 0, texture);

									r = max(min((GzIntensity)ctoi(color_temp[RED]), 4095), 0);
									g = max(min((GzIntensity)ctoi(color_temp[GREEN]), 4095), 0);
									b = max(min((GzIntensity)ctoi(color_temp[BLUE]), 4095), 0);
								}

								GzPut(i, j, r, g, b, 1, newZ);
							}

						}
					}
				}
				else {
					float x1 = v2[X] + s23x * (deltaY - (v2[Y] - v1[Y]));
					float x2 = v1[X] + s13x * deltaY;
					float zValue1 = v2[Z] + s23z * (deltaY - (v2[Y] - v1[Y]));
					float zValue2 = v1[Z] + s13z * deltaY;

					float c_rValue1, c_rValue2, c_gValue1, c_gValue2, c_bValue1, c_bValue2;
					float n_xValue1, n_xValue2, n_yValue1, n_yValue2, n_zValue1, n_zValue2;

					if (interp_mode == GZ_COLOR) {
						c_rValue1 = color_v2[RED] + s23c_r * (deltaY - (v2[Y] - v1[Y]));
						c_rValue2 = color_v1[RED] + s13c_r * deltaY;

						c_gValue1 = color_v2[GREEN] + s23c_g * (deltaY - (v2[Y] - v1[Y]));
						c_gValue2 = color_v1[GREEN] + s13c_g * deltaY;

						c_bValue1 = color_v2[BLUE] + s23c_b * (deltaY - (v2[Y] - v1[Y]));
						c_bValue2 = color_v1[BLUE] + s13c_b * deltaY;
					}
					else if (interp_mode == GZ_NORMALS) {
						n_xValue1 = n2[X] + s23n_x * (deltaY - (v2[Y] - v1[Y]));
						n_xValue2 = n1[X] + s13n_x * deltaY;

						n_yValue1 = n2[Y] + s23n_y * (deltaY - (v2[Y] - v1[Y]));
						n_yValue2 = n1[Y] + s13n_y * deltaY;

						n_zValue1 = n2[Z] + s23n_z * (deltaY - (v2[Y] - v1[Y]));
						n_zValue2 = n1[Z] + s13n_z * deltaY;
					}

					if (x1 > x2) {
						swap(x1, x2);
						swap(zValue1, zValue2);
						if (interp_mode == GZ_COLOR) {
							swap(c_rValue1, c_rValue2);
							swap(c_gValue1, c_gValue2);
							swap(c_bValue1, c_bValue2);
						}
						else if (interp_mode == GZ_NORMALS) {
							swap(n_xValue1, n_xValue2);
							swap(n_yValue1, n_yValue2);
							swap(n_zValue1, n_zValue2);
						}
					}

					//cal z slope in one y
					float slopeZX = (zValue2 - zValue1) / (x2 - x1);
					float slopec_rX, slopec_gX, slopec_bX;
					float slopen_xX, slopen_yX, slopen_zX;

					if (interp_mode == GZ_COLOR) {
						slopec_rX = (c_rValue2 - c_rValue1) / (x2 - x1);
						slopec_gX = (c_gValue2 - c_gValue1) / (x2 - x1);
						slopec_bX = (c_bValue2 - c_bValue1) / (x2 - x1);
					}
					else if (interp_mode == GZ_NORMALS) {
						slopen_xX = (n_xValue2 - n_xValue1) / (x2 - x1);
						slopen_yX = (n_yValue2 - n_yValue1) / (x2 - x1);
						slopen_zX = (n_zValue2 - n_zValue1) / (x2 - x1);
					}

					for (int i = ceil(x1); i <= x2; i++) { //x		

						GzIntensity r = 0, g = 0, b = 0, a = 0;
						GzDepth z = MAXINT;

						if (!(i >= xres || i < 0 || j >= yres || j < 0)) {
							GzGet(i, j, &r, &g, &b, &a, &z);
							float newZ = zValue1 + slopeZX * (i - x1);

							GzTextureIndex cUV;
							float texture[3];

							if (tex_fun != NULL) {
								float para_uv = (float)newZ / ((float)MAXINT - (float)newZ);
								cUV[0] = -(uPanelA*i + uPanelB * j + uPanelD) / uPanelC;
								cUV[1] = -(vPanelA*i + vPanelB * j + vPanelD) / vPanelC;

								cUV[0] *= (para_uv + 1.0);
								cUV[1] *= (para_uv + 1.0);
								tex_fun(cUV[0], cUV[1], texture);
							}


							if (newZ < z && newZ >= 0) {
								//if (newZ < z) {
								if (interp_mode == GZ_FLAT) {
									r = max(min((GzIntensity)ctoi(color_v1[0]), 4095), 0);
									g = max(min((GzIntensity)ctoi(color_v1[1]), 4095), 0);
									b = max(min((GzIntensity)ctoi(color_v1[2]), 4095), 0);
								}
								else if (interp_mode == GZ_COLOR) {
									float newR = c_rValue1 + slopec_rX * (i - x1);
									float newG = c_gValue1 + slopec_gX * (i - x1);
									float newB = c_bValue1 + slopec_bX * (i - x1);

									if (tex_fun != NULL) {
										newR *= texture[0];
										newG *= texture[1];
										newB *= texture[2];
									}

									r = max(min((GzIntensity)ctoi(newR), 4095), 0);
									g = max(min((GzIntensity)ctoi(newG), 4095), 0);
									b = max(min((GzIntensity)ctoi(newB), 4095), 0);
								}
								else if (interp_mode == GZ_NORMALS) {
									float newX = n_xValue1 + slopen_xX * (i - x1);
									float newY = n_yValue1 + slopen_yX * (i - x1);
									float newZ = n_zValue1 + slopen_zX * (i - x1);
									GzCoord n_temp = { newX, newY, newZ };
									GzColor color_temp = { 0, 0, 0 };

									getcolor(numlights, spec, lights, ambientlight, Ka, Kd, Ks, n_temp, color_temp, (tex_fun != NULL) ? 1 : 0, texture);
									
									r = max(min((GzIntensity)ctoi(color_temp[RED]), 4095), 0);
									g = max(min((GzIntensity)ctoi(color_temp[GREEN]), 4095), 0);
									b = max(min((GzIntensity)ctoi(color_temp[BLUE]), 4095), 0);
								}
								GzPut(i, j, r, g, b, 1, newZ);
							}

						}
					}
				}
				//
			}
		}

		if (flag == 1) {	//line V1; line v2, v3
			float s12x = (v2[X] - v1[X]) / (v2[Y] - v1[Y]);
			float s12z = (v2[Z] - v1[Z]) / (v2[Y] - v1[Y]);
			float s13x = (v3[X] - v1[X]) / (v3[Y] - v1[Y]);
			float s13z = (v3[Z] - v1[Z]) / (v3[Y] - v1[Y]);
			float deltaY = 0;

			float s12c_r, s13c_r, s12c_g, s13c_g, s12c_b, s13c_b;
			float s12n_x, s13n_x, s12n_y, s13n_y, s12n_z, s13n_z;


			if (interp_mode == GZ_COLOR) {
				s12c_r = (color_v2[RED] - color_v1[RED]) / (v2[Y] - v1[Y]);
				s13c_r = (color_v3[RED] - color_v1[RED]) / (v3[Y] - v1[Y]);

				s12c_g = (color_v2[GREEN] - color_v1[GREEN]) / (v2[Y] - v1[Y]);
				s13c_g = (color_v3[GREEN] - color_v1[GREEN]) / (v3[Y] - v1[Y]);

				s12c_b = (color_v2[BLUE] - color_v1[BLUE]) / (v2[Y] - v1[Y]);
				s13c_b = (color_v3[BLUE] - color_v1[BLUE]) / (v3[Y] - v1[Y]);
			}
			else if (interp_mode == GZ_NORMALS) {
				s12n_x = (n2[X] - n1[X]) / (v2[Y] - v1[Y]);
				s13n_x = (n3[X] - n1[X]) / (v3[Y] - v1[Y]);

				s12n_y = (n2[Y] - n1[Y]) / (v2[Y] - v1[Y]);
				s13n_y = (n3[Y] - n1[Y]) / (v3[Y] - v1[Y]);

				s12n_z = (n2[Z] - n1[Z]) / (v2[Y] - v1[Y]);
				s13n_z = (n3[Z] - n1[Z]) / (v3[Y] - v1[Y]);
			}

			for (int j = ceil(v1[Y]); j <= v3[Y]; j++) { //y

				deltaY = j - v1[Y];

				float x1 = v1[X] + s12x * deltaY;
				float x2 = v1[X] + s13x * deltaY;
				float zValue1 = v1[Z] + s12z * deltaY;
				float zValue2 = v1[Z] + s13z * deltaY;

				float c_rValue1, c_rValue2, c_gValue1, c_gValue2, c_bValue1, c_bValue2;
				float n_xValue1, n_xValue2, n_yValue1, n_yValue2, n_zValue1, n_zValue2;

				if (interp_mode == GZ_COLOR) {
					c_rValue1 = color_v1[RED] + s12c_r * deltaY;
					c_rValue2 = color_v1[RED] + s13c_r * deltaY;

					c_gValue1 = color_v1[GREEN] + s12c_g * deltaY;
					c_gValue2 = color_v1[GREEN] + s13c_g * deltaY;

					c_bValue1 = color_v1[BLUE] + s12c_b * deltaY;
					c_bValue2 = color_v1[BLUE] + s13c_b * deltaY;
				}
				else if (interp_mode == GZ_NORMALS) {
					n_xValue1 = n1[X] + s12n_x * deltaY;
					n_xValue2 = n1[X] + s13n_x * deltaY;

					n_yValue1 = n1[Y] + s12n_y * deltaY;
					n_yValue2 = n1[Y] + s13n_y * deltaY;

					n_zValue1 = n1[Z] + s12n_z * deltaY;
					n_zValue2 = n1[Z] + s13n_z * deltaY;
				}

				if (x1 > x2) {
					swap(x1, x2);
					swap(zValue1, zValue2);

					if (interp_mode == GZ_COLOR) {
						swap(c_rValue1, c_rValue2);
						swap(c_gValue1, c_gValue2);
						swap(c_bValue1, c_bValue2);
					}
					else if (interp_mode == GZ_NORMALS) {
						swap(n_xValue1, n_xValue2);
						swap(n_yValue1, n_yValue2);
						swap(n_zValue1, n_zValue2);
					}
				}

				//cal z slope in one y
				float slopeZX = (zValue2 - zValue1) / (x2 - x1);
				float slopec_rX, slopec_gX, slopec_bX;
				float slopen_xX, slopen_yX, slopen_zX;

				if (interp_mode == GZ_COLOR) {
					slopec_rX = (c_rValue2 - c_rValue1) / (x2 - x1);
					slopec_gX = (c_gValue2 - c_gValue1) / (x2 - x1);
					slopec_bX = (c_bValue2 - c_bValue1) / (x2 - x1);
				}
				else if (interp_mode == GZ_NORMALS) {
					slopen_xX = (n_xValue2 - n_xValue1) / (x2 - x1);
					slopen_yX = (n_yValue2 - n_yValue1) / (x2 - x1);
					slopen_zX = (n_zValue2 - n_zValue1) / (x2 - x1);
				}

				for (int i = ceil(x1); i <= x2; i++) { //x		

					GzIntensity r = 0, g = 0, b = 0, a = 0;
					GzDepth z = MAXINT;

					if (!(i >= xres || i < 0 || j >= yres || j < 0)) {
						GzGet(i, j, &r, &g, &b, &a, &z);
						float newZ = zValue1 + slopeZX * (i - x1);

						GzTextureIndex cUV;
						float texture[3];

						if (tex_fun != NULL) {
							float para_uv = (float)newZ / ((float)MAXINT - (float)newZ);
							cUV[0] = -(uPanelA*i + uPanelB * j + uPanelD) / uPanelC;
							cUV[1] = -(vPanelA*i + vPanelB * j + vPanelD) / vPanelC;

							cUV[0] *= (para_uv + 1.0);
							cUV[1] *= (para_uv + 1.0);
							tex_fun(cUV[0], cUV[1], texture);
						}


						if (newZ < z && newZ >= 0) {
							//if (newZ < z) {
							if (interp_mode == GZ_FLAT) {
								r = max(min((GzIntensity)ctoi(color_v1[0]), 4095), 0);
								g = max(min((GzIntensity)ctoi(color_v1[1]), 4095), 0);
								b = max(min((GzIntensity)ctoi(color_v1[2]), 4095), 0);
							}
							else if (interp_mode == GZ_COLOR) {
								float newR = c_rValue1 + slopec_rX * (i - x1);
								float newG = c_gValue1 + slopec_gX * (i - x1);
								float newB = c_bValue1 + slopec_bX * (i - x1);

								if (tex_fun != NULL) {
									newR *= texture[0];
									newG *= texture[1];
									newB *= texture[2];
								}

								r = max(min((GzIntensity)ctoi(newR), 4095), 0);
								g = max(min((GzIntensity)ctoi(newG), 4095), 0);
								b = max(min((GzIntensity)ctoi(newB), 4095), 0);
							}
							else if (interp_mode == GZ_NORMALS) {
								float newX = n_xValue1 + slopen_xX * (i - x1);
								float newY = n_yValue1 + slopen_yX * (i - x1);
								float newZ = n_zValue1 + slopen_zX * (i - x1);
								GzCoord n_temp = { newX, newY, newZ };
								GzColor color_temp = { 0, 0, 0 };
								
								getcolor(numlights, spec, lights, ambientlight, Ka, Kd, Ks, n_temp, color_temp, (tex_fun != NULL)?1:0, texture);
								
								r = max(min((GzIntensity)ctoi(color_temp[RED]), 4095), 0);
								g = max(min((GzIntensity)ctoi(color_temp[GREEN]), 4095), 0);
								b = max(min((GzIntensity)ctoi(color_temp[BLUE]), 4095), 0);
							}
							GzPut(i, j, r, g, b, 1, newZ);
						}

					}
				}
			}
		}

		if (flag == 0) { //line: V2, V1; line V3
			float s13x = (v3[X] - v1[X]) / (v3[Y] - v1[Y]);
			float s13z = (v3[Z] - v1[Z]) / (v3[Y] - v1[Y]);
			float s23x = (v3[X] - v2[X]) / (v3[Y] - v2[Y]);
			float s23z = (v3[Z] - v2[Z]) / (v3[Y] - v2[Y]);
			float deltaY = 0;

			float s13c_r, s23c_r, s13c_g, s23c_g, s13c_b, s23c_b;
			float s13n_x, s23n_x, s13n_y, s23n_y, s13n_z, s23n_z;


			if (interp_mode == GZ_COLOR) {
				s13c_r = (color_v3[RED] - color_v1[RED]) / (v3[Y] - v1[Y]);
				s23c_r = (color_v3[RED] - color_v2[RED]) / (v3[Y] - v2[Y]);

				s13c_g = (color_v3[GREEN] - color_v1[GREEN]) / (v3[Y] - v1[Y]);
				s23c_g = (color_v3[GREEN] - color_v2[GREEN]) / (v3[Y] - v2[Y]);

				s13c_b = (color_v3[BLUE] - color_v1[BLUE]) / (v3[Y] - v1[Y]);
				s23c_b = (color_v3[BLUE] - color_v2[BLUE]) / (v3[Y] - v2[Y]);
			}
			else if (interp_mode == GZ_NORMALS) {
				s13n_x = (n3[X] - n1[X]) / (v3[Y] - v1[Y]);
				s23n_x = (n3[X] - n2[X]) / (v3[Y] - v2[Y]);

				s13n_y = (n3[Y] - n1[Y]) / (v3[Y] - v1[Y]);
				s23n_y = (n3[Y] - n2[Y]) / (v3[Y] - v2[Y]);

				s13n_z = (n3[Z] - n1[Z]) / (v3[Y] - v1[Y]);
				s23n_z = (n3[Z] - n2[Z]) / (v3[Y] - v2[Y]);
			}

			for (int j = ceil(v1[Y]); j <= v3[Y]; j++) { //y

				deltaY = j - v1[Y];

				float x1 = v2[X] + s23x * deltaY;
				float x2 = v1[X] + s13x * deltaY;
				float zValue1 = v2[Z] + s23z * deltaY;
				float zValue2 = v1[Z] + s13z * deltaY;

				float c_rValue1, c_rValue2, c_gValue1, c_gValue2, c_bValue1, c_bValue2;
				float n_xValue1, n_xValue2, n_yValue1, n_yValue2, n_zValue1, n_zValue2;

				if (interp_mode == GZ_COLOR) {
					c_rValue1 = color_v2[RED] + s23c_r * deltaY;
					c_rValue2 = color_v1[RED] + s13c_r * deltaY;

					c_gValue1 = color_v2[GREEN] + s23c_g * deltaY;
					c_gValue2 = color_v1[GREEN] + s13c_g * deltaY;

					c_bValue1 = color_v2[BLUE] + s23c_b * deltaY;
					c_bValue2 = color_v1[BLUE] + s13c_b * deltaY;
				}
				else if (interp_mode == GZ_NORMALS) {
					n_xValue1 = n2[X] + s23n_x * deltaY;
					n_xValue2 = n1[X] + s13n_x * deltaY;

					n_yValue1 = n2[Y] + s23n_y * deltaY;
					n_yValue2 = n1[Y] + s13n_y * deltaY;

					n_zValue1 = n2[Z] + s23n_z * deltaY;
					n_zValue2 = n1[Z] + s13n_z * deltaY;
				}

				if (x1 > x2) {
					swap(x1, x2);
					swap(zValue1, zValue2);
					if (interp_mode == GZ_COLOR) {
						swap(c_rValue1, c_rValue2);
						swap(c_gValue1, c_gValue2);
						swap(c_bValue1, c_bValue2);
					}
					else if (interp_mode == GZ_NORMALS) {
						swap(n_xValue1, n_xValue2);
						swap(n_yValue1, n_yValue2);
						swap(n_zValue1, n_zValue2);
					}
				}

				//cal z slope in one y
				float slopeZX = (zValue2 - zValue1) / (x2 - x1);
				float slopec_rX, slopec_gX, slopec_bX;
				float slopen_xX, slopen_yX, slopen_zX;

				if (interp_mode == GZ_COLOR) {
					slopec_rX = (c_rValue2 - c_rValue1) / (x2 - x1);
					slopec_gX = (c_gValue2 - c_gValue1) / (x2 - x1);
					slopec_bX = (c_bValue2 - c_bValue1) / (x2 - x1);
				}
				else if (interp_mode == GZ_NORMALS) {
					slopen_xX = (n_xValue2 - n_xValue1) / (x2 - x1);
					slopen_yX = (n_yValue2 - n_yValue1) / (x2 - x1);
					slopen_zX = (n_zValue2 - n_zValue1) / (x2 - x1);
				}

				for (int i = ceil(x1); i <= x2; i++) { //x		

					GzIntensity r = 0, g = 0, b = 0, a = 0;
					GzDepth z = MAXINT;

					if (!(i >= xres || i < 0 || j >= yres || j < 0)) {
						GzGet(i, j, &r, &g, &b, &a, &z);
						float newZ = zValue1 + slopeZX * (i - x1);

						GzTextureIndex cUV;
						float texture[3];

						if (tex_fun != NULL) {
							float para_uv = (float)newZ / ((float)MAXINT - (float)newZ);
							cUV[0] = -(uPanelA*i + uPanelB * j + uPanelD) / uPanelC;
							cUV[1] = -(vPanelA*i + vPanelB * j + vPanelD) / vPanelC;

							cUV[0] *= (para_uv + 1.0);
							cUV[1] *= (para_uv + 1.0);
							tex_fun(cUV[0], cUV[1], texture);
						}


						if (newZ < z && newZ >= 0) {
							//if (newZ < z) {
							if (interp_mode == GZ_FLAT) {
								r = max(min((GzIntensity)ctoi(color_v1[0]), 4095), 0);
								g = max(min((GzIntensity)ctoi(color_v1[1]), 4095), 0);
								b = max(min((GzIntensity)ctoi(color_v1[2]), 4095), 0);
							}
							else if (interp_mode == GZ_COLOR) {
								float newR = c_rValue1 + slopec_rX * (i - x1);
								float newG = c_gValue1 + slopec_gX * (i - x1);
								float newB = c_bValue1 + slopec_bX * (i - x1);

								if (tex_fun != NULL) {
									newR *= texture[0];
									newG *= texture[1];
									newB *= texture[2];
								}

								r = max(min((GzIntensity)ctoi(newR), 4095), 0);
								g = max(min((GzIntensity)ctoi(newG), 4095), 0);
								b = max(min((GzIntensity)ctoi(newB), 4095), 0);
							}
							else if (interp_mode == GZ_NORMALS) {
								float newX = n_xValue1 + slopen_xX * (i - x1);
								float newY = n_yValue1 + slopen_yX * (i - x1);
								float newZ = n_zValue1 + slopen_zX * (i - x1);
								GzCoord n_temp = { newX, newY, newZ };
								GzColor color_temp = { 0, 0, 0 };
								getcolor(numlights, spec, lights, ambientlight, Ka, Kd, Ks, n_temp, color_temp, (tex_fun != NULL)?1:0, texture);
								r = max(min((GzIntensity)ctoi(color_temp[RED]), 4095), 0);
								g = max(min((GzIntensity)ctoi(color_temp[GREEN]), 4095), 0);
								b = max(min((GzIntensity)ctoi(color_temp[BLUE]), 4095), 0);
							}
							GzPut(i, j, r, g, b, 1, newZ);
						}

					}
				}
			}

		}
	}


	return GZ_SUCCESS;
}

void swap(float& a, float& b)
{
	float c(std::move(a));
	a = std::move(b);
	b = std::move(c);
}

void sort_base_y(GzCoord v1, GzCoord v2, GzCoord v3) {

	if (v1[1] > v2[1]) {
		for (int i = 0; i < 3; i++) {
			swap(v1[i], v2[i]);
		}
	}

	if (v2[1] > v3[1]) {
		for (int i = 0; i < 3; i++) {
			swap(v2[i], v3[i]);
		}
	}

	if (v1[1] > v2[1]) {
		for (int i = 0; i < 3; i++) {
			swap(v1[i], v2[i]);
		}
	}
}

int getcolor(int numlights, float spec, GzLight* lights, GzLight ambientlight, GzColor Ka, GzColor Kd, GzColor Ks, GzCoord normal, GzColor color, int flag, GzColor texture) {

	GzCoord E = { 0, 0, -1 }, R;
	float nde, ndl, rde;
	GzColor diffuse_intens, specular_intens;
	for (int i = 0; i < 3; i++) {
		diffuse_intens[i] = 0;
		specular_intens[i] = 0;
	}

	nde = normal[0] * E[0] + normal[1] * E[1] + normal[2] * E[2];

	GzCoord newN = { 0, 0, 0 };

	for (int i = 0; i < numlights; i++) {
		ndl = normal[0] * lights[i].direction[0] + normal[1] * lights[i].direction[1] + normal[2] * lights[i].direction[2];
		if (ndl > 0 && nde > 0) {

			for (int j = 0; j < 3; j++)
				R[j] = 2 * ndl * normal[j] - lights[i].direction[j];
		}

		else if (ndl < 0 && nde < 0) {

			for (int j = 0; j < 3; j++)
				newN[j] = -1 * normal[j];

			ndl = newN[0] * lights[i].direction[0] + newN[1] * (lights[i].direction[1]) + newN[2] * (lights[i].direction[2]);
			for (int j = 0; j < 3; j++)
				R[j] = 2 * (ndl)* newN[j] - lights[i].direction[j];
		}
		else
		{
			//skip
			continue;
		}

		float modR = sqrt((R[0] * R[0]) + (R[1] * R[1]) + (R[2] * R[2]));

		for (int j = 0; j < 3; j++)
			R[j] = R[j] / modR;

		rde = R[0] * E[0] + R[1] * E[1] + R[2] * E[2];

		if (rde < 0) {
			rde = 0;
		}
		else if (rde > 1) {
			rde = 1;
		}

		for (int j = 0; j < 3; j++) {
			diffuse_intens[j] += ndl * lights[i].color[j];
			specular_intens[j] += pow(rde, spec) * lights[i].color[j];
		}
	}



	//color = specular + diffuse + ambient;
	if (flag == 0) {
		for (int i = 0; i < 3; i++)
			color[i] = Ks[i] * specular_intens[i] + Kd[i] * diffuse_intens[i] + Ka[i] * ambientlight.color[i];
	}
	else {
		for (int i = 0; i < 3; i++)
			color[i] = Ks[i] * specular_intens[i] + texture[i] * diffuse_intens[i] + texture[i] * ambientlight.color[i];
	}
	return GZ_SUCCESS;
}

