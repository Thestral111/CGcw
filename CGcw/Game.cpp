#include <Windows.h>
#include "Window.h"
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"
#include "geometry.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXCore dxcore;
	Shader shader;
	Triangle tri;
	plane plane;
	//Timer timer;
	win.init(1024, 1024, "My Window");
	dxcore.init(1024, 1024, win.hwnd, 0);
	shader.init("3D_vertexShader.txt", "pixelShader.txt", dxcore);
	//tri.init(dxcore);
	plane.init(dxcore);
		while (true)
		{
			//Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
			Vec3 from = Vec3(10, 5, 10);
			Matrix v = v.LookAt(from, Vec3(0, 1, 0), Vec3(0, 1, 0));


			dxcore.clear();
			win.processMessages();
			shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &plane); // planeWorld
			//shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);


			shader.apply(dxcore);
			//tri.draw(dxcore);
			plane.draw(dxcore);
			/*win.processMessages();*/
			dxcore.present();
		}

}
