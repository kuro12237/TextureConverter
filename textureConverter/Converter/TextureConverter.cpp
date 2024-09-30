#include "TextureConverter.h"



using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string& directory, const std::string& filePath)
{
	size_t posl;
	std::wstring exceptExt;
	HRESULT hr;

	std::wstring wFilePath = ConvertStringToWstring(directory);
	std::wstring loadFileName = ConvertStringToWstring(directory + "input/" + filePath);

	fileName_ = ConvertStringToWstring(filePath);
	//����
	posl = fileName_.rfind('.');
	if (posl != std::wstring::npos)
	{
		fileExt_ = fileName_.substr(posl + 1, fileName_.size() - posl - 1);
		exceptExt = fileName_.substr(0, posl);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = fileName_;
	}

	directryPath_ = wFilePath;
	fileName_ = exceptExt;

	std::wcout << L"FilePath :: " + loadFileName << std::endl;

	hr = LoadFromWICFile(loadFileName.c_str(), WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(hr));
	SaveDDSTextureToFile();
}

void TextureConverter::ConvertTextureWICToCubemap(const std::string& directorys, const std::vector<std::string>& filePaths)
{

	std::wstring wDirectory = ConvertStringToWstring(directorys);
	std::wstring outputName = wDirectory + L"output/CubeMap.dds";

	std::wstring posY, negY, negX, posZ, posX, negZ;

	for (size_t i = 0; i < 6; i++)
	{
		if (filePaths[i] == "x.png")
		{
			posX = ConvertStringToWstring(directorys + "input/" + filePaths[i]);
			continue;
		}
		if (filePaths[i] == "-x.png")
		{
			negX = ConvertStringToWstring(directorys + "input/" + filePaths[i]);
			continue;
		}if (filePaths[i] == "y.png")
		{
			posY = ConvertStringToWstring(directorys + "input/" + filePaths[i]);
			continue;
		}
		if (filePaths[i] == "-y.png")
		{
			negY = ConvertStringToWstring(directorys + "input/" + filePaths[i]);
			continue;
		}
		if (filePaths[i] == "z.png")
		{
			posZ = ConvertStringToWstring(directorys + "input/" + filePaths[i]);
			continue;
		}
		if (filePaths[i] == "-z.png")
		{
			negZ = ConvertStringToWstring(directorys + "input/" + filePaths[i]);
			continue;
		}
	}

	CreateCubeMapFromTextures(posY, negY, negX, posZ, posX, negZ, outputName);
}

void TextureConverter::ConvertTextureHdrToDDS(const std::string directorys, const std::string& filePath)
{

	size_t posl;
	std::wstring exceptExt;
	HRESULT hr;

	std::wstring wFilePath = ConvertStringToWstring(directorys);
	std::wstring loadFileName = ConvertStringToWstring(directorys + "input/" + filePath);

	fileName_ = ConvertStringToWstring(filePath);
	//����
	posl = fileName_.rfind('.');
	if (posl != std::wstring::npos)
	{
		fileExt_ = fileName_.substr(posl + 1, fileName_.size() - posl - 1);
		exceptExt = fileName_.substr(0, posl);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = fileName_;
	}

	directryPath_ = wFilePath;
	fileName_ = exceptExt;

	std::wcout << L"FilePath :: " + loadFileName << std::endl;

	hr = LoadFromHDRFile(loadFileName.c_str(), &metadata_, scratchImage_);
	assert(SUCCEEDED(hr));
	SaveHdrToDDSTextureToFile();
}

std::wstring TextureConverter::ConvertStringToWstring(const std::string& name)
{
	size_t len = name.size() + 1; // +1 for the null terminator
	std::wstring wstr(len, L'\0');
	size_t convertedChars = 0;

	mbstowcs_s(&convertedChars, &wstr[0], len, name.c_str(), len - 1);
	wstr.resize(convertedChars - 1); // Remove the null terminator from the wstring

	return wstr;
}

void TextureConverter::SeparateFilePath(std::wstring& filePath)
{
	size_t posl;
	std::wstring exceptExt;

	posl = filePath.rfind('.');

	if (posl != std::wstring::npos)
	{
		fileExt_ = filePath.substr(posl + 1, filePath.size() - posl - 1);
		exceptExt = filePath.substr(0, posl);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = filePath;
	}

	posl = exceptExt.rfind('\\');
	if (posl != std::wstring::npos)
	{
		directryPath_ = exceptExt.substr(0, posl + 1);
		fileName_ = exceptExt.substr(posl + 1, exceptExt.size() - posl - 1);
		return;
	}

	posl = exceptExt.rfind('/');
	if (posl != std::wstring::npos)
	{
		directryPath_ = exceptExt.substr(0, posl + 1);
		fileName_ = exceptExt.substr(posl + 1, exceptExt.size() - posl - 1);
	}
	directryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::CreateCubeMapFromTextures(const std::wstring& posY, const std::wstring& negY, const std::wstring& negX, const std::wstring& posZ, const std::wstring& posX, const std::wstring& negZ, const std::wstring& outputName)
{
	HRESULT hr;
	ScratchImage images[6];
	TexMetadata metadata;

	// Load the images into ScratchImage objects
	hr = LoadFromWICFile(posX.c_str(), WIC_FLAGS_NONE, &metadata, images[0]);
	assert(SUCCEEDED(hr));
	hr = LoadFromWICFile(negX.c_str(), WIC_FLAGS_NONE, &metadata, images[1]);
	assert(SUCCEEDED(hr));
	hr = LoadFromWICFile(posY.c_str(), WIC_FLAGS_NONE, &metadata, images[2]);
	assert(SUCCEEDED(hr));
	hr = LoadFromWICFile(negY.c_str(), WIC_FLAGS_NONE, &metadata, images[3]);
	assert(SUCCEEDED(hr));
	hr = LoadFromWICFile(posZ.c_str(), WIC_FLAGS_NONE, &metadata, images[4]);
	assert(SUCCEEDED(hr));
	hr = LoadFromWICFile(negZ.c_str(), WIC_FLAGS_NONE, &metadata, images[5]);
	assert(SUCCEEDED(hr));

	// �L���[�u�}�b�v���\��
 // �L���[�u�}�b�v�p�Ƀ��^�f�[�^��ݒ�
	TexMetadata cubeMapMetadata = metadata;
	cubeMapMetadata.miscFlags |= TEX_MISC_TEXTURECUBE;
	cubeMapMetadata.arraySize = 6;

	// �L���[�u�}�b�v���\�����邽�߂̃o�b�t�@���m��
	ScratchImage cubeMap;
	hr = cubeMap.Initialize2D(
		cubeMapMetadata.format,
		cubeMapMetadata.width,
		cubeMapMetadata.height,
		6,
		cubeMapMetadata.mipLevels);
	assert(SUCCEEDED(hr));

	// �L���[�u�}�b�v�ɉ摜�����蓖��
	for (size_t i = 0; i < 6; ++i)
	{
		memcpy(cubeMap.GetImages()[i].pixels, images[i].GetImages()->pixels, images[i].GetImages()->slicePitch);
	}

	// Mipmap�̍쐬
	ScratchImage mipChain;
	hr = GenerateMipMaps(
		cubeMap.GetImages(), cubeMap.GetImageCount(), cubeMap.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(hr))
	{
		cubeMap = std::move(mipChain);
		cubeMapMetadata = cubeMap.GetMetadata();
	}

	//���k1024�ȉ��̏ꍇ���k����ȏ�͈��k���Ȃ�
	const uint32_t MAX_SIZE = 1024; // �ő�T�C�Y
	if (metadata.width <= MAX_SIZE && metadata.height <= MAX_SIZE)
	{
		ScratchImage converted;
		hr = Compress(cubeMap.GetImages(), cubeMap.GetImageCount(), cubeMapMetadata,
			DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL,
			1.0f, converted);
		if (SUCCEEDED(hr))
		{
			cubeMap = std::move(converted);
			cubeMapMetadata = cubeMap.GetMetadata();
		}
	}

	// ���ۂɍ쐬
	cubeMapMetadata.miscFlags |= TEX_MISC_TEXTURECUBE;

	std::wstring filePath = directryPath_ + outputName;

	// �L���[�u�}�b�v��DDS�t�@�C���Ƃ��ĕۑ�
	hr = SaveToDDSFile(cubeMap.GetImages(), cubeMap.GetImageCount(), cubeMapMetadata, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}

void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT hr;

	//mipmap�̍쐬
	ScratchImage mipChain;
	hr = GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(hr))
	{
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	//���k
	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL,
		1.0f, converted
	);
	if (SUCCEEDED(hr))
	{
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	//���ۂɍ쐬
	metadata_.format = MakeSRGB(metadata_.format);
	std::wstring filePath = directryPath_ + L"output/" + fileName_ + L".dds";


	std::wcout << L"FilePath :: " + filePath + L"\n" << std::endl;
	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}

void TextureConverter::SaveHdrToDDSTextureToFile()
{
	HRESULT hr;

	//mipmap�̍쐬
	ScratchImage mipChain;
	hr = GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(hr))
	{
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	//���k
	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DXGI_FORMAT_BC6H_UF16, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL,
		1.0f, converted
	);
	if (SUCCEEDED(hr))
	{
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	//���ۂɍ쐬
	metadata_.format = MakeSRGB(metadata_.format);
	std::wstring filePath = directryPath_ + L"output/" + fileName_ + L".dds";

	std::wcout << L"FilePath :: " + filePath + L"\n" << std::endl;
	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}



