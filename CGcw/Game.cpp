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
	Matrix planeWorld;
	GamesEngineeringBase :: Timer timer;
	win.init(1024, 1024, "My Window");
	dxcore.init(1024, 1024, win.hwnd, 0);
	shader.init("3D_vertexShader.txt", "pixelShader.txt", dxcore);
	//tri.init(dxcore);
	plane.init(dxcore);
		while (true)
		{
			float t = timer.dt();
			Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
			//Vec3 from = Vec3(10, 5, 10);
			Matrix v = v.LookAt(from, Vec3(0, 1, 0), Vec3(0, 1, 0));
			Matrix p = p.Perspective(90.f, 1.f, 0.1f, 10.f);
			Matrix vp = v.mul(p);

			dxcore.clear();
			win.processMessages();
			// planeworld is the rotation, translation and scale
			// vp is lookat and projection
			shader.updateConstantVS("staticMeshBuffer", "staticMeshBuffer", "W", &planeWorld); // planeWorld
			shader.updateConstantVS("staticMeshBuffer", "staticMeshBuffer", "VP", &vp); // StaticModel


			shader.apply(dxcore);
			//tri.draw(dxcore);
			plane.draw(dxcore);
			/*win.processMessages();*/
			dxcore.present();
		}

}
