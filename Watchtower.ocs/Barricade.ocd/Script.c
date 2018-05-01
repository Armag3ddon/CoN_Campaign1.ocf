/**
	Barricade

	Final objective will be to take this down.
*/

func Initialize()
{
	MakeInvincible();
	AddTimer("Trespassers", 35);
}

func Trespassers() // will be shot!
{
	var clonks = FindObjects(Find_InRect(-15, -90, 300, 180), Find_ID(Clonk), Find_OCF(OCF_Alive), Find_Hostile(GetOwner()));
	for (var clonk in clonks)
	{
		var y = AbsY(clonk->GetY());
		var x = -14;
		while(GBackSolid(x,y) && x < 31)
			x++;
		x+=3;
		if (PathFree(GetX() + x, GetY() + y, clonk->GetX(), clonk->GetY()))
		{
			// Shoot five bullets at the same time
			var angle = Angle(GetX() + x, GetY() + y, clonk->GetX(), clonk->GetY());
			for (var cnt = 0; cnt < 5; cnt++)
			{
				var bullets = FindContents(LeadBullet);
				if (!bullets)
					bullets = CreateContents(LeadBullet);
				var shot = Contents(0)->TakeObject();
				shot->Launch(this, angle * 100 + RandomX(-Blunderbuss.BulletSpread, Blunderbuss.BulletSpread), RandomX(-1, 1), 200 + RandomX(-4, 4), x, y, 100);
			}
		}
	}
}

func Damage()
{
	var all_hp = this.HitPoints * GetPlayerCount(C4PT_User);
	if (GetDamage() > all_hp && !g_destruction)
	{
		g_destruction = true;
	} else if (GetDamage() > all_hp / 2 && !this.half_damage)
	{
		Sound("WilhelmScream", true);
		this.half_damage = true;
	} else {
		Sound("Fire::Blast?", true);
	}
}

public func Destroy()
{
	AddTimer("Boom", 5);
}

func Boom()
{
	var boom = CreateObject(Dummy, RandomX(-14,14), RandomX(-70,70), NO_OWNER);
	boom->Sound("Fire::Blast?", true);
	boom->Explode(RandomX(10, 40));
}

local Name = "$Name$";
local Description = "$Description$";
local HitPoints = 200;