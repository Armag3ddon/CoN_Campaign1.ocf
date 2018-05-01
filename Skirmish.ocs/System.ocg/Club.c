// The club won't do friendly fire
// And the club of pioneers will chuck dynamite sticks farer

#appendto Club

func DoStrike(clonk, angle)
{
	// hit all objects in the direction of the Clonk - the angle is only important for the direction of the flinging
	var x = Sin(angle, 7);
	var y = -Cos(angle, 7);
	var found = false;
	for (var obj in FindObjects(Find_Distance(15, 0, 0), Find_Or(Find_OCF(OCF_Alive), Find_Category(C4D_Object), Find_Category(C4D_Vehicle)), Find_Exclude(clonk), Find_Not(Find_And(Find_OCF(OCF_Alive), Find_Allied(clonk->GetOwner()))), Find_NoContainer(), Find_Layer(GetObjectLayer()), Sort_Distance()))
	{
		if (obj->Stuck()) continue;

		// don't hit objects behind the Clonk
		if (x < 0)
		{
			if (obj->GetX() > GetX()) continue;
		}
		else if (obj->GetX() < GetX()) continue;

		// vehicles are only hit if they are pseudo vehicles. Bad system - has to be changed in the future
		if (obj->GetCategory() & C4D_Vehicle)
			if (!GetEffect("HitCheck", obj)) continue;

		var en = Format("CannotBeHitTwiceBy%d", this->ObjectNumber());
		if (GetEffect(en, obj)) continue;

		if (obj->GetOCF() & OCF_Alive)
		{
			var damage = ClubDamage();
			ApplyWeaponBash(obj, 400, angle, clonk);
			obj->DoEnergy(-damage, true, FX_Call_EngGetPunched, clonk->GetOwner());
		}
		else
		{
			var div = ClubVelocityPrecision();
			if(obj->GetContact(-1)) div*=10;

			// the better you hit, the more power you have
			var precision = BoundBy(Distance(obj->GetX(), obj->GetY(), GetX() + x, GetY() + y), 1, 15);

			// mass/size factor
			var fac1 = 10000 / Max(5, obj->GetMass());
			var fac2 = BoundBy(10-Abs(obj->GetDefCoreVal("Width", "DefCore")-obj->GetDefCoreVal("Height", "DefCore")), 1, 10);
			var speed = (3000 * fac1 * fac2) / 2 / 1000 / precision;
			if (IsValueInArray(g_pioneers, clonk) && obj->GetID() == Dynamite)
			{
				speed = (3000 * fac1 * fac2) / 1000 / precision; // double speed!
				speed = BoundBy(speed, 500, 2000);
				obj->SetXDir(+ Sin(angle, speed) / 2, div);
				obj->SetYDir(- Cos(angle, speed) / 2, div);
				obj->SetController(clonk->GetController());
			} else {
				speed = BoundBy(speed, 500, 1500);

				obj->SetXDir((obj->GetXDir(100) + Sin(angle, speed)) / 2, div);
				obj->SetYDir((obj->GetYDir(100) - Cos(angle, speed)) / 2, div);
				obj->SetController(clonk->GetController());
			}
		}
		AddEffect(en, obj, 1, 15, nil);
		found=true;
		break;
	}

	if (found)
	{
		RemoveEffect("DuringClubShoot", clonk);
		Sound("Hits::Materials::Wood::WoodHit?", {pitch = -10});
	}
}