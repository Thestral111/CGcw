#pragma once
#include "cgmath.h"
#include "Device.h"
#include "Shaders.h"
#include "Texture.h"
#include "GEMLoader.h"
#include "Camera.h"
#include "Collision.h"



DXCore core1;

void getDXCore(DXCore& _core) {
	core1 = _core;
}

Texture normalMapTexture;
// load normal map
void loadTextures(DXCore& dxcore) {
	//diffuseTexture.load(dxcore, "Textures/diffuse.png");
	normalMapTexture.load(dxcore, "Textures/bark09_normal.png");
}

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

struct ANIMATED_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
};


class mesh
{
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, DXCore& core) {
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		core.device->CreateBuffer(&bd, &data, &indexBuffer);
		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		core.device->CreateBuffer(&bd, &data, &vertexBuffer);
		indicesSize = numIndices;
		strides = vertexSizeInBytes;

	}

	void init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core)
	{
		init(&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(), core);
	}

	void init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& device)
	{
		init(&vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size(), device);
	}

	void draw(DXCore& core) {
		UINT offsets = 0;
		//DXCore core;
		core.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		core.devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		core.devicecontext->DrawIndexed(indicesSize, 0, 0);
	}


};

class plane {
public:
	mesh geometry;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		//Frame frame;
		//frame.fromVector(n);
		//v.tangent = frame.u; // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DXCore& core) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		geometry.init(vertices, indices, core);

	}

	void draw(DXCore& core) {
		geometry.draw(core);
	}


};


class cube {
public:
	mesh geometry;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		//Frame frame;
		//frame.fromVector(n);
		//v.tangent = frame.u; // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DXCore& core) {
		std::vector<STATIC_VERTEX> vertices;
		Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
		Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
		Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
		Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
		Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
		Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
		Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

		// Add faces (Front, Back, Left)
		vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		// Add faces (Right, Top, Bottom)
		vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

		// Add indices
		std::vector<unsigned int> indices;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);
		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);
		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);
		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);
		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(23);

		geometry.init(vertices, indices, core);

	}

	void draw(DXCore& core) {
		geometry.draw(core);
	}

};

// static model e.g. tree
class StaticModel
{
public:
	std::vector<std::string> textureFilenames;
	vector<mesh> meshes;
	Texture texture;
	Shader shader;
	AABB boundingBox;
	//TextureManager* textureManager;

	void computeTangents(std::vector<STATIC_VERTEX>& vertices, std::vector<unsigned int>& indices) {
		for (size_t i = 0; i < indices.size(); i += 3) {
			// Get triangle vertices
			STATIC_VERTEX& v0 = vertices[indices[i]];
			STATIC_VERTEX& v1 = vertices[indices[i + 1]];
			STATIC_VERTEX& v2 = vertices[indices[i + 2]];

			// Calculate edge vectors
			Vec3 edge1 = v1.pos - v0.pos;
			Vec3 edge2 = v2.pos - v0.pos;

			// Calculate UV delta
			float deltaU1 = v1.tu - v0.tu;
			float deltaV1 = v1.tv - v0.tv;
			float deltaU2 = v2.tu - v0.tu;
			float deltaV2 = v2.tv - v0.tv;

			float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

			Vec3 tangent;
			tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
			tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
			tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);
			tangent = tangent.normalize();

			// Assign tangent to all vertices of the triangle
			v0.tangent = tangent;
			v1.tangent = tangent;
			v2.tangent = tangent;
		}
	}

	void load(DXCore& dxcore, string filename, TextureManager& textureManager) {

		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);
		std::vector<Vec3> allVertices;
		for (int i = 0; i < gemmeshes.size(); i++) {
			mesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
				//get all vertices to compute bounding box
				allVertices.push_back(v.pos);
			}
			computeTangents(vertices, gemmeshes[i].indices);
			mesh.init(vertices, gemmeshes[i].indices, dxcore);
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			textureManager.load(dxcore, gemmeshes[i].material.find("diffuse").getValue());
			
			meshes.push_back(mesh);
			
			// load normal map
			loadTextures(dxcore);
			
		}
		boundingBox = boundingBox.computeAABB(allVertices);
		
		OutputDebugString(to_wstring(boundingBox.min.x).c_str());
		OutputDebugString(to_wstring(boundingBox.min.y).c_str());
		OutputDebugString(to_wstring(boundingBox.min.z).c_str());
		wstring str = L"\n";
		OutputDebugString(str.c_str());
	}
	void draw(DXCore& dxcore, TextureManager& textureManager)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			//shader.updateShaderResource(dxcore, "normalMap", normalMapTexture.srv);
			shader.updateShaderResource(dxcore, "tex", textureManager.find(textureFilenames[i]));
			//shader.updateShaderResource(dxcore, "normal", textureManager.find(textureFilenames[i]));
			shader.updateShaderResource(dxcore, "normalMap", normalMapTexture.srv);
			meshes[i].draw(dxcore);
		}
	}
};



class Skybox {
public:
	cube skyCube;            // The skybox cube
	Shader skyShader;        // Skybox shader
	Texture cubemapTexture;  // Cubemap texture

	void init(DXCore& dxcore, const std::vector<std::string>& cubemapFaces) {
		// Initialize the cube mesh
		skyCube.init(dxcore);
		// Scale the cube to a large size (e.g., 100 units)
		
		// Initialize the shader
		skyShader.init("skyVertexShader.txt", "skyPixelShader.txt", dxcore);

		// Load the cubemap texture
		cubemapTexture.loadCubemap(dxcore, cubemapFaces);
	}

	void render(DXCore& dxcore, Camera& camera, const Matrix& projection) {
		// Disable depth writes
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		ID3D11DepthStencilState* depthStencilState;
		dxcore.device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		dxcore.devicecontext->OMSetDepthStencilState(depthStencilState, 0);

		// Set the view matrix without translation so you never reach the end of the skybox
		Matrix view = camera.getLookat();
		view.m[3] = 0;
		view.m[7] = 0;
		view.m[11] = 0;

		// Pass matrices to the shader
		Matrix scaleMatrix = Matrix::scaling(Vec3(10.0f, 10.0f, 10.0f));
		Matrix vp =  scaleMatrix * view * projection;
		skyShader.updateConstantVS("staticMeshBuffer", "VP", &vp);

		// Bind the cubemap texture
		skyShader.updateShaderResource(dxcore, "tex", cubemapTexture.srv);

		// Apply the shader and render the skybox
		skyShader.apply(dxcore);
		skyCube.draw(dxcore);

		// Restore the original depth state
		depthStencilState->Release();
		dxcore.devicecontext->OMSetDepthStencilState(NULL, 0);
	}
};

