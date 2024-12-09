#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Device.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <D3Dcompiler.h>
#include <d3d11shader.h>
#include "ShaderReflection.h"

class Sampler {
public:
    ID3D11SamplerState* state;

    void init(DXCore& core) {
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        core.device->CreateSamplerState(&samplerDesc, &state);

    }

    void bind(DXCore& core) {
        core.devicecontext->PSSetSamplers(0, 1, &state);

    }

};

class Texture {
public:
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* srv;
    std::map<std::string, int> textureBindPoints;
    Sampler sampler;



	void load(DXCore& dxcore, std :: string filename) {
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char* texels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (channels == 3) {
            channels = 4;
            unsigned char* texelsWithAlpha = new unsigned char[width * height * channels];
            for (int i = 0; i < (width * height); i++) {
                texelsWithAlpha[i * 4] = texels[i * 3];
                texelsWithAlpha[(i * 4) + 1] = texels[(i * 3) + 1];
                texelsWithAlpha[(i * 4) + 2] = texels[(i * 3) + 2];
                texelsWithAlpha[(i * 4) + 3] = 255;
            }
            // Initialize texture using width, height, channels, and texelsWithAlpha
            init(dxcore, width, height, channels, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, texelsWithAlpha);
            delete[] texelsWithAlpha;
        }
        else {
            init(dxcore, width, height, channels, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, texels);
            // Initialize texture using width, height, channels, and texels
        }
        sampler.init(dxcore);
        sampler.bind(dxcore);
        stbi_image_free(texels);

	}

    void loadCubemap(DXCore& dxcore, const std::vector<std::string>& faces) {
        // Ensure the faces vector has 6 textures
        if (faces.size() != 6) {
            throw std::runtime_error("Cubemap requires exactly 6 images (one for each face).");
        }

        // Load the images for each face
        int width = 0, height = 0, channels = 0;
        std::vector<unsigned char*> faceData;
        for (const auto& face : faces) {
            unsigned char* data = stbi_load(face.c_str(), &width, &height, &channels, STBI_rgb_alpha); // Load as RGBA
            if (!data) {
                throw std::runtime_error("Failed to load cubemap face: " + face);
            }
            faceData.push_back(data);
        }

        // Create the texture description
        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 6; // 6 faces for the cubemap
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Ensure format is compatible with the image data
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;

        // Subresource data for each face
        D3D11_SUBRESOURCE_DATA subresourceData[6] = {};
        for (int i = 0; i < 6; ++i) {
            subresourceData[i].pSysMem = faceData[i];
            subresourceData[i].SysMemPitch = width * 4; // 4 bytes per pixel (RGBA)
        }

        // Create the cubemap texture
        ID3D11Texture2D* cubemapTexture;
        HRESULT hr = dxcore.device->CreateTexture2D(&texDesc, subresourceData, &cubemapTexture);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create cubemap texture.");
        }

        // Create a shader resource view for the cubemap
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = texDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        srvDesc.TextureCube.MostDetailedMip = 0;
        srvDesc.TextureCube.MipLevels = 1;

        hr = dxcore.device->CreateShaderResourceView(cubemapTexture, &srvDesc, &srv);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create cubemap shader resource view.");
        }

        // Release image data and the texture resource
        cubemapTexture->Release();
        for (auto data : faceData) {
            stbi_image_free(data);
        }

        // Bind the sampler
        sampler.init(dxcore);
        sampler.bind(dxcore);
    }

    void init(DXCore& dxcore, int width, int height, int channels, DXGI_FORMAT format, unsigned char* data) {
        D3D11_TEXTURE2D_DESC texDesc;
        memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = format;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;
        // Create texture with texel data
        D3D11_SUBRESOURCE_DATA initData;
        memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
        initData.pSysMem = data;
        initData.SysMemPitch = width * channels;
        dxcore.device->CreateTexture2D(&texDesc, &initData, &texture);
        // Create shader resource view for texture
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;
        dxcore.device->CreateShaderResourceView(texture, &srvDesc, &srv);


    }

    void apply(DXCore& dxcore) {
        dxcore.devicecontext->PSSetShaderResources(0, 1, &srv);
        // call the setshader in shader.h
    }

    void free() {
        srv->Release();
        texture->Release();
    }

};



class TextureManager
{
public:
    std::map<std::string, Texture*> textures;
    void load(DXCore& core, std::string filename)
    {
        std::map<std::string, Texture*>::iterator it = textures.find(filename);
        if (it != textures.end())
        {
            return;
        }
        Texture* texture = new Texture();
        texture->load(core, filename);
        textures.insert({ filename, texture });
    }
    ID3D11ShaderResourceView* find(std::string name)
    {
        return textures[name]->srv;
    }
    void unload(std::string name)
    {
        textures[name]->free();
        textures.erase(name);
    }
    ~TextureManager()
    {
        for (auto it = textures.cbegin(); it != textures.cend(); )
        {
            it->second->free();
            textures.erase(it++);
        }
    }
};

//class Sampler {
//public:
//    ID3D11SamplerState* state;
//
//    void init(DXCore& core) {
//        D3D11_SAMPLER_DESC samplerDesc;
//        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
//        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//        samplerDesc.MinLOD = 0;
//        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//        core.device->CreateSamplerState(&samplerDesc, &state);
//
//    }
//
//    void bind(DXCore& core) {
//        core.devicecontext->PSSetSamplers(0, 1, &state);
//
//    }
//
//};
