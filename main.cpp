#include"Utility/pch.h"
#include"Converter/TextureConverter.h"

enum Argument {
	kApplicationPath,
	kFilePath,
	NumArgument
};

int main(int argc, char* argv[]) {

	CoInitializeEx(0, COINIT_MULTITHREADED);

	bool isEnd = false;
	bool isPng6toCubeMap = false;
	bool isTexPngToDDS = false;
	bool isTexHdrToDDS = false;
	// �쐬����t�H���_�̃p�X
	std::filesystem::path folderPath = "output";
	uint32_t loopCount = 0;

	printf("����ϊ����܂���\n");
	printf("png��DDS�ɕϊ� :: 1key\n");
	printf("png6����CubeMapDDS�ɕϊ� :: 2key\n");
	printf("Hdr��DDS�ɕϊ� :: 3key\n");

	int choice;
	std::cin >> choice;

	switch (choice) {
	case 1:
		isTexPngToDDS = true;
		std::cout << "png��DDS�ɕϊ����܂�...\n" << std::endl;
		std::cout << "�ϊ�����t�@�C�������ׂ�Input�t�@�C���ɓ���Ă�������\n����I�������key����͂��Ă�������" << std::endl;
		break;
	case 2:
		isPng6toCubeMap = true;
		std::cout << "png6����CubeMapDDS�ɕϊ����܂�...\n\n" << std::endl;
		std::cout << "x.png -x.png�̂悤�ɂ��ׂĂ̖ʂɖ��O����Input�t�@�C���ɓ���Ă�������" << std::endl;

		break;
	case 3:
		isTexHdrToDDS = true;
		std::cout << "hdr��DDS�ɕϊ����܂�...\n" << std::endl;
		std::cout << "�ϊ�����t�@�C�������ׂ�Input�t�@�C���ɓ���Ă�������\n����I�������key����͂��Ă�������" << std::endl;
		break;
	default:
		std::cout << "�����ȑI�����ł�\n" << std::endl;
		break;
	}

	std::cout << "����I�������key����͂��Ă�������" << std::endl;
	system("pause");

	//�t�H���_�T��

	// �Ώۃt�H���_�̃p�X
	std::filesystem::path inputFolderPath = "Input";
	std::vector<std::string>fileNames;
	//exe�̃p�X
	std::filesystem::path exePath = argv[0];
	std::filesystem::path directoryfilePath = exePath.parent_path().string();
	// �f�B���N�g���p�X�̎擾
	std::string directoryPath = directoryfilePath.string();


	try {
		// �f�B���N�g�����̃t�@�C�������[�v����
		for (const auto& entry : std::filesystem::directory_iterator(inputFolderPath)) {
			// �t�@�C���ł���A�g���q�� ".png" �̏ꍇ�ɏ���
			if (isTexPngToDDS || isPng6toCubeMap)
			{
				if (entry.is_regular_file() && entry.path().extension() == ".png") {
					// �t�@�C�����̕\��
					std::string fileName = entry.path().filename().string();
					std::cout << fileName << std::endl;
					fileNames.push_back(fileName);
				}
			}
			if (isTexHdrToDDS)
			{
				if (entry.is_regular_file() && entry.path().extension() == ".hdr") {
					// �t�@�C�����̕\��
					std::string fileName = entry.path().filename().string();
					std::cout << fileName << std::endl;
					fileNames.push_back(fileName);
				}
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "�G���[: " << e.what() << std::endl;
	}

	printf("�ǂݍ��񂾃t�@�C���̊m�F\n\n");
	system("pause");
	printf("�ϊ����I���܂ő҂��Ă�������\n\n");
	//std::filesystem::create_directory ���g�p���ăt�H���_���쐬
	std::filesystem::create_directory(folderPath);

	TextureConverter converter;
	//png�쐬
	if (isTexPngToDDS)
	{
		for (size_t i = 0; i < fileNames.size(); i++)
		{
			converter.ConvertTextureWICToDDS(directoryPath + "\\", fileNames[i]);
		}
		printf("�ϊ�����\n �A�v���P�[�V�������I�����Ă�������\n\n");
	}

	//hdr
	if (isTexHdrToDDS)
	{
		for (size_t i = 0; i < fileNames.size(); i++)
		{
			converter.ConvertTextureHdrToDDS(directoryPath + "\\", fileNames[i]);
		}
		printf("�ϊ�����\n �A�v���P�[�V�������I�����Ă�������\n\n");
	}



	//Cubemap
	if (isPng6toCubeMap)
	{
		std::vector < std::string > names;

		names.resize(fileNames.size());

		for (size_t i = 0; i < fileNames.size(); i++)
		{
			names[i] = fileNames[i];
		}

		converter.ConvertTextureWICToCubemap(directoryPath + "\\", names);

		printf("�ϊ�����\n �A�v���P�[�V�������I�����Ă�������\n\n");
	}

	system("pause");
	CoUninitialize();
	return 0;
}