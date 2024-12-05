#include <Windows.h>
#include "Window.h"
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"
#include "geometry.h"
#include "GamesEngineeringBase.h"
#include "GEMLoader.h"
#include "Texture.h"
#include "Anim.h"


//class StaticModel
//{
//public:
//	std::vector<std::string> textureFilenames;
//	vector<mesh> meshes;
//	Texture texture;
//	Shader shader;
//	//TextureManager* textureManager;
//
//	void load(DXCore& dxcore, string filename, TextureManager& textureManager) {
//
//		GEMLoader::GEMModelLoader loader;
//		std::vector<GEMLoader::GEMMesh> gemmeshes;
//		loader.load(filename, gemmeshes);
//		for (int i = 0; i < gemmeshes.size(); i++) {
//			mesh mesh;
//			std::vector<STATIC_VERTEX> vertices;
//			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
//				STATIC_VERTEX v;
//				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
//				vertices.push_back(v);
//			}
//			mesh.init(vertices, gemmeshes[i].indices, dxcore);
//			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
//			textureManager.load(dxcore, gemmeshes[i].material.find("diffuse").getValue());
//			//textureManager->load(dxcore, gemmeshes[i].material.find("diffuse").getValue());
//			//textureManager.load(dxcore, "bark09.png");
//			//textureManager.load(dxcore, "pine branch.png");
//			meshes.push_back(mesh);
//		}
//	}
//	void draw(DXCore& dxcore, TextureManager& textureManager)
//	{
//		for (int i = 0; i < meshes.size(); i++)
//		{
//			shader.updateShaderResource(dxcore, "tex", textureManager.find(textureFilenames[i]));
//			meshes[i].draw(dxcore);
//		}
//	}
//};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXCore dxcore;
	Shader shader;
	Shader animShader;
	//Triangle tri;
	plane plane;
	cube cube;
	Matrix planeWorld;
	Matrix world;
	TextureManager textureManager;
	AnimationInstance instance;
	
	world = world.scaling(Vec3(0.01f, 0.01f, 0.01f));
	GamesEngineeringBase :: Timer timer;
	win.init(1024, 1024, "My Window");
	dxcore.init(1024, 1024, win.hwnd, 0);
	shader.init("3D_vertexShader.txt", "newPixelShader.txt", dxcore);
	animShader.initAnim("Animation_vertexShader.txt", "pixelShader.txt", dxcore);

	instance.init("TRex.gem", dxcore);
	// 
	textureManager.load(dxcore, "Textures/bark09.png");
	//std::vector<mesh> meshes;

	//loadTree(dxcore, meshes);

	//tri.init(dxcore);
	//plane.init(dxcore);
	//cube.init(dxcore);
	Matrix x;

	StaticModel tree;
	tree.load(dxcore, "Resource/pine.gem", textureManager);

	float t = 0;
	//x = x.LookAt(Vec3(10, 5, 10), Vec3(0, 1, 0), Vec3(0, 1, 0));
	for(float var : x.m)
	{
		cout << var << endl;
	}

		while (true)
		{
			//float t;
			float dt = timer.dt();
			t += dt;
			Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
			//Vec3 from = Vec3(10, 5, 10);
			//Matrix v = v.lookAt(from, Vec3(0, 1, 0), Vec3(0, 1, 0));
			Matrix p = p.Perspective(20.f, 1.f, 0.1f, 100.f);
			//Matrix vp = v.mul(p);
			Vec3 to = Vec3(0.0f, 0.0f, 0.0f);
			Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
			Matrix vp = vp.lookAt(from, to, up).mul(p); //vp.PerPro(1.f, 1.f, 20.f, 100.f, 0.1f)
			//Matrix vp = vp.lookAt(from, to, up).mul(p);

			dxcore.clear();
			win.processMessages();

			
			// planeworld is the rotation, translation and scale
			// vp is lookat and projection
			//shader.updateConstantVS("staticMeshBuffer", "staticMeshBuffer", "W", &world); // planeWorld
			//shader.updateConstantVS("staticMeshBuffer", "staticMeshBuffer", "VP", &vp); // StaticModel
			//shader.updateConstantVS("Animated", "staticMeshBuffer", "bones", instance.matrices); //anim

			//shader.apply(dxcore);
			//tree.draw(dxcore, textureManager);

			Matrix w1;
			w1 = Matrix::translation(Vec3(1, 0, 0));
			instance.update("Run", dt);
			animShader.updateConstantVS("animatedMeshBuffer", "W", &w1); // planeWorld
			animShader.updateConstantVS("animatedMeshBuffer", "VP", &vp); // StaticModel
			animShader.updateConstantVS("animatedMeshBuffer", "bones", instance.matrices); //anim
			
			
			//shader.apply(dxcore);
			//animShader.apply(dxcore);
			//tri.draw(dxcore);
			//plane.draw(dxcore);
			//cube.draw(dxcore);
			//tree.draw(dxcore, textureManager);
			animShader.apply(dxcore);
			
			instance.draw(dxcore);

			/*win.processMessages();*/
			dxcore.present();
		}

}
