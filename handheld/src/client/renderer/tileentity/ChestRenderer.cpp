#include "ChestRenderer.h"
#include "TileEntityRenderer.h"
#include "../gles.h"
#include "../../model/ChestModel.h"
#include "../../../world/level/tile/entity/ChestTileEntity.h"
#include "../../../world/level/tile/ChestTile.h"
#include "../../../util/Mth.h"


void ChestRenderer::render( TileEntity* entity, float x, float y, float z, float a )
{
	ChestTileEntity* chest = (ChestTileEntity*) entity;
	int data = 0;

	if (chest->level) {
		Tile* tile = chest->getTile();
		data = chest->getData();

		if (tile != NULL && data == 0) {
			((ChestTile*)tile)->recalcLockDir(chest->level, chest->x, chest->y, chest->z);
			data = chest->getData();
		}

		chest->checkNeighbors();
	}

	if (chest->e != NULL || chest->s != NULL) {
		bindTexture("item/chest/double_normal.png");
	} else
	{
		bindTexture("item/chest/normal.png");
	}

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glColor4f(1, 1, 1, 1);
	glTranslatef(x, y + 1, z + 1);
	glScalef(1, -1, -1);

	glTranslatef(0.5f, 0.5f, 0.5f);
	GLfloat rot = 0;
	if (data == 2) rot = 180;
	if (data == 3) rot = 0;
	if (data == 4) rot = 90;
	if (data == 5) rot = -90;

	glRotatef(rot, 0, 1, 0);
	glTranslatef(-0.5f, -0.5f, -0.5f);

	float open = chest->oOpenness + (chest->openness - chest->oOpenness) * a;
	if (chest->n != NULL) {
		float open2 = chest->n->oOpenness + (chest->n->openness - chest->n->oOpenness) * a;
		if (open2 > open) open = open2;
	}
	if (chest->w != NULL) {
		float open2 = chest->w->oOpenness + (chest->w->openness - chest->w->oOpenness) * a;
		if (open2 > open) open = open2;
	}

	open = 1 - open;
	open = 1 - open * open * open;
    float xRot = -(open * Mth::PI / 2);

	if (chest->e != NULL || chest->s != NULL) {
        largeChestModel.lid.xRot = xRot;
        largeChestModel.render();
	} else {
        chestModel.lid.xRot = xRot;
        chestModel.render();
    }
	glPopMatrix();
	glColor4f(1, 1, 1, 1);
}
