#include "boulders.h"

Boulder::Boulder() {}

Boulder::Boulder(COORD location)
{
	this->m_cLocation = location;
}


void Boulder::moveBoulder(Map* map, SGameChar* player, Console* console)
{
	if (console->isInsideConsole(this->m_cLocation)) //if at console edge
	{
		if (!map->findCharExists(ADDCOORDS(this->m_cLocation, { player->direction.X << 1,  player->direction.Y << 1 }), 'Z')
			&& !map->collideWithWall(ADDCOORDS(this->m_cLocation, { player->direction.X,  player->direction.Y }))) //if boulder not colliding with walls
		{
			map->removeChar(this->m_cLocation, this->prevChar); //remove the B and replace with prevchar
			this->m_cLocation = ADDCOORDS(this->m_cLocation, player->direction);
			this->prevChar = map->addChar(this->m_cLocation, 'B'); //places the new B and store as prevchar
			player->m_cLocation = ADDCOORDS(player->m_cLocation, player->direction);
			PlaySound(TEXT("Sounds/moveboulder.wav"), NULL, SND_SYNC | SND_ASYNC);
		}
	}
}

void Boulder::destroyBoulder(Map* map)
{
	map->removeChar(this->m_cLocation, this->prevChar);
	//deletion handled when re-initializing
	PlaySound(TEXT("Sounds/pickaxe.wav"), NULL, SND_SYNC | SND_ASYNC);
}