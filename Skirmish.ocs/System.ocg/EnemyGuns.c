// You cannot simply cross the battlefield, you'll get shot.

static const EnemyGuns = new Effect {
	Timer = func() {
		var clonks = FindObjects(Find_InRect(1630, 0, LandscapeWidth() - 1630, 750), Find_OCF(OCF_CrewMember), Find_Allied(REPUBLIC));
		for (var clonk in clonks) {
			if (Random(3))
			{
				// Shoot five bullets at the same time
				var angle = Angle(clonk->GetX() + 450, clonk->GetY() - 450, clonk->GetX(), clonk->GetY());
				for (var cnt = 0; cnt < 5; cnt++)
				{
					var bullets = CreateObject(LeadBullet, clonk->GetX() + 450, clonk->GetY() - 450, KUTOR);
					var shot = bullets->TakeObject();
					shot->Launch(g_kutor_frontsoldier, angle * 100 + RandomX(-Blunderbuss.BulletSpread, Blunderbuss.BulletSpread), RandomX(-1, 1), 200 + RandomX(-4, 4), 0, 0, 100);
					shot.BulletFlightTime = 100;
					bullets->RemoveObject();
				}
			} else {
				var angle = Angle(clonk->GetX() + 450, clonk->GetY() - 450, clonk->GetX(), clonk->GetY());
				var shell = CreateObject(ArtilleryShell, clonk->GetX() + 450, clonk->GetY() - 450, KUTOR);
				shell.boosted = true;
				shell->SetSpeed(-450, 450);
				shell->OnCannonShot();
			}
		}
	}
};