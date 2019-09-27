#pragma once



/*typedef struct Vec3 
{
	float x;
	float y;
	float z;
} VEC3;*/

struct MonEpee;

struct MaBotte
{
	MonEpee * x;
};

struct MonEpee 
{
	MaBotte y;
};

struct LIST;

/*typedef struct List 
{
	List p;
} LIST;*/

typedef int MonInt;
typedef int Id;

class Player
{

	Id KeyId;
	Id PasseportId;
};