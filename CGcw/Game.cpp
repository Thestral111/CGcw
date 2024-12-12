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
#include "Camera.h"
#include "Light.h"


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
	Shader shader1;
	Shader animShader;
	//Triangle tri;
	plane plane;
	cube cube;
	Matrix planeWorld;
	Matrix world;
	Matrix v1;
	Matrix p1;
	TextureManager textureManager;
	TextureManager textureManager1;
	AnimationInstance instance;
	Camera camera;
	Skybox skybox;
	Light light;
	
	world = world.scaling(Vec3(0.01f, 0.01f, 0.01f));
	Matrix w1;
	w1 = Matrix::translation(Vec3(2, 0, 0));
	GamesEngineeringBase :: Timer timer;
	win.init(1024, 1024, "My Window");
	dxcore.init(1024, 1024, win.hwnd, 0);
	//shader.init("3D_vertexShader.txt", "newPixelShader.txt", dxcore);
	//shader.init("normalVertexShader.txt", "normalPixelShader.txt", dxcore); //with normal and light
	shader.init("3D_vertexShader.txt", "newPixelShader.txt", dxcore); //with normal and light
	shader1.init("3D_vertexShader.txt", "pixelShader.txt", dxcore); // skybox
	animShader.initAnim("Animation_vertexShader.txt", "newPixelShader.txt", dxcore);

	instance.init("TRex.gem", dxcore, textureManager1);
	// 
	//textureManager.load(dxcore, "Textures/bark09.png");
	//std::vector<mesh> meshes;

	//loadTree(dxcore, meshes);

	//tri.init(dxcore);
	plane.init(dxcore);
	//cube.init(dxcore);
	Matrix x;

	StaticModel tree;
	tree.load(dxcore, "Resource/pine.gem", textureManager);
	/*StaticModel tree;
	tree.load(dxcore, "Resource/pine.gem", textureManager);*/

	std::vector<std::string> cubemapFaces = {
		"Standard-Cube-Map/px.png",  "Standard-Cube-Map/nx.png",
		"Standard-Cube-Map/py.png",    "Standard-Cube-Map/ny.png",
		"Standard-Cube-Map/pz.png",  "Standard-Cube-Map/nz.png"
	};
	skybox.init(dxcore, cubemapFaces);

	float t = 0;
	//x = x.LookAt(Vec3(10, 5, 10), Vec3(0, 1, 0), Vec3(0, 1, 0));
	for(float var : x.m)
	{
		cout << var << endl;
	}
	int lastMouseX = win.mousex;
	int lastMouseY = win.mousey;

		while (true)
		{
			//float t;
			float dt = timer.dt();
			t += dt;
			Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
			//Vec3 from = Vec3(10, 5, 10);
			//Matrix v = v.lookAt(from, Vec3(0, 1, 0), Vec3(0, 1, 0));
			Matrix p = p1.Perspective(20.f, 1.f, 0.1f, 100.f);
			//Matrix p = p.PerPro(1.f, 1.f, 20.f, 100.f, 0.1f);
			//Matrix vp = v.mul(p);
			Vec3 to = Vec3(0.0f, 0.0f, 0.0f);
			Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
			//Matrix vp = vp.lookAt(from, to, up).mul(p); //vp.PerPro(1.f, 1.f, 20.f, 100.f, 0.1f)
			Matrix v = v1.lookAt(from, to, up);

			Vec3 forward = camera.getForwardDirection();
			Vec3 right = camera.getRightDirection();

			if (win.keys['W']) camera.move(forward, dt);
			if (win.keys['S']) camera.move(-forward, dt);
			if (win.keys['D']) camera.move(right, dt);
			if (win.keys['A']) camera.move(-right, dt);

			// Mouse input for rotation

			if (win.mouseButtons[0]) { // Left mouse button is pressed
				int currentMouseX = win.mousex;
				int currentMouseY = win.mousey;

				float xOffset = static_cast<float>(currentMouseX - lastMouseX);
				float yOffset = static_cast<float>(currentMouseY - lastMouseY);

				camera.rotate(xOffset, yOffset);

				lastMouseX = currentMouseX;
				lastMouseY = currentMouseY;
			}
			else {
				lastMouseX = win.mousex;
				lastMouseY = win.mousey;
			}

			Matrix view = camera.getLookat();
			Matrix vp = view * p;
			//Matrix vp = vp.lookAt(from, to, up).mul(p);

			dxcore.clear();
			win.processMessages();
			skybox.render(dxcore, camera, p);
			

			shader1.updateConstantVS("staticMeshBuffer", "W", &w1); // planeWorld
			//shader.updateConstantVS("staticMeshBuffer", "W", &world); // planeWorld
			shader1.updateConstantVS("staticMeshBuffer", "VP", &vp);
			shader1.apply(dxcore);
			plane.draw(dxcore);
			// planeworld is the rotation, translation and scale
			// vp is lookat and projection
			shader.updateConstantVS("staticMeshBuffer", "W", &world); // planeWorld
			//shader.updateConstantVS("staticMeshBuffer", "W", &world); // planeWorld
			shader.updateConstantVS("staticMeshBuffer", "VP", &vp); // StaticModel
			//shader.updateConstantVS("Animated", "staticMeshBuffer", "bones", instance.matrices); //anim
			light.position = Vec3(5.0f, 10.0f, 5.0f);
			shader.updateConstantVS("staticMeshBuffer", "lightPos", &light.position);
			shader.updateConstantPS("staticMeshBuffer", "lightColour", &light.color);
			shader.updateConstantPS("staticMeshBuffer", "lightIntensity", &light.intensity);

			shader.apply(dxcore);
			tree.draw(dxcore, textureManager);

			
			instance.update("roar", dt);
			animShader.updateConstantVS("animatedMeshBuffer", "W", &w1); // planeWorld
			animShader.updateConstantVS("animatedMeshBuffer", "VP", &vp); // StaticModel
			animShader.updateConstantVS("animatedMeshBuffer", "bones", instance.matrices); //anim
			
			
			//shader.apply(dxcore);
			//animShader.apply(dxcore);
			//tri.draw(dxcore);
			//cube.draw(dxcore);
			//tree.draw(dxcore, textureManager);
			animShader.apply(dxcore);
			
			instance.draw(dxcore, animShader, textureManager1);
			
			
			//instance.draw(dxcore);
			/*win.processMessages();*/
			dxcore.present();
		}

}
