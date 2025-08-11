#pragma once

enum class GameObjectType
{
	None,
	Player,
	Monster,
	Projectile,
};

class Object
{
public:
	Object(GameObjectType type);
	virtual ~Object();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Render(HDC hdc) abstract;

public:
	GameObjectType GetObjectType() { return _type; }

	Pos GetPos() { return _pos; }
	void SetPos(Pos pos) { _pos = pos; }

protected:
	GameObjectType _type = GameObjectType::None;
	Stat _stat = {};
	Pos _pos = {};
};

