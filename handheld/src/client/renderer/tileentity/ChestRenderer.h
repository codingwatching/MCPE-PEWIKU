#ifndef NET_MINECRAFT_CLIENT_RENDERER_TILEENTITY__ChestRenderer_H__
#define NET_MINECRAFT_CLIENT_RENDERER_TILEENTITY__ChestRenderer_H__

//package net.minecraft.client.renderer.tileentity;

#include "TileEntityRenderer.h"
#include "../../model/ChestModel.h"
#include "../../model/LargeChestModel.h"

class ChestTileEntity;

class ChestRenderer: public TileEntityRenderer
{
public:
    virtual void render( TileEntity* entity, float x, float y, float z, float a );

private:
    ChestModel chestModel;
    LargeChestModel largeChestModel;
};

#endif /*NET_MINECRAFT_CLIENT_RENDERER_TILEENTITY__ChestRenderer_H__*/
