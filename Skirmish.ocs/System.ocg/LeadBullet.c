#appendto LeadBullet

public func FlightTime()
{
	if (this.BulletFlightTime)
		return this.BulletFlightTime;
	else
		return _inherited();
}