#pragma once
#include"../Utility/pch.h"

class TextureConverter
{
public:
	TextureConverter() {};
	~TextureConverter() {};

	/// <summary>
	/// DDSに変換
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="filePath"></param>
	void ConvertTextureWICToDDS(const std::string& directory,const std::string& filePath);

	//6枚のpngをcubeMapに変換
	void ConvertTextureWICToCubemap(const std::string& directorys, const std::vector<std::string>& filePaths);
	
	/// <summary>
	/// 未完成
	/// </summary>
	/// <param name="directorys"></param>
	/// <param name="filePath"></param>
	void ConvertTextureHdrToDDS(const std::string directorys, const std::string& filePath);

	std::wstring ConvertStringToWstring(const std::string& name);

	void SeparateFilePath(std::wstring& filePath);

	void CreateCubeMapFromTextures(
		const std::wstring& posY,
		const std::wstring& negY,
		const std::wstring& negX,
		const std::wstring& posZ,
		const std::wstring& posX,
		const std::wstring& negZ,
		const std::wstring& outputName);

private:

	void SaveDDSTextureToFile();

	void SaveHdrToDDSTextureToFile();

	DirectX::TexMetadata metadata_;
	DirectX::ScratchImage scratchImage_;

	std::wstring directryPath_;
	std::wstring fileName_;
	std::wstring fileExt_;
};