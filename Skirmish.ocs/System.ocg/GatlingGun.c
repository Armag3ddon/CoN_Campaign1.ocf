// After being shot, the bullets should switch to the regular layer

#appendto GatlingGun

protected func DoFire(object iammo, object clonk, int angle)
{
	var musk_front = 21;
	var musk_up = 25;
	var musk_y_offset = -8;
	var musk_x_offset = Cos(angle, 8, angPrec) * -(turnDir * 2 - 1); // To addjust for the graphics, offsetted barrel
	// Calculate offset for shot and effects.
	var off_x = Sin(angle, musk_front, angPrec) + musk_x_offset;
	var off_y = -Cos(angle, musk_up, angPrec) + musk_y_offset;

	iammo->Launch(clonk, angle + RandomX(-this.BulletSpread, this.BulletSpread), RandomX(-1, 1), RandomX(this.BulletSpeed[0], this.BulletSpeed[1]), off_x, off_y, angPrec);

	iammo->SetObjectLayer();

	// Muzzle Flash & gun smoke.
	var x = Sin(angle, 20, angPrec);
	var y = -Cos(angle, 20, angPrec);
	CreateParticle("Smoke", off_x, off_y, PV_Random(x - 20, x + 20), PV_Random(y - 20, y + 20), PV_Random(40, 60), Particles_Smoke(), 20);
	CreateMuzzleFlash(off_x, off_y, angle / angPrec, 20);
	CreateParticle("Flash", 0, 0, 0, 0, 8, Particles_Flash());
	// Gun blast sound.
	Sound("Objects::Weapons::Blunderbuss::GunShoot?");

	AddEffect("IntCooldown", this, 1, 5);
}