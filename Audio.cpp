#include "Audio.h"//自前のAudio名前空間の宣言をインクルード
#include <Audio.h>
#include <memory>
#include <list>
#include <unordered_map> //マップの中でも順番が関係ないもの

namespace
{
    std::unique_ptr<DirectX::AudioEngine> audioEngine_;//オーディオのシステム自体
    //名前と、サウンドメモリのペアを管理するマップ
    std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> sounds_;

}
bool Audio::Initialize()
{
    audioEngine_ = std::make_unique<DirectX::AudioEngine>();
    return true;
}

void Audio::Update()
{
    if (audioEngine_ != nullptr)
    {
        audioEngine_->Update();
    }
    
}

void Audio::Release()
{
    sounds_.clear();
}
bool Audio::Load(const std::string& name, file_path& path)
{
    if (audioEngine_ == nullptr)
    {
        return false;
    }

    sounds_[name] = std::make_unique<DirectX::SoundEffect>(audioEngine_.get(), path.c_str());
    return true;
}

void Audio::Play(const std::string& name)
{
    auto it = sounds_.find(name);//mapからnameに対応するサウンドを探す(確認)
    if (it != sounds_.end())
    {
        it->second->Play();
    }
}
