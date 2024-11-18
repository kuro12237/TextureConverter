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
	//png作成
	
	converter.ConvertTextureWICToDDS(argv[kFilePath]);


	printf("変換完了\n アプリケーションを終了してください\n\n");

	system("pause");
	CoUninitialize();
	return 0;
}