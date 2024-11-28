#pragma once
#include "cgmath.h"
#include "Window.h"
#include "Device.h"
struct Vertex
{
	Vec3 position;
	Colour colour;
	
};



class Triangle {
public:
	//Vertex vertices[3];
	ID3D11Buffer* vertexBuffer;

	void init(DXCore& core) {
		Vertex vertices[3];
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA uploadData;
		bd.ByteWidth = sizeof(Vertex) * 3; //N
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		uploadData.pSysMem = vertices;
		uploadData.SysMemPitch = 0;
		uploadData.SysMemSlicePitch = 0;
		core.device->CreateBuffer(&bd, &uploadData, &vertexBuffer);

	}

	void draw(DXCore& core) {
		UINT offsets;
		offsets = 0;
		UINT strides = sizeof(Vertex);
		core.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		core.devicecontext->Draw(3, 0);

	}

};

