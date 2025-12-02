#pragma once
#include "Engine//GameObject.h"
enum BLOCK_TYPE

{

    DEFAULT,

    WATER,

    SAND
};


struct sData

{

    BLOCK_TYPE type;//ブロックのタイプ

    int height;//ブロックを積む高さ
    int sTable;
   
    
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
    void Draw() override;
    void Update()override;
    void Release()override;
private:
   
    //  sData sTable[XSIZE][ZSIZE];
    sData sTable[5][5];
};


