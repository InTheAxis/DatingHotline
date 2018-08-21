#include "enemies.h"

Enemy::Enemy(COORD loc, Map* map, Console* console)
{
	this->m_cLocation = loc;
	this->direction = { 0, 0 }; //default val
	srand((unsigned int)time(NULL));
	locationGen(map, console);
}

void Enemy::locationGen(Map* map, Console* console)
{
	COORD nonce;
	COORD originalPos = this->m_cLocation;
	do {
		originalPos = this->m_cLocation;
		nonce.X = rand() % this->spawnNonce.X;
		nonce.Y = rand() % this->spawnNonce.Y;
		originalPos = ADDCOORDS(originalPos, nonce);
	} while (!console->isInsideConsole(originalPos) || map->collideWithWall(originalPos));
	this->m_cLocation = originalPos;
	moveEnemy(map, console); // move to new location
}

COORD Enemy::directionGen(double seed = 1)
{
	int random = (int)(rand() * seed) % 5;
	switch (random)
	{
	case 0:
		return { 1, 0 };
		break;
	case 1:
		return { -1, 0 };
		break;
	case 2:
		return { 0, 1 };
		break;
	case 3:
		return { 0, -1 };
		break;
	default:
		return { 0, 0 };
		break;
	}
}
bool Enemy::isAggro(COORD playerPos)
{
	return ((this->m_cLocation.X + this->aggroRange >= playerPos.X
		&& this->m_cLocation.X - this->aggroRange <= playerPos.X)
		&& (this->m_cLocation.Y + this->aggroRange >= playerPos.Y
			&& this->m_cLocation.Y - this->aggroRange <= playerPos.Y));
}

void Enemy::setGeneralDir(COORD playerPos)
{
	if (playerPos.X - this->m_cLocation.X > 0)
		this->dirToPlayerX = { 1, 0 };
	else if (playerPos.X - this->m_cLocation.X < 0)
		this->dirToPlayerX = { -1, 0 };
	else
		this->dirToPlayerY = { 0, 0 };

	if (playerPos.Y - this->m_cLocation.Y > 0)
		this->dirToPlayerY = { 0, 1 };
	else if (playerPos.Y - this->m_cLocation.Y < 0)
		this->dirToPlayerY = { 0, -1 };
	else
		this->dirToPlayerY = { 0, 0 };

	dirToPlayerInvX.X = dirToPlayerX.X * -1;
	dirToPlayerInvY.Y = dirToPlayerY.Y * -1;
	this->direction = dirToPlayerX; //default to X
}

//imperfect pathfinding
void Enemy::setAltDir(Map* map, COORD playerPos)
{
	if (!map->collideWithWall(ADDCOORDS(this->m_cLocation, this->dirToPlayerX)))
	{
		this->direction = dirToPlayerX;
	}
	else if (this->m_cLocation.X == dirToPlayerX.X)
	{
		this->direction = { 0, dirToPlayerY.Y };
	}

	if (!map->collideWithWall(ADDCOORDS(this->m_cLocation, this->dirToPlayerY)))
	{
		this->direction = dirToPlayerY;
	}
	else if (this->m_cLocation.Y == dirToPlayerY.Y)
	{
		this->direction = { dirToPlayerX.X, 0 };
	}
}

void Enemy::moveEnemy(Map* map, Console* console)
{
	if (this->m_bActive && console->isInsideConsole(this->m_cLocation))
	{
		if (!EQUCOORDS(this->direction, { 0,0 }))
			map->removeChar(this->m_cLocation, this->prevChar); //remove the E and replace with prevchar
		this->m_cLocation = ADDCOORDS(this->m_cLocation, this->direction);
		this->prevChar = map->addChar(this->m_cLocation, 'E'); //places the new E and store as prevchar
	}
}

void Enemy::destroyEnemy(Map* map)
{
	map->removeChar(this->m_cLocation, this->prevChar);
	this->m_bActive = false;
	//deletion handled when re-initializing
}

bool Enemy::enemyAttack(COORD playerPos)
{
	return (this->m_bActive && EQUCOORDS(this->m_futureLocation, playerPos));
}