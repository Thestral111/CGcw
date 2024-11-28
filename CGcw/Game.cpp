#include <Windows.h>
#include "Window.h"
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXCore dxcore;
	Shader shader;
	Triangle tri;
	win.init(1024, 1024, "My Window");
	dxcore.init(1024, 1024, win.hwnd, 0);
	shader.init("3D_vertexShader.txt", "pixelShader.txt", dxcore);
	tri.init(dxcore);
		while (true)
		{
			dxcore.clear();
			win.processMessages();
			
			shader.apply(dxcore);
			tri.draw(dxcore);
			/*win.processMessages();*/
			dxcore.present();
		}

}
