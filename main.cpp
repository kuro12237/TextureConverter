#include"Utility/pch.h"
#include"Converter/TextureConverter.h"

enum Argument {
	kApplicationPath,
	kFilePath,
	NumArgument
};

int main(int argc, char* argv[]) {

	CoInitializeEx(0, COINIT_MULTITHREADED);

	system("pause");

	assert(argc >= NumArgument);

	TextureConverter converter;
	//png�쐬
	
	converter.ConvertTextureWICToDDS(argv[kFilePath]);


	printf("�ϊ�����\n �A�v���P�[�V�������I�����Ă�������\n\n");

	system("pause");
	CoUninitialize();
	return 0;
}