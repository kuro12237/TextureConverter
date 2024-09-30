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
	printf(argv[1]);
	system("pause");
	CoUninitialize();
	return 0;
}