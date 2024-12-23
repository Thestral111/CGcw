#pragma once
#include "Device.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <D3Dcompiler.h>
#include <d3d11shader.h>
#include "ShaderReflection.h"

#pragma comment(lib, "dxguid.lib")

using namespace std;

class Shader {
public:
    //DXCore* core;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* layout;
    std::map<std::string, int> textureBindPoints;



	std::string readfile(std::string filename) {
        std::ifstream shaderFile(filename);

        // Check if the file is open
        if (!shaderFile.is_open()) {
            throw std::runtime_error("Failed to open shader file: " + filename);
        }

        // Read the file content into a string
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        // Close the file
        //shaderFile.close();

        // Return the file content as a string
        return shaderStream.str();
	}

    

    void apply(DXCore& core) {
        core.devicecontext->IASetInputLayout(layout);
        core.devicecontext->VSSetShader(vertexShader, NULL, 0);
        core.devicecontext->PSSetShader(pixelShader, NULL, 0);
        
        for (int i = 0; i < vsConstantBuffers.size(); i++) {
            vsConstantBuffers[i].upload(&core);
        }
        for (int i = 0; i < psConstantBuffers.size(); i++) {
            psConstantBuffers[i].upload(&core);
        }
        


    }

    void updateShaderResource(DXCore& core, string name,ID3D11ShaderResourceView* srv) {
        
        if (name == "tex") {
            core.devicecontext->PSSetShaderResources(0, 1, &srv); // Bind to t0
        }
        else if (name == "normalMap") {
            core.devicecontext->PSSetShaderResources(1, 1, &srv); // Bind to t1
        }
    }

    void updateConstantVS(string constantBufferName, string variableName, void* data) {
        for (int i = 0; i < vsConstantBuffers.size(); i++) {
            if (vsConstantBuffers[i].name == constantBufferName) {
                vsConstantBuffers[i].update(variableName, data);
            }
        }
    }

    void updateConstantPS(string constantBufferName, string variableName, void* data) {
        for (int i = 0; i < psConstantBuffers.size(); i++) {
            if (psConstantBuffers[i].name == constantBufferName) {
                psConstantBuffers[i].update(variableName, data);
            }
        }
    }


	void init(std::string vsfilename, std::string psfilename, DXCore& dxcore) {
        
        std::string vs = readfile(vsfilename);
        std::string ps = readfile(psfilename);
        
        loadVS(dxcore, vs);
        
        loadPS(dxcore, ps);
        

	}

    std::vector<ConstantBuffer> psConstantBuffers;
    std::vector<ConstantBuffer> vsConstantBuffers;
    std::map<std::string, int> textureBindPointsVS;
    std::map<std::string, int> textureBindPointsPS;

    void loadVS(DXCore& core, std::string vsStr) {
        ID3DBlob* compiledVertexShader;
        ID3DBlob* status;
        HRESULT hr = D3DCompile(vsStr.c_str(), strlen(vsStr.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
        if (FAILED(hr)) {
            MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
            exit(0);
        }
        core.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);

        
        D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
        {
            { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        core.device->CreateInputLayout(layoutDesc, 4, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);
        ConstantBufferReflection reflection;
        reflection.build(&core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);

    }

    void loadAnimVS(DXCore& core, std::string vsStr) {
        ID3DBlob* compiledVertexShader;
        ID3DBlob* status;
        HRESULT hr = D3DCompile(vsStr.c_str(), strlen(vsStr.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
        if (FAILED(hr)) {
            MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
            exit(0);
        }
        core.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);


        D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
            { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        core.device->CreateInputLayout(layoutDesc, 6, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

        ConstantBufferReflection reflection;
        reflection.build(&core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);

    }

    void loadPS(DXCore core, std::string hlsl)
    {
        ID3DBlob* shader;
        ID3DBlob* status;
        HRESULT hr = D3DCompile(hlsl.c_str(), strlen(hlsl.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &shader, &status);
        if (FAILED(hr))
        {
            MessageBoxA(NULL, (char*)status->GetBufferPointer(), "PS ERROR", 0);
            exit(0);
        }
        core.device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, &pixelShader);
        ConstantBufferReflection reflection;
        reflection.build(&core, shader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
    }


    void initAnim(std::string vsfilename, std::string psfilename, DXCore& dxcore) {
        
        std::string vs = readfile(vsfilename);
        std::string ps = readfile(psfilename);
        
        loadAnimVS(dxcore, vs);
        //compilePS(ps, dxcore);
        loadPS(dxcore, ps);


    }

    void initWithNormal(std::string vsfilename, std::string psfilename, DXCore& dxcore) {
        
        std::string vs = readfile(vsfilename);
        std::string ps = readfile(psfilename);
        
        loadVS(dxcore, vs);
        
        loadPS(dxcore, ps);
        // Texture binding points
        textureBindPoints["tex"] = 0;        // Diffuse texture (t0)
        textureBindPoints["normalMap"] = 1; // Normal map (t1)

    }


};

