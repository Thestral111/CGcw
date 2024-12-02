#include <Windows.h>
#include "Window.h"
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"
#include "geometry.h"
#include "GamesEngineeringBase.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXCore dxcore;
	Shader shader;
	//Triangle tri;
	plane plane;
	cube cube;
	Matrix planeWorld;
	GamesEngineeringBase :: Timer timer;
	win.init(1024, 1024, "My Window");
	dxcore.init(1024, 1024, win.hwnd, 0);
	shader.init("3D_vertexShader.txt", "pixelShader.txt", dxcore);
	//tri.init(dxcore);
	//plane.init(dxcore);
	cube.init(dxcore);
	Matrix x;
	float t = 0;
	//x = x.LookAt(Vec3(10, 5, 10), Vec3(0, 1, 0), Vec3(0, 1, 0));
	for(float var : x.m)
	{
		cout << var << endl;
	}

		while (true)
		{
			//float t;
			t += timer.dt();
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
			shader.updateConstantVS("staticMeshBuffer", "staticMeshBuffer", "W", &planeWorld); // planeWorld
			shader.updateConstantVS("staticMeshBuffer", "staticMeshBuffer", "VP", &vp); // StaticModel


			shader.apply(dxcore);
			//tri.draw(dxcore);
			//plane.draw(dxcore);
			cube.draw(dxcore);
			/*win.processMessages();*/
			dxcore.present();
		}

}
