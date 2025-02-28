// Application5.cpp: implementation of the Application5 class.
//
//////////////////////////////////////////////////////////////////////

/*
 * application test code for homework assignment #5
*/

#include "stdafx.h"
#include "CS580HW.h"
#include "Application5.h"
#include "Gz.h"
#include "rend.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define INFILE  "ppot.asc"
#define OUTFILE "output.ppm"


extern int tex_fun(float u, float v, GzColor color); /* image texture function */
extern int ptex_fun(float u, float v, GzColor color); /* procedural texture function */
extern int GzFreeTexture();

void shade(GzCoord norm, GzCoord color);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Application5::Application5()
{

}

Application5::~Application5()
{
	Clean();
}

int Application5::Initialize()
{
	GzCamera	camera;
	GzCamera    Light;
	int		    xRes, yRes;	/* display parameters */

	GzToken		nameListShader[9]; 	    /* shader attribute names */
	GzPointer   valueListShader[9];		/* shader attribute pointers */
	GzToken     nameListLights[10];		/* light info */
	GzPointer   valueListLights[10];
	int			shaderType, interpStyle;
	float		specpower;
	int		status;

	status = 0;

	/*
	 * Allocate memory for user input
	 */
	m_pUserInput = new GzInput;

	/*
	 * initialize the display and the renderer
	 */
	m_nWidth = 900;		// frame buffer and display width
	m_nHeight = 900;    // frame buffer and display height

	m_pRender = new GzRender(m_nWidth, m_nHeight);
	m_pRender->GzDefault();

	m_pFrameBuffer = m_pRender->framebuffer;

	/* Translation matrix */
	GzMatrix	scale =
	{
		3.25,	0.0,	0.0,	0.0,
		0.0,	3.25,	0.0,	-3.25,
		0.0,	0.0,	3.25,	3.5,
		0.0,	0.0,	0.0,	1.0
	};

	GzMatrix	rotateX =
	{
		1.0,	0.0,	0.0,	0.0,
		0.0,	.7071,	.7071,	0.0,
		0.0,	-.7071,	.7071,	0.0,
		0.0,	0.0,	0.0,	1.0
	};

	GzMatrix	rotateY =
	{
		.866,	0.0,	-0.5,	0.0,
		0.0,	1.0,	0.0,	0.0,
		0.5,	0.0,	.866,	0.0,
		0.0,	0.0,	0.0,	1.0
	};

	//Light.position[X] = -3;
	//Light.position[Y] = -25;
	//Light.position[Z] = 4;
	//
	//Light.worldup[X] = -0.2;
	//Light.worldup[Y] = 1.0;
	//Light.worldup[Z] = 0.0;
	//
	//Light.FOV = 63.7;
	//status |= m_pRender->GzPutLight(Light);

#if 1 	/* set up app-defined camera if desired, else use camera defaults */
	camera.position[X] = -3;
	camera.position[Y] = -25;
	camera.position[Z] = -4;

	camera.lookat[X] = 7.8;
	camera.lookat[Y] = 0.7;
	camera.lookat[Z] = 6.5;

	camera.worldup[X] = -0.2;
	camera.worldup[Y] = 1.0;
	camera.worldup[Z] = 0.0;

	camera.FOV = 63.7;              /* degrees *              /* degrees */

	Light.position[X] = -3;
	Light.position[Y] = -25;
	Light.position[Z] = -4;

	//Light.lookat[X] = 7.8;
	//Light.lookat[Y] = 0.7;
	//Light.lookat[Z] = 6.5;

	Light.worldup[X] = -0.2;
	Light.worldup[Y] = 1.0;
	Light.worldup[Z] = 0.0;

	Light.FOV = 63.7;                    /* degrees *              /* degrees */
	status |= m_pRender->GzPutCamera(camera);
	status |= m_pRender->GzPutLight(Light);
#else
	Light.position[X] = -3;
	Light.position[Y] = -25;
	Light.position[Z] = -4;

	//Light.lookat[X] = 7.8;
	//Light.lookat[Y] = 0.7;
	//Light.lookat[Z] = 6.5;

	Light.worldup[X] = -0.2;
	Light.worldup[Y] = 1.0;
	Light.worldup[Z] = 0.0;

	Light.FOV = 63.7;                    /* degrees *              /* degrees */
	status |= m_pRender->GzPutLight(Light);
#endif 



	/* Light */
	GzLight	light1 = { {0, 0, -1}, {0.5, 0.5, 0.9} }; //image space
	GzLight	ambientlight = { {0, 0, 0}, {0.3, 0.3, 0.3} };

	/* Material property */
	GzColor specularCoefficient = { 0.3, 0.3, 0.3 };
	GzColor ambientCoefficient = { 0.1, 0.1, 0.1 };
	GzColor diffuseCoefficient = { 0.7, 0.7, 0.7 };

	/*
	  renderer is ready for frame --- define lights and shader at start of frame
	*/

	/*
	 * Tokens associated with light parameters
	 */
	nameListLights[0] = GZ_DIRECTIONAL_LIGHT;
	valueListLights[0] = (GzPointer)&light1;

	status |= m_pRender->GzPutAttribute(1, nameListLights, valueListLights);

	nameListLights[0] = GZ_AMBIENT_LIGHT;
	valueListLights[0] = (GzPointer)&ambientlight;
	status |= m_pRender->GzPutAttribute(1, nameListLights, valueListLights);

	/*
	 * Tokens associated with shading
	 */
	nameListShader[0] = GZ_DIFFUSE_COEFFICIENT;
	valueListShader[0] = (GzPointer)diffuseCoefficient;

	/*
	* Select either GZ_COLOR or GZ_NORMALS as interpolation mode
	*/
	nameListShader[1] = GZ_INTERPOLATE;
	//interpStyle = GZ_COLOR;         /* Gouraud shading */
	interpStyle = GZ_NORMALS;         /* Phong shading */
	valueListShader[1] = (GzPointer)&interpStyle;

	nameListShader[2] = GZ_AMBIENT_COEFFICIENT;
	valueListShader[2] = (GzPointer)ambientCoefficient;
	nameListShader[3] = GZ_SPECULAR_COEFFICIENT;
	valueListShader[3] = (GzPointer)specularCoefficient;
	nameListShader[4] = GZ_DISTRIBUTION_COEFFICIENT;
	specpower = 32;
	valueListShader[4] = (GzPointer)&specpower;

	nameListShader[5] = GZ_TEXTURE_MAP;
#if 1   /* set up null texture function or valid pointer */
	valueListShader[5] = (GzPointer)0;
#else
	valueListShader[5] = (GzPointer)(tex_fun);	/* or use ptex_fun */
#endif
	status |= m_pRender->GzPutAttribute(6, nameListShader, valueListShader);

	/* Start Renderer */
	status |= m_pRender->GzBeginRender();
	status |= m_pRender->GzPushMatrix(scale);
	status |= m_pRender->GzPushMatrix(rotateY);
	status |= m_pRender->GzPushMatrix(rotateX);
	status |= m_pRender->GzPushLightMatrix(scale);
	status |= m_pRender->GzPushLightMatrix(rotateY);
	status |= m_pRender->GzPushLightMatrix(rotateX);

	if (status) exit(GZ_FAILURE);

	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}

int Application5::Render()
{
	GzToken		nameListTriangle[3]; 	/* vertex attribute names */

	GzPointer	valueListTriangle[3]; 	/* vertex attribute pointers */

	GzPointer	valueListGrassTriangle[3];

	GzCoord		vertexList[3];	/* vertex position coordinates */
	GzCoord		normalList[3];	/* vertex normals */
	GzTextureIndex uvList[3];		/* vertex texture map indices */

	GzCoord		vertexGrassList[3];
	GzCoord		normalGrassList[3];
	GzTextureIndex uvGrassList[3];

	char		dummy[256];
	int			status;


	/* Initialize Display */
	status |= m_pRender->GzDefault();  /* init for new frame */

	/*
	* Tokens associated with triangle vertex values
	*/
	nameListTriangle[0] = GZ_POSITION;
	nameListTriangle[1] = GZ_NORMAL;
	nameListTriangle[2] = GZ_TEXTURE_INDEX;

	// I/O File open
	FILE *infile;
	if ((infile = fopen(INFILE, "r")) == NULL)
	{
		AfxMessageBox("The input file was not opened\n");
		return GZ_FAILURE;
	}

	FILE *outfile;
	if ((outfile = fopen(OUTFILE, "wb")) == NULL)
	{
		AfxMessageBox("The output file was not opened\n");
		return GZ_FAILURE;
	}

	/*
	* Walk through the list of triangles, set color
	* and render each triangle
	*/
	while (fscanf(infile, "%s", dummy) == 1) { 	/* read in tri word */
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[0][0]), &(vertexList[0][1]),
			&(vertexList[0][2]),
			&(normalList[0][0]), &(normalList[0][1]),
			&(normalList[0][2]),
			&(uvList[0][0]), &(uvList[0][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[1][0]), &(vertexList[1][1]),
			&(vertexList[1][2]),
			&(normalList[1][0]), &(normalList[1][1]),
			&(normalList[1][2]),
			&(uvList[1][0]), &(uvList[1][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[2][0]), &(vertexList[2][1]),
			&(vertexList[2][2]),
			&(normalList[2][0]), &(normalList[2][1]),
			&(normalList[2][2]),
			&(uvList[2][0]), &(uvList[2][1]));

		/*
		 * Set the value pointers to the first vertex of the
		 * triangle, then feed it to the renderer
		 * NOTE: this sequence matches the nameList token sequence
		 */
		valueListTriangle[0] = (GzPointer)vertexList;
		valueListTriangle[1] = (GzPointer)normalList;
		valueListTriangle[2] = (GzPointer)uvList;

		//extraGrassList, same as input file format
		float **extraGrassList = (float **)malloc(3 * sizeof(float *));
		for (int i = 0; i < 3; i++)
			extraGrassList[i] = (float *)malloc(8 * sizeof(float));

		extraGrassList = m_pRender->GzAddGrassWithModelSpace(3, nameListTriangle, valueListTriangle);
		for (int i = 0; i < 5; i++) {
			vertexGrassList[0][0] = extraGrassList[0 + i * 3][0];
			vertexGrassList[0][1] = extraGrassList[0 + i * 3][1];
			vertexGrassList[0][2] = extraGrassList[0 + i * 3][2];
			vertexGrassList[1][0] = extraGrassList[1 + i * 3][0];
			vertexGrassList[1][1] = extraGrassList[1 + i * 3][1];
			vertexGrassList[1][2] = extraGrassList[1 + i * 3][2];
			vertexGrassList[2][0] = extraGrassList[2 + i * 3][0];
			vertexGrassList[2][1] = extraGrassList[2 + i * 3][1];
			vertexGrassList[2][2] = extraGrassList[2 + i * 3][2];

			normalGrassList[0][0] = extraGrassList[0 + i * 3][3];
			normalGrassList[0][1] = extraGrassList[0 + i * 3][4];
			normalGrassList[0][2] = extraGrassList[0 + i * 3][5];
			normalGrassList[1][0] = extraGrassList[1 + i * 3][3];
			normalGrassList[1][1] = extraGrassList[1 + i * 3][4];
			normalGrassList[1][2] = extraGrassList[1 + i * 3][5];
			normalGrassList[2][0] = extraGrassList[2 + i * 3][3];
			normalGrassList[2][1] = extraGrassList[2 + i * 3][4];
			normalGrassList[2][2] = extraGrassList[2 + i * 3][5];

			uvGrassList[0][0] = extraGrassList[0 + i * 3][6];
			uvGrassList[0][1] = extraGrassList[0 + i * 3][7];
			uvGrassList[1][0] = extraGrassList[1 + i * 3][6];
			uvGrassList[1][1] = extraGrassList[1 + i * 3][7];
			uvGrassList[2][0] = extraGrassList[2 + i * 3][6];
			uvGrassList[2][1] = extraGrassList[2 + i * 3][7];

			valueListGrassTriangle[0] = (GzPointer)vertexGrassList;
			valueListGrassTriangle[1] = (GzPointer)normalGrassList;
			valueListGrassTriangle[2] = (GzPointer)uvGrassList;

			m_pRender->GzCalShadowDepth(3, nameListTriangle, valueListGrassTriangle);
		}

		m_pRender->GzCalShadowDepth(3, nameListTriangle, valueListTriangle);

	}
	if (fclose(infile))
		AfxMessageBox(_T("The input file was not closed\n"));
	if ((infile = fopen(INFILE, "r")) == NULL)
	{
		AfxMessageBox("The input file was not opened\n");
		return GZ_FAILURE;
	}

	while (fscanf(infile, "%s", dummy) == 1) { 	/* read in tri word */
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[0][0]), &(vertexList[0][1]),
			&(vertexList[0][2]),
			&(normalList[0][0]), &(normalList[0][1]),
			&(normalList[0][2]),
			&(uvList[0][0]), &(uvList[0][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[1][0]), &(vertexList[1][1]),
			&(vertexList[1][2]),
			&(normalList[1][0]), &(normalList[1][1]),
			&(normalList[1][2]),
			&(uvList[1][0]), &(uvList[1][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[2][0]), &(vertexList[2][1]),
			&(vertexList[2][2]),
			&(normalList[2][0]), &(normalList[2][1]),
			&(normalList[2][2]),
			&(uvList[2][0]), &(uvList[2][1]));

		/*
		 * Set the value pointers to the first vertex of the
		 * triangle, then feed it to the renderer
		 * NOTE: this sequence matches the nameList token sequence
		 */
		valueListTriangle[0] = (GzPointer)vertexList;
		valueListTriangle[1] = (GzPointer)normalList;
		valueListTriangle[2] = (GzPointer)uvList;

		//extraGrassList, same as input file format
		float **extraGrassList = (float **)malloc(3 * sizeof(float *));
		for (int i = 0; i < 3; i++)
			extraGrassList[i] = (float *)malloc(8 * sizeof(float));

		extraGrassList = m_pRender->GzAddGrassWithModelSpace(3, nameListTriangle, valueListTriangle);
		for (int i = 0; i < 5; i++) {
			vertexGrassList[0][0] = extraGrassList[0 + i * 3][0];
			vertexGrassList[0][1] = extraGrassList[0 + i * 3][1];
			vertexGrassList[0][2] = extraGrassList[0 + i * 3][2];
			vertexGrassList[1][0] = extraGrassList[1 + i * 3][0];
			vertexGrassList[1][1] = extraGrassList[1 + i * 3][1];
			vertexGrassList[1][2] = extraGrassList[1 + i * 3][2];
			vertexGrassList[2][0] = extraGrassList[2 + i * 3][0];
			vertexGrassList[2][1] = extraGrassList[2 + i * 3][1];
			vertexGrassList[2][2] = extraGrassList[2 + i * 3][2];

			normalGrassList[0][0] = extraGrassList[0 + i * 3][3];
			normalGrassList[0][1] = extraGrassList[0 + i * 3][4];
			normalGrassList[0][2] = extraGrassList[0 + i * 3][5];
			normalGrassList[1][0] = extraGrassList[1 + i * 3][3];
			normalGrassList[1][1] = extraGrassList[1 + i * 3][4];
			normalGrassList[1][2] = extraGrassList[1 + i * 3][5];
			normalGrassList[2][0] = extraGrassList[2 + i * 3][3];
			normalGrassList[2][1] = extraGrassList[2 + i * 3][4];
			normalGrassList[2][2] = extraGrassList[2 + i * 3][5];

			uvGrassList[0][0] = extraGrassList[0 + i * 3][6];
			uvGrassList[0][1] = extraGrassList[0 + i * 3][7];
			uvGrassList[1][0] = extraGrassList[1 + i * 3][6];
			uvGrassList[1][1] = extraGrassList[1 + i * 3][7];
			uvGrassList[2][0] = extraGrassList[2 + i * 3][6];
			uvGrassList[2][1] = extraGrassList[2 + i * 3][7];

			valueListGrassTriangle[0] = (GzPointer)vertexGrassList;
			valueListGrassTriangle[1] = (GzPointer)normalGrassList;
			valueListGrassTriangle[2] = (GzPointer)uvGrassList;

			m_pRender->GzPutTriangle(3, nameListTriangle, valueListGrassTriangle);
		}

		m_pRender->GzPutTriangle(3, nameListTriangle, valueListTriangle);
	}

	//m_pRender->GzFlushDisplay2File(outfile); 	/* write out or update display to file*/
	m_pRender->GzFlushDisplay2DepthFile(outfile);
	m_pRender->GzFlushDisplay2FrameBuffer();	// write out or update display to frame buffer

	/*
	 * Close file
	 */

	if (fclose(infile))
		AfxMessageBox(_T("The input file was not closed\n"));

	if (fclose(outfile))
		AfxMessageBox(_T("The output file was not closed\n"));

	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}

int Application5::Clean()
{
	/*
	 * Clean up and exit
	 */
	int	status = 0;

	free(m_pRender);
	status |= GzFreeTexture();

	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}



