#pragma once
#include <filesystem>

using file_path = std::filesystem::path;

namespace Audio
{
	//Main.cppで呼び出す
	bool Initialize();
	void Update();
	void Release();

	//使う時に適宜呼び出す
	bool Load(const std::string& name,file_path& path);
	void Play(const std::string& name);
};

//Audio::Load("Jump1","Assets//Audio//Jump1.wav");
//Audio::Play("Jump1");
