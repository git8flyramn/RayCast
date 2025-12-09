#pragma once
#include "Engine//GameObject.h"
#include "vector"
#include "Engine//Fbx.h"
enum BLOCK_TYPE

{
    DEFAULT,
    WATER,
    SAND,
    GLASS,
    LENGA
};

struct sData

{

    BLOCK_TYPE type;//ブロックのタイプ

    int height;//ブロックを積む高さ
   
    
};
class Stage : public GameObject
{ 
public:
    void SetBlock(BLOCK_TYPE type, int x, int z) { GetT(x, z).type = type; }
    void SetBlockHeight(int x, int z, int height);
    sData& GetT(int x, int z) { return sTable[z][x]; }
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Draw() override;
    void Update()override;
    void Release()override;
private:
    std::vector<string> ModelName;
    //sData sTable[XSIZE][ZSIZE];
      sData sTable[5][5];
      Fbx* fbx;

};


