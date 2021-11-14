#pragma once

#include "Includes.h"
#include "Ntdefs.h"
#include "Ntfuncs.h"

#include "Images/Images.h"

/*
typedef HRESULT(WINAPI* tD3DX11CreateShaderResourceViewFromMemory)(
	ID3D11Device* pDevice,
	LPCVOID                    pSrcData,
	SIZE_T                     SrcDataSize,
	D3DX11_IMAGE_LOAD_INFO* pLoadInfo,
	ID3DX11ThreadPump* pPump,
	ID3D11ShaderResourceView** ppShaderResourceView,
	HRESULT* pHResult);

static tD3DX11CreateShaderResourceViewFromMemory FD3DX11CreateShaderResourceViewFromMemory = NULL;
static HMODULE loadlibrary = NULL;
*/

class Image
{
public:
	std::string name;
	ID3D11ShaderResourceView* pShaderResource = nullptr;
	int width = 0;
	int height = 0;

	Image(std::string name, char* ImageBytes, int Size, ID3D11Device* pDevice)
	{
		this->name = name;

		// Load from byte array into a raw RGBA buffer
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load_from_memory((unsigned char*)ImageBytes, Size, &image_width, &image_height, NULL, 4);

		// Create texture
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = image_data;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pShaderResource);
		pTexture->Release();

		width = image_width;
		height = image_height;

		stbi_image_free(image_data);

		// old method that required loadlibrary and leaves d3dx11_43.dll module in memory

		//ID3DX11ThreadPump* pump{ nullptr };
		//D3DX11_IMAGE_LOAD_INFO info;

		//if (!loadlibrary)
		//	loadlibrary = LoadDll("d3dx11_43.dll"); //LI_FN(LoadLibraryA).cached()("d3dx11_43.dll");

		//if (!FD3DX11CreateShaderResourceViewFromMemory)
		//	FD3DX11CreateShaderResourceViewFromMemory = (tD3DX11CreateShaderResourceViewFromMemory)GetProcedureAddress(loadlibrary, XorStr("D3DX11CreateShaderResourceViewFromMemory"));

		//FD3DX11CreateShaderResourceViewFromMemory(pDevice, ImageBytes, Size, NULL/*&info*/,
		//	nullptr/*pump*/, &this->pShaderResource, 0);

		//if (pump) { pump->Release(); pump = nullptr; };
	}
};

class ImageManager
{
public:
	ImageManager()
	{
	}

	ImageManager(ID3D11Device* pDevice)
	{
		this->pDevice = pDevice;
	}

	~ImageManager()
	{
		Unload();
	}

	void Unload()
	{
		for (auto obj : this->vImages)
		{
			if (obj.pShaderResource)
			{
				obj.pShaderResource->Release(); 
				obj.pShaderResource = nullptr;
			};
		}
		if (this->pDevice)
		{
			pDevice->Release();
			pDevice = nullptr;
		}
	}

	void SetDevice(ID3D11Device* pDevice)
	{
		this->pDevice = pDevice;
	}

	int ImagesCount()
	{
		return this->vImages.size();
	}

	void AddImage(std::string name, char* Bytes, int Size)
	{
		this->vImages.push_back(Image(name, Bytes, Size, this->pDevice));
	}

	bool HasImage(std::string name)
	{
		for (auto obj : this->vImages)
		{
			if (name.compare(obj.name) == 0)
				return true;
		}
		return false;
	}

	ID3D11ShaderResourceView* GetImageByName(std::string name)
	{
		for (auto obj : this->vImages)
		{
			if (name.compare(obj.name) == 0)
			{
				return obj.pShaderResource;
			}
		}

		return nullptr;
	}

	Image GetImageInfoByName(std::string name)
	{
		for (auto obj : this->vImages)
		{
			if (name.compare(obj.name) == 0)
			{
				return obj;
			}
		}
		// todo return a value
	}

	std::vector<Image>* GetVector()
	{
		return &vImages;
	}

private:
	ID3D11Device* pDevice = nullptr;
	std::vector<Image> vImages;
};

inline ImageManager imageManager;
