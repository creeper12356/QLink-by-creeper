#include "map.h"
Map::Map(int w,int h,QPoint p,int bs):wScale(w),hScale(h),corner(p),blockSize(bs)
{
    data = new int*[hScale];
    for(int i = 0;i <= hScale - 1;++i)
    {
        data[i] = new int[wScale];
        for(int j = 0;j <= wScale - 1;++j)
        {
            data[i][j] = 0;
        }
    }
    initBlocks();
}
Map::~Map()
{
    for(int i = 0;i <= hScale - 1;++i)
    {
        delete [] data[i];
    }
    delete [] data;
    int blocksSize = blocks.size();
    for(int i = 0;i <= blocksSize - 1;++i)
        delete blocks[i];
}
void Map::addBlock(Block* newBlock)
{
    blocks.push_back(newBlock);
}
void Map::initBlocks()
{
    Block* handle;
    //空气
    handle = new Block;
    handle->speedFactor = 1.2;
    handle->pic.load(":/images/blocks/air.jpg");
    handle->pic.scaledToWidth(blockSize);
    addBlock(handle);

    //木板
    handle = new Block;
    handle->speedFactor = 4.2;
    handle->pic.load(":/images/blocks/yello_air.jpg");
    handle->pic.scaledToWidth(blockSize);
    addBlock(handle);


}
