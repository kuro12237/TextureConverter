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
	// 作成するフォルダのパス
	std::filesystem::path folderPath = "output";
	uint32_t loopCount = 0;

	printf("何を変換しますか\n");
	printf("pngをDDSに変換 :: 1key\n");
	printf("png6枚をCubeMapDDSに変換 :: 2key\n");
	printf("HdrをDDSに変換 :: 3key\n");

	int choice;
	std::cin >> choice;

	switch (choice) {
	case 1:
		isTexPngToDDS = true;
		std::cout << "pngをDDSに変換します...\n" << std::endl;
		std::cout << "変換するファイルをすべてInputファイルに入れてください\n入れ終わったらkeyを入力してください" << std::endl;
		break;
	case 2:
		isPng6toCubeMap = true;
		std::cout << "png6枚をCubeMapDDSに変換します...\n\n" << std::endl;
		std::cout << "x.png -x.pngのようにすべての面に名前をつけInputファイルに入れてください" << std::endl;

		break;
	case 3:
		isTexHdrToDDS = true;
		std::cout << "hdrをDDSに変換します...\n" << std::endl;
		std::cout << "変換するファイルをすべてInputファイルに入れてください\n入れ終わったらkeyを入力してください" << std::endl;
		break;
	default:
		std::cout << "無効な選択肢です\n" << std::endl;
		break;
	}

	std::cout << "入れ終わったらkeyを入力してください" << std::endl;
	system("pause");

	//フォルダ探索

	// 対象フォルダのパス
	std::filesystem::path inputFolderPath = "Input";
	std::vector<std::string>fileNames;
	//exeのパス
	std::filesystem::path exePath = argv[0];
	std::filesystem::path directoryfilePath = exePath.parent_path().string();
	// ディレクトリパスの取得
	std::string directoryPath = directoryfilePath.string();


	try {
		// ディレクトリ内のファイルをループ処理
		for (const auto& entry : std::filesystem::directory_iterator(inputFolderPath)) {
			// ファイルであり、拡張子が ".png" の場合に処理
			if (isTexPngToDDS || isPng6toCubeMap)
			{
				if (entry.is_regular_file() && entry.path().extension() == ".png") {
					// ファイル名の表示
					std::string fileName = entry.path().filename().string();
					std::cout << fileName << std::endl;
					fileNames.push_back(fileName);
				}
			}
			if (isTexHdrToDDS)
			{
				if (entry.is_regular_file() && entry.path().extension() == ".hdr") {
					// ファイル名の表示
					std::string fileName = entry.path().filename().string();
					std::cout << fileName << std::endl;
					fileNames.push_back(fileName);
				}
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "エラー: " << e.what() << std::endl;
	}

	printf("読み込んだファイルの確認\n\n");
	system("pause");
	printf("変換が終わるまで待ってください\n\n");
	//std::filesystem::create_directory を使用してフォルダを作成
	std::filesystem::create_directory(folderPath);

	TextureConverter converter;
	//png作成
	if (isTexPngToDDS)
	{
		for (size_t i = 0; i < fileNames.size(); i++)
		{
			converter.ConvertTextureWICToDDS(directoryPath + "\\", fileNames[i]);
		}
		printf("変換完了\n アプリケーションを終了してください\n\n");
	}

	//hdr
	if (isTexHdrToDDS)
	{
		for (size_t i = 0; i < fileNames.size(); i++)
		{
			converter.ConvertTextureHdrToDDS(directoryPath + "\\", fileNames[i]);
		}
		printf("変換完了\n アプリケーションを終了してください\n\n");
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

		printf("変換完了\n アプリケーションを終了してください\n\n");
	}

	system("pause");
	CoUninitialize();
	return 0;
}